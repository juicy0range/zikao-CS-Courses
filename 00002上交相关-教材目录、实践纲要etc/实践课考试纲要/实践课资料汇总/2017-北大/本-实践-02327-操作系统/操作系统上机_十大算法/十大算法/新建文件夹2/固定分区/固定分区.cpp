#include <iostream.h>
#include <string.h>
#include<stdlib.h>
const int M=5;

struct MEM
{
	int number;//区号
	int length;//分区大小
	int beginaddress;//分区始址
	int status;//分区状态 0 表示未占用，1 表示占用
	char jobname[20];//作业名
};

void allocate(MEM table[],char name[],int length)
{
	int i=0;  int flag=-1;  
	for(i=0;i<M;i++)
	{//找到合适的就跳出
		if(table[i].status==0&&table[i].length>=length) 
		{flag=i;break;}
	}
	if(flag==-1)
	{
		cout<<"\n没有找到满足需求的分区!\n";return;
	}
	strcpy(table[i].jobname,name);
	table[i].status=1;
	cout<<"已成功为作业"<<name<<" 分配"<<length<<"大小的内存空间n"<<endl;
	return;
}

void reclaim(MEM table[],char name[])
{
	int   i=0;
	while(i<M&&strcmp(table[i].jobname,name)!=0)
		i++;//不是所找的文件才继续
	if(i>=5)
	{
		cout<<"\n输入的作业在内存中没有找到!"<<endl;
		return;
	}
	table[i].status=0;
	strcpy(table[i].jobname,"##");
	cout<<"已经成功收回作业"<<name<<"占用的内存区域!"<<endl;
}
void main()
{
	char name[20];
	int length;
	int select=0;
	int i;
	MEM table[M]={  //初始化分区表，作业名为“##”表空
		{0,8,0,0,"##"},
		{1,16,10,0,"##"},
		{2,24,30,0,"##"},
		{3,56,60,0,"##"},
		{4,120,120,0,"##"}};  
		
		cout<<"浏览分配后的"<<endl;
		cout<<"分区号  长度 起始地址 分区状态 作业名"<<endl;
		for(i=0;i<M;i++)
		{
			cout<<"  "<<table[i].number<<"  "<<table[i].length<<" "<<table[i].beginaddress ;
			cout<<"  "<<table[i].status <<" "<<table[i].jobname <<endl;
		}
		while(1)
		{
			cout<<"请选择菜单: "<<endl;
			cout<<"0_退出 1_分配内存 2_回收内存 3_显示内存情况"<<endl;
			
			cin>>select;
			switch(select)
			{
			case 0: exit(0);
			case 1: 
				cout<<"输入作业名: ";
				cin>>name;
				cout<<"输入作业大小: ";
				cin>>length;
				allocate(table,name,length);
				break;
			case 2:
				cout<<"输入作业名: ";
				cin>>name;
				reclaim(table,name);
				break;
			case 3:
				cout<<"分区号  长度 起始地址 分区状态 作业名"<<endl;
				for(i=0;i<M;i++)
				{
					cout<<"  "<<table[i].number<<"  "<<table[i].length <<" "<<table[i].beginaddress ;
					cout<<"  "<<table[i].status <<" "<<table[i].jobname <<endl;
				}
				break;
			default:
				cout<<"输入错误，请重新输入!"<<endl;
				break;
			}
		}
}


