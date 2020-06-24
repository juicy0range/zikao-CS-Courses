/*

2008.6.21  模拟磁盘驱动调度过程：
20个磁头（0-19） 共有200个柱面；
由外向内（0-199）
程序运行：
（1），老师给出的文件名，文件中存有磁盘访问请求的柱面号。
（2），按照先来先服务磁盘调度算法，在题目上显示调度结果。（磁盘访问请求的磁道号以及磁头移动的总磁道数）
（3），假设当前磁头的位置在57磁道上按照最短寻道时间优先磁道访问算法，在屏幕上显示调度结果以及磁头移动的总磁道数。

包括: fcfs, ssTf(最短寻道),scan(电梯调度),cscan(循环调度算法)
*/
#include <iostream>               
#include <valarray> 
using namespace std;

#define maxNum 20
#define inifity 65500

typedef struct ciPanXuLie
{
   int cpNum;
   int flag;  //0表示未访问，1表示已访问
}cpxl;

int readFile(cpxl arrCpxl[]);  //读入请求的磁盘序列
void outCpxl(cpxl arrCpxl[],int arrNum);
void new_line();  //去掉没用的字符

void firstCp(cpxl arrCpxl[],int arrNum); //先来先服务

void shortCp(cpxl arrCpxl[],int arrNum); //最短寻道时间优先
int smallPos(cpxl arrCpxl[],int arrJuli[],int arrNum);  //求离当前磁头最近的磁道所在数组下标

void scanCp(cpxl arrCpxl[],int arrNum);  //电梯调度
void smallToBig(cpxl arrCpxl[],int arrNum);

void cScanCp(cpxl arrCpxl[],int arrNum); //循环调度算法
 
void main()
{
    int flag=1,arrNum,selNum;	
	cpxl arrCpxl[maxNum];

    cout<<"功能：实现磁盘调度(先来先服务 最短寻道 电梯调度 循环调度算法)\n\n";
   
	//读文件
    arrNum=readFile(arrCpxl);
    outCpxl(arrCpxl,arrNum);

	while(flag!=0)
	{
		cout<<"\n\n磁盘调度，请选择; 1:先来先服务; 2:最短寻道时间优先 3:电梯调度 4:循环调度算法; 0:退出系统 \n";
	    cin>>selNum;
		switch(selNum)
		{
		   case 1:
			   {
				 firstCp(arrCpxl,arrNum);
				 break;
			   }
		   case 2:
			   {
				 shortCp(arrCpxl,arrNum);
				 break;
			   }
		   case 3:
			   {
			      scanCp(arrCpxl,arrNum);
			   }
		   case 4:
			   {
			      cScanCp(arrCpxl,arrNum);
			   }
		   case 0:
			   {
				 flag=0;
				 break;
			   }
		   default:
			   {
				   cout<<"选择错误\n";
				   break;		   
			   }	
		}
	}
}


int readFile(cpxl arrCpxl[])  //读入请求的磁盘序列
{
    char fName[50];
	FILE *stream;
	int  kxqNum=0;

    cout<<"请输入 存有磁盘访问请求的柱面号 的文件名";
	cin.getline(fName,50,'\n');
	stream  = fopen( fName, "r" ); //打开文件
	while(stream==NULL)
    {
       cout<<"文件名或路径错误，请重新输入文件名\n";
	   //new_line();
	   cin.getline(fName,50,'\n');
       stream  = fopen( fName, "r" );
	}
    
	while(!feof(stream))
	{	
	    fscanf(stream,"%d",&arrCpxl[kxqNum].cpNum);
        arrCpxl[kxqNum].flag=0;  
        //kxqNum++;
        kxqNum=kxqNum+1;
	}    
    fclose(stream);	
    return kxqNum;  
}

void outCpxl(cpxl arrCpxl[],int arrNum)
{
   cout<<"\n文件中的磁盘序列为:\n";
   for(int i=0;i<arrNum;i++)
   {
      cout<<arrCpxl[i].cpNum<<"\t";   
   }

}


void new_line()   //去掉没用字符
{
    char symbol;
    do
	{
	   cin.get(symbol);
	}while(symbol!='\n');
}


void firstCp(cpxl arrCpxl[],int arrNum)  //先来先服务
{
	int currNum,sum=0;
    cout<<"请输入磁头所在的磁头号\n";
    cin>>currNum;

    cout<<"\n按照先来先服务磁盘调度算法,磁盘访问请求的磁道号为:\n";
    for(int i=0;i<arrNum;i++)
	{
	    cout<<arrCpxl[i].cpNum<<"\t";
	}
	//计算总磁道数
    for(i=0;i<arrNum;i++)
	{
	   if(i==0)
	   {
	       sum+=abs(arrCpxl[i].cpNum-currNum);
	   }
	   else
	   {
	       sum+=abs(arrCpxl[i].cpNum-arrCpxl[i-1].cpNum);
	   }
	}
     cout<<"\n 磁头移动的总磁道数为:"<<sum<<endl;
}


