/*****************************************************\
7．  编一C程序，它能根据输入的完全二叉树层次序列来构造该完全二叉树，并能输出该完全二叉树的后序序列和叶结点的个数。（完全二叉树层次序列是从1层到最高层、同层内从左到右的次序排列的结点序列）。
\*****************************************************/
#include "iostream.h"
#include "string.h"
#define MAX 1000
char CNodeList[MAX]={0} ;
int length = 0;		//节点个数
int count = 0;		//叶子数目的2倍
struct STreeNode{
	STreeNode(){node = 0;lChild = NULL;rChild = NULL; }
	char node;
	STreeNode *lChild;	// 右子树 
	STreeNode *rChild;	// 左子树
};
void MakeTree(STreeNode &p,int i)
{
	if( i< length)
	{
		p.node =  CNodeList[i];
		p.lChild = NULL;
		p.rChild = NULL;
		if(i*2+1<length)
		{
			STreeNode *r = new STreeNode();
			p.rChild = r;			
			MakeTree(*(p.rChild),i*2+1);
		}
		if(i*2+2 < length)
		{
			STreeNode *l = new STreeNode();
			p.lChild = l;
			MakeTree(*(p.lChild),i*2+2);
		}
	}
}
void ShowTree(STreeNode *p)
{
	if(p)
	{
		ShowTree(p->rChild);
		ShowTree(p->lChild);
		cout<<p->node;
	}
	else
	{
		count++;
	}
}
void main()
{
	cout<<"Please enter the tree node list"<<endl;
	cin>> CNodeList;
	STreeNode Head ;
	STreeNode *p = NULL;
	length = strlen(CNodeList);
	MakeTree(Head,0);
	ShowTree(&Head);
	cout<<endl;
	cout<<"叶节点个数为 "<<count/2<<endl;
}
