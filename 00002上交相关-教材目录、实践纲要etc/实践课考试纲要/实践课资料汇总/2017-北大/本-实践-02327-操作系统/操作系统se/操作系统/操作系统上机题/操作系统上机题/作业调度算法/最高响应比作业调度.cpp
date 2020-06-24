/* 6月四号上午的题目及答案！阿鲲

6月四号上午的考题
模拟最高响应比优先作业调度算法
**********************
输入：作业流文件，其中存储的是一系列要执行的作业，
每个作业包括三个数据项：
作业号、作业进入系统的时间（用一整数表示，如10：10，表示成1010）、估计执行时间（单位分）
参数用空格隔开，下面是示例：
1 800 50
2 815 30
3 830 25
4 835 20
5 845 15
6 900 10
7 920 5
其中调度时刻为最后一个作业到达系统的时间！
输出作业号！进入内存的时间！每行输出一个作业信息！
*/
#include<string.h>
#include<iostream.h>
#include<stdio.h>
#include<stdlib.h>
struct job{
	int jnum;
	int arrive;
	int need;
	double bi;
};
int count=0;
void main()
{
	job jobs[10];
	FILE *fp;
	if((fp=fopen("job.txt","r+"))==NULL)
	{
		cout<<"file can not open!"<<endl;
	}
	else
	{
		int i=0;
		while(!feof(fp))
		{
			fscanf(fp,"%d%d%d",&jobs[i].jnum,&jobs[i].arrive,&jobs[i].need);
			i++;
			count++;
		}
		fseek(fp,-1,SEEK_END);
	    char c;
	    fscanf(fp,"%c",&c);
	    if((c=='\n')||(c=='\n'))
		{
	    	count--;
		}
		fclose(fp);
	}
	for(int i=0;i<count;i++)
	{
		jobs[i].bi=(jobs[count-1].arrive-jobs[i].arrive)/jobs[i].need;
	}
	for(i=0;i<count;i++)
	{
		cout<<jobs[i].jnum<<" "<<jobs[i].arrive<<" "<<jobs[i].need<<" "<<jobs[i].bi<<endl;
	}

}