/*
三、 虚拟存储管理器的页面调度
1． 输入：
页面流文件，其中存储的是一系列页面号（页面号用整数表示，用空格作为分隔符），用来模拟待换入的页面。
下面是一个示意：
1 2 3 4 1 2 5 1 2 3 4 5
2． 处理要求：
程序运行时，首先提示"请输入页面流文件的文件名："，输入一个文件名后，程序将读入该文件中的有关数据。
初始条件：采用三个页框，初始时均为空
3． 输出要求：
每换入一个页面（即：每读入一个页面号），判断是否有页面需要被换出。若有，把被换出的页面号输出到屏幕上；
若没有，则输出一个"*"号。
4． 文件名约定
提交的源程序名字：sourceXXX.c或者sourceXXX.cpp（依据所用语言确定）
输入文件名字：可由用户指定
其中：XXX为账号。
*/
#include <stdio.h>
#include <iostream>            

using namespace std;

const int pageFrameNum=3;
const int arrNum=12;

typedef struct pageFrame
{
   int page;
   int tag;
}pFrame;   //页框



int readFile(int request[arrNum]);  //
void new_line();  //去掉没用的字符

void showRPage(int requestP[arrNum],int rPageNum);
void FIFO(int requestP[arrNum],int rPageNum);

void main()
{
   int request[arrNum];  //保存请求的页面号
   int rPNum;   
   rPNum=readFile(request);
   showRPage(request,rPNum);
   FIFO(request,rPNum);
}


int readFile(int request[arrNum])
{
    char fName[50];
	FILE *stream;
	int  rPNum=0;

    cout<<"请输入页面流文件的文件名\n";
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
	    fscanf(stream,"%d",&request[rPNum]);
        ++rPNum;
	}   
   
    fclose(stream);	

    return rPNum;  
}

void new_line()   //去掉没用字符
{
    char symbol;
    do
	{
	   cin.get(symbol);
	}while(symbol!='\n');
}

void showRPage(int requestP[arrNum],int rPageNum)
{
   cout<<"\n请求的页面号为:\n";
   for(int i=0;i<rPageNum;i++)
   {
       cout<<requestP[i]<<"  ";
   }
}

void FIFO(int requestP[arrNum],int rPageNum)
{  
   pFrame pF[pageFrameNum];
   int i,j,k,exist,count=0;
  //初始时，页框为空，直接调入请求页；
   for(i=0;i<pageFrameNum;i++)
   {
	   pF[i].page=requestP[i];
	   if(i==0)
	   {
	      pF[i].tag=1;   //替换标志
	   }
	   else
	   {
	      pF[i].tag=0;
	   }  
	   ++count;     //中断次数加1
	   
   }

   //当页框满时，采用FIFO算法进行替换
   cout<<"\n\n被淘汰的页面号依次为:";
   for(j=i;j<rPageNum;j++)
   {
       exist=0;   //看请求页是否在内存中
	   for(k=0;k<pageFrameNum;k++)
	   {
	      if(requestP[j]==pF[k].page)
		  {
			  cout<<"*   ";
		      exist=1;
			  break;
		  }
	   }
	   if(exist==0)
	   {
          for(k=0;k<pageFrameNum;k++)
		  {
		     if(pF[k].tag==1)
			 {
			    cout<<pF[k].page<<"  ";
                ++count;
				//换入请求页
				pF[k].page=requestP[j];
				pF[k].tag=0;
				//找新的替换标识
				k=(k+1)%pageFrameNum;
				pF[k].tag=1;
			 }
		  }
	   }  
   }
   //输出中断次数
   cout<<"\n\n"<<"采用FIFO的中断次数为"<<count<<endl;
}

