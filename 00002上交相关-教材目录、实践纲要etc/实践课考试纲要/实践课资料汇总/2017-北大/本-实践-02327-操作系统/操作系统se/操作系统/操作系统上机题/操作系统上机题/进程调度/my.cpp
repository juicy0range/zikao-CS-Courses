#include<iostream.h>
#include<string.h>
#include<stdio.h>
#define MAX 10
typedef struct node{
	char name[20];
	struct node *next;
	int time;
	int youxian;
	char zhuangtai[20];
}pcb;
pcb b[MAX];
int tp(0);
void in(){
	int i=0;
	FILE *fp;
	char fname[20];
	cout<<"请出入文件名字"<<endl;
	//cin>>fname;
	strcpy(fname,"x.txt");
	if((fp=fopen(fname,"r"))==NULL)
		cout<<"无法打开文件，请检查文件名"<<endl;
	else
		while(!feof(fp))
		{
			fscanf(fp,"%s %d %d",b[tp].name,&b[tp].time,&b[tp].youxian);
			strcpy(b[tp].zhuangtai,"READY");
			b[tp].next=NULL;
			tp++;
		}
	cout<<"输出显示，文件中的数据信息"<<endl;
	cout<<"进程名"<<"  "<<"要求运行时间"<<"  "<<"优先级"<<"  "<<"进程状态"<<endl;
	for(i=0;i<tp;i++)
	{
		cout<<b[i].name<<"            "<<b[i].time<<"         "<<b[i].youxian<<"     "<<b[i].zhuangtai<<endl;
	}
cout<<tp<<"个进程么"<<endl;
}



void in2(pcb bb[]){
	int i,j;
	for(i=0;i<tp;i++)
	{
		for(j=0;j<tp-1;j++)
		{
		if(bb[j].youxian<bb[j+1].youxian)
			{
			bb[MAX-1]=bb[j];
			bb[j]=bb[j+1];
			bb[j+1]=bb[MAX-1];
			}	
		}
	}
	for(i=0;i<tp;i++)
	{
		bb[i].next=&bb[i+1];

	}

/*	cout<<"**********************************"<<endl<<"优先级调度算法"<<endl;	
	cout<<"进程名"<<"  "<<"要求运行时间"<<"  "<<"优先级"<<"  "<<"进程状态"<<"  "<<"链接指针"<<endl;

	while(bb->next)
	{
		cout<<bb->name<<"            "<<bb->time<<"         "<<bb->youxian<<"     "<<bb->zhuangtai<<"       "<<(bb->next)->name<<endl;

		bb=bb->next;
	
	}*/		
		
}
void youxianji(pcb p[])
{
	int i;
	int ktime=0;int wtime=0;
	int ptime=0;
	cout<<"**********************************"<<endl<<"优先级调度算法"<<endl;	
	cout<<"进程名"<<"  "<<"运行时间"<<"  "<<"开始时间"<<"  "<<"完成状态"<<endl;
	for(i=0;i<tp;i++)
	{
	
		wtime=ktime+p[i].time;
	cout<<" "<<p[i].name<<"        "<<p[i].time<<"        "<<ktime<<"        "<<wtime<<endl;
		ktime=ktime+p[i].time;
		ptime=ptime+wtime;

	}
	cout<<"平均等待时间:"<<ptime/tp<<endl;
}

void time(pcb p[])
{
	int a[MAX];
	int i,j;int t=0;
	for(i=0;i<tp;i++)
	{
		a[i]=(p+i)->time;
		t=t+(p+i)->time;
	}
	cout<<"*******************"<<endl<<"时间片轮转法"<<endl;

	for(j=0;j<t;j++)
	{
		for(i=0;i<5;i++)
		if(a[i]>=1)
		{	
		cout<<(p+i)->name<<"  ";
		a[i]--;
		}
		

	}
}
void main(){

	in();
	//in2(b);
	youxianji(b);
	time(b);
}