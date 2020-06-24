//头文件
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <iostream.h>
//数据结构
struct{
	int PageNo;
	int access;
}PageArea[3];
int oldest=0;
//初始化
void Init_mpage()
{
	for(int i=0;i<3;i++)
	{
		PageArea[i].PageNo=0;
		PageArea[i].access=-1;

	}
}
//页面置换算法
void Replace()
{
	char filename[20];
	FILE *fp;
	int p,i,flag;
	cout<<"请输入页面流文件名:";
	cin>>filename;
	fp=fopen(filename,"r+");
	if(fp==NULL)
	{
		cout<<"file can not open!"<<endl;
		exit(0);
	}
	fscanf(fp,"%d",&p);
	while(feof(fp)==0)
	{
		if(PageArea[oldest].access==-1)
		{
			cout<<"*"<<endl;
		PageArea[oldest].PageNo=p;
		}
		else{int qq=111;
			flag=0;
			for(i=0;i<3;i++)
			{
				if(flag==0)
				{
					cout<<PageArea[oldest].PageNo<<endl;
					PageArea[oldest].PageNo=p;
				}
			}
			
		}//end if
		oldest=(oldest+1)%3;
		fscanf(fp,"%d",&p);
	}
	    fclose(fp);
}
//主函数
	void main()
	{
		Init_mpage();
		Replace();
	}