void shortCp(cpxl arrCpxl[],int arrNum)  //最短寻道时间优先
{
    
	int juli[maxNum],pos=0,sum=0;
	
    int currNum=57;  //开始时当前磁头号
    cout<<"\n开始时当前磁头号为"<<currNum<<endl; 
	cout<<"\n按照最短寻道时间优先磁道访问算法,磁盘访问请求的磁道号为:";

    for(int j=1;j<=arrNum;j++)     //每次找一个离当前磁头近的磁道访问
	{
		for(int i=0; i<arrNum ;i++)    
		{		   
		   juli[i]=abs(arrCpxl[i].cpNum-currNum);		   
		} 

		pos = smallPos(arrCpxl,juli,arrNum);
		
		cout<<arrCpxl[pos].cpNum<<"\t";
		sum+=juli[pos];       
	     
		arrCpxl[pos].flag=1;          //代表此磁道已访问过
        currNum=arrCpxl[pos].cpNum;   //修改当前磁道
    }
    cout<<"\n 磁头移动的总磁道数为:"<<sum<<endl;

}


int smallPos(cpxl arrCpxl[],int arrJuli[],int arrNum)
{
    int temp=0,pos;

	//为temp赋初值
	for(int j=0;j<arrNum;j++)
	{
	    if(arrCpxl[j].flag==0)
		{
		    temp=arrCpxl[j].cpNum;
			pos=j;
			break;
		}
	}

    //找未访问过的磁道中 找 距离最小的
	for(int i=0;i<arrNum;i++)
	{
	   if(arrCpxl[i].flag==0 && temp>arrJuli[i] ) 
	   {
	       temp=arrJuli[i];
		   pos=i;
	   }
	}
	return pos;
}

//电梯调度
void scanCp(cpxl arrCpxl[],int arrNum)  
{
   int i,j,sum=0;
   int currNum;    //开始时磁头号 
   int adj;  //存放最近一次访问过的磁道
   
   cout<<"\n请输入当前磁头号(如100)："<<endl; 
   cin>>currNum; 

   smallToBig(arrCpxl,arrNum);
   cout<<"\n按照电梯调度算法,磁盘访问请求的磁道号为:";
   for(i=0;i<arrNum;i++)
   {
      if(arrCpxl[i].cpNum>currNum)
	  {
	     break;
	  }
   }

   sum=0;
   adj=currNum;
   for(j=i;j<arrNum;j++)
   {
      cout<<arrCpxl[j].cpNum<<"  ";
	  sum+=abs(arrCpxl[j].cpNum-adj);
	  adj=arrCpxl[j].cpNum;
   }
   for(j=i-1;j>=0;j--)
   {
      cout<<arrCpxl[j].cpNum<<"  ";
	  sum+=abs(arrCpxl[j].cpNum-adj);
	  adj=arrCpxl[j].cpNum;
   }
   cout<<"\n按照电梯调度算法,总的磁道移动次数为:"<<sum<<endl;
 
}

void smallToBig(cpxl arrCpxl[],int arrNum)
{
	int temp;
    for(int i=0;i<arrNum;i++)
	{
	   for(int j=i+1;j<arrNum;j++)
	   {
	       if(arrCpxl[i].cpNum>arrCpxl[j].cpNum)
		   {
			   temp=arrCpxl[i].cpNum;
		       arrCpxl[i].cpNum=arrCpxl[j].cpNum;
               arrCpxl[j].cpNum=temp;
		   }
	   }
	}	
}

//循环调度算法:对电梯调度算法改一下
void cScanCp(cpxl arrCpxl[],int arrNum) 
{
   int i,j,sum=0;
   int currNum;    //开始时磁头号 
   int adj;  //存放最近一次访问过的磁道
   
   cout<<"\n请输入当前磁头号(如100)："<<endl; 
   cin>>currNum; 

   smallToBig(arrCpxl,arrNum);
   cout<<"\n按照循环调度算法,磁盘访问请求的磁道号为:";
   for(i=0;i<arrNum;i++)
   {
      if(arrCpxl[i].cpNum>currNum)
	  {
	     break;
	  }
   }

   sum=0;
   adj=currNum;
   for(j=i;j<arrNum;j++)
   {
      cout<<arrCpxl[j].cpNum<<"  ";
	  sum+=abs(arrCpxl[j].cpNum-adj);
	  adj=arrCpxl[j].cpNum;
   }
   for(j=0;j<=(i-1);j++)   //修改点
   {
      cout<<arrCpxl[j].cpNum<<"  ";
	  sum+=abs(arrCpxl[j].cpNum-adj);
	  adj=arrCpxl[j].cpNum;
   }
   cout<<"\n按照 循环调度算法,总的磁道移动次数为:"<<sum<<endl;
}