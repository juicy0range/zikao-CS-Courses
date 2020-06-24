//1.进程调度/十大题型算法全实现

/*
一、 进程调度 
进程调度算法有FIFO，优先数调度算法，时间片轮转调度算法，分级调度算法，目前主要是考FIFO和优先数调度算法（静态优先级）。

输入：进程流文件，其中存储的是一系列要执行的进程，
每个作业包括四个数据项：

进程名 进程状态(1就绪 2等待 3运行)  所需时间  优先数(0级最高)

输出:
进程执行流 和等待时间 平均等待时间


  本程序包括:FIFO，优先数调度算法，时间片轮转调度算法

  */


#include<stdio.h>
#include<string.h>
#include<iostream.h>

const int timecard=19; //定义时间片的长度为5秒
const int MAX=100; //定义最大进程数


//定义进程结构体
typedef struct node{
char name[20];
int status;
int time;
int privilege;

int finished;
int waittime;
}pcb;

pcb pcbs[MAX];
pcb sxl[MAX];
pcb s;
int t;

//初始化函数
void initial(){
int i;
for(i=0;i<MAX;i++){
  strcpy(pcbs[i].name,"");
  pcbs[i].status=0;
  pcbs[i].time=0;
  pcbs[i].privilege=0;

  pcbs[i].finished=0;
  pcbs[i].waittime=0;
}
for(i=0;i<MAX;i++){
  strcpy(sxl[i].name,"");
  sxl[i].status=0;
  sxl[i].time=0;
  sxl[i].privilege=0;

  sxl[i].finished=0;
  sxl[i].waittime=0;
}
t=0;
}

//读数据函数
int readData(){
FILE *fp;
char fname[20];
int i;
cout<<"请输入进程流文件名:";
cin>>fname;
if((fp=fopen(fname,"r"))==NULL){
  cout<<"错误,文件打不开,请检查文件名"<<endl;
}
else{
  while(!feof(fp)){
   fscanf(fp,"%s %d %d %d",pcbs[t].name,&pcbs[t].status,&pcbs[t].time,&pcbs[t].privilege);
   t++;
   
  }
  
  //输出所读入的数据
  cout<<"输出所读入的数据"<<endl;
  cout<<"进程名 进程状态  所需时间  优先数"<<endl;
  for(i=0;i<t;i++){
   cout<<"  "<<pcbs[i].name<<"     "<<pcbs[i].status<<"          "<<pcbs[i].time<<"        "<<pcbs[i].privilege<<endl;
  }
  return(1);
}
return(0);

}

//重置数据,以供另一个算法使用
void init()
{ 
for(int i=0;i<t;i++){
	strcpy(sxl[i].name,pcbs[i].name);
	sxl[i].status=pcbs[i].status;
	sxl[i].time=pcbs[i].time;
	sxl[i].privilege=pcbs[i].privilege;
	sxl[i].finished=pcbs[i].finished;
	sxl[i].waittime=pcbs[i].waittime;
	}
}


//先进先出算法
void FIFO()
{
int i;
int sumtime;

//输出FIFO算法执行流
cout<<endl<<"---------------------------------------------------------------"<<endl;
cout<<"FIFO算法执行流:"<<endl;
cout<<"进程名    等待时间"<<endl;
for(i=0;i<t;i++){
  cout<<"  "<<sxl[i].name<<"         "<<sxl[i].waittime<<endl;
  sxl[i+1].waittime=sxl[i].time+sxl[i].waittime;
}
sumtime=0;
for(i=0;i<t;i++){
  sumtime+=sxl[i].waittime;
}
cout<<"总等待时间:"<<sumtime<<"  平均等待时间:"<<sumtime/t<<endl;

}

//优先数调度算法
void privilege()
{
  int sumtime,i;	
     for(i=0;i<t;i++)
	{
	  for(int j=0;j<t-i-1;j++)
	  {
		  if(sxl[j].privilege>sxl[j+1].privilege)
		  {
			 strcpy(s.name,sxl[j].name);
             s.status=sxl[j].status;
             s.time=sxl[j].time;
             s.privilege=sxl[j].privilege;
			 s.finished=sxl[j].finished;
			 s.waittime=sxl[j].waittime;
             strcpy(sxl[j].name,sxl[j+1].name);
			 sxl[j].status=sxl[j+1].status;
			 sxl[j].time=sxl[j+1].time;
			 sxl[j].privilege=sxl[j+1].privilege;
			 sxl[j].finished=sxl[j+1].finished;
			 sxl[j].waittime=sxl[j+1].waittime;
			 strcpy(sxl[j+1].name,s.name);
			 sxl[j+1].status=s.status;
			 sxl[j+1].time=s.time;
			 sxl[j+1].privilege=s.privilege;
			 sxl[j+1].finished=s.finished;
			 sxl[j+1].waittime=s.waittime;
		  }
	  }
	}
cout<<endl<<"---------------------------------------------------------------"<<endl;
cout<<"优先数调度算法"<<endl;
cout<<"进程名      等待时间"<<endl;
for(i=0 ; i<t ; i++ ){
  cout<<"   "<<sxl[i].name<<"           "<<sxl[i].waittime<<endl;
  sxl[i+1].waittime=sxl[i].time+sxl[i].waittime;
  }
  sumtime=0;
  for(i=0;i<t;i++){
   sumtime+=sxl[i].waittime;
  }
   cout<<"总等待时间:"<<sumtime<<"  平均等待时间:"<<sumtime/t<<endl;
}

//时间片轮转调度算法
void timer()
{cout<<endl<<"---------------------------------------------------------------"<<endl;
cout<<"时间片轮转调度算法"<<endl;
cout<<"进程名"<<endl;
	for( int j=0 ; j<t ; j++ ){
	for( int i=0; i<t ; i++ )
	{
		if(sxl[i].finished==0)
		{
			sxl[i].time-=timecard;
			cout<<sxl[i].name<<"   ";
			if(sxl[i].time<=0)
			{
				sxl[i].finished=1;
			}
		}
	}

	}
cout<<endl;
}



//主函数
void main()
{
int flag;


initial();

flag=readData();

if(flag==1){
 
	init();
    FIFO();
    init();
    privilege();
   init();
   timer();
	}
}


