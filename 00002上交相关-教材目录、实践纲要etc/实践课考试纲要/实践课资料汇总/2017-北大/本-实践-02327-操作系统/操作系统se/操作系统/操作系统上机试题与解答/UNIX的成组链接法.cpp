/*
UNIX系统文件管理成组连接算法：
把空闲块分成若干组，把指向一组中各空闲块的指针集中一起。
这样既可方便查找，又可减少为修改指针而启动磁盘的次数。
UNIX系统：采用空闲块成组连接的方法。
UNIX系统把每100个空闲块作为一组，每一组的第一个空闲块中登记下一组空闲块的块号和空闲块数，余下不足100块的那部分空闲块的块号及块数登记在一个专用块中，登记最后一组块号的那个空闲块其中第2个单元填“0”，表示该块中指出的块号是最后一组的块号，空闲块链到此结束。
系统初始化时先把专用块内容读到内存，当需分配空闲块时，就直接在内存中可找到哪些块是空闲的，每分配一块后把空闲块数减1。但要把一组中的第一个空闲块分配出去之前应把登记在该块中的下一组的块号及块数保存到专用块中。
当一组空闲块被分配完后，则再把专用块的内容读到内存，指出另一组可供分配的空闲块。当归还一块时，只要把归还块的块号登记到当前组中且空闲块数加1。如果当前组已满100块，则把内存中的内容写到归还的那块中，该归还块作为新组的第一块。假设初始化时系统已把专用块读入内存L单元开始的区域中，分配和回收的算法如下：
  分配一个空闲块
查L单元内容（空闲块数）：
当空闲块数>1 i : ＝L＋空闲块数；
从i单元得到一空闲块号；
把该块分配给申请者；
空闲块数减1。
当空闲块数＝1 取出L＋1单元内容（一组的第一块块号或0）；
其值＝0无空闲块，申请者等待
不等于零把该块内容复制到专用块；
该块分配给申请者；
把专用块内容读到主存L开始的区域。
归还一块
查L单元的空闲块数；
当空闲块数<100 空闲块数加1；
j : ＝L＋空闲块数；
归还块号填入j单元。
当空闲块数＝100 把主存中登记的信息写入归还块中；
把归还块号填入L＋1单元；
将L单元置成1。
采用成组连接后，分配回收磁盘块时均在内存中查找和修改，只是在一组空闲块分配完或空闲的磁盘块
构成一组时才启动磁盘读写。比单块连接方式效率高。
*/
/*
6月5日下午是UNIX的成组链接法的设计与实现，
主要考的是用文件输入一堆空闲块的 号码，然后利用UNIX的成组链接法的管理方法，10块成一组，
并且可以实现输入一个数字N,然后把N个空闲块占用，然后输出专用块的大小，空闲号。 
********************************************************************************************/
/*writer:何良坤 傅仁军(仅供参考)如你有好的看法和建议,欢迎与本人探讨.furenjun@sohu.com*/

# include <stdio.h>
# include <iostream.h>
# include <iomanip.h>
# include <stdlib.h>

typedef struct node  *pointer;
struct node 
{
	int frenum;
	int frespa[10];
	pointer next;
};

typedef pointer lklist;

static int totalnum=0;
static int flag11=0;
int  wrinum()
{
	FILE *fp;
	if((fp=fopen("freeblock.txt","w+"))==NULL)
	{
		cout<<"Sorry! file can not open!"<<endl;
		exit(0);
	}
	int m=1;
	for(int i=0;i<105;i++)
	{
        fprintf(fp,"%d",m);
		m++;
		fprintf(fp,"%s"," ");
		if((i+1)%10==0)
			fprintf(fp,"%c",'\n');
	}
	cout<<"数据已成功写入文件."<<endl;
	fclose(fp);
	return 1;
}

lklist readfile( lklist head)   //设head 为专用块
{
	FILE *fp;
	if((fp=fopen("freeblock.txt","r+"))==NULL)
	{
		cout<<"Sorry! file can not open!"<<endl;
		exit(0);
	}
	pointer p,q;
	head=new node;
	head->frenum=0; 
	for(int i=0;i<10;i++)
		head->frespa[i]=0;
	head->next =NULL;
	p=head;
	int CouNum=0,s=0;
    while(!feof(fp))
	{
		i=0; 
		int temp=0;
		q=new node ;  //注意生成新结点应写在while(1)之外.
		if(CouNum==0)
        {
			q->frespa[i++]=0; 
			++totalnum;
			++CouNum;
		}
		
		while(1)
        {
			int m=0;
			fscanf(fp,"%d",&m);
        	q->frespa[i++]=m;
			++totalnum;
			//cout<<"m"<<setw(3)<<m;
			//cout<<"i"<<setw(3)<<i;
		    temp=i;
			if((i)%10==0)             //注意循环退出的条件.
				break;
			if(feof(fp))
				break;
		
		}   
		    if(temp%10==0)
            q->frenum=10;
			else
            q->frenum=temp-1;
			if(s==0)
            q->frenum=temp;
			s++;
			q->next=head;
			head=q;
	}
	fclose(fp);
	return head;
}

void show(lklist head)
{
	pointer p;
	p=head ;
    cout<<"下面输出各组数据:"  <<endl;
	int i=1;
	while(p!=NULL )
    {
        cout<<"第"<<i++<<"块中数据为:"<<endl;  
		cout<<"总块数:"<<setw(3)<<p->frenum <<endl;
		int n=p->frenum ;
		for(int j=n-1;j>=0;j--)
		{
			cout<<setw(3)<<p->frespa[j]<<endl;
		}
		p=p->next ;
	}
	return ;
}


lklist allotblock(lklist head)
{
	pointer p;
	cout<<"请输入您要分配的块数:>";
	int num,count=0,i=0;
	cin>>num;
	p=head;
	cout<<"total="<<totalnum-2<<endl;
   if((num>(totalnum-2))||(num<=0))
   {
	   cout<<"Sorry! can not fulfil your request."<<endl;
	   exit(0);
   }

   int j=0;
	cout<<"分给您的块号为:"<<endl;
	
	while((p!=NULL)&&(count<num))
	{
       i=0;
	   int n=p->frenum;
       if(i>=n )
		   break;

	   flag11=n-1;
	   for( j=n-1;j>=0;j--)
	   {
		   cout<<setw(5)<<p->frespa[j] ;
		   --totalnum;
           ++count;
		   if(count%10==0)
			   cout<<endl;
		   if(count>=num)
		   {
			   flag11=j;
			   
			   break;
		   }
	   }
	   if(count>=num)
		   {
			   
			  // cout<<"flag="<<flag<<endl;
			   break;
		   }
       head=head->next ;
	   p=head;
	}
	//cout<<"flag11="<<flag11<<endl;
	if( j==0)
	{  
		cout<<endl<<"专用块所含内容为:"<<endl;
		cout<<"总块数为:"<<setw(3)<<p->frenum <<endl;
		head=head->next ;
		p=head;
		cout<<"所含数据:"<<endl;
		for(i=p->frenum-1;i>=0;i-- )
			cout<<setw(5)<<p->frespa[i]<<endl;
	}
	else
	{
		cout<<endl<<"专用块所含内容为:"<<endl;
		cout<<"总块数为:"<<setw(3)<<j<<endl;
		cout<<"所含数据:"<<endl;
        for(i=j-1;i>=0;i-- )
			cout<<setw(5)<<p->frespa[i]<<endl;   
	}
    return head;


}

void main()
{
	lklist head=NULL;
	wrinum();
	head=readfile(head);
    show(head);
	allotblock(head);
}








