/*
真2002年的
内存被划分成2048块（页）。用32位字长的字存放位示图，为0的位表示该块尚未分配，为1的位表示该块已分配
实习检查： 
1、运行程序，由检查教师给出文件名，该文件中存有内存目前状况的位示图的数据（0和1的文件）。（程序应做提示，界面友好）。 
2、你所编制的程序应读入数据，存放在相应的数据结构中。 
3、显示友好的用户界面，由检查教师输入内存申请（总块数）。 
4、根据申请和位示图状态，为用户分配内存，并建立页表。 
5、输出位示图和页表。

 测试文件: 只需把main函数中的  writeFile();  前 //去掉即可  
 
简单的测试文件1:4_3.txt;  (需把修改代码 const int row=3；const int col=4;  

*/
#include <stdlib.h>

#include <iostream>     

using namespace std;

/*const int row=64;   //因( 2048/32=64; 所以可用 [64][32]的二维数组记录2048块内存 的使用情况)
const int col=32; 
//const int ncMax=20; */

const int row=3;   
const int col=4;

void readFile(int arr[row][col]); 
void outData(int arr[row][col]);
void new_line();  //去掉没用的字符

void writeFile();  //生成测试数据(与工程文件在同一目录下)

 
void main()
{
	int arr[row][col],ncNum,k=0,kxqTotalNum=0;	
	int b,page=0;       //为申请内存分配的盘块号,页号
	int i,j;

    cout<<"\n4_3文件的存储管理:位示图的使用 \n\n"; 

	// writeFile();  // 先生成测试数据4_3data.txt文件

    readFile(arr);
	outData(arr);

    cout<<"\n请您 输入内存申请（总块数） \n";
    cin>>ncNum;

	
	//
    for(i=0;i<row;i++)
	{
	   for(j=0;j<col;j++)
	   {
	       if(arr[i][j]==0)
		   {
		      ++kxqTotalNum;
		   }
	   }
	}
	if(ncNum>kxqTotalNum)
	{
	   cout<<"\n当前没有足够的内存块，无法分配申请失败\n";
	}
	else
	{
		//建页表，并修改位示图
		cout<<"\n页表为：\n逻辑页号 物理块号\n";
		for(i=0;i<row;i++)
		{
		  for(j=0;j<col;j++)
		  {
			 if(ncNum>0 && arr[i][j]==0 )
			 {
				b=i*col+j;   //计算出盘块号 
				cout<<page<<"    "<<b<<"  \n";
				++page;
				arr[i][j]=1;  //修改位示图
                
				--ncNum;			
			 }
		  }
		}
    
		outData(arr);		
	}
}


void readFile(int arr[row][col])  
{
    char fName[50];
	FILE *stream;
	int  num,i,j;

    cout<<"请输入文件名(包括当前内存使用情况的位示图的数据:0 未使用;1 已分配） \n"; 
	cin.getline(fName,50,'\n');
	stream  = fopen( fName, "r" ); //打开文件
	while(stream==NULL)
    {
       cout<<"文件名或路径错误，请重新输入文件名\n";
	   //new_line();
	   cin.getline(fName,50,'\n');
       stream  = fopen( fName, "r" );
	}
    
    num=0;  //表示读入第一个数字
	while(!feof(stream))
	{
	   i=num/col;   //看数字在二维数组的哪个位置
	   j=num%col; 
	   fscanf(stream,"%d",&arr[i][j]);
	   ++num;       
	}   
	
    fclose(stream);	
   
}

void outData(int arr[row][col])
{
   cout<<"\n位示图为:\n";
   for(int i=0;i<row;i++)
   {
	  for(int j=0;j<col;j++)
	  {
         cout<<arr[i][j]<<"\t";  
	  }
	  cout<<"\n";
   }
}


void new_line()   //去掉没用字符
{
    char symbol;
    do
	{
	   cin.get(symbol);
	}while(symbol!='\n');
}

void writeFile()
{
    FILE *fStream;

	fStream=fopen("4_2data.txt","w+"); 
    
	if(fStream==NULL)
	{
	   cout<<"打开文件失败"<<endl;
	   exit(1);
	}

	for(int i=0;i<row;i++)    //我的测试数据为:（如0 2 4 等行）的第2,3列值为0；（如1 3 5 等行）的第63 64列值为0；其他均为1
	{
	   for(int j=0;j<col;j++)
	   {
	       if(i%2==0 && (j==2 || j==3))
		   {
		      fprintf(fStream,"%d  ",0);
		   }
		   else if(i%2 !=0 && (j==62 ||j==63))
		   {
		      fprintf(fStream,"%d  ",0);
		   }
		   else
		   {
		      fprintf(fStream,"%d  ",1);
		   }
	   }
	   

	}

	fclose(fStream);
}

