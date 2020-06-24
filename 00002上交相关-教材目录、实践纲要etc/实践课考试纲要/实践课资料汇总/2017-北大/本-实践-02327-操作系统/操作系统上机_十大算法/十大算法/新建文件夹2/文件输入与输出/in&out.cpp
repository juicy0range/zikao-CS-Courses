/*
**********对于文件的输入，可以分为两类。
若文件中的各数据项是以"空白符"(空格，回车，制表符)作为分隔的。用"<<"即可。
若文件中的各数据项是以其它字符(如',')和回车符作为分隔的。可用"get(char ch)"实现
也可用getline(char * buf,int len,char='\n')来输入一行。
**********对于文件的输出,可用">>"实现也可用"put(ch)"实现。
*/
#include <iostream.h>
#include <stdlib.h>
#include <fstream.h>
void main()
{	
	ifstream f1,f2;
	ofstream f3;
	char fname[20];
	int kind;
	cout<<"!!!!!!!!!!文件的输入!!!!!!!!!!"<<endl;
	cout<<"选择文件的种类(1 或 2):";
	cin>>kind;
	if(kind==1)
	{//第一类文件的输入
		int x;char a[30];
		cout<<"请输入文件的名称!";
		cin>>fname;
		f1.open(fname,ios::in|ios::nocreate);
		if(!f1)
		{
			cout<<"输入的文件名错误！"<<endl;
			exit(1);
		}
		f1.getline (a,30);
		cout<<a;
		while(f1>>x)
			cout<<x<<' ';
		cout<<endl;
	}
	
	if(kind==2)
	{//第二类文件的输入
		char ch='\0';int space=1;int x;
		char str[10];int i=0;
		cout<<"请输入文件的名称!";
		cin>>fname;
		f2.open(fname,ios::in|ios::nocreate);
		if(!f2)
		{
			cout<<"输入的文件名错误！"<<endl;
			exit(1);
		}
		//至"end #####"是处理数字型数据的输入
		while(f2.get(ch)){
			if(ch>='0'&&ch<='9') { str[i]=ch;i++; space=0; } 
			else if(ch==','||ch=='\n') 
			{
				if(space==0) {
					str[i]='\0'; x=atoi(str);
					i=0;space=1;
					cout<<x<<' ';
					cout<<endl;}
			}	
		}
		//若文件尾无回车则加入以下三行，否则不加
		str[i]='\0'; x=atoi(str);
		i=0;space=1;
		cout<<x<<' ';
		//end #####
	}
	
	//文件的输出
	cout<<"!!!!!!!!!!文件的输出!!!!!!!!!!"<<endl;
	cout<<"输入文件的名称!"<<endl;
	cin>>fname;
	f3.open(fname,ios::out);
    if(!f3)
	{
		cout<<"输入的文件名错误！"<<endl;exit(1);
	}
	f3<<"文件的输出!!"<<endl;
	int y=100;
	f3<<y<<endl;
	char ch1;
	cout<<"输入一个字符："<<endl;
	cin>>ch1;
	f3.put(ch1);
	f3<<endl;
	f3<<"over"<<endl;
}
