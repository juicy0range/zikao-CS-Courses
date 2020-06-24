/*
可变分区的(最坏适应算法)
 题：
1．程序运行时首先接收输入：空闲区数据文件，包括若干行，每行有两个数据项：起始地址、长度（均为整数），各数据项以逗号隔开。
2．建立空闲区表并在屏幕上显示输出空闲区表内容，空闲区表中记录了内存中可供分配的空闲区的始址和长度，用标志位指出该分区是否是未分配的空闲区。
3．从用户界面根据用户提示接收一个内存申请，格式为：作业名、申请空间的大小。
4．按照最差（最坏）适配算法选择一个空闲区，分割并分配，修改相应的数据结构（空闲区表），填写内存已分配区表（起始地址、长度、标志位），其中标志位的一个作用是指出该区域分配给哪个作业。
5．重复3、4，直到输入为特殊字符（0）。
6．在屏幕上显示输出新的空闲区表和已分配区表的内容。

----测试用例：
a
新建3_1worst.txt:输入下边内容
1001,30
2003,50
5006,10 
2运行程序：按要求输入即可
(如用户申请:a.exe 40  b.txt 20
*/

#include <stdio.h>   
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

const int maxNum=10;
//空闲区
typedef struct  KongXianQu
{
  int sAddress;
  int kxqLength;
  int kxqFlag;
}kxq;

//内存分配表
typedef struct NeiCunFengPeiBiao
{
   int sAddress;
   int ncLength;
   string pName;
}NCFPB;

int readFile(kxq arrKxq[]);   //读空闲区文件,返回空闲区数
void sort(kxq arr[],int num);  //简单选择排序
void showResult(kxq arrKxq[],int kxqNum,NCFPB nc[],int ncNum);
void new_line();  //去掉没用的字符


void main()
{
   kxq arrKxq[maxNum];
   NCFPB arrNcfpb[maxNum];
   int i=0,j=0,sAddress=0,kLength=0,proLength=0;
   int kxqNum=0,ncNum=0;
   char ch, pName[20];   

   cout<<"----------------功能：实现可变分区的最坏适应算法---------------\n";

    //1:读入空闲区文件的数据
    kxqNum=readFile(arrKxq);

    //2:输出空闲区表内容
    cout<<"\n2:------------系统当前空闲区为:\n";
    cout<<"起始地址   长度   标志位\n";
	for(i=0;i<kxqNum;i++)
	{
	    cout<<arrKxq[i].sAddress<<"\t";
		cout<<arrKxq[i].kxqLength<<"\t";
		cout<<arrKxq[i].kxqFlag<<"\n";
	}   

	 //(3)接收用户申请
	 cout<<"3:--------请输入作业名、申请空间的大小 完成内存的申请\n";
     do
	 {
		 cout<<"请您输入待申请内存:作业名为\n";
		 //new_line();      //把cout语句剩下没用的字符过滤掉
		 cin.getline(pName,20,'\n');
		 cout<<"请输入申请空间的大小:\n";
		 cin>>proLength;    
     
	   //----开始分配----------
        //3_1对空闲区排序：从大到小(这是最坏适配算法的前提),
        sort(arrKxq,kxqNum);

		if(arrKxq[0].kxqFlag==0 || proLength>arrKxq[0].kxqLength )   //空闲区不可用或 申请内存>第0个空闲区,则无法分配
		{
		   cout<<"无法分配内存\n";
		}
		else 
		{
			//b填写内存已分配区表  
			arrNcfpb[ncNum].sAddress=arrKxq[0].sAddress;
			arrNcfpb[ncNum].ncLength=proLength;
			arrNcfpb[ncNum].pName=pName;
			++ncNum;

               //并  修改空闲区表
			arrKxq[0].sAddress=arrKxq[0].sAddress+proLength; 		
			arrKxq[0].kxqLength=arrKxq[0].kxqLength-proLength;
			if(arrKxq[0].kxqLength==0)   //当空闲区的长度为0时，用0 表示不可用了
			{
			   arrKxq[0].kxqFlag=0;   
			   //把不可用的空闲区去掉
               for(i=1;i<kxqNum;i++)
			   {
			      arrKxq[i-1]=arrKxq[i];
			   }
               kxqNum=kxqNum-1;
			}
			else
			{
			   arrKxq[0].kxqFlag=1;   
			} 			 
		} 
        cout<<"继续进行内存的申请吗？请输入(yes/no)\n";
        cin>>ch;
		new_line();		
	 }while( (ch!='N') && (ch!='n'));

      //6输出结果
	  showResult(arrKxq,kxqNum,arrNcfpb,ncNum);	
}


int readFile(kxq arrKxq[])
{
	char fName[50];
	FILE *stream;
	int  kxqNum=0;
    cout<<"请输入空闲区数据文件名\n";
    //new_line();
	cin.getline(fName,50,'\n');
	stream  = fopen( fName, "r" ); //打开文件
	while(stream==NULL)
    {
       cout<<"文件名或路径错误，请重新输入文件名\n";
	   new_line();
	   cin.getline(fName,50,'\n');
       stream  = fopen( fName, "r" );
	}
    
	while(!feof(stream))
	{	
	    fscanf(stream,"%d,%d",&arrKxq[kxqNum].sAddress,&arrKxq[kxqNum].kxqLength);
        arrKxq[kxqNum].kxqFlag=1;  //1表示空闲区可用,0表示不可用
        //kxqNum++;
        kxqNum=kxqNum+1;
	}    
    fclose(stream);

	return kxqNum;
}

void sort(kxq arr[],int num)
{
   int i=0,j=0;
   kxq tem;
   for(i=0;i<num;i++)
   {	  
      for(j=i+1;j<num;j++)
	  {
	     if(arr[i].kxqLength < arr[j].kxqLength)
		 {   
			 tem=arr[i];
			 arr[i]=arr[j];
			 arr[j]=tem;		 
		 }	  
	  }  
   }
}

void showResult(kxq arrKxq[],int kxqNum,NCFPB nc[],int ncNum)
{
   int i=0,j=0;
    cout<<"4-----新的空闲区表为:\n";
    cout<<"起始地址   长度   标志位(1表示可用，0表示不可用)\n";
	for(i=0;i<kxqNum;i++)
	{
	    cout<<arrKxq[i].sAddress<<"\t";
		cout<<arrKxq[i].kxqLength<<"\t";
		cout<<arrKxq[i].kxqFlag<<"\n";
	}

    cout<<"\n--- -内存已分配区表:\n";
    cout<<"---起始地址   长度   程序名(标志位)\n";
    for(j=0;j<ncNum;j++)
	{
	    cout<<nc[j].sAddress<<"\t";
		cout<<nc[j].ncLength<<"\t";
		cout<<nc[j].pName<<"\n";
	}
}

void new_line()
{
    char symbol;
    do
	{
	   cin.get(symbol);
	}while(symbol!='\n');
}