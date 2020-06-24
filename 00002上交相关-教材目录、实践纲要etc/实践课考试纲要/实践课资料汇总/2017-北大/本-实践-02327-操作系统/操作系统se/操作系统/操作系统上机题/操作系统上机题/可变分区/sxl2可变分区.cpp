//可变分区存储管理方案中的内存分配算法全实现

/*可变分区存储管理方案中的内存分配

  可变分区调度算法有:最先适应分配算法,最优适应分配算法,最坏适应算法
用户提出内存空间的申请；系统根据申请者的要求，按照一定的分配策略分析内存空间的使用情况，找出能
满足请求的空闲区，分给申请者；当程序执行完毕或主动归还内存资源时，系统要收回它所占用的内存空间
或它归还的部分内存空间。

1．程序运行时首先接收输入：空闲区数据文件，包括若干行，每行有两个数据项：起始地址、长度（均为
整数），各数据项以逗号隔开。

2．建立空闲区表并在屏幕上显示输出空闲区表内容，空闲区表中记录了内存中可供分配的空闲区的始址和
长度，用标志位指出该分区是否是未分配的空闲区。

3．从用户界面根据用户提示接收一个内存申请，格式为：作业名、申请空间的大小。

4．按照最差（最坏）适配算法选择一个空闲区，分割并分配，修改相应的数据结构（空闲区表），填写内
存已分配区表（起始地址、长度、标志位），其中标志位的一个作用是指出该区域分配给哪个作业。

5．重复3、4，直到输入为特殊字符（0）。

6．在屏幕上显示输出新的空闲区表和已分配区表的内容。

本程序包括:FIFO,最优适应分配算法,最坏适应算法

*/

#include<stdio.h>
#include<iostream.h>
#include<string.h>
#include<iomanip.h>

const int MAX=100;//定义表最大记录数

typedef struct node{
int start;
int length;
char jobname[20];
}job;

job frees[MAX];//定义空闲区表
int freet;

job occupys[MAX];//定义已分配区表
int occupyt;

job copy[MAX];
job s;
//初始化函数
void initial()
{
int i;

for(i=0;i<MAX;i++){
  frees[i].start=-1;
  frees[i].length=0;
  strcpy(frees[i].jobname,"free");

  occupys[i].start=-1;
  occupys[i].length=0;
  strcpy(occupys[i].jobname,"");
}
freet=0;
occupyt=0;
}

void init()
{
	for( int i=0 ; i<freet ;i++)
	{
		copy[i].start=frees[i].start;
		copy[i].length=frees[i].length;
		strcpy(copy[i].jobname,frees[i].jobname);
	}
}
//读数据函数
int readData()
{
FILE *fp;
char fname[20];

cout<<"请输入初始空闲表文件名:";
cin>>fname;

if((fp=fopen(fname,"r"))==NULL){
  cout<<"错误,文件打不开,请检查文件名"<<endl;
}
else{
  while(!feof(fp))
  {
   fscanf(fp,"%d,%d",&frees[freet].start,&frees[freet].length);
   freet++;
  }
	freet--;
  return 1;
	}
return 0;
}

//FIFO
void sortaddress()
{
  int i,j;
  for(i=0;i<freet;i++)
	{
		for(j=0;j<freet-i-1;j++)
		{
			if(copy[j].start>copy[j+1].start)
			{
				s=copy[j];
				copy[j]=copy[j+1];
				copy[j+1]=s;
			}
		}
		
		
	}
}
//最优适应分配算法

void sortadlength()
{
  int i,j;
  for(i=0;i<freet;i++)
	{
		for(j=0;j<freet-i-1;j++)
		{
			if(copy[j].length>copy[j+1].length)
			{
				s=copy[j];
				copy[j]=copy[j+1];
				copy[j+1]=s;
	
			}
		}
		
		
	}
}
//最坏适应算法
void sortadmax()
{
  int i,j;
  for(i=0;i<freet;i++)
	{
		for(j=0;j<freet-i-1;j++)
		{
			if(copy[j].length<copy[j+1].length)
			{
				s=copy[j];
				copy[j]=copy[j+1];
				copy[j+1]=s;
	
			}
		}
		
		
	}
}
//显示函数
void view()
{
	int i;
	cout<<endl<<"----------------------------------------------------------"<<endl;
	cout<<"当前空闲表:"<<endl;
	cout<<"起始地址    长度      状态"<<endl;
	for(i=0;i<freet;i++)
	{
		cout<<"  "<<copy[i].start<<"          "<<copy[i].length<<"       "<<copy[i].jobname<<endl;
	}

	cout<<endl<<"----------------------------------------------------------"<<endl;
	cout<<"当前已分配表:"<<endl;
	cout<<"起始地址    长度   占用作业名"<<endl;
	for(i=0;i<occupyt;i++)
	{
		cout<<"  "<<occupys[i].start<<"          "<<occupys[i].length<<"         "<<occupys[i].jobname<<endl;
	}
}

