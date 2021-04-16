#ifndef _RSA_H_
#define _RSA_H_

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
#define ull unsigned long long
//����[left,right)�������
int random(int left,int right){
    int res=left+rand()%right;
    return res;
}
//ģ�����㣺(xy)%n
ull MulMod(ull a, ull b,ull n){
    return (a%n)*(b%n)%n;
}
//ģ�����㣺(base^pow)%n
ull PowMod(ull base,ull pow,ull n){
    ull a=base,b=pow,c=1;
    while(b){
        while(!(b&1)){
            b>>=1;
            a=MulMod(a,a,n);
        }
        b--;
        c=MulMod(a,c,n);
    }
    return c;
}
//RabinMiller���飬�������������
long RabinMillerKnl(ull &n){
    ull a,q,k,v;
    q=n-1;
    k=0;
    //ֻҪq��������
    while(!(q&1)){
        ++k;
        q>>=1;
    }
    a=random(2,n-1);
    v=PowMod(a,q,n);
    if(v==1)return 1;//������
    //ѭ�������Ƿ�Ϊ����
    for(int j=0;j<k;++j){
        unsigned int z=1;
        for(int w=0;w<j;++w){
            z<<=1;
        }
        if(PowMod(a,z*q,n)==n-1){
            return 1;
        }
    }
    return 0;//��������
}
//����ظ����飬��֤�ɿ���
long RabinMiller(ull &n,long loop=100){
    for(long i=0;i<loop;++i){
        if(!RabinMillerKnl(n)){
            return 0;
        }
    }
    return 1;
}
//�������ɵ����
ull RandomPrime(char bits){
    ull base;
    //�ظ���ֱ���������
    do{
        base=(unsigned long)1<<(bits-1);//��֤���λ��1  
        base+=random(0,base);
        base|=1;//�������һ������
    }while(!RabinMiller(base,30));
    return base;
}

//շת����������Լ��
ull Gcd(ull &p,ull &q){
    ull a=p>q?p:q;
    ull b=p<q?p:q;
    ull t;
    if(p==q){
        return p;
    }
    else{
        while(b){
            a=a%b;
            t=a;
            a=b;
            b=t;
        }
        return a;
    }
}

//����d*e%phi(n)=1���˽Կd-n��d��phi_n��ŷ������
ull Euclid(ull e,ull phi_n){
    ull Max=0xffffffffffffffff-phi_n;//��ֵ
    ull i=1;
    //���ϳ���phi_n�ı���
    while(1){
        if((i*phi_n+1)%e==0){//i*phi(n)=d*e-1
            return (i*phi_n+1)/e;//����d
        }
        ++i;
        ull tmp=(i+1)*phi_n;
        if(tmp>Max){//������ֵ��û�ҵ�d
            return 0;
        }
    }
    return 0;
}

//���ܣ�C=M^e%n
ull Encry(unsigned short M,ull e,ull n){
    return PowMod(M,e,n);
}
//���ܣ�M=C^d%n
unsigned short Decry(ull C,ull d,ull n){
    ull nRes=PowMod(C,d,n);//64bit
    unsigned short *pRes=(unsigned short*)&(nRes);//��nRes�ָ���ĸ�16bit������
    //�������ֻ��16bit
    if(pRes[1]||pRes[2]||pRes[3]){
        return 0;
    }
    else{
        return pRes[0];
    }
}

struct RSA{
    ull p,q,n,f,e,d,s;
    void print(){
        cout<<"p="<<p<<endl;
        cout<<"q="<<q<<endl;
        cout<<"n="<<n<<endl;
        cout<<"f="<<f<<endl;
        cout<<"e="<<e<<endl;
        cout<<"d="<<d<<endl;
        cout<<"s="<<s<<endl;
    }
};
//����RSA��Կen-˽Կdn�����������Ϣ
RSA RsaGetParam(){
    RSA rsa;
    rsa.p=RandomPrime(16);
    rsa.q=RandomPrime(16);
    rsa.n=rsa.p*rsa.q;
    rsa.f=(rsa.p-1)*(rsa.q-1);//ŷ��������phi_n
    do{
        rsa.e=random(0,65536);
        rsa.e|=1;
    }while(Gcd(rsa.e,rsa.f)!=1);

    rsa.d=Euclid(rsa.e,rsa.f);
    rsa.s=0;//n��λ��
    ull t=rsa.n>>1;
    while(t){
        rsa.s++;
        t>>=1;
    }
    return rsa;
}

#endif