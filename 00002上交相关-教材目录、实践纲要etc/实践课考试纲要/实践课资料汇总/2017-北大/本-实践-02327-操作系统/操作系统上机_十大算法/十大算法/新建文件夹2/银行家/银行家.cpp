#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
const int M=5;

struct ZRQK
{ 
	char name;
	int cdj;
	int hty;
	int dyj;
	int cdr;
};

void create(ZRQK table[],int k)
{
	ifstream f1;
	char filname[20];
	int i;
	if(k==1) cout<<"请输入存储进程占有资源的文件名:"<<endl;
	else cout<<"请输入存储进程还需要资源的文件名:"<<endl;
	cin>>filname;
	f1.open(filname,ios::in|ios::nocreate);
	if(!f1){cout<<"不能打开指定文件!"<<endl;
	exit(1);}
	for(i=0;i<M;i++)
	{
		f1>>table[i].name;
		f1>>table[i].cdj;
		f1>>table[i].hty;	f1>>table[i].dyj;	f1>>table[i].cdr;
	}
	if(k==1) cout<<"输入的进程占有的资源为:"<<endl;
	else cout<<"输入的进程还需要的资源为:"<<endl;
	for(i=0;i<M;i++)
	{
		cout<<"  "<<table[i].name<<"  "<<table[i].cdj<<"  "<<table[i].hty;
		cout<<"   "<<table[i].dyj<<"  "<<table[i].cdr<<endl;
	}
	f1.close();
}
void main()
{ 
	int i;int flag[M];int j;int order[M];
	ZRQK ZYQY[M],XYZY[M],SE={' ',6,3,4,2},SA=SE;
	cout<<"系统的总资源为:"<<endl;
	cout<<"  "<<SE.name<<"  "<<SE.cdj<<"  "<<SE.hty;
	cout<<"   "<<SE.dyj<<"  "<<SE.cdr<<endl;
	create(ZYQY,1);
	create(XYZY,2);
	SA.name =' ';	
	for(i=0;i<M;i++)
	{
		SA.cdj =SA.cdj-ZYQY[i].cdj ;
		SA.cdr =SA.cdr-ZYQY[i].cdr ;
		SA.dyj =SA.dyj-ZYQY[i].dyj ;
		SA.hty =SA.hty-ZYQY[i].hty ;
	}
	cout<<"系统的剩余资源为:"<<endl;
	cout<<"  "<<SA.name<<"  "<<SA.cdj<<"  "<<SA.hty;
	cout<<"   "<<SA.dyj<<"  "<<SA.cdr<<endl;
	for(i=0;i<M;i++)  
	{
		flag[i]=1;order[i]=-1;
	}
	for(j=0;j<M;j++)
	{
		for(i=0;i<M;i++)
		{
			if(flag[i]==1)
				if(SA.cdj>=XYZY[i].cdj &&SA.cdr>=XYZY[i].cdr )
					if(SA.dyj>=XYZY[i].dyj &&SA.hty>=XYZY[i].hty )
					{
						flag[i]=0;
						SA.cdj =SA.cdj +ZYQY[i].cdj ;
						SA.cdr =SA.cdr +ZYQY[i].cdr;
						SA.dyj =SA.dyj +ZYQY[i].dyj;
						SA.hty =SA.hty+ZYQY[i].hty ;
						order[j]=i;
						break;
					}	 
		}
	}
	int  	flag1=0;
	for(i=0;i<M;i++)
	{
		if(flag[i]==1) {flag1=1;break;}
	}
	if(flag1==0)
	{
		cout<<"系统能够找到一个执行序列，使得所有进程得以执行"<<endl;
		cout<<"序列为:"<<endl;
		for(i=0;i<M;i++) 
			cout<<ZYQY[order[i]].name <<"  ";
		cout<<endl;cout<<"系统是'安全'的"<<endl;
	}
	else 
	{
		cout<<"系统不能找到一个执行序列，使得所有进程得以执行  !"<<endl;
		cout<<"系统是'不安全'的!"<<endl;
	}
}
