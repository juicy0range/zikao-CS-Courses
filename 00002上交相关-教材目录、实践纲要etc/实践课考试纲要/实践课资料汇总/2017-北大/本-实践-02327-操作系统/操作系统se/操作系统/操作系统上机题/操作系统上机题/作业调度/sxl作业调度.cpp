///作业调度/算法/

/*

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

*/

#include<stdio.h>
#include<iostream.h>
#include<iomanip.h>
#include<string.h>

const int MAXJOB=50; //定义最大作业

//定义数据结构体
typedef struct node{
int number;
int time;
int need;
int q;
int xyb;
int visited;
}job;

job jobs[MAXJOB];
job copy[MAXJOB];
int t;
job a;
//初始化函数
void initial()
{
for(int i=0;i<MAXJOB;i++)
{
  jobs[i].number=0;
  jobs[i].time=0;
  jobs[i].need=0;
  jobs[i].q=0;
  jobs[i].xyb=0;
  jobs[i].visited=0;
}
t=0;
}

//重置作业数据函数
void reset()
{ 
	for(int i=0;i<MAXJOB;i++)
	{
		
		jobs[i].visited=0;
	}
}

//读入作业数据函数
int  readData()
{
FILE *fp;
char fname[20];
int i;

cout<<"请输入作业数据文件名:";
//strcpy(fname,"8job.txt");
cin>>fname;

if((fp=fopen(fname,"r"))==NULL){
  cout<<"错误,文件打不开,请检查文件名:)"<<endl;
  return 0;
}
else{
  while(!feof(fp)){
   fscanf(fp,"%d %d %d %d",&jobs[t].number,&jobs[t].time,&jobs[t].need,&jobs[t].q);
   t++;
  }
  //输出初始作业数据
  cout<<"输出初始作业数据"<<endl;
  cout<<"---------------------------------------------------------------"<<endl;
  cout<<"        "<<"作业号"<<"         "<<"到达时间"<<"         "<<"所需时间(分)"<<"           "<<"优先级(0>1)"<<endl;

  for(i=0;i<t;i++){
   cout<<"       "<<jobs[i].number<<"         "<<jobs[i].time<<"      "<<jobs[i].need<<"          "<<jobs[i].q<<endl;
  }
}
return 1;
}

//FIFO算法
void FIFO()
{int current,v;
int total=0;
cout<<endl;
cout<<endl<<"FIFO算法作业流"<<endl;
cout<<"---------------------------------------------------------------"<<endl;
current=jobs[0].time;
v=current;
cout<<"         "<<"作业号"<<"          "<<"到达时间"<<"       "<<"开始时间"<<"          "<<"周转时间(分)"<<endl;
cout<<"       "<<jobs[0].number<<"         "<<jobs[0].time<<"      "<<current<<"          "<<jobs[0].need<<endl;
for(int i=1;i<t;i++)
{
	current+=jobs[i-1].need;
	if(current>jobs[i].time)
	{
	
		cout<<"       "<<jobs[i].number<<"         "<<jobs[i].time<<"      "<<current<<"          "<<jobs[i].need<<endl;
		
	}
	else
	{
		cout<<"       "<<jobs[i].number<<"         "<<jobs[i].time<<"      "<<jobs[i].time<<"          "<<jobs[i].need<<endl;
		
	}
}
total=current-v;
cout<<endl<<"总周转时间:"<<total<<"   平均周转时间:"<<total/t<<endl;
}


//运算时间短的作业优先算法
void shorter()
{
	int current,j=0,v;
	int total=0;
	cout<<endl<<"运算时间短的作业优先算法"<<endl;
	cout<<"---------------------------------------------------------------"<<endl;
	current=jobs[0].time;
	v=current;
	cout<<"         "<<"作业号"<<"          "<<"到达时间"<<"       "<<"开始时间"<<"          "<<"周转时间(分)"<<endl;
	for(int m=0;m<t;m++)
	{	for(int i=0;i<t;i++)
		{
			if((current>=jobs[i].time)&&(jobs[i].visited==0))
			{
				copy[j]=jobs[i];
				j++;
			}
			
		}

	int k=j;
	for(i=0;i<k;i++)
		for(j=0;j<k-i-1;j++)
		{
			if(copy[j].need>copy[j+1].need)
			{
				a=copy[j];
				copy[j]=copy[j+1];
				copy[j+1]=a;
			}
		}
		for( i=0;i<t;i++)
		{
			if(copy[0].number==jobs[i].number)
			{
				jobs[i].visited=1;
			}
		}
	cout<<"       "<<copy[0].number<<"         "<<copy[0].time<<"      "<<current<<"          "<<copy[0].need<<endl;
	current+=copy[0].need;
	}
	total=current-v;
	cout<<endl<<"总周转时间:"<<total<<"   平均周转时间:"<<total/t<<endl;
	
}

