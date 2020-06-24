#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
const int M=3;
struct  JCZY
{
	char jc;
	char zy;
};
void main()
{
	char filename[20];
	ifstream f1,f2;
	int flag=0;
	JCZY zy[M];
	JCZY xy[M];
	int b[M][M];int ss[M];
	int i=0;int j=0;int k;
	cout<<"请输入资源分配表文件的文件名:"<<endl;
	cin>>filename;
	f1.open(filename,ios::in|ios::nocreate);
	if(!f1) 
	{
		cout<<"不能找到指定文件！"<<endl;exit(1);
	}
	for(i=0;i<M;i++)
	{
		f1>>zy[i].zy;
		f1>>zy[i].jc;
	}
	cout<<"资源分配表文件的内容为："<<endl;
	for(i=0;i<M;i++) 
	{
		cout<<zy[i].zy<<"  "<<zy[i].jc<<endl;
	}
	f1.close ();
	
	cout<<"请输入进程等待表文件的文件名:"<<endl;cin>>filename;
	f2.open(filename,ios::in|ios::nocreate);
	if(!f2)
	{
		cout<<"不能找到指定文件！"<<endl;exit(1);
	}
	for(i=0;i<M;i++)
	{
		f2>>xy[i].jc;f2>>xy[i].zy;
	}
	cout<<"资源分配表文件的内容为："<<endl;
	for(i=0;i<M;i++)
	{
		cout<<xy[i].jc<<"  "<<xy[i].zy<<endl;
	}
	f2.close ();
	
	for(i=0;i<M;i++) 
	{
		for(j=0;j<M;j++)
		{
			if(xy[i].zy ==zy[j].zy )
				b[i][j]=1;
			else
				b[i][j]=0;
		}
	}	
	for(k=0;k<M;k++)
	{
		for(i=0;i<M;i++)
		{
			for(j=0;j<M;j++)
			{
				b[i][j]=b[i][j]||(b[i][k]&&b[k][j]);
			}
		}
	}
	flag=0;
	for(i=0;i<M;i++)
	{
		if(b[i][i]==1) { flag=1;break;}
	}
	for(i=0;i<M;i++) 
	{
		ss[i]=0;
	}
	if(flag==0) cout<<"系统不中存在死锁!"<<endl;
	if(flag==1)
	{
		cout<<"系统中存在死锁! "<<endl;
		cout<<"进程循环等待队列:"<<endl;
		for(i=0;i<M;i++)
			if(b[i][i]==1&&ss[i]==0) 
			{
				int t=1;
				int p=i;
				while(t)
				{
					cout<<zy[p].jc<<"->";
					for(j=0;j<M;j++)
					{
						
						if(zy[p].zy==xy[j].zy)
						{
							ss[j]=1;
							p=j; break;
						}
					}
					if(p==i) t=0;
				}
				cout<<zy[i].zy<<endl;
			}
	}
}
