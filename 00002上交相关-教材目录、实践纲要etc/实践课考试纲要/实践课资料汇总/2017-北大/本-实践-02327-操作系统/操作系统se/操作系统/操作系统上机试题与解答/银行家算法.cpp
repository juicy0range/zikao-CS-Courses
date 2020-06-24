/*writer:何良坤 傅仁军(仅供参考)如你有好的看法和建议,欢迎与本人探讨.furenjun@sohu.com*/

#include<iostream.h>
#include<iomanip.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
/*
int Own[4]={6,3,4,2};
int Available[4];
int Allocation[5][4]={{3,0,1,1},
						{0,1,0,0},
						{1,1,1,0},
						{1,1,0,1},
						{0,0,0,0}};
int Request[5][4]={{1,1,0,0},
{}};
*/

typedef struct {
int disk;
int draw;
int print;
//int cdrom;
char name[10];
}node;
int finish[4]={0,0,0,0};
node Allocation[5];
node Request[5];
//int Own[4]={6,3,4,2};
int Own[3]={10,5,7};
//int Available[4]={1,0,2,0};
int Available[3]={3,3,2};

 void init(){
	 char c[3];
	// int a[25]={3,0,1,1,0,1,1,0,1,1,1,0,1,1,0,1,0,0,0,0};
	 int a[15]={0,1,0,2,0,0,3,0,2,2,1,1,0,0,2};

	 //int b[25]={1,1,0,0,0,1,0,2,3,1,0,0,0,0,1,0,2,1,1,0};
	 	 int b[15]={7,4,3,1,2,2,6,0,0,0,1,1,4,3,1};

	 int j=0;
	 for(int i=0;i<5;i++){
		Allocation[i].disk=a[j+0];
		Allocation[i].draw=a[j+1];
		Allocation[i].print=a[j+2];
		//Allocation[i].cdrom=a[j+3];
		c[0]='p';
		c[1]= 49+i;
		c[2]='\0';
		strcpy(Allocation[i].name,c);
		//j+=4;
		j+=3;
	 }
	  j=0;
	  for( i=0;i<5;i++){
		Request[i].disk=b[j+0];
		Request[i].draw=b[j+1];
		Request[i].print=b[j+2];
		//Request[i].cdrom=b[j+3];
	    c[0]='p';
		c[1]= 49+i;
		c[2]='\0';
		strcpy(Request[i].name,c);
		//j=j+4;
		j=j+3;

	 }

    

}
 void show(){
	int j=0;
	 cout<<setw(26)<<"资源占有表"<<endl;
	 cout<<setw(10)<<"procss"<<setw(8)<<"disk"<<setw(8)<<"draw"<<setw(8)<<"print"<<endl;//<<setw(8)<<"cdrom"<<endl;
	 for(int i=0;i<5;i++){

		cout<<setw(8)<<Allocation[i].name;
		cout<<setw(8)<<Allocation[i].disk;
		cout<<setw(8)<<Allocation[i].draw;
		cout<<setw(8)<<Allocation[i].print;
		//cout<<setw(8)<<Allocation[i].cdrom;
	//	j+=4;
		j+=3;
		cout<<endl;
	 }
	  j=0;
	 cout<<setw(26)<<"资源请求表"<<endl;
	 cout<<setw(10)<<"procss"<<setw(8)<<"disk"<<setw(8)<<"draw"<<setw(8)<<"print"<<endl;//<<setw(8)<<"cdrom"<<endl;
	 
	  for( i=0;i<5;i++){
		cout<<setw(8)<<Request[i].name;
		cout<<setw(8)<<Request[i].disk;
		cout<<setw(8)<<Request[i].draw;
		cout<<setw(8)<<Request[i].print;
	//	cout<<setw(8)<<Request[i].cdrom;
	
		//j=j+4;
		j=j+3;
		cout<<endl;//

	 }
    cout<<"available: ";
   
	   for( i=0;i<3;i++)
	{
		cout<<setw(3)<<Available[i];
	}
	cout<<endl;


	
    
 
 
 }
char name[5][4];
 int bankermethod(){
	 int w=0;
	 for(int k=0;k<5;k++){
		 
		for(int i=0;i<5;i++){
		 
		//if((finish[i]==0)&&((Request[i].disk+Allocation[i].disk)<=Available[0])&&((Request[i].draw+Allocation[i].draw)<=Available[1])&&((Request[i].print+Allocation[i].print)<=Available[2]))//&&((Request[i].cdrom+Allocation[i].cdrom)<=Available[3]))
	if((finish[i]==0)&&(Request[i].disk<=Available[0])&&(Request[i].draw<=Available[1])&&(Request[i].print<=Available[2]))//&&(Request[i].cdrom<=Available[3]))

		{
			//cout<<"!";	
			finish[i]=1;
			Available[0]+=Allocation[i].disk;
			Available[1]+=Allocation[i].draw;
			Available[2]+=Allocation[i].print;
		//	Available[3]+=Allocation[i].cdrom;
			strcpy(name[w++],Allocation[i].name );
		}

/* 	  cout<<"available: ";
   // for( i=0;i<4;i++)
	   for( int u=0;u<3;u++)
	   {
		cout<<setw(3)<<Available[u];
	   }
	   cout<<endl;
	 
	 */
	}
	 }
	 
	 
	int flag=0,i;
	 //for(int i=0;i<4;i++){
		 for( i=0;i<3;i++){
//cout<<"finish["<<i<<"]="<<finish[i];
		 if(finish[i]==0){
			 flag=1;
		     //break;
		 }
	 }
	 if(flag==0){
		 cout<<"进程安全序列是:  ";
		 for(i=0;i<5;i++){
		 cout<<setw(4)<<name[i];
		 }
		 cout<<endl;
	 return 1;
	 }
	 else 
	 {
		 /*cout<<"进程不安全序列是:  ";
		 for(i=0;i<w;i++){
		 cout<<setw(4)<<name[i];
         }
		 */
		 cout<<"系统进入不安全状态!";
		 cout<<endl;
		 return 0;
		 
	 }
 }


 void main(){
int n;
	 init();
	 show();
	 int m=1;
	 //bankermethod();
	 while(m){
		 
		 finish[0]=0;
		  finish[1]=0;
	 finish[2]=0;
	 cout<<"现在开始申请:"<<endl;
	 cout<<"请选择进程号:(1: p1 2: p2 3:p3 4:p4 5:p5)"<<endl;
	 cout<<"请选择:>";
	 cin>>n;
	 cout<<"请求设备数量:"<<endl;
	 cout<<"disk:";
	 int disk;
	 cin>>disk;
	 cout<<"draw:";
	 int draw;
	 cin>>draw;
	 cout<<"print:";
	 int print;
	 cin>>print;
	// cout<<"cdrom:";
	// int cdrom;
	// cin>>cdrom;
	 if((disk<=Available[0])&&(draw<=Available[1])&&(print<=Available[2]))
	 {
		 Request[n-1].disk-=disk;
	     Allocation[n-1].disk+=disk;
         Request[n-1].draw-=draw;
	     Allocation[n-1].draw+=draw;
         Request[n-1].print-=print;
	     Allocation[n-1].print+=print;
//Request[n-1].cdrom-=cdrom;
//	 Allocation[n-1].cdrom+=cdrom;

         Available[0]-=disk;
         Available[1]-=draw;
         Available[2]-=print;
//Available[3]-=cdrom;
        show();
        m=bankermethod();
	 }
	 else
	 {
		 cout<<"系统现有资源不能满足您的要求!"<<endl;
		 exit(0);
	 }
       if(m==0)break;}

 
 }