//优先数调度算法
void q()
{
int current,j=0,v;
	int total=0;
	cout<<endl<<"优先数调度算法"<<endl;
	cout<<"---------------------------------------------------------------"<<endl;
	current=jobs[0].time;
	v=current;
	cout<<"         "<<"作业号"<<"          "<<"到达时间"<<"       "<<"开始时间"<<"          "<<"周转时间(分)"<<endl;
	for(int m=0;m<t;m++)
	{	for(int i=0;i<t;i++)
		{
			if((current>=jobs[i].time)&&(jobs[i].visited==0))
			{
				copy[j]=jobs[i];
				j++;
			}
			
		}

	int k=j;
	for(i=0;i<k;i++)
		for(j=0;j<k-i-1;j++)
		{
			if(copy[j].q>copy[j+1].q)
			{
				a=copy[j];
				copy[j]=copy[j+1];
				copy[j+1]=a;
			}
		}
		for( i=0;i<t;i++)
		{
			if(copy[0].number==jobs[i].number)
			{
				jobs[i].visited=1;
			}
		}
	cout<<"       "<<copy[0].number<<"         "<<copy[0].time<<"      "<<current<<"          "<<copy[0].need<<endl;
	current+=copy[0].need;
	}
	total=current-v;
	cout<<endl<<"总周转时间:"<<total<<"   平均周转时间:"<<total/t<<endl;
	

}

//响应比最高者优先调度算法
void excellent()
{
int current,j=0,v;
	int total=0;
	cout<<endl<<"响应比最高者优先调度算法"<<endl;
	cout<<"---------------------------------------------------------------"<<endl;
	current=jobs[0].time;
	v=current;
	cout<<"         "<<"作业号"<<"          "<<"到达时间"<<"       "<<"开始时间"<<"          "<<"周转时间(分)"<<endl;
	for(int m=0;m<t;m++)
	{	for(int i=0;i<t;i++)
		{
			if((current>=jobs[i].time)&&(jobs[i].visited==0))
			{
				jobs[i].xyb=(current-jobs[i].time)/jobs[i].need;
				copy[j]=jobs[i];
				j++;
			}
			
		}

	int k=j;
	for(i=0;i<k;i++)
		for(j=0;j<k-i-1;j++)
		{
			if(copy[j].xyb<copy[j+1].xyb)
			{
				a=copy[j];
				copy[j]=copy[j+1];
				copy[j+1]=a;
			}
		}
		for( i=0;i<t;i++)
		{
			if(copy[0].number==jobs[i].number)
			{
				jobs[i].visited=1;
			}
		}
	cout<<"       "<<copy[0].number<<"         "<<copy[0].time<<"      "<<current<<"          "<<copy[0].need<<endl;
	current+=copy[0].need;
	}
	total=current-v;
	cout<<endl<<"总周转时间:"<<total<<"   平均周转时间:"<<total/t<<endl;
	

}



void main()
{
int f=1;
int c;
initial();
if(readData())
{
	while(f)
	{
		cout<<endl<<"-------------------------------------------------------------------------"<<endl;
		cout<<"  0:退出     1:FIFO      2:运算时间短的作业优先     3:优先数调度算法    4:响应比最高者优先调度算法"<<endl;
		cout<<endl<<"-------------------------------------------------------------------------"<<endl;
		cout<<"请选择:"<<endl;
		cin>>c;
		switch(c)
		{
		case 1:
			FIFO();
			break;
		case 2:
			shorter();
			break;
		case 3:
			reset();
			q();
			break;
		case 4:
			reset();
			excellent();
			break;
		case 0:
			f=0;
			break;
		default:
			cout<<"选择错误"<<endl<<endl;

		}


	}
}

}

