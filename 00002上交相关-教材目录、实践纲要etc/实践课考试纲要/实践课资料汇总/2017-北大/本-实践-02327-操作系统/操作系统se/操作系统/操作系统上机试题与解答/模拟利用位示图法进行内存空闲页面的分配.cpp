/***********************************************************************************************
6月1号下午:
模拟利用位示图法进行内存空闲页面的分配
1． 输入：
“位示图”文件用于表示内存空闲页面，由0（表示空闲）和1（表示分配）组成。每行32个，共16行。
输入文件包括若干行，每行是位示图的一行，用‘0’‘1’字符串（不能有空格）表示每一行各位的值。
input.txt是一个输入文件格式的示例
2． 处理要求：
程序运行时，首先提示“请输入位示图文件的文件名：”；输入文件名后，程序将读入该文件中的有关数据。
再提示“请输入申请的页面个数：”，输入一个页面个数N后，程序执行，选择可以分配的满足
要求的连续N个页面。（申请内存块时，可在位示图中从头查找为0的字位，将其改为1。）
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
5． 测试说明：测试教师将事先准备好一组测试文件，格式如前所述，从中为每个程序随机指定
一至三个作为输入文件（被测试者需从键盘输入指定文件的文件名），并查看程序输出结果。
***************************************************************************************/
/*writer:何良坤 傅仁军(仅供参考)如你有好的看法和建议,欢迎与本人探讨.furenjun@sohu.com*/
# include <iostream.h>
# include <stdio.h>
# include <stdlib.h>
# include <iomanip.h>
# include <string.h>
static int  a[560];
static int i=0;       //用于统计读入数组中的数据个数;   
int  readdata()
{
	FILE *fp;
	char filename[20];
	cout<<"please input file name(如data.txt):>";
	cin>>filename;
	if((fp=fopen(filename,"r+"))==NULL)
	{
		cout<<"sorry!file can not open! "<<endl;
		exit(0);
	}
	char c;
	int  count=0;
	while(!feof(fp))
	{
		c=fgetc(fp);
		if(c=='0')
			++count;
		if(c=='0'||c=='1')
        a[i++]=c-48;
	}
	fclose(fp);
     return  count;
}
void show()   //显示位示图中的数据;
{
	for(int j=0;j<i;j++)
	{
		cout<<setw(1)<<a[j];
		if((j+1)%32==0)
			cout<<endl;
	}
	cout<<endl<<"总共读入数据个数为:"<<i<<endl;
	return  ;

}


void findpos(int num)         //找到第一个满足要求的位示图空间,并将相应结果存入结果文件.
{
	int pos,j,m,count; 
    pos=0;
	int flag=1;
    int start=0;
	for(j=0;j<i;j++)
	{  
		flag=1;
		start=j;
		count=1;
		for(m=j;m<i;m++)
		{
			if(a[m]==1)
			break;
			if(count>=num)
			{
				flag=0;
				break;
			}
			++count;
		}
		if(flag==0)
			break;

	}
	    if(count<num)
		cout<<"soory!Could not fulfil your request!"<<endl;
		else
		if(count>=num)
		{
			FILE *fp;
	        if((fp=fopen("result2.txt","w+"))==NULL)
			{
		     cout<<"sorry!file can not open! "<<endl;
		     exit(0);
			}
			cout<<"found!"<<endl;
			int m=(start+1)/32;
			int n=(start+1)%32;
			cout<<"分配的连续空间起始位置的行,列号(下标均从1开始)分别为:"<<m+1<<"  "<<n<<endl;
			char b[60]={"分配的连续空间起始位置的行,列号(下标均从1开始)分别为:"};
			char c[20];
            strcpy(c,"分配后的位示图为:");

			fprintf(fp,"%s%6d,%6d,%c",b,m+1,n,'\n');
			fprintf(fp,"%c",'\n');
            fprintf(fp,"%s%c",c,'\n');
			for(j=0;j<num;j++)  //修改位示图值;
				a[start++]=1;
			cout<<"分配后的位示图为:"<<endl;
		   show();
           for(j=0;j<i;j++) 
		   {
			   fprintf(fp,"%d",a[j]);
			   if((j+1)%32==0)
			   {
                   fprintf(fp,"%c",'\n');
			   }
		   }
		   fclose(fp);
		}
		return;
}

void main()
{
 int total=0;

  total=readdata();
  cout<<"总空闲块数为:"<<total<<endl;
  show();
  int judge=1;
  while (judge)
  {
  cout<<"请输入申请的内存块个数:>";
  int num;
  cin>>num;
  if(num==0)
  {
	  cout<<"Sorry!,your input error!please input again."<<endl;
      judge=1;
  }
  else
  {
  if(num>total)
  {
	  cout<<"Sorry,can not fulfil your quest.please input again."<<endl;
	  judge=1;
  }
  else
  {
   judge=0;   
   findpos(num);
  }
  }
  }
}






