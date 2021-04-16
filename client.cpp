#include"des.h"
#include"rsa.h"
#include"tool.h"
#include<Winsock2.h> 
#include<stdio.h>
#include<string.h>
#pragma comment(lib,"ws2_32")
#define SERVER_PORT 6666
RSA rsa;
void setRSA(char *e_n){
    ull e,n;
    for(int i=0;i<20;++i){
        if(e_n[i]==' ')continue;
        char e_str[20];
        get_substr(e_str,e_n,i,20);
        e=str2ull(e_str);
        break;
    }
    for(int i=20;i<40;++i){
        if(e_n[i]==' ')continue;
        char n_str[20];
        get_substr(n_str,e_n,i,40);
        n=str2ull(n_str);
        break;
    }
    rsa.e=e;
    rsa.n=n;
}
int main(){
    WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2,1); 
	WSAStartup(wVersionRequested, &wsaData);

    char sendbuf[200],recvbuf[200];
    int serverSocket=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//ȥ��6666�˿ڷ���������
    connect(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    //����DES��Կ
    char key[]="abcdefgh";
    Des_SetKey(key);
    //����RAS��Կ�������н�����ǰ20λ��e����20λ��n
    char e_n[40];
    recv(serverSocket, e_n, 40, 0); 
    setRSA(e_n);//��������enд��ȫ��rsa
    cout<<"���յ�RAS��Կ��e="<<rsa.e<<", n="<<rsa.n<<endl;

    //��RAS��Կ����DES��Կ
    ull C=0;//����
    unsigned short M;//��Ϊ������ushort�ͣ�2B��16bit��������Ҫ��8�ַ���Կ��64bit���ָ���RAS���ܣ�Ϊ�˼��ÿ���ַ�һ��
    char C_str[500]="",temp[25];
    for(int i=0;i<8;++i){
        M=(unsigned short)key[i];
        C=Encry(M,rsa.e,rsa.n);
        ull2str(temp,C);
        cout<<"C="<<temp<<endl;
        strcat(C_str,temp);
    }
    cout<<"���ܺ��DES��ԿΪ��"<<C_str<<endl;
    send(serverSocket, C_str, strlen(C_str), 0);//����DES��Կ��RSA����
    cout<<"�ѷ��ͼ��ܺ��DES��Կ"<<endl;
    
    
    while(1)
	{
		printf("������Ϣ��");
        scanf("%s",sendbuf);
		if(strcmp(sendbuf, "quit") == 0) break;
        //DES����
        DES_Encrypt(sendbuf,sendbuf);

		send(serverSocket, sendbuf, strlen(sendbuf), 0); //�����˷�����Ϣ
        cout<<"�ȴ�������Ϣ��...\n"; 


		printf("���ܵ���Ϣ��");
		int len=recv(serverSocket, recvbuf, 200, 0); //���շ���˷�������Ϣ
		recvbuf[len]='\0';//����Ĳ�Ҫ��
        //DES����
        DES_Decrypt(recvbuf,recvbuf);
		printf("%s\n", recvbuf);
	}
    closesocket(serverSocket);
	WSACleanup();
}
