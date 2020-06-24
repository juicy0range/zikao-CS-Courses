/* 6月4号下午题~~~
模拟第二次机会页面置换算法
1． 输入：
页面流文件，其中存储的是一系列页面号（页面号用整数表示，用空格作为分隔符），用来模拟待换入的页面。
下面是一个示意：
1 2 3 4 1 2 5 1 2 3 4 5
2． 处理要求：
程序运行时，首先提示“请输入页面流文件的文件名：”，输入一个文件名后，程序将读入该文件中的有关数据。
初始条件：采用三个页框，初始时均为空。
根据第二次机会算法对数据进行处理。
3． 输出要求：
每换入一个页面（即：每读入一个页面号），判断是否有页面需要被换出。若有，把被换出的页面号输出到屏幕上；若没有，则输出一个“*”号。
4． 文件名约定
提交的源程序名字：sourceXXX.c或者sourceXXX.cpp（依据所用语言确定）
输入文件名字：可由用户指定
其中：XXX为账号。
5． 测试说明：测试教师将事先准备好一组文件（格式为*.txt），从中为每个程序随机指定一至三个
作为输入文件（被测试者需从键盘输入指定文件的文件名），并查看程序输出结果。
6． 第二次机会算法：对FIFO算法做如下简单的修改：发生替换时，先检查最老页面的R（访问）位。
如果为0，那么此页面是最早被换入的，而且近期没有被访问，可以立刻被替换掉；如果R位为1，
就清除R位，并修改它的装入时间，使它就像刚被装入的新页面一样，然后继续搜索可替换的最老页面。
****************************************************************************************/
/*writer:何良坤 傅仁军(仅供参考)如你有好的看法和建议,欢迎与本人探讨.furenjun@sohu.com*/
# include <iostream.h>
# include <stdio.h>
# include <stdlib.h>
# include <iomanip.h>
static int a[100];
int  readfile()
{
	char filename[20];
	int i,j,m;
	i=0;j=0;
	cout<<"请输入文件名(test1.txt):>";
	cin>>filename;
    FILE *fp;
	if((fp=fopen(filename,"r+"))==NULL)
	{
		cout<<"sorry,file can not open!"<<endl;
		exit(0);
	}
	while(!feof(fp))
	{
		fscanf(fp,"%d",&m);
		a[i++]=m;
	}
	cout<<"刚才读入的数据个数为:"<<i--<<endl;
	for(j=0;j<=i;j++)
	{
		cout<<setw(5)<<a[j];
	//	if((j+1)%5==0)
	//		cout<<endl;
	}
	cout<<endl;
	return i;
}
typedef struct node *pointer;
struct node 
{
	int data;
	int r;
	pointer next;
};
typedef pointer lklist;

 lklist init_lklist ()
 {
	 lklist head;
	 pointer p,q;
	 head=new node;
	 head->next=NULL;
	 p=head;
	for(int i=0;i<3;i++)
	 {
		 q=new node;
		 q->data=65535;
		 q->r=0;
		 q->next =NULL;
		 p->next=q;
		 p=q;
	 }
	 q->next=NULL;
	 return head;
 }
 
lklist show(lklist head)
{
	pointer p;
	p=head->next ;
	int n=0;
	while(p!=NULL)
	{
		if((p->data)==65535)
			cout<<setw(3)<<n;
		else
		cout<<setw(3)<<p->data;
		cout<<"("<<p->r<<")";
		p=p->next;
	}
	cout<<endl;
	return  head;
}

int   find_lklist (lklist head,int x) //查找新进入的页面在内存中是否已经存在.
{
	pointer p,q;
	int flag=0;
	p=head ;
    q=p->next;
    while(q !=NULL)
	{
		if(q->data==x)
		{
            (q->r)+=1;
			flag=1;
			cout<<setw(3)<<"*";  
            return 1;
		}
		    q=q->next;
	}
	if(flag==0)
	return 0;
	else
	return 1;
}

