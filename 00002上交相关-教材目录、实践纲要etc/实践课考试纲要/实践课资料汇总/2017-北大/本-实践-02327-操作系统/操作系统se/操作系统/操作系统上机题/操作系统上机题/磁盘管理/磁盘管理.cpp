// 磁盘管理.cpp : Defines the entry point for the console application.
//

#include<stdio.h>
#include<string.h>
#include<iostream.h>
const int MAXGROUP=10;//定义组的大小
const int MAXBLOCK=10;
const int MAXJOB=10;//定义一个作业最大能申请的块数
//结构体定义
typedef struct node{
	int quantity;
	int cell[MAXBLOCK];
	struct node *next;
}group;  

typedef struct node1{
	char name[20];
	int quantity;
	int cell[MAXJOB];  /*asdfcomputer*/
	struct node1 *next;
}job;

group *head,*b;
int total;

job *jhead;

//初始化组函数
void initial()
	{
		int i,j;
		group *q;
		q=head;
		group *p;
		job *w;
		w=new job;
		strcpy(w->name,"null");
		w->quantity=0;
		w->next=NULL;
		jhead=w;
	
	for( i=0;i<MAXGROUP;i++)
	{
		p=new group;
		p->quantity=0;
		p->next=NULL;
		q->next=p;
		q=q->next;

	}
	q=head;
	for(i=0;i<MAXGROUP && (q!=NULL);i++)
	{
		for(j=0;j<MAXBLOCK;j++)
		{
			q->cell[j]=i*MAXBLOCK+j;

		}
		q->quantity=MAXBLOCK;
		q=q->next;
	}
	total=MAXBLOCK*MAXGROUP;
	cout<<"总的空闲块数："<<total;

}

void bid()
{
	int i,j,num,m;
	char jobname[20];
	m=0;
	group *p;
	job *q=new job;
	//job *jhead1=jhead;
	p=head;
	
	cout<<"请输入作业的名字";
	cin>>jobname;

	cout<<"请输入作业所需要的块数:";
	cin>>num;
	if((num<=total)&&(num<=MAXJOB))
	{
		strcpy(q->name,jobname);
		total-=num;
		q->quantity=num;
		for(i=0;(i<MAXGROUP) && (m<num);i++)
		{
			for(j=0;(j<MAXBLOCK) && (m<num);j++)
			{
				if(p->cell[j]!=-1){
					p->cell[j]=-1;
					q->cell[m]=i*MAXBLOCK+j;
					m++;
					p->quantity--;
				}
				b=p;
			}
			p=p->next;
		}
		//b用于记录所分配的块数
		q->next=jhead->next;
		jhead->next=q;
	}
	else
	{
		cout<<"所需要的块数错误"<<endl;
	}
}
void finish()
{
	int f=0,m=0;
	char jobname[20];
	group *p;
	job *q,*s;
	p=head;
	s=jhead;
	q=jhead->next;
	cout<<"请输入作业的名字";
	cin>>jobname;
while((q!=NULL)&&(f==0))
{
	if(!strcmp(q->name,jobname))
		{	
		
			total+=q->quantity;
			for(int i=0;(i<MAXGROUP)&&(f==0);i++)
			{
				if(p->next==b)
				{
					f=1;
					for(int j=0;(j<MAXBLOCK) && (m<q->quantity);j++)
					{
						if(b->cell[j]==-1)
						{	
							b->cell[j]=q->cell[m];
							m++;
							b->quantity++;
						}
					}
						if(m<q->quantity)
						{		
							for( j=0;(j<MAXBLOCK) && (m<q->quantity);j++)
								if(p->cell[j]==-1)
								{
									p->cell[j]=q->cell[m];
									m++;
								
									p->quantity++;
									b=p;

								}
							/*	cout<<p->quantity;
						for( j=0;(j<MAXBLOCK);j++)
						{
							cout<<endl<<p->cell[j]<<" ";
						}*/

						}
						
				
					
				}
				p=p->next;
				
				}
			s->next=q->next;
			delete q;
		}
	cout<<endl;
	q=q->next;
	s=s->next;
}
	if(q==NULL)
	{
		cout<<"没有这个作业"<<endl;
	}

}
void view_pages()
{
job *p;
int i;

p=jhead->next;
if(p==NULL)cout<<"当前没有用户作业"<<endl;
else
  cout<<"当前所有的用户作业页表情况"<<endl;
while(p!=NULL){
  cout<<"作业名:"<<p->name<<"    所用块数:"<<p->quantity<<endl;
  cout<<"本作业所点块的序列是:"<<endl;
  for(i=0;i<p->quantity;i++){
   cout<<p->cell[i]<<"  ";
  }
cout<<endl;
p=p->next;
}
}
/*typedef struct node{
	int quantity;
	int cell[MAXBLOCK];
	struct node *next;
}group;*/
/*const int MAXGROUP=10;//定义组的大小
const int MAXBLOCK=100;
const int MAXJOB=100;//定义一个作业最大能申请的块数*/
void view()
{	
		group *p;   /*上面定义了group *head 和 *b */	            
		p=head;
		for(int i=0;(i<MAXGROUP)&&(p!=NULL);i++)
		{
			for(int j=0;j<MAXBLOCK;j++)
			{
				cout<<"  "<<p->cell[j];	
				
			}
			cout<<" "<<"有的空闲块数："<<p->quantity<<endl;
			p=p->next;
		}
		cout<<"总有的空闲块数"<<total;
}
void main()
{		

	head=new group;
	initial();
		int f=1;
		int chioce;
		while(f){
		  cout<<endl<<"===================================================="<<endl;
		  cout<<"           模拟UNIX的成组链接法    "<<endl;
		  cout<<"===================================================="<<endl;
		  cout<<"1.申请空闲块  2.撤消作业 3.查看专用块  4：查看作业 0.退出"<<endl;
		  cout<<"请选择:";
		  cin>>chioce;

		  switch(chioce)
		  {
		  case 1:
			   bid();
			   break;
		  case 2:
			   finish();
			   break;
		  case 3:
				view();
				break;
		  case 4:
				view_pages();
				break;
		  case 0:
				f=0;
				break;
		  default:
		   cout<<"选择错误!"<<endl;
		  }
			}

}
 

