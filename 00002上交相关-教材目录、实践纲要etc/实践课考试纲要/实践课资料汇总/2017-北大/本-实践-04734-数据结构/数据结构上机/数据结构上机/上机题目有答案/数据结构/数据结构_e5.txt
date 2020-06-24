//   cl e5.cpp
#include <iostream.h>
#include <string.h>
#define  MAX 100
char FList[MAX]={0};//前序队列	a b d f g c e
char MList[MAX]={0};//中序队列	d b g f a c e
struct node
{
	node(){d = 0;lChild = NULL;rChild = NULL;}
	char d;
	node *lChild;
	node *rChild;
};
void MakeTree(int FStart,int FEnd,int MStart,int MEnd,node &T)
{
	T.d = FList[FStart];
	bool Found = false;
	for(int i = MStart;i<= MEnd && !Found;i++)
	{
		if( FList[FStart] == MList[i])
		{
			Found = true;
			if( i != MStart)
			{
				node *p = new node();
				T.rChild = p;
				MakeTree(FStart+1,FStart+i-MStart,MStart,i-1,*(T.rChild));
			}
			if( i != MEnd)
			{
				node *p = new node();
				T.lChild = p;
				MakeTree(FStart+i-MStart+1,FEnd,i+1,MEnd,*(T.lChild));
			}
		}
	}
	if(!Found)
	{
		cout<<"\n error"<<endl;
	}
}
void BackOrder(node *r)
{
	if(r)
	{
		BackOrder(r->rChild);
		BackOrder(r->lChild);
		cout<<r->d<<" ";
	}
}
int leaf(node *r)
{
	if(!r)
		return 0;
	else
	{
		if( NULL == r->lChild &&
			NULL == r->rChild )
			return 1;
		else
			return leaf(r->rChild) + leaf(r->lChild);
	}
}
int Height(node *r,int i)
{
	if(!r)
		return 0;
	else
	{
		if( NULL == r->lChild &&
			NULL == r->rChild )
			return 0;
		else
		{
			i++;
			int il = 0;
			il = Height(r->lChild,i);
			int ir = 0;
			ir = Height(r->rChild,i);
			return il>ir?il:ir;
		}
	}
}
void main()
{
	node Head;
	cout<<"请输入前序序列"<<endl;
	cin>>FList;
	cout<<"请输入中序序列"<<endl;
	cin>>MList;

	MakeTree(0,strlen(FList)-1,0,strlen(MList)-1,Head);
	cout<<"后序序列为"<<endl;
	BackOrder(&Head);
	cout<<"数有 "<<leaf(&Head)<<" 个叶子"<<endl;
	cout<<"数高 "<<Height(&Head,0)<<endl;
}
                                                                                                                                                                             