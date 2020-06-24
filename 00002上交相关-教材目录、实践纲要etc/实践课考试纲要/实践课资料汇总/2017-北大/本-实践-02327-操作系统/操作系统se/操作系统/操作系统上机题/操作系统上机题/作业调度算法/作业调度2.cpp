/*
六、 作业调度 
作业调度主要有FIFO，运算时间短的作业优先，优先数调度算法，响应比最高者优先调度算法，均衡调度算法


6月四号上午的考题
模拟最高响应比优先作业调度算法

输入：作业流文件，其中存储的是一系列要执行的作业，
每个作业包括三个数据项：
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


本程序包括:FIFO，运算时间短的作业优先，优先数调度算法，响应比最高者优先调度算法


  VC++调试通过

  (C)copyright by Neo

  欢迎大家测试 请问题请Email:sony006@163.com
*/

#include<stdio.h>
#include<iostream.h>
#include<iomanip.h>
#include<string.h>

const int MAXJOB=50; //定义最大作业

//定义数据结构体
typedef struct node{
int number;
int reach_time;
int reach_hour;
int reach_minite;
int need_time;

int privilege;

double excellent;

int start_time;
int start_hour;
int start_mini;

int wait_time;
int finish_time;
int finish_hour;
int finish_mini;
int zhouzh;
int visited;
}job;

job jobs[MAXJOB];
int quantity;
int sumtime;
int average;
void init()
{
	FILE *fp;int i=0;quantity=0;
	if((fp=fopen("a.txt","r+"))==NULL)
	{
		cout<<"file can not open!"<<endl;
	}
	else 
	{
		while(!feof(fp))
		{
			fscanf(fp,"%d%d%d",&jobs[i].number,&jobs[i].reach_time,&jobs[i].need_time);
			jobs[i].finish_time=0;
			jobs[i].finish_hour=0;
			jobs[i].finish_mini=0;
			jobs[i].excellent=0;
			jobs[i].privilege=0;
			jobs[i].reach_hour=jobs[i].reach_time/100;
			jobs[i].reach_minite=jobs[i].reach_time%100;
			jobs[i].start_time=0;
			jobs[i].start_hour=0;
			jobs[i].start_mini=0;
			jobs[i].zhouzh=0;
			jobs[i].visited=0;
			quantity++;
			cout<<jobs[i].number<<"    "<<jobs[i].reach_time<<"      "<<jobs[i].reach_hour<<"    "<<jobs[i].reach_minite<<"     "<<jobs[i].need_time<<endl;
			i++;
		}
		fseek(fp,-1,SEEK_END);
	    char c;
	    fscanf(fp,"%c",&c);
	    if((c=='\n')||(c==' '))
		{
	    	quantity--;
		}
	}
	fclose(fp);
//	cout<<quantity<<endl;
}
void excellent()
{
	sumtime=0;
	int start_hour=jobs[quantity-1].reach_hour;
	int start_mini=jobs[quantity-1].reach_minite;
	int start_time=jobs[quantity-1].reach_time;
	int k=0;
	for(int i=0;i<quantity;i++)
	{
		jobs[i].wait_time=(start_hour-jobs[i].reach_hour)*60+start_mini-jobs[i].reach_minite;
		jobs[i].excellent=double(jobs[i].wait_time)/double(jobs[i].need_time);
		if(jobs[i].excellent>jobs[k].excellent)
		{
			k=i;
		}
	}
	job temp;
	temp=jobs[0];
	jobs[0]=jobs[k];
	jobs[k]=temp;
	int need_hour=jobs[0].need_time/60;
	 int need_mini=jobs[0].need_time%60;

	jobs[0].start_time=start_time;
	jobs[0].start_hour=start_hour;
	jobs[0].start_mini=start_mini;

	jobs[0].finish_mini=(start_mini+jobs[0].need_time)%60;
	jobs[0].finish_hour=start_hour+(start_mini+jobs[0].need_time)/60;
	jobs[0].finish_time=jobs[0].finish_hour*100+jobs[0].finish_mini;

    jobs[0].zhouzh=(jobs[0].finish_hour-jobs[0].reach_hour)*60+jobs[0].finish_mini-jobs[0].reach_minite;
	sumtime=jobs[0].zhouzh;
	cout<<"作业号 到达时间 所需时间 响应比  执行时间 完成时间 周转时间"<<endl;
	cout<<jobs[0].number<<setw(10)<<jobs[0].reach_time<<setw(10)<<jobs[0].need_time<<setw(10)<<jobs[0].excellent<<setw(10)<<jobs[0].start_time<<setw(10)<<jobs[0].finish_time<<setw(10)<<jobs[0].zhouzh<<endl;
	for(i=0;i<quantity-1;i++)
	{
		k=i+1;
		for(int j=i+1;j<quantity;j++)
		{
		   jobs[j].wait_time=(jobs[i].finish_hour-jobs[j].reach_hour)*60+jobs[i].finish_mini-jobs[j].reach_minite;
		   jobs[j].excellent=double(jobs[j].wait_time)/double(jobs[j].need_time);
	    	if(jobs[j].excellent>jobs[k].excellent)
			{
			   k=j;
			}
		}
			temp=jobs[i+1];
	        jobs[i+1]=jobs[k];
         	jobs[k]=temp;
			need_hour=jobs[i+1].need_time/60;
	        need_mini=jobs[i+1].need_time%60;
			jobs[i+1].start_time=jobs[i].finish_time;
	        jobs[i+1].start_hour=jobs[i].finish_hour;
			jobs[i+1].start_mini=jobs[i].finish_mini;

			jobs[i+1].finish_mini=(jobs[i+1].need_time+jobs[i+1].start_mini)%60;
			jobs[i+1].finish_hour=(jobs[i+1].start_hour+(jobs[i+1].need_time+jobs[i+1].start_mini)/60);
			jobs[i+1].finish_time=jobs[i+1].finish_hour*100+jobs[i+1].finish_mini;

			jobs[i+1].zhouzh=(jobs[i+1].finish_hour-jobs[i+1].reach_hour)*60+jobs[i+1].finish_mini-jobs[i+1].reach_minite;
	        sumtime=jobs[i+1].zhouzh+sumtime;
		cout<<jobs[i+1].number<<setw(10)<<jobs[i+1].reach_time<<setw(10)<<jobs[i+1].need_time<<setw(10)<<jobs[i+1].excellent<<setw(10)<<jobs[i+1].start_time<<setw(10)<<jobs[i+1].finish_time<<setw(10)<<jobs[i+1].zhouzh<<endl;
	}
average=sumtime/quantity;
cout<<"总周转时间："<<sumtime<<"; 平均周转时间："<<average<<endl;
return;
}
		
void main()
{
init();
excellent();
}