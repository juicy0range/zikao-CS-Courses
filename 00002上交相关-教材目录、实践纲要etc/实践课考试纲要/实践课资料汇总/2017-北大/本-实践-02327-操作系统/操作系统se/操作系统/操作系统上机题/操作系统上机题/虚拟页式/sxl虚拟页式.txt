//虚拟存储管理器的页面调度算法全实现

/*
三、 虚拟存储管理器的页面调度

页面调度算法主要有：FIFO，最近最少使用调度算法（LRU），最近最不常用调度算法（LFU），最佳算法（OPT） 

1． 输入：
页面流文件，其中存储的是一系列页面号（页面号用整数表示，用空格作为分隔符），用来模拟待换入的页面。
下面是一个示意：
1 2 3 4 1 2 5 1 2 3 4 5
2． 处理要求：
程序运行时，首先提示“请输入页面流文件的文件名：”，输入一个文件名后，程序将读入该文件中的有关数据。
初始条件：采用三个页框，初始时均为空。
根据第二次机会算法对数据进行处理。
3． 输出要求：
每换入一个页面（即：每读入一个页面号），判断是否有页面需要被换出。若有，把被换出的页面号输出到屏幕上；
若没有，则输出一个“*”号。
4． 文件名约定
提交的源程序名字：sourceXXX.c或者sourceXXX.cpp（依据所用语言确定）
输入文件名字：可由用户指定
其中：XXX为账号。
5． 测试说明：测试教师将事先准备好一组文件（格式为*.txt），从中为每个程序随机指定一至三个作为输入文件
（被测试者需从键盘输入指定文件的文件名），并查看程序输出结果。
6． 第二次机会算法：对FIFO算法做如下简单的修改：发生替换时，先检查最老页面的R（访问）位。如果为0，
那么此页面是最早被换入的，而且近期没有被访问，可以立刻被替换掉；如果R位为1，就清除R位，并修改它的装入时间，
使它就像刚被装入的新页面一样，然后继续搜索可替换的最老页面。



  页面调度算法主要有：FIFO，最近最少使用调度算法（LRU），最近最不常用调度算法（LFU），最佳算法（OPT） 
这几种算法的调度都有可能在考试中碰到。 
关于这一类型，大家还可以参看书本251页的实验指导。 
如2001年考题： 
要求： 

1。实现三种算法： FIFO，最近最少使用调度算法（LRU），最近最不常用调度算法（LFU） 

2。页面序列从指定的文本文件（TXT文件）中取出 

3。输出： 

第一行：每次淘汰的页面号 

第二行：显示缺页的总次数 


   本程序包括:FIFO，最近最少使用调度算法（LRU），最近最不常用调度算法（LFU） 第二次机会算法

  */


#include<stdio.h>
#include<string.h>
#include<iostream.h>

const int MAXSIZE=1000;//定义最大页面数
const int MAXQUEUE=3;//定义页框数

typedef struct node{
int ym;
int number;
}page;

page pages[MAXQUEUE]; //定义页框表
int queue[MAXSIZE];
int t;
page z;

//初始化结构函数
void initial()
{
	int i;
	for(i=0;i<MAXQUEUE;i++)
	{
		pages[i].ym=-1;
		pages[i].number=0;
	}
	for(i=0;i<MAXSIZE;i++)
	{
		queue[i]=-1;
	}
	t=0;
}

//初始化页框函数
void init()
{
int i;

for(i=0;i<MAXQUEUE;i++){
  pages[i].ym=-1;
  pages[i].number=0;
}
}

//读入页面流
int readData()
{
	FILE *fp;
	char fname[20];
	int i;

	cout<<"请输入页面流文件名:";
	cin>>fname;

	if((fp=fopen(fname,"r"))==NULL)
	{
		cout<<"错误,文件打不开,请检查文件名"<<endl;
		return 0;
	}
	else
	{
		while(!feof(fp))
		{
			fscanf(fp,"%d ",&queue[t]);
			t++;
		}
	}

	cout<<"读入的页面流:";
	for(i=0;i<t;i++)
	{
		cout<<queue[i]<<"  ";
	}
	return 1;
}

//FIFO调度算法

void  FIFO()
{
	int a=0;
	for( int i=0 ; i<MAXQUEUE ; i++ )
	{
		pages[i].ym=queue[i];
		cout<<queue[i]<<"  ";
		a++;
	}
	for( i=MAXQUEUE ; i<t ;i++)
	{	int p=0;
		for( int j=0 ; (j<MAXQUEUE)&&(p==0) ; j++ )
		{
			if(queue[i]==pages[j].ym)
			{
				p=1;
			}
		}
		if(p==0)
		{
			cout<<queue[i]<<"   ";
			a++;
			for( int k=0 ; k<MAXQUEUE-1 ;k++)
			{
				pages[k].ym=pages[k+1].ym;
			}
			pages[k].ym=queue[i];
		}
	}
cout<<endl<<"总缺页数:"<<a<<endl;

}
//最近最少使用调度算法（LRU）

