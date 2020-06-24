/*6月１号上午：ＯＳ上机，磁盘旋转调度算法．．ＴＸＴ文件已经给出．*/
/*writer:何良坤 傅仁军(仅供参考)如你有好的看法和建议,欢迎与本人探讨.furenjun@sohu.com*/
# include <iostream.h>
# include <fstream.h>
# include <math.h>
# include <iomanip.h>
# include <stdlib.h>
# include <stdio.h>

typedef struct node *pointer;
static int len=0;
int res[100];

 struct node 
{
	int reqord;
	int cylnum;
	int disnum;
	int secnum;
	pointer next;
};


typedef  pointer lklist ;



lklist readfile (lklist head)
{
	FILE *fp;
	//char filename[20];
	//cout<<"Please input file name(disciratt.txt):>";
	//cin>>filename;
	if((fp=fopen("disciratt.txt","r+"))==NULL)
	{
		cout<<"sorry,file can not open!"<<endl;
		exit(0);
	}
    //初始化链表
	pointer p,q;
	head=new node ;
	head->next=NULL;
	head->reqord=65535;
	head->cylnum=65535;
	head->disnum=65535;
    head->secnum=65535;
	p=head;
    
	while(!feof(fp))
	{
		q=new node ;
		fscanf(fp,"%d",&q->reqord);
		fscanf(fp,"%d",&q->cylnum);
		fscanf(fp,"%d",&q->disnum);
		fscanf(fp,"%d",&q->secnum);
		++len;
		q->next=NULL;
		p->next=q;
		p=q;
	}
	fclose(fp);
	return head;
}  

lklist show( lklist head)
{
    pointer p;
	p=head->next;
	cout<<setw(12)<<"请求次序"<<setw(10)<<" 柱面号"<<setw(10)<<"磁盘号"<<setw(10)<<" 扇区号 "<<endl;
	while(p->next !=NULL)
	{
		cout<<setw(10)<<p->reqord<<setw(10)<<p->cylnum<<setw(10)<<p->disnum<<setw(10)<<p->secnum<<endl;
		p=p->next;
	}
    return head;
}

int SelectSort(lklist head)
{
	int t1,t2,t3,t4;
	lklist p,q,s;
	p=head;
	while(p!=NULL)
	{
		s=p;
		q=p->next ;
		while(q!=NULL)
		{
			if(q->secnum <s->secnum )
               s=q;
			q=q->next ;
		}
		if(s!=p)
		{
			t1=p->reqord ;
			t2=p->cylnum ;
			t3=p->disnum ;
			t4=p->secnum ;
			p->reqord =s->reqord ;
			p->cylnum =s->cylnum ;
			p->disnum =s->disnum ;
			p->secnum =s->secnum ;
			s->reqord =t1;
			s->cylnum =t2;
			s->disnum =t3;
			s->secnum =t4;

		}
		p=p->next ;
	}
	return 1;
}




int disciratt(lklist head,int sec1)
{
	pointer p,q;
	int count=0;
    //int res[100],i=0;
	int i=0;
    p=head ;
	//res[i++]=sec1;
	
	while((p->next!=NULL)&&(p->next->secnum<sec1))  //找到大于等于当前扇区号的结点;
	{
		p=p->next;
		++count;
		//cout<<count<<setw(3);

	}
	cout<<"count="<<count<<endl;;
	cout<<"len="<<len<<endl;
	if(count<len-1)          //如果当前磁头所在扇区号比所请求的任何扇区号都大(或都小),则从链表头部开始.下面的操作将不执行.
	{

	q=p->next;              //将大于等于当前结点号的结点删除;
	res[i++]=q->reqord ;    
    int s=q->secnum ;
	p->next =q->next ;
	delete q;            

	q=p;
	p=p->next ;

	while((p->next!=NULL))  //将比当前磁道号大且不等于当前磁道号的请求次序号入结果数组,且将该结点删除;

	{
		if(p->secnum!=s)
		{
		res[i++]=p->reqord ;  //将比当前磁道号大且不等于当前磁道号的请求次序号入结果数组,且将该结点删除;
		s=p->secnum;
		q->next=p->next  ;
		delete p;
		p=q->next ;
		}
		else
		{
			q=q->next ;
			p=p->next ;
		}
	}
	//cout<<"i="<<i<<endl;
	//cout<<"len="<<len<<endl;
	}
	cout<<endl;
	while(i<len-1 )
	{
		q=head;                 //每次开始总是删第一个结点,这样保证循环能正常结束.
		p=head->next;
		res[i++]=p->reqord;     //将比当前磁道号大且不等于当前磁道号的请求次序号入结果数组,且将该结点删除;
		int s=p->secnum;
		q->next=p->next ;
		delete p;
		p=q->next;
		while(p->next !=NULL)
		{
          if(p->secnum!=s)
		  {
		   res[i++]=p->reqord ; //将比当前磁道号大且不等于当前磁道号的请求次序号入结果数组,且将该结点删除;
		   s=p->secnum;
		   q->next=p->next  ;
		   delete p;
		   p=q->next ;
		  }
		 else
		 {
			q=q->next ;
			p=p->next ;
		 }
		}
	} 

  
	cout<<"访问磁盘顺序为(按请求次序号排列):"<<endl;
    for(int j=0;j<len-1;j++)
	{
		
		cout<<setw(3)<<res[j]<<endl;
	}
	
	return 1;
}



void main()
{
	lklist head=NULL;
    head=readfile(head);
	cout<<"读入的数据为:"<<endl;
	show(head);
    SelectSort(head);
	cout<<"排好序的数据为:"<<endl;
	show(head);
	cout<<"请输入当前磁头所在扇区号:>";
	int sec;
	cin>>sec;
	disciratt( head, sec);

	

}


		
	