lklist update_lklist(lklist head,int x )  //第二次机会算法,更新页面.
{
	pointer p,q,w,j;
		p=head;
		int flag=0;
		while(p->next !=NULL)
		{       q=p->next ; 
				if(q->r>=1)
				{
					w=new node;
					w->data=q->data;
					w->r=0;
					w->next=NULL;
					p->next=q->next;
						delete q;
					j=head;
                    while(j->next!=NULL)
					j=j->next;
                               //删除时不能再w->next=j->next;(不是中间)
				    j->next=w;
				}
             if(q->r==0)
			{
				if(q->data ==65535)
					cout<<setw(6)<<" NULL ";   
				else
				cout<<setw(3)<<q->data;
				w=new node;
				w->data=x;
				w->r=0;
		        w->next=NULL;
				p->next=q->next;
                delete q;
				j=head;
                while(j->next!=NULL)
					j=j->next;
                 j->next=w;
				 flag=1;
				 break;
			}
		}
return head;
}



lklist fifo_lklist(lklist head,int x )
{
	        pointer p,q,w,j;
        	p=head;
			q=p->next ;
        	int flag=0;
				if(q->data ==65535)
					cout<<" NULL ";
				else
				cout<<setw(3)<<q->data;
				w=new node;
				w->data=x;
				w->r=0;
		        w->next=NULL;
				p->next=q->next;
                delete q;
				j=head;
                while(j->next!=NULL)
					j=j->next;
                 j->next=w;
		  return head;
}


int   LRU_find (lklist head,int x) //查找新进入的页面在内存中是否已经存在.
{
	pointer p,q;
	int flag=0;
    p=head ;  
    q=p->next;
    while(q !=NULL)
	{
		if(q->data==x)
		{
			flag=1;
			cout<<setw(3)<<"*";  //如果存在则不产生缺页中断,输出"*".// 并删除此元素
			p->next=q->next;
			delete q;
			p=head;
	        while(p->next!=NULL)
		    p=p->next ;
	        w=new node;
	        w->data =x;
	        w->r=0;
	        w->next =NULL;
	        p->next =w;
            return 1;
		}
		    p=p->next;
		    q=q->next;
	}
	if(flag==0)
	return 0;
	else
	return 1;
}

int LRU_notfind(lklist head)  //如果没找到则输出队头元素
{
	pointer p;
	p=head->next ;
	if(p->data ==65535)
		cout<<" NULL ";
	else
	cout<<setw(3)<<p->data ;
	return 1;
}

lklist LRU_update(lklist head,int x)  //更新页面,链表第一个元素出队,新元素链入队尾.
{ 
	pointer p,q,w;
	p=head ;
    
    q=p->next;
    while(q !=NULL)
	{
		(q->r)+=1;  //所有页面进入时间加1;
        q=q->next;
	}

	p=head->next ;
	head->next =p->next ;
	delete p;
	p=head;
	while(p->next!=NULL)
		p=p->next ;
	w=new node;
	w->data =x;
	w->r=0;
	w->next =NULL;
	p->next =w;
	return head;
}


int LFU_find(lklist head, int x)
{
	pointer p;
	p=head->next ;
	int flag=0;
	while(p!=NULL )
	{
		if(p->data ==x)
		{
			(p->r)+=1; 
			cout<<setw(3)<<"*";
			flag=1;
			break;
		}
        p=p->next ;
	}
    if(flag==0)
		return 0;
	else
	return 1;
}


lklist LFU_notfind(lklist head,int x)
{
	pointer p,q,w,j,z;
	p=head;
	j=p;
	q=p->next ;
	w=q;
	int y=q->r ;
    while(q!=NULL)    
	{
		if(q->r<y)
		{
			y=q->r ;
			w=q;
			j=p;
		}
        p=p->next ;
		q=q->next ;
	}
	if(w->data==65535)
		cout<<" NULL ";
	else
    cout<<setw(3)<<w->data ;
    j->next=w->next ;
    delete w;
	p=head;
	while(p->next!=NULL)
		p=p->next ;
    z=new node ;
	z->data =x;
	z->r =0;
    z->next=NULL;
    p->next =z;
    return head;
}


int OPT_find(lklist head, int x)
{
	pointer p;
	p=head->next ;
	int flag=0;
	while(p!=NULL )
	{
		if(p->data ==x)
		{
			cout<<setw(3)<<"*";

			//show(head);
			flag=1;
			break;
		}
        p=p->next ;
	}
    if(flag==0)
		return 0;
	else
	return 1;
}


