//磁盘调度算法

/*

磁盘调度算法 
要求： 
1。实现三种算法：
1。先来先服务 
2。最短寻道优先（老师会给当前磁头的位置） 
3。电梯算法 
2。磁道服务顺序从指定的文本文件（TXT文件）中取出 
3。输出： 
第一行：磁道的服务顺序 
第二行：显示移动总道数 
本程序包括:FIFO，最短寻道优先调度算法，电梯算法
*/

#include<stdio.h>
#include<iostream.h>
#include<string.h>
#include<math.h>

const int MAX=200;  //定义队列最大数

//结构体定义
typedef struct node{
int texe;
int t ;
}qu;

qu queue[MAX],copy[MAX];
int q=0,p=0;

int start;  //定义开始时磁头所在位置

//初始化函数
void initial()
{
 	for(int i=0;i<MAX;i++)
 	{
  		queue[i].texe=-1;
  		queue[i].t=0;
	}

}
void init()
{for(int i=0;i<q;i++)
{
	copy[i]=queue[i];

}
p=q;
}
//读入磁道号流
int readData()
{
FILE *fp;
char fname[20];
int temp;
cout<<"请输入磁道号流文件名:";
cin>>fname;

if((fp=fopen(fname,"r"))==NULL){
  cout<<"错误,文件打不开,请检查文件名"<<endl;
  return 0;
}
else{
  while(!feof(fp)){
   fscanf(fp,"%d ",&temp);
   queue[q].texe=temp;
   q++;
  }
  cout<<endl<<"---------------------------------------------"<<endl;
  cout<<"所读入的磁道号流:";
  for(int i=0;i<q;i++){
   cout<<queue[i].texe<<"  ";
  }
  cout<<endl<<"请求数为:"<<q<<endl;
}
return 1;
}

//FIFO算法
void FIFO()
{
int i;
int total=0;
int current;

cout<<endl<<"---------------------------------------------"<<endl;
cout<<"FIFO算法的访问磁道号顺序流:";

current=start;
for(i=0;i<q;i++){
  cout<<queue[i].texe<<"  ";
  total+=abs(queue[i].texe-current);
  current=queue[i].texe;
}
cout<<endl<<"磁头移过的柱面数:"<<total;
}
//最短寻道优先调度算法
void shortest()
{
	int total=0;
	qu n;
	int m;
	int current=start;
	for(int i=0;i<q;i++)
	{
		for(int j=0;j<p;j++)
		{
			copy[j].t=abs(copy[j].texe-current);
		}
		n=copy[0];
		m=0;
		for(int k=0;k<p-1;k++)
		{
			if(n.t>copy[k+1].t)
			{
				n=copy[k+1];
				m=k+1;
			}
		}
		cout<<n.texe<<"  ";
		total+=n.t;
		current=n.texe;;
		for(j=m;j<p-1;j++)
		{
			copy[j]=copy[j+1];
		}
		p--;
	}
	

cout<<endl<<"磁头移过的柱面数:"<<total;
}