void  LRU()
{
	int a=0;
	for( int i=0 ; i<MAXQUEUE ; i++ )
	{
		pages[i].ym=queue[i];
		cout<<queue[i]<<"  ";
		a++;
	}
	for( i=MAXQUEUE ; i<t ;i++)
	{	int p=0;
		for( int j=0 ; (j<MAXQUEUE)&&(p==0) ; j++ )
		{
			if(queue[i]==pages[j].ym)
			{
				p=1;
				for( ; j< MAXQUEUE-1 ; j++ )
				{
					pages[j].ym=pages[j+1].ym;
				}
				pages[j].ym=queue[i];
			}
		}
		if(p==0)
		{
			cout<<queue[i]<<"   ";
			a++;
			for( int k=0 ; k<MAXQUEUE-1 ;k++)
			{
				pages[k].ym=pages[k+1].ym;
			}
			pages[k].ym=queue[i];
		}
	}
cout<<endl<<"总缺页数:"<<a<<endl;

} 
//最近最不常用调度算法(LFU)

void  LFU()
{
	int a=0;
	for( int i=0 ; i<MAXQUEUE ; i++ )
	{
		pages[i].ym=queue[i];
		cout<<queue[i]<<"  ";
		a++;
	}
	for( i=MAXQUEUE ; i<t ; i++)
	{	int p=0;
		for( int j=0 ; (j<MAXQUEUE)&&(p==0) ; j++ )
		{
			if(queue[i]==pages[j].ym)
			{
				p=1;
				pages[j].number++;
			}
		}
		if(p==0)
		{
			cout<<queue[i]<<"   ";
			a++;
			for( int q=0 ; q<MAXQUEUE-1 ;q++)
				for(int j=0 ; j<MAXQUEUE-q-1 ; j++ )
					if(pages[j].number>pages[j+1].number)
					{
						/*z.ym=pages[j].ym;
						z.number=pages[j].number;
						pages[j].ym=pages[j+1].ym;
						pages[j].number=pages[j+1].number;
						pages[j+1].ym=z.ym;
						pages[j+1].number=z.number*/			
						z=pages[j];
						pages[j]=pages[j+1];
						pages[j+1]=z;
					}
			for( int k=0 ; k<MAXQUEUE-1 ; k++)
			{
				pages[k].ym=pages[k+1].ym;
				pages[k].number=pages[k+1].number;
			}
			pages[k].ym=queue[i];
			pages[k].number=0;
		}
	}
cout<<endl<<"总缺页数:"<<a<<endl;

} 

//第二次机会算法

void second()
{	
	int a=0;
	for( int i=0 ; i<MAXQUEUE ; i++ )
	{
		pages[i].ym=queue[i];
		cout<<queue[i]<<"  ";
		a++;
	}
	for( i=MAXQUEUE ; i<t ;i++)
	{	int f=0;
		int p=0;
		for( int j=0 ; (j<MAXQUEUE)&&(p==0) ; j++ )
		{
			if(queue[i]==pages[j].ym)
			{
				p=1;
				pages[j].number=1;
			}
		}
		if(p==0)
		{
			cout<<queue[i]<<"   ";
			a++;
			for(int k=0 ; k<MAXQUEUE ;k++)
			{
				if(pages[k].number==0)
				{
					f=1;
					for( ; k<MAXQUEUE-1 ;k++)
					{
						pages[k].ym=pages[k+1].ym;
						pages[k].number=pages[k+1].number;
					}
					
				}
			}
			if(f==0)
			{
				for( k=0 ; k<MAXQUEUE ; k++)
				{
					pages[k].number=0;
				}
				for( int k=0 ; k<MAXQUEUE-1 ;k++)
				{
					pages[k].ym=pages[k+1].ym;
					pages[k].number=pages[k+1].number;
				}
			}
			k--;
			pages[k].ym=queue[i];
			pages[k].number=0;
		}
	}
cout<<endl<<"总缺页数:"<<a<<endl;

}

void main()
{
initial();
int c,m=1;

if(readData()){
while(m)
{
cout<<endl<<"--------------------------------------"<<endl;
cout<<"请选择要的算法："<<endl;
cout<<"1：先来先服务  2：LRU  3：LFU  4：第二次页面调度  5：退出"<<endl;
cout<<"--------------------------------------"<<endl;
cin>>c;
	switch(c)
	{
	case 1:
		init();
		FIFO();
		break;
	case 2:
		init();
		LRU();
		break;
	case 3:
		init();
		LFU();
		break;
	case 4:
		init();
		second();
		break;
	case 5:
		m=0;
		break;
	default:
		cout<<"选择错误"<<endl;
	}
	}
}
}


