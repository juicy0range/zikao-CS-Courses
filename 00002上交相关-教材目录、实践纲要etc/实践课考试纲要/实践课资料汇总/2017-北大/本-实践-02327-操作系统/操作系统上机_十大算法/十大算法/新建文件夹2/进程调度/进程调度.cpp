#include <iostream.h> 
#include <stdlib.h>
const int pover=0,running=1,already=2,wating=3,timeblock=5;


struct pcb{char name;int status;
int time ;pcb* next;
int precendence;};

void FIFO(pcb *head)
{
	cout<<"*******FIFO*******"<<endl;
	pcb *p;	
	p=head;
	int flag=0;
	while(p!=NULL)
	{
		if(p->status==2) 
		{flag=1;break;}
		p=p->next;
	}
	if(flag==1)
	{
		cout<<"按顺序运行以下就绪进程:"<<endl;
		cout<<"进程名   时间  标志"<<endl;
		while(p!=NULL)
		{
			if(p->status==2)
			{
				cout<<p->name<<"  "<<p->time<<"  "<<"运行结束"<<endl; 
				p->status=pover;
			}
			p=p->next;
		}
	}
	p=head; 
    flag=0;
	while(p!=NULL)
	{
		if(p->status==3)
		{
			flag=1;break;
		}
		p=p->next;
	}
    p=head;
	if(flag==1)
	{
		cout<<"按顺序运行以下等待进程"<<endl;
		cout<<"进程名    时间  标志"<<endl;
		while(p!=NULL)
		{
			if(p->status==3)
			{
				cout<<p->name<<"  "<<p->time<<"  "<<"运行结束"<<endl; 
				p->status=pover;
			}
			p=p->next;
		}
	}
}



void RR(pcb* head)
{
	cout<<"******RR*****"<<endl;
	pcb *p;pcb* q;int flag=0;p=head;	
	while(p!=NULL)
	{
		if(p->status==2) 
		{flag=1;break;}
		p=p->next;
	}
	if(flag==1)
	{
		cout<<"按顺序运行以下就绪进程:"<<endl;
		cout<<"进程名     标志"<<endl;
		int	flag1=1;
		while(flag1==1)
		{
			flag1=0;p=head;
			while(p!=NULL)
			{
				if(p->time<=timeblock)
				{
					if(p->status==2)
					{
						cout<<p->name<<"        "<<"运行结束"<<endl; 
						p->status=pover;p->time=0;
					}
				}
				if(p->time>timeblock)
				{
					if(p->status==2)
					{
						cout<<p->name<<"        "<<"已运行一个时间片"<<endl; 
						p->time=p->time-timeblock;
					}
				}
				p=p->next ;
			}
			cout<<"时间片循环一次!"<<endl;
			q=head;
			while(q!=NULL)
			{
				if((q->status==2)&&(q->time>0)) 
				{
					flag1=1;
					break;
				}
				q=q->next ;
			}
		}
	}
	
	
	flag=0;p=head;
	while(p!=NULL)
	{
		if(p->status==3)
		{
			flag=1;
			break;
		}
		p=p->next;
	}
	if(flag==1)
	{
		cout<<"按顺序运行以下等待进程:"<<endl;
		cout<<"进程名     标志"<<endl;
		int	flag1=1;
		while(flag1==1)
		{
			flag1=0;p=head;
			while(p!=NULL)
			{
				if(p->time<=timeblock)
				{
					if(p->status==3)
					{
						cout<<p->name<<"        "<<"运行结束"<<endl; 
						p->status=pover;p->time=0;
					}
				}
				if(p->time>timeblock)
				{
					if(p->status==3)
					{
						cout<<p->name<<"        "<<"已运行一个时间片"<<endl; 
						p->time=p->time-timeblock;
					}
				}
				p=p->next ;
			}
			cout<<"时间片循环一次!"<<endl;
			q=head;
			while(q!=NULL)
			{
				if((q->status==3)&&(q->time>0)) 
				{
					flag1=1;break;
				}
				q=q->next ;
			}
		}
	}	
}

pcb* create(pcb* head)
{
	char name;
	pcb* p,*q=NULL;
	cout<<"创建进程的链式结构,当进程名为'@'时表示结束!"<<endl;
	cout<<"输入进程名:"<<endl;
	cin>>name;
	while(name!='@')
	{
		p=new pcb;
		p->name =name;
		cout<<"输入进程的状态, 2为就绪,3为等待:";
		cin>>p->status;
		cout<<"输入进程的运行时间:";
		cin>>p->time ;
		if(head==NULL) head=p;
		else q->next=p;
		q=p;
		cout<<"输入进程名:";
		cin>>name;
	}
	q->next=NULL;
	
	cout<<"你所输入的进程的信息为:"<<endl;
	cout<<" 名  状态  时间"<<endl;
	
	p=head;
	while(p!=NULL)
	{
		cout<<p->name<<"  ";
		if(p->status==2) cout<<"就绪";
		else cout<<"等待";
		cout<<"  "<<p->time <<endl;
		p=p->next ;
	}
	return head;
}



pcb *InsertCreate(pcb *head)
{
	pcb *p,*f;
	char name;
	if(head==NULL)
	{  head=new pcb;
	head->next=NULL;}
	cout<<"创建进程的链式结构,当进程名为'@'时表示结束!"<<endl;
	cout<<"输入进程名:"<<endl;
	cin>>name;
	while(name!='@')
	{ 
		p=new pcb;
		p->name=name;
		cout<<"输入进程的状态, 2为就绪,3为等待:";
		cin>>p->status;
		cout<<"输入进程的运行时间:";
		cin>>p->time ;
		cout<<"输入进程的优先级:";
		cin>>p->precendence;  
		f=head;  
		while((f->next)&&(p->precendence<=f->next->precendence))
			f=f->next;
		p->next=f->next;
		f->next=p;
		cout<<"输入进程名:";
		cin>>name;
	}
	
	cout<<"你所输入的进程的信息为:"<<endl;
	cout<<" 名  状态  优先级"<<endl;
	head=head->next;
	p=head;
	while(p!=NULL)
	{
		cout<<p->name<<"     ";
		if(p->status==2) cout<<"就绪"<<"  ";
		else cout<<"等待";
		cout<<"  "<<p->precendence <<endl;
		p=p->next ;
	}
	return head;
}



void main()
{
	pcb* head=NULL;	  
	
	cout<<"选择调度算法:   1 为 先进先出;  2为时间片轮转"<<endl;
	cout<<"3 为优先级(先进先出); "<<endl;
	int z;
	cin>>z;
	
	if(z==1)
	{
		head=create(head);
		FIFO(head) ;
	}
	if(z==2)
	{
		head=create(head);
		RR(head);
	}
	if(z==3)
	{
		head=InsertCreate(head);
		FIFO(head) ;
	}
	
}