//电梯算法
void elevator()
{
	int total=0;
	int current=start;
	int f=0;
	qu s;
	cout<<endl<<"---------------------------------------------"<<endl;
	cout<<"电梯调度算法"<<endl;
	for(int i=0;i<p;i++)
		for(int j=0;j<p-i-1;j++)
		{
			if(copy[j].texe>copy[j+1].texe)
			{
				s=copy[j];
				copy[j]=copy[j+1];
				copy[j+1]=s;
			}
		}
		

	for(i=0;(i<p)&&(f==0);i++)
	{
		if(copy[i].texe>=current)
		{
			f=1;
		}
	}
	i--;
	//磁头初始向里
	cout<<"磁头初始向里的访问磁道号顺序流:";
	int k=i;
	for(i;i<p;i++)
	{
		cout<<" "<<copy[i].texe<<"  ";
		total+=abs(copy[i].texe-current);
		current=copy[i].texe;
	}
	i=k;
	i--;
	for(i;i>=0;i--)
	{
		cout<<" "<<copy[i].texe<<"  ";
		total+=abs(copy[i].texe-current);
		current=copy[i].texe;
	}
cout<<endl<<"磁头移过的柱面数:"<<total<<endl;

	//磁头初始向外
	total=0;
	cout<<"磁头初始向外的访问磁道号顺序流:";
	i=k;
	i--;
	for(i;i>=0;i--)
	{
		cout<<" "<<copy[i].texe<<"  ";
		total+=abs(copy[i].texe-current);
		current=copy[i].texe;
	}
	i=k;
	for(i;i<p;i++)
	{
		cout<<" "<<copy[i].texe<<"  ";
		total+=abs(copy[i].texe-current);
		current=copy[i].texe;
	}
	
cout<<endl<<"磁头移过的柱面数:"<<total<<endl;

/*int i,j,p,flag;
int total=0;
int current;

cout<<endl<<"---------------------------------------------"<<endl;
cout<<"电梯调度算法"<<endl;

//磁头初始向里
cout<<"磁头初始向里的访问磁道号顺序流:";

current=start;
for(i=0;i<q;i++){
  flag=1000;
  p=-1;
  for(j=0;j<q;j++){
   if((queue[j].t==0)&&(queue[j].texe>=current)){
    if(abs(queue[j].texe-current)<flag){
     p=j;
     flag=abs(queue[j].texe-current);
    }
   }
  }
  if(p!=-1){
    cout<<queue[p].texe<<"  ";
    total+=abs(queue[p].texe-current);
    current=queue[p].texe;
    queue[p].t=1;
  }
  else{
   for(j=0;j<q;j++){
    if((queue[j].t==0)&&(queue[j].texe<current)){
     if(abs(queue[j].texe-current)<flag){
      p=j;
      flag=abs(queue[j].texe-current);
     }
    }
   }
   cout<<queue[p].texe<<"  ";
   total+=abs(queue[p].texe-current);
   current=queue[p].texe;
   queue[p].t=1;
  }
}
cout<<endl<<"磁头移过的柱面数:"<<total<<endl;

//磁头初始向外
for(i=0;i<q;i++){
  queue[i].t=0;
}
total=0;

cout<<"磁头初始向外的访问磁道号顺序流:";

current=start;
for(i=0;i<q;i++){
  flag=1000;
  p=-1;
  for(j=0;j<q;j++){
   if((queue[j].t==0)&&(queue[j].texe<=current)){
    if(abs(queue[j].texe-current)<flag){
     p=j;
     flag=abs(queue[j].texe-current);
    }
   }
  }
  if(p!=-1){
    cout<<queue[p].texe<<"  ";
    total+=abs(queue[p].texe-current);
    current=queue[p].texe;
    queue[p].t=1;
  }
  else{
   for(j=0;j<q;j++){
    if((queue[j].t==0)&&(queue[j].texe>current)){
     if(abs(queue[j].texe-current)<flag){
      p=j;
      flag=abs(queue[j].texe-current);
     }
    }
   }
   cout<<queue[p].texe<<"  ";
   total+=abs(queue[p].texe-current);
   current=queue[p].texe;
   queue[p].t=1;
  }
}
cout<<endl<<"磁头移过的柱面数:"<<total;

*/}


void main()
{
initial();
	if(readData())
	{cout<<"请输入磁头的起始位置："<<endl;
	cin>>start;
	int c,f=1;
	while(f)
	{
	cout<<endl<<"-------------------------"<<endl;
	cout<<" 0：退出   1：FIFO     2：最短寻道优先调度算法    3：电梯算法 "<<endl;
	cout<<"-------------------------"<<endl;
	cout<<"请选择："<<endl;
	cin>>c;
	switch(c)
		{case 1:
			init();
			FIFO();
			break;
		case 2:
			init();
			shortest();
			break;
		case 3:
			init();
			elevator();
			break;
		case 0:
			f=0;
			break;
		default:
			cout<<"选择错误!"<<endl;
		}

	}
	cout<<"欢迎使用本系统"<<endl;
	}
}