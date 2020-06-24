/*2_2
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

*/

#include <stdio.h>
#include <iostream> 
#include <iomanip> 
using namespace std;

const int arrNum=20;
const int infinity=100000;
typedef struct job{
   int jobNo;
   int arrivalTime;
   int runTime;
   int priority;  //响应比的优先级，默认为0
   int finishFlag; //0:表示等待中 1：已运行完
}JobNode;

int readFile(JobNode JN[arrNum]);
void comXYB(JobNode JN[arrNum],int jbNum,int startTime);  //计算 等待 作业的响应比
int  findSmallJob(JobNode JN[arrNum],int jbNum);  //找  等待 响应比最小的作业号
void outJob(JobNode JN[arrNum],int jobNo,int startTime);
void new_line();

void main()
{
	int jbNum,startTime,runJob,k;
	int sHour,sMinute;

	JobNode JN[arrNum];
	cout<<"--模拟最高响应比优先作业调度算法---\n";
    jbNum=readFile(JN);

    cout<<"\n采用最高响应比优先作业调度算法，作业的调度情况\n";
	cout<<"\n作业号  进入内存的时间  运行时间 \n";
    
    startTime=JN[jbNum-1].arrivalTime;
	k=jbNum;
	while(k>0)
	{
	  
       comXYB(JN,jbNum,startTime);
       runJob=findSmallJob(JN,jbNum);
       //输出运行情况
       outJob(JN,runJob,startTime);
       //startTime=startTime+JN[runJob].runTime;
	   //为下一个作业的开始时刻 （如: 8:50 ，运行30分钟； 则下个作业的开始时间为 9:20（即920） （8+1）*100+20
       sHour=startTime/100;   
	   sMinute=startTime%100;
       startTime=(sHour+(sMinute+JN[runJob].runTime)/60)*100+ (sMinute+JN[runJob].runTime)%60;  //以60为进制计算，但表示时为10进制

	   --k;
	}    
}


int readFile(JobNode JN[arrNum])
{
    char fName[50];
	FILE *stream;
    int i=0;

    cout<<"\n输入：作业流文件，其中存储的是一系列要执行的作业，每个作业包括三个数据项：\n";
    cout<<"\n作业号、作业进入系统的时间（用一整数表示，如10：10，表示成1010）、估计执行时间（单位分）优先级（0级最高）\n\n";
    cout<<"请输入作业流文件名\n";
   
	cin.getline(fName,50,'\n');
	stream  = fopen( fName, "r" ); //打开文件
	while(stream==NULL)
    {
       cout<<"文件名或路径错误，请重新输入文件名\n";
	   new_line();
	   cin.getline(fName,50,'\n');
       stream  = fopen( fName, "r" );
	}
    
	while(!feof(stream))
	{	
	    fscanf(stream,"%d %d %d",&JN[i].jobNo,&JN[i].arrivalTime,&JN[i].runTime);
        JN[i].finishFlag=0; 
		JN[i].priority=0;

		++i;
       
	}    
    fclose(stream);
    return i-1;
}


void comXYB(JobNode JN[arrNum],int jbNum,int startTime)  //计算 等待中 作业的响应比
{
   int sHour,sMinute,aHour,aMinute;
   for(int i=0;i<jbNum;i++)
   {
      if(JN[i].finishFlag==0)
      {
          sHour=startTime/60;     //如9:20 则sHour ：9  sMinute：20
          sMinute=startTime%60; 
          aHour=JN[i].arrivalTime/60;
          aMinute=JN[i].arrivalTime%60;
          //把时间统一为分钟数计算响应比（减法)          
	      JN[i].priority=( double( (sHour-aHour)*60+sMinute-aMinute+JN[i].runTime) )/JN[i].runTime;  //注意整除
	  }
   }

}

int  findSmallJob(JobNode JN[arrNum],int jbNum)  //找 等待中 响应比最小的作业号
{
   int min=infinity,pos;   //很大的一个数
   for(int i=0;i<jbNum;i++)
   {
      if(JN[i].finishFlag==0 && min>JN[i].priority)
      {
           min=JN[i].priority;
		   pos=i;
	  }
   }
   return pos;
}

void outJob(JobNode JN[arrNum],int jobNo,int startTime)
{
  
   cout<<JN[jobNo].jobNo<<"  ";
   cout<<startTime<<"  ";
   cout<<JN[jobNo].runTime<<endl;
  
   JN[jobNo].finishFlag=1;
}

void new_line()
{
    char symbol;
    do
	{
	   cin.get(symbol);
	}while(symbol!='\n');
}