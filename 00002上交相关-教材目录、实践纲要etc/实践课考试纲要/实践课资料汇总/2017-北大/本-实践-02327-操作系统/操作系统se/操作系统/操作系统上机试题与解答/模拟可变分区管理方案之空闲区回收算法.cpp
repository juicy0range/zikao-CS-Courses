/***************************************************************************************
6月2号
模拟可变分区管理方案之空闲区回收算法
1． 输入：
（1）“空闲区表”文件，每一行包含空闲区起始地址、长度两项（均用整数表示，并用空格
分隔开）。
（2）进程新归还的空闲区起始地址和长度。
下面是一个示意：
15 23
48 20
98 12
2． 处理要求：
程序运行时，首先提示“请输入空闲区表文件的文件名：”，输入一个文件名后，程序将读
入该文件中的有关数据。
再提示“请输入新归还空闲区的起始地址和长度：”，输入两个整数后，按照回收算法完成
空闲区的回收工作（要考虑各种可能的情况）。
3． 输出要求：
输出新的空闲区表，每一行包括空闲区起始地址、长度两项。
4． 文件名约定
 提交的源程序名字：sourceXXX.c或者sourceXXX.cpp（依据所用语言确定）
 输入文件名字：可由用户指定
 结果输出到resultXXX.txt中
其中：XXX为账号。
5． 测试说明：测试教师将事先准备好一组文件（格式为*.txt），从中为每个程序随机指定
一至三个作为输入文件（被测试者需从键盘输入指定文件的文件名），并查看程序输出结果。
***************************************************************************************/
/*writer:何良坤 傅仁军(仅供参考)如你有好的看法和建议,欢迎与本人探讨.furenjun@sohu.com*/
# include <iostream.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <iomanip.h>

typedef struct 
{
	int start;
	int len;
	int end;
}node;
node fre[20];
static int len=0;

int readdata()
{
	FILE *fp;
	/*
	char filename[20];
	cout<<"please input file name(如freearea.txt):>";
	cin>>filename;
     */
	if((fp=fopen("freearea.txt","r+"))==NULL)
	{
		cout<<"sorry! file can not open!"<<endl;
		exit(0);
	}
	int a[100],m=0,i=0;
	while(!feof(fp))
	{

		fscanf(fp,"%d",&m);
		a[i++]=m;
	}
	m=i/2;
	cout<<"读入数据组数为:"<<m<<endl;
	int w=0;
	for(int j=0;j<m;j++)
	{
		fre[j].start=a[w+0];
		fre[j].len=a[w+1];
		fre[j].end=fre[j].start+fre[j].len;
		w+=2;
	}
    return m;
}

void show( )
{
   cout<<"空闲区情况如下"<<endl;
   cout<<setw(12)<<"起始地址"<<setw(6)<<"长度"<<setw(10)<<"结束地址"<<endl;
   for(int i=0;i<len;i++)
   {
	   cout<<setw(10)<<fre[i].start<<setw(7)<<fre[i].len<<setw(9)<<fre[i].end<<endl;
   }
   return ;
}

void reclaim()
{
	cout<<"请输入新归还空闲区的起始地址和长度"<<endl;
	cout<<"起始地址:";
	int startpos;
    cin>>startpos;
    cout<<"长    度:";
	int len2;
    cin>>len2;
	int end2=startpos+len2;
	int maxend=fre[0].end;

	for(int y=0;y<len;y++)
	{
		if(maxend<fre[y].end)
			maxend=fre[y].end;
	}

    for( y=0;y<len;y++)      //检查输入数据的有效性                  
	{
		if((startpos<fre[0].start)&&(end2>fre[0].start))
		{
			cout<<endl<<"您输入的回收空闲区长度非法!"<<endl<<endl;
			return;
		}
        if(startpos>=maxend)
			break;
		if((startpos>=fre[y].start)&&(startpos<fre[y].end))
		{
			cout<<endl<<"您输入的回收空闲区起始地址非法!"<<endl<<endl;
			return;
		}
	}
	int t=0;
	for(y=0;y<len;y++)
	{
		if(startpos>=fre[y].end)
		{
			 t=y;
		}
	}
   
        if(startpos<maxend)     //如果它大于最大未地址,显然应该补加在最后.
		{
        if((startpos>=fre[t].end)&&(end2>fre[t+1].start))   
		{
			cout<<endl<<"您输入的回收空闲区长度非法!"<<endl<<endl;
			return;
		}
		}
	
	int shali=0,xiali=0;   //查找其是否有上临,下临或者没有上下临;
	int s=65535,x=65535;
	for(int j=0;j<len;j++)
	{
		if(fre[j].start==end2)
		{
			x=j;
		}
		if(fre[j].end==startpos)
		{
			s=j;
		}
	}
	if(x==65535&&s==65535)
	{
		cout<<"不存在上下邻;"<<endl;
	
		fre[len].start =startpos;
		fre[len].len =len2;
		fre[len].end =end2;
		len+=1;
	}
	if(x==65535&&s!=65535)
	{  
		cout<<"存在上邻;"<<endl;
        fre[s].len+=len2;        //修改原表的结束地址和长度;
		fre[s].end+=len2;
	}
	if(x!=65535&&s==65535)
	{
		cout<<"存在下临;"<<endl;
		fre[x].start=startpos;  //修改原表的起始地址和长度;
		fre[x].len+=len2;
		
	}
	if(s!=65535&&x!=65535)
	{
        cout<<"存在上下邻;"<<endl;
		fre[s].len+=len2;        //修改原表的长度;
		fre[s].len+=fre[x].len;  
		fre[s].end =fre[x].end;  //修改原表的结束地址;
		fre[x].start =0;
		fre[x].len =0;
		fre[x].end =0;
	}
    
	node temp;                 //对空闲区表中的数据进行冒泡排序
    int i,ok;
	for(i=1;i<len;i++)
	{
		ok=1;
		for(j=0;j<len-i;j++)
		{
			if(fre[j].start>fre[j+1].start  )
			{
				temp=fre[j] ;
				fre[j] =fre[j+1];
				fre[j+1]=temp;
				ok=0;
			}
			
		}
		if(ok)              //注意if(ok)语句应写在大循环内.
				break;
	}
	return ;
}

int outres()
{
	FILE *fp;
	int flag=1;
	if((fp=fopen("result3.txt","w+"))==NULL)
	{
		cout<<"Sorry! file can not open! "<<endl;
		flag=0;
		exit(0);
	}
	fprintf(fp,"%s","空闲区情况如下");
    fprintf(fp,"%c",'\n');
	fprintf(fp,"%12s%6s%10s","起始地址","长度","结束地址");
	fprintf(fp,"%c",'\n');
	for(int i=0;i<len;i++)
	{
		fprintf(fp,"%10d%7d%9d",fre[i].start ,fre[i].len ,fre[i].end );
		fprintf(fp,"%c",'\n');
	}
	fclose(fp);
	return flag;
}

void main()
{
	len=readdata();   
    show( );
	int run=1;
	while(run)
	{
	reclaim();
	show( );
	int bj=0;
    bj=outres();
	if(bj==1)
	cout<<"Result have saved to result3.txt."<<endl;
	cout<<"Are you  continue?(Y/N)";
	char c[2],b[2]={"Y"};
	cin>>c;
    strcpy(c,strupr(c)) ;
	if(strcmp(c,"Y")!=0)
		run=0;
	else
		run=1;
	}
}

   







