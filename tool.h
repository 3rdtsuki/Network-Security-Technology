#include<iostream>
#include<cstring>
using namespace std;
#define ull unsigned long long
//��ull�͵Ĺ�Կe-nתΪ�ַ���out��ull��8B��ʮ���������20bit������Out��Ҫ40���ַ�
//���磺Out="               18089          2312472469"
void en2str(char *Out,ull e,ull n){
    for(int i=0;i<40;++i){//ǰ���λ�ÿո����
        Out[i]=' ';
    }
    int Lcnt=19,Rcnt=39;
    while(e){
        Out[Lcnt--]=e%10+'0';
        e/=10;
    }
    while(n){
        Out[Rcnt--]=n%10+'0';
        n/=10;
    }
}
//����Ӵ�[left,right)
void get_substr(char *Out,char *In,int left,int right){
    int cnt=0;
    for(int i=left;i<right;++i){
        Out[cnt++]=In[i];
    }
    Out[cnt]='\0';
}
//�ַ�������תull "526"->526
ull str2ull(const char *str){
    int n=strlen(str);
    ull res=0;
    for(int i=0;i<n;++i){
        res=res*10+str[i]-'0';
    }
    return res;
}
//ullתΪ20λ�ַ���Out
void ull2str(char *Out,ull In){
    for(int i=0;i<20;++i){//ǰ���λ�ÿո����
        Out[i]=' ';
    }
    int cnt=19;
    while(In){
        Out[cnt--]=In%10+'0';
        In/=10;
    }
    Out[20]='\0';
}