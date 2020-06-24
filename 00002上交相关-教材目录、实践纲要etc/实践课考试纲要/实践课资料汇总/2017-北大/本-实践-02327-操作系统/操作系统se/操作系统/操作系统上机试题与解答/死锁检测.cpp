/*模拟死锁检测算法

1． 输入：
“资源分配表”文件，每一行包含资源编号、进程编号两项（均用整数表示，并用空格分隔开），记录资源分配给了哪个进程。
“进程等待表”文件，每一行包含进程编号、资源编号两项（均用整数表示，并用空格分隔开），记录进程正在等待哪个资源。
下面是一个示例：
资源分配表：
1 1
2 2
3 3
进程等待表：
1 2
2 3
3 1
2． 处理要求：
程序运行时，首先提示“请输入资源分配表文件的文件名：”；再提示“请输入进程等待表文件的文件名：”。
输入两个文件名后，程序将读入两个文件中的有关数据，并按照死锁检测算法进行检测。
3． 输出要求：
第一行输出检测结果：有死锁 或 无死锁。
第二行输出进程循环等待队列，即进程编号（如果有死锁）。
4． 文件名约定
提交的源程序名字：sourceXXX.c或者sourceXXX.cpp（依据所用语言确定）
输入文件名字：可由用户指定
结果输出到resultXXX.txt中
其中：XXX为账号。
5． 死锁检测算法：当任一进程Pj申请一个已被其他进程占用的资源ri时，进行死锁检测。检测算法通过反复查找进程等待表和资源分配表，来确定进程Pj对资源ri的请求是否导致形成环路，若是，便确定出现死锁。
6． 测试说明：测试教师将事先准备好一组文件（格式为*.txt），从中为每个程序随机指定一至三个作为输入文件（被测试者需从键盘输入指定文件的文件名），并查看程序输出结果。
***********************************************************************************************/
/*writer:何良坤 傅仁军(仅供参考)如你有好的看法和建议,欢迎与本人探讨.furenjun@sohu.com*/
#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<iomanip.h>

typedef struct {
int proc;
int resource;
}node;
node allocate[10];
node request[10];


int readrfile(){
char name[20];
FILE *fp;
int m,i,j;
cout<<"请输入资源分配表文件名(alloc.txt):";
//cin>>name;

cout<<endl;
if((fp=fopen("test.txt","r+"))==NULL){

cout<<"文件不能打开1!!!";exit(0);
}
i=0;
while(!feof(fp)){
fscanf(fp,"%d",&m);
allocate[i].resource=m;
fscanf(fp,"%d",&m);
allocate[i].proc=m;
i++;
}
cout<<"资源编号"<<setw(10)<<"进程编号"<<endl;
for(j=0;j<i;j++){
cout<<setw(4)<<allocate[j].resource<<setw(12)<<allocate[j].proc<<endl;

}
fclose(fp);
return i;
}


int readqfile(){
char name[20];
FILE *fp;
int m,i,j;
cout<<"请输入资源等待表文件名(req.txt):";
//cin>>name;
cout<<endl;
//if((fp=fopen(name,"r+"))==NULL){
if((fp=fopen("waitfor.txt","r+"))==NULL){

cout<<"文件不能打开!!!";exit(0);
}
i=0;
while(!feof(fp)){
fscanf(fp,"%d",&m);
request[i].proc=m;
fscanf(fp,"%d",&m);
request[i].resource=m;

i++;
}
cout<<"进程编号"<<setw(10)<<"资源编号"<<endl;
for(j=0;j<i;j++){
cout<<setw(4)<<request[j].proc<<setw(12)<<request[j].resource<<endl;

}
fclose(fp);
return i;
}


int  lockcheck(node alloc[],node req[],int totalnum,int temp)
{
   //cout<<"temp"<<temp<<endl;
   int  i,j,k,x,w,a[20],z;
   j=0; k=0;
   a[k++]=req[temp].proc;
   int t;
   t=temp;
   for(i=0;i<=temp;i++)  //注意此处的变量要进行保存;
   {
    for(z=0;z<totalnum;z++) 
	   {
		   if((req[t].resource==alloc[z].resource)) //如果资源号相等则将其对应进程号存入数组.
		   {    //t为temp的变量,它是一个变化的量.
			   a[k++]=alloc[z].proc;  
			   //cout<<"ok1"<<endl;
		       j=z; 
			   break;//修改下面要用到的请求资源号下标
		   }
	   }
	   int flag=0; //在此处设标记是防止下面出现不能修改t 的值的情况,否则下一次循环将继续使用未改变的t值,从而出错.
       for(w=0;w<=temp;w++)             //每次对资源等待表的检测次数为进入资源等待表的进程数量.
	   {

		   if((alloc[j].proc ==req[w].proc ))  //如果资源分配表中的资源号等于进程等待表
			                                         //中资源号,则其相应的进程号进入进程队列.
		   {
			 
			   {
				flag=1;
		        t=w;  //修改t所指示的下标值.
				break;
			   }
		   }
	   }
	   if(flag==0)
		   break;

   }


 int flag=0,k2=0;
     x=a[0];
      for( i=1;i<k;i++)  //检测进程队列中是否存在环
	  {
		  flag=0;
			  if(x==a[i])
			  {
				  flag=1;
				  k2=i;
				  break;
			  }
	  }
     

    if(flag==1)
	  {
	     cout<<"  此进程队列有死锁!"<<endl;
	  }
   	 if(flag==0)
	 { 
	     cout<<"  没死锁!"<<endl;
	 }

    //将结果写入文件:

   FILE *fp;
   if((fp=fopen("result.txt","ab+"))==NULL)
   {
	   cout<<"soory! can not open file! "<<endl;
	   exit(0);
   }

   char c='\n';
   fputc(c,fp);
   char s[20]=" the number\n";
   fputs(s,fp);
    // cout<<endl<<"k="<<k<<endl;
     cout<<"  该进程队列为:"<<endl;
     for(i=0;i<=k2;i++)
	 {
		 cout<<setw(6)<<a[i];
		 fprintf(fp,"%5d",a[i]);

	 }
     fprintf(fp,"%c",'\n');
     fclose(fp)    ;

	
    
     return 0;
}

void main(){
int n;
n=readrfile();
cout<<endl<<n<<endl;
int m=readqfile();
cout<<endl<<m<<endl;
for(int i=0;i<m;i++)
{
	cout<<endl<<"  第"<<i+1<<"个请求出现时";
    lockcheck ( allocate,request,m,i);
	
}
cout<<endl;
return ;
}
