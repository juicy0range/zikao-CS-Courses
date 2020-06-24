#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include <math.h>
const int M=20,MAX=200;
int FIFO(int cd[])
{//先来先服务
	cout<<"***********FIFO******"<<endl;
	int num=0,num1=0;
	int i=0;
	cout<<"输入当前柱面号："; cin>>cd[0];
	cout<<endl;
	cout<<"依次访问以下柱面："<<endl;
	for(i=1;i<M;i++)
	{
		if(cd[i]!=-1) 
		{cout<<cd[i]<<"  ";
		num1=abs(cd[i]-cd[i-1]);
		num=num+num1;
		}
		if(cd[i]==-1) break;
	}
	return  num;}

int ZDXZ(int cd[])
{//最短寻找时间
	cout<<"**********ZDXZ********"<<endl;
	int num=0,num1=0;int i=0,j=0;int flag[M],xs=MAX,sm=0,bj;
	int zx=0;
	cout<<"输入当前柱面号："; cin>>cd[0];
	cout<<endl;
	cout<<"依次访问以下柱面："<<endl;
	for(i=1;i<M;i++) {if(cd[i]!=-1) sm++;
	if(cd[i]==-1) break;}
	for(i=1;i<M;i++) flag[i]=0;
	for(i=0;i<sm;i++)
	{
		for(j=1;j<=sm;j++)
		{
			if(flag[j]==0)
			{
				bj=abs(cd[j]-cd[0]);
				if(bj<=xs)
				{xs=bj;zx=j;}
			}
		}
		num1=abs(cd[zx]-cd[0]);
		num=num+num1;
		cd[0]=cd[zx];
		cout<<cd[zx]<<"  ";
		flag[zx]=1;
		xs=MAX;
	}
	return num;
}


int DTDD(int cd[])
{//电梯调度
	cout<<"*********DTDD*******"<<endl;
	int min=MAX;int max=0;int num=0;
	int fx=0;int sm=0;int i=0;int j;
	for(i=1;i<M;i++)
	{
		if(cd[i]!=-1) sm++;
		if(cd[i]==-1) break;
	}
	for(i=1;i<sm;i++) 
	{
		if(cd[i]<=min) min=cd[i];
		if(cd[i]>=max) max=cd[i];
	}
	
	cout<<"输入当前柱面号："; cin>>cd[0];
	
	cout<<"输入磁头移动方向：";
	cout<<"1 为由外向内移动（正方向）,"<<endl;
	cout<<"2 为由内向外移动（负方向）。"<<endl;
	cin>>fx;
	cout<<"依次访问以下柱面："<<endl;
	if(fx==1) 
	{
		if(cd[0]<=min) 
		{
			num=max-cd[0];
			for(i=min;i<=max;i++)
				for(j=1;j<=sm;j++) {if(cd[j]==i) cout<<cd[j]<<"  "; 
				} 
		}
		if(cd[0]>min&&cd[0]<max)
		{
			num=2*(max-cd[0])+cd[0]-min;	
			for(i=cd[0];i<=max;i++)
				for(j=1;j<=sm;j++)
				{
					if(cd[j]==i) cout<<cd[j]<<"  ";
				}
				for(i=cd[0];i>=min;i--)
					for(j=1;j<=sm;j++) {if(cd[j]==i) cout<<cd[j]<<"  "; 
					}
		}
		if(cd[0]>=max) 
		{
			num=cd[0]-min;
			for(i=cd[0];i>=min;i--)
				for(j=1;j<=sm;j++) {if(cd[j]==i) cout<<cd[j]<<"  ";
				}
		}
	}
	
	if(fx==2) 
	{
		if(cd[0]>=max) 
		{
			num=cd[0]-min;
			for(i=max;i>=min;i--)
				for(j=1;j<=sm;j++)
				{
					if(cd[j]==i) cout<<cd[j]<<"  "; 
				}
		}
		if(cd[0]<max&&cd[0]>min)
		{
			num=2*(cd[0]-min)+max-cd[0];	
			for(i=cd[0];i>=min;i--)
				for(j=1;j<=sm;j++)
				{
					if(cd[j]==i) cout<<cd[j]<<"  "; 
				}
				for(i=cd[0];i<=max;i++)
					for(j=1;j<=sm;j++)
					{
						if(cd[j]==i) cout<<cd[j]<<"  "; 
					}
		}
		if(cd[0]<=min)
		{
			num=max-cd[0];
			for(i=cd[0];i<=max;i++)
				for(j=1;j<=sm;j++) {if(cd[j]==i) cout<<cd[j]<<" ";
				}
		}
	}
	return num;
}






void main()
{
	int cd[M];
	char name[20];int x;int i=1;
	cout<<"input the file name:"<<endl;cin>>name;
	ifstream f1;
	f1.open(name,ios::in|ios::nocreate);
	if(!f1) 
	{
		cout<<"the flie is not find";exit(1);
	}
	f1>>x;while(x!=-1) {cd[i]=x;i++;f1>>x;
	}
	cd[i]=-1;
	for(i=1;i<M;i++) 
	{
		if(cd[i]!=-1) cout<<cd[i]<<" ";
		if(cd[i]==-1) 
		{
			cout<<-1;break;
		}
	}
	cout<<endl;
	int num;
	num=FIFO(cd);
	cout<<" 磁头移动数量："<<num<<endl;
	num=ZDXZ(cd);
	cout<<" 磁头移动数量："<<num<<endl;
	num=DTDD(cd);
	cout<<" 磁头移动数量："<<num<<endl;
	
}