lklist OPT_notfind(lklist head,int x,int start,int len)
{
	pointer p,q;
	p=head;
	
	q=head->next ;
	int num,j,s1;
	for(int i=1;i<=3;i++)  //设一个计数器看谁是最长时间才访问到的.
	{  
		j=1;
		int flag=0;
		num=q->data ;
		s1=start;   //此处一定要用另一个变量来执行start值,这样在下一次for循环start值不会改变
    	while((len-s1)>=0)
		{   
 
		
			if(num==a[s1++])
			{
				flag=1;
				q->r=j;
				break;
			}
			j++;
		}
		if(flag==0)
			q->r=65535;
			
		q=q->next ;

	}
	pointer w,y;
	p=head;
	w=p;
	q=head->next ;
	y=q;
    int max;
	max=q->data ;
	while(q!=NULL)   //找出最长时间才访问到的页面
	{
		if(max<=q->r )
		{
			max=q->r ;
            w=p;
			y=q;
		}
		p=p->next ;
		q=q->next ;
	}
	if(y->data ==65535)
		cout<<" NULL ";
	else
	cout<<setw(3)<<y->data ;
//	show(head);
	w->next =y->next ;  //删掉找到的页面
	delete y;
	pointer z;           
    z=new node ;        //链入新页面 
	z->data =x;
	z->next =NULL;
	z->r =0;
	p=head->next ;
	while(p->next !=NULL)
		p=p->next ;
	p->next =z;
	return head;
}


	
void main()
{
	int n,x;
	n=readfile();
	pointer p;
	p=init_lklist();
	int i;
	//cout<<"check"<<endl;
	//for( i=0;i<=n;i++)
    //cout<<setw(6)<<a[i];
    //cout<<endl;
    //cout<<"init :  ";
    //show(p);
	cout<<"start : "<<endl;
	cout<<"( * )"<<"表示没有产生缺页中断.其余代表所换出的页号."<<endl<<endl;
	cout<<"第二次机会算法."<<endl;
    int find=0; 
	for( i=0;i<=n;i++)
	{
	   x=a[i];
       find=find_lklist (p, x);
	   if (find==0)
		   update_lklist(p,x);
	   //show(p);        //加上此句将显示整个页面变化过程.
	}
	cout<<endl;
    p=init_lklist();
    cout<<"(FIFO)先进先出算法." <<endl;
	find=0;
	for( i=0;i<=n;i++)
	{
	   x=a[i];
       find=find_lklist (p, x);
	   if (!find)
		   fifo_lklist(p,x);
	   //show(p);   //加上此句将显示整个页面变化过程.
	}
	cout<<endl;
    cout<<"(LRU)最近最少使用算法:"<<endl;
    p=init_lklist();
	find=0;
	for(i=0;i<=n;i++)
	{
		x=a[i];
        find=LRU_find (p,x) ; //查找内存中新进入的页面是否已经存在,存在则输出"*"号.
		
		if(!find)
		{
		LRU_notfind(p);   //不存在则输出链表第一个元素.
        LRU_update(p,x);  //更新页面,链表第一个元素出队,新元素链入队尾.
		}
		//show(p);        //加上此句将显示整个页面变化过程.
	}   
	cout<<endl;
	cout<<"(LFU)最近最不常使用算法:"<<endl;
    p=init_lklist();
	find=0;
    for(i=0;i<=n;i++)
	{
		x=a[i];
        find=LFU_find (p,x) ; //查找内存中新进入的页面是否已经存在,存在则输出"*"号.
		if(!find)
			LFU_notfind(p,x);
		//show(p);
	}
	cout<<endl;

	cout<<"(OPT)理想页面置换算法:"<<endl;
    p=init_lklist();
	find=0;
    for(i=0;i<=n;i++)
	{
		x=a[i];
        find=OPT_find (p,x) ; //查找内存中新进入的页面是否已经存在,存在则输出"*"号.
		if(!find)
			OPT_notfind(p,x,i,n);
	
	}
	cout<<endl;

}


