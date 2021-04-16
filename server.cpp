#include"des.h"
#include"rsa.h"
#include"tool.h"
#include<Winsock2.h> 
#include<stdio.h>
#include<string.h>
#include<iostream>
#pragma comment(lib,"ws2_32")
#define SERVER_PORT 6666
using namespace std;

int main(){
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2,1); 
	WSAStartup(wVersionRequested, &wsaData);
	
	SOCKET sockSrv=socket(AF_INET,SOCK_STREAM,0);//������������socket
	struct sockaddr_in server_addr;//�������˵�ַ
	
	char buffer[500];
	int addrlen=sizeof(server_addr);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);//�������˿ںŶ�Ϊ6666
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sockSrv,(SOCKADDR*)&server_addr,sizeof(SOCKADDR));//�׽��ְ󶨷������˵�ַ
	listen(sockSrv,5);//���е���󳤶�Ϊ5
	
	
    printf("�����˿ڣ�%d\n",SERVER_PORT);
    struct sockaddr_in addrClient;//�ͻ��˵�ַ
    SOCKET client = accept(sockSrv, (SOCKADDR*)&addrClient, &addrlen);//�������׽������ӿͻ�
    cout<<"��ǰ�ͻ��Ķ˿ںţ�"<<addrClient.sin_port<<endl;//�����ǰ�ͻ��Ķ˿ں�

    
    //����RSA��Կe-n��˽Կd-n
    RSA rsa;
    srand(time(NULL));
    rsa=RsaGetParam();
    //�ѹ�Կ�����ͻ���
    char e_n[40];
    en2str(e_n,rsa.e,rsa.n);//��ull�͵�RSA��ԿתΪ�ַ���������e_n
    send(client, e_n, strlen(e_n), 0);//����Կ�����ͻ���
    cout<<"�ѽ���Կ�����ͻ��ˣ�e="<<rsa.e<<", n="<<rsa.n<<endl;
    Sleep(100);//��һ�¶��淢�� 
    
    //���չ�Կ���ܺ��DES��Կ����RSA˽Կ����
    int len=recv(client, buffer, sizeof(buffer), 0);
    buffer[len]='\0';//�������Ҫ�ӣ�����������ֵĶ���qwq 
    cout<<"�յ����ܺ��DES��ԿΪ"<<buffer<<endl;
    
    char key[9];//RSA���ܺ��DES��Կ
    len=strlen(buffer);
    for(int i=0;i<len;i+=20){//20���ַ���Ӧһ��ull������Կ��һ���ַ���asc�룩
        char temp[20];
        for(int j=0;j<20;++j){
        	if(buffer[i+j]==' ')continue;//ȥ��ǰ��Ŀո� 
        	get_substr(temp,buffer,i+j,i+20);
        	break;
		}
        //תΪull
        ull C=str2ull(temp);
        cout<<"C="<<C<<endl;
        //RSA����
		unsigned short asc=Decry(C,rsa.d,rsa.n);
		cout<<asc<<endl;
        key[i/20]=(char)asc;
    }
    
    key[8]='\0'; 
    cout<<"DES��Կ���ܺ�Ϊ"<<key<<endl;
    Des_SetKey(key);

    while(1){
        len=recv(client, buffer, sizeof(buffer), 0);
        buffer[len]='\0';//����Ĳ�Ҫ��
        //DES����
        DES_Decrypt(buffer,buffer);

        if(strcmp(buffer,"quit")==0)break;
        printf("���յ���Ϣ��%s",buffer);
        printf("\n");	


        printf("������Ϣ��");
        scanf("%s",buffer);
        if(strcmp(buffer,"quit")==0)break;
        //DES����
        DES_Encrypt(buffer,buffer);

        send(client, buffer, strlen(buffer), 0);
        cout<<"�ȴ�������Ϣ��...\n"; 
    }
	
	closesocket(sockSrv);
	WSACleanup();

}
//g++ server.cpp -o server.exe -lwsock32
