/*每个作业包括三个数据项：
作业号、作业进入系统的时间（用一整数表示，如10：10，表示成1010）、估计执行时间（单位分）优先级（0级最高）
参数用空格隔开，下面是示例：
1 800 50
2 815 30
3 830 25
4 835 20
5 845 15
6 900 10
7 920 5
其中调度时刻为最后一个作业到达系统的时间！
输出作业号！进入内存的时间！每行输出一个作业信息
本程序包括:FIFO，运算时间短的作业优先，优先数调度算法，响应比最高者优先调度算法*/
#include<iostream.h>
#include<string.h>
#include<stdio.h>
#define MAX 50
typedef struct node{
	char name[20];
	int intime;
	int time;
	int youxian;
}job;
job a[MAX];
int b(0);
void in(){
	FILE *fp;
	int i;
	char fname[20];
	cout<<"输入文件名字:"<<endl;
	//cin>>fname;
	strcpy(fname,"a.txt");
	if((fp=fopen(fname,"r"))==NULL)
	{	cout<<"找不到文件"<<endl;}
	
	else 
	{
		while(!feof(fp))
		{
		fscanf(fp,"%s %d %d",a[b].name,&a[b].intime,&a[b].time);
		a[b].youxian=0;
		b++;
		}
	
	}
	cout<<"作业"<<"  "<<"进入时间"<<"  "<<"需要时间"<<"  "<<"优先级"<<endl;

	for(i=0;i<b;i++)
	{ 
	cout<<a[i].name<<"       "<<a[i].intime<<"     "<<a[i].time<<"           "<<a[i].youxian<<endl;
	}
}

void fifo(job p[])
{
	cout<<"作业"<<"  "<<"进入时间"<<"  "<<"需要时间"<<"  "<<"开始时间"<<"完成时间"<<"周转时间"<<endl;
	cout<<p[]
}
void main(){
	in(a);

}
