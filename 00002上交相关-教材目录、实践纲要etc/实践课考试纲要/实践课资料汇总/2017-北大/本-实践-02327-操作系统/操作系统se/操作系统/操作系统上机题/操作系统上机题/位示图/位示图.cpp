/*模拟利用位示图法进行内存空闲页面的分配

1． 输入：
“位示图”文件用于表示内存空闲页面，由0（表示空闲）和1（表示分配）组成。每行32个，共16行。输入文件包括若干行，每行是位示图的一行，用‘0’‘1’字符串（不能有空格）表示每一行各位的值。
input.txt是一个输入文件格式的示例
2． 处理要求：
程序运行时，首先提示“请输入位示图文件的文件名：”；输入文件名后，程序将读入该文件中的有关数据。
再提示“请输入申请的页面个数：”，输入一个页面个数N后，程序执行，选择可以分配的满足要求的连续N个页面。（申请内存块时，可在位示图中从头查找为0的字位，将其改为1。）
要考虑一些边界问题。
3． 输出要求：
（1）显示本次分配的第1个页面在位示图中的行号和列号。
（2）显示分配完成后，新的位示图布局。
输出文件格式：
第一行输出分配起始位置的行列号，行号在前，列号在后，以空格相隔
第二行开始按照输入文件的格式输出更新后的位示图
输出内容中除了要求的内容不要包括任何其他内容
4． 文件名约定
提交的源程序名字：source.c或者source.cpp（依据所用语言确定）
输入文件名字：可由用户指定
结果输出到result.txt中
5． 测试说明：测试教师将事先准备好一组测试文件，格式如前所述，从中为每个程序随机指定一至三个作为输入文件（被测试者需从键盘输入指定文件的文件名），并查看程序输出结果.*/

#include"stdio.h"
#include"stdlib.h"
#include"malloc.h"
void main()
{
	FILE * fp;
	char filename[20];
	char a[100][100],ch;
	int i=1,j=1;
	printf("请输入位示图文件的文件名：");
	gets(filename);
	fp=fopen(filename,"r");
	if(fp==NULL)
	{
		printf("此文件不存在！\n");
		exit(0);
	}
	int m=1,b[600];
	while(i<=16)
	   {
		   while(j<=33)
		   {
			   ch=fgetc(fp);
			   if(ch=='\n')
				   break;
			   else
			   {
				   a[i][j]=ch-'0';
				   b[m]=a[i][j];
				   m++;
				   j++;
			   }
		   }
		   i++;
		   j=1;
	   }
/*      for(i=1;i<=16;i++)
	   {
	    	for(int j=1;j<=32;j++)
			{
		        printf("%d",a[i][j]);
			}
	    	printf("\n");
	   }
	   int b[600],m=1;
	   for(i=1;i<=16;i++)
	   {
		   for(j=1;j<=32;j++)
		   {
			   b[m]=a[i][j];
			   m++;
		   }
	   }
	   for(i=1;i<m-1;i++)
	   {
		   printf("%d",b[i]);
	   }*/
	   printf("\n");
       int n;
	   printf("请输入申请的页面个数：");
	   scanf("%2d",&n);
	   int d=1,q=0,t=1,p=1;
	   while(d<m)
	   {
		   if(b[d]==0)
		   {
			   t=d;
			   p=d;
			   while(b[t]!=1)
			   {
				   t++;
				   q++;   
			   }
		      if(q>=n)
			  {   
			      for(i=p;i<p+n;i++)
				  {
				     b[i]=1;
				  }
				  break;
			  }
			  else
			  {
				  d=t;
				  q=0;
			  }
			     
		   }
		   else
			   d++;
	   }
	   if(d==m)
		   printf("没有足够的空间可以分配！\n");
	   int f,g;
	   f=p/32;
	   f++;
	   printf("起始位置的行号是：");
	   printf("%2d\n",f);
       g=p%32;
	   printf("起始位置的列号是：");
	   printf("%2d\n",g);
	   int l=1;
	   for(i=1;i<=16;i++)
	   {
		   for(j=1;j<=32;j++)
		   {
			   a[i][j]=b[l];
			   printf("%d",a[i][j]);
			   l++;
		   }
		   printf("\n");
			   
	   }
}
	




