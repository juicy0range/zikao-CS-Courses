/*页式存储管理方案算法

页式存储管理方案

  位示图

  2002（？）的北京大学主考的上机试题

内存被划分成2048块（页）。用32位字长的字存放位示图，为0的位表示该块尚未分配，为1的位表示该块已分配?
实习检查： 

1、运行程序，由检查教师给出文件名，该文件中存有内存目前状况的位示图的数据（0和1的文件）。（程序应做提示，界面友好）。 
2、你所编制的程序应读入数据，存放在相应的数据结构中。 
3、显示友好的用户界面，由检查教师输入内存申请（总块数）。 
4、根据申请和位示图状态，为用户分配内存，并建立页表。 
5、输出位示图和页表。

  
本程序包括主要实现位示图算法,比较简单

 
*/

#include<stdio.h>
#include<iostream.h>
#include<string.h>

const int PAGES=256;   //定义总块数
const int WORD=32;      //定义字长

const int WORDNUM=PAGES/WORD;   //定义总字数

typedef struct node{
char jobname[20];
int num;
int nums[PAGES];
struct node *next;
}jobs;

int table[WORDNUM][WORD];
int freenum=0;
jobs *head;

//初始化函数
void initial()
{
	jobs *p;

	//初始化位示图
	for( int i=0 ; i<WORDNUM ; i++ )
   for( int j=0 ;  j<WORD; j++ )
   {
    table[i][j]=0;
   }


	//初始化作业表头
	p=new jobs;
	strcpy(p->jobname,"null");
	p->num=0;
	p->next=NULL;
	head=p;


}

//读入位示图初始数据
int readData()
{
	FILE *fp;
	char fname[20];
	int i,j;

	cout<<"请输入初始位示图数据文件名:"<<endl;
		cin>>fname;

	if((fp=fopen(fname,"r"))!=NULL){
		for( i=0;i<WORDNUM;i++)
			for( j=0;j<WORD;j++)
			{
				fscanf(fp,"%d",&table[i][j]);
				if(table[i][j]==0)
				freenum++;
			}
 
		cout<<"初始位示图"<<endl;
		for(i=0;i<WORDNUM;i++)
		{
			for(j=0;j<WORD;j++)
			{
				cout<<table[i][j]<<" ";
			}
			cout<<endl;
		}
	cout<<"总空闲块数:"<<freenum;
  
	return 1;
	}
else
{
  cout<<"文件不能打开"<<endl;
 return 0;
}

}

//新加入作业函数
void add()
{
	char jobname[20];
	int num,f=0;
	jobs *p;
	cout<<"请输入新增的作业名:";
	cin>>jobname;
	cout<<"新增作业所需页数:";
	cin>>num;
	if(num<=freenum)
	{
		freenum-=num;
		p=new jobs;
		strcpy(p->jobname,jobname);
		p->num=num;
		int k=0;
  
		for(int i=0;(i<WORDNUM)&&(f==0);i++)
			for(int j=0;(j<WORD)&&(f==0);j++)
			{
				if(table[i][j]==0)
				{
					p->nums[k]=i*WORD+j;
					table[i][j]=1;
					k++;
					num--;
				}
				
				if(num<=0)
				{
					f=1;
				}
			}
	
		p->next=head->next;
		head->next=p;
   
	}
	else
	{
		cout<<"错误,当前剩余页数小于所需页数,请稍候再试:)"<<endl;
	}

}

//完成作业函数
/*void finish()
{
	char jobname[20];
	jobs *p,*q;
	int n,i,j;
	p=head->next;
	if(p==NULL)
	{
		cout<<"这里没有作业"<<endl;
	//	return;
	}
	cout<<"请输入完成的作业名:";
	cin>>jobname;


	while(p!=NULL)
	{
		if(strcmp(p->jobname,jobname))
		{
			freenum+=p->num;
			for(int k=0;k<p->num;k++)
			{
				n=p->nums[k];
				i=n/WORD;
				j=n%WORD;
				table[i][j]=0;
			}
			q->next=p->next;
			delete p;

		}
		else 
		{	q=p;
			p=p->next;
		}
	}


}*/


//显示当前位示图函数
void view_table()
{
int i,j;
cout<<"当前位示图"<<endl;
  for(i=0;i<WORDNUM;i++){
   for(j=0;j<WORD;j++){
    cout<<table[i][j]<<" ";
   }
   cout<<endl;
  }
  cout<<"总空闲块数:"<<freenum<<endl;
}

//显示所有页表函数
void view_pages()
{
	jobs *p;
	int i;

	p=head->next;
	if(p==NULL)
		cout<<"当前没有用户作业"<<endl;
	else
		{
			cout<<"当前所有的用户作业页表情况"<<endl;
			while(p!=NULL)
			{
				cout<<"作业名:"<<p->jobname<<"    所用块数:"<<p->num<<endl;
				cout<<"本作业所点块的序列是:"<<endl;
				for(i=0;i<p->num;i++)
				{
					cout<<p->nums[i]<<"  ";
				}
			cout<<endl;
			p=p->next;
		}
	}
}


void main()
{
int t=1,chioce;
initial();
if(readData())
{
	while(t==1)
	{
		cout<<endl<<"==========================================="<<endl;
		cout<<"            页式内存管理系统模拟程序"<<endl;
		cout<<"==========================================="<<endl;
		cout<<"1.加入新作业    2.完成作业    3.显示当前内存位示图    4.显示所有作业页表  0.退出"<<endl;
		cout<<"请选择:";
		cin>>chioce;

		switch(chioce)
		{
			case 1:
				add();
				break;
			case 2:
				{
	char jobname[20];
	jobs *p,*q;
	int n,i,j;
	p=head->next;
	q=head;
	if(p==NULL)
	{
		cout<<"这里没有作业"<<endl;
	//	return;
	}
	cout<<"请输入完成的作业名:";
	cin>>jobname;


	while(p!=NULL)
	{
		if( !strcmp(p->jobname,jobname))
		{
			freenum+=p->num;
			for(int k=0;k<p->num;k++)
			{
				n=p->nums[k];
				i=n/WORD;
				j=n%WORD;
				table[i][j]=0;
			}
			q->next=p->next;
			delete p;
			break;

		}
		else 
		{	q=p;
			p=p->next;
		}
	}


}
				break;
			case 3:
				view_table();
				break;
			case 4:
				view_pages();
				break;
			case 0:
				t=0;
				break;
			default:
				cout<<"选择错误"<<endl;
		}
	}
}
}