//算法
void suanfa()
{
  char jobname[20];
  int joblength;
  cout<<"请输入新申请内存空间的作业名";
  cin>>jobname;
  cout<<"请输入新申请内存空间大小:";
  cin>>joblength;
  int flag=0;
  for(int i=0;i<freet&&flag==0;i++)
  {
	if(copy[i].length>=joblength)
	{
		flag=1;
	}
  }
	if(flag==0)
	{
		cout<<endl<<"当前没有能满足你申请长度的空闲内存,请稍候再试"<<endl;
	}
    else
	{
		int t=0;
        i--;
		occupys[occupyt].start=copy[i].start;
		strcpy(occupys[occupyt].jobname,jobname);
		occupys[occupyt].length=joblength;
		occupyt++;
		if(copy[i].length!=joblength)
		{
			copy[i].start+=joblength;
			copy[i].length-=joblength;
		}
		else
		{
			for(int j=i;j<freet-1;j++)
			{
				copy[j]=copy[j+1];
			}
			freet--;
			cout<<"内存空间成功"<<endl;
		 }
	}
}


//撤消作业   
void finished()
{   int start,length,f=0;
	char jobname[20];
	cout<<"请输入要撤消的作业名:";
	cin>>jobname;
	int flag=-1;
	for( int i=0;i<occupyt;i++)
	{
		if(!strcmp(occupys[i].jobname,jobname))
		{
			flag=i;
			start=occupys[i].start;
			length=occupys[i].length;
		}
	}
	if(flag==-1)
	{
		cout<<"没有这个作业名"<<endl;
	}
	else
	{
		for( i=0; (i<freet)&&(f==0); i++)
		{
			if((copy[i].start+copy[i].length==start)&&(copy[i+1].start==start+length)&&((i+1) < freet ))
			{
				copy[i].length+=(length+copy[i+1].length);
				i++;
				for( ; i<freet-1;i++)
				{
					copy[i]=copy[i+1];
				}
				freet--;
				f=1;
			}
			else if(copy[i].start+copy[i].length==start)
			{
				copy[i].length+=length;
				f=1;
			}
			else if((copy[i].start==start+length))
			{
				copy[i].start=start;
				copy[i].length+=length;
				f=1;
			}
			
		}
		if(i>=freet)
		{
			copy[freet].start=start;
			copy[freet].length=length;
			freet++;
		}
  //加入空闲表
		/*for(i=0;i<freet;i++)
		{
			if((copy[i].start+copy[i].length)==start)
			{
				if( ( (i+1) < freet )&&(copy[i+1].start==start+length))
				{
					frees[i].length=frees[i].length+frees[i+1].length+length;
					for(int j=i+1;j<freet;j++)
					{
						copy[j]=copy[j+1];
					}
					freet--;
					p=1;
				}
				else
				{
				copy[i].length+=length;
				 p=1;
				}
			}
			if(copy[i].start==(start+length))
			{
				copy[i].start=start;
				copy[i].length+=length;
				p=1;
			}
			}

		if(p==0)
		{
			copy[freet].start=start;
			copy[freet].length=length;
			freet++;
		}*/



  //删除分配表中的该作业

		for(i=flag;i<occupyt-1;i++)
		{
			occupys[i]=occupys[i+1];
		}
			occupyt--;
	}
}

void main()
{
	int flag=1;
 int chioce=0;
 initial();
 flag=readData();
init();
 while(flag==1){
  cout<<endl<<endl<<"========================================================="<<endl;
  cout<<"              可变分区存储管理模拟系统"<<endl;
  cout<<"========================================================="<<endl;
  cout<<"   1.申请空间   2.撤消作业   3.显示空闲表和分配表  0.退出"<<endl;
  cout<<"请选择:";
  cin>>chioce;
  switch(chioce){
  case 1:
	  {
    cout<<endl<<endl<<"========================================"<<endl;
	cout<<"请选择一个算法"<<endl;
	cout<<"   1.先来先服务   2.最优算法   3.最坏算法  0.退出"<<endl;
	cout<<"请选择:";
	int i;
	cin>>i;
    switch(i)
	{
    case 1:
       sortaddress();
		suanfa();
		break;
	case 2:
		sortadlength();
		suanfa();
		break;
    case 3:
        sortadmax();
        suanfa();
		break;
    case 0:
		flag=0;
		break;
	default:
		cout<<"选择错误!请从新输入"<<endl;
	}
	  }
   break;
  case 2:
   finished();
   break;
  case 3:
   view();
   break;
  case 0:
   flag=0;
   break;
  default:
   cout<<"选择错误!请从新输入"<<endl;
  }
}


}

