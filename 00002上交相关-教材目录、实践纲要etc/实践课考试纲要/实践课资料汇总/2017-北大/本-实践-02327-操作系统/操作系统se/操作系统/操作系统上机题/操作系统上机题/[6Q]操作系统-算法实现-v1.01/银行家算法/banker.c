#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef enum
{
	FALSE = 0,
	TRUE = !FALSE
}BOOL;
#define MAX_STRLEN	256

//定义资源总数
#define TOTAL_A			10
#define TOTAL_B			5
#define TOTAL_C			7

//进程节点
typedef struct _p_node
{
	char Name[MAX_STRLEN];
	int MaxA;
	int MaxB;
	int MaxC;
	int UsingA;
	int UsingB;
	int UsingC;
	struct _p_node *Next;
	struct _p_node *Prev;
}P_NODE;

//进程列表
typedef P_NODE P_LIST;

//新建节点
P_NODE *NewNode( const char * name,int maxA, int maxB, int maxC )
{
	P_NODE *pN = NULL;
	pN = (P_NODE*)malloc( sizeof( P_NODE ) );
	strcpy( pN->Name, name );
	pN->Next = NULL;
	pN->Prev = NULL;
	pN->UsingA = 0;
	pN->UsingB = 0;
	pN->UsingC = 0;
	pN->MaxA = maxA;
	pN->MaxB = maxB;
	pN->MaxC = maxC;
	return pN;
}

//销毁节点
void DelNode( P_NODE *pN )
{
	free( pN );
}

//插入节点，在*pP后插入*pN
void InsertAt( P_NODE *pP, P_NODE *pN )
{
	if( pP->Next != NULL )
		pP->Next->Prev = pN;
	pN->Prev = pP;
	pN->Next = pP->Next;
	pP->Next = pN;
}

//分离节点
void Isolate( P_NODE *pN )
{
	if( pN->Prev != NULL )
		pN->Prev->Next = pN->Next;
	if( pN->Next != NULL )
		pN->Next->Prev = pN->Prev;
		
	pN->Next = NULL;
	pN->Prev = NULL;
}

P_LIST	g_PList;
int			g_UsingA = 0;
int			g_UsingB = 0;
int			g_UsingC = 0;

//根据进程名寻找节点
//成功返回结点指针，失败返回NULL
P_NODE *FindNodeByName( const char *name )
{
	P_NODE *pN = g_PList.Next;
	while( pN !=  NULL )
	{
		if( 0 == strcmp( pN->Name, name ) )
			break;
		pN = pN->Next;
	}
	return pN;
}

//屏幕打印资源分配表
void PrintResourceTable()
{
	P_NODE *pN = g_PList.Next;
	
	printf("================================================\n");
	printf("                  资源分配表                    \n");
	printf("------------------------------------------------\n");
	printf("          | 目前占用量 | 最大需求量 | 尚需要量 \n");
	printf(" 进程名称 |------------+------------+-----------\n");
	printf("          | A | B | C  | A | B | C  | A | B | C \n");
	printf("----------+------------+------------+-----------\n");
	while( pN != NULL )
	{
		printf(" %8s |%3i|%3i|%3i |%3i|%3i|%3i |%3i|%3i|%3i\n",
			pN->Name,pN->UsingA,pN->UsingB,pN->UsingC,pN->MaxA,pN->MaxB,pN->MaxC,
			pN->MaxA-pN->UsingA,pN->MaxB-pN->UsingB,pN->MaxC-pN->UsingC );
		pN = pN->Next;
	}
	printf("================================================\n");
	printf(" 资源名称 |     A      |     B      |     C    \n");
	printf("----------+------------+------------+-----------\n");
	printf(" 资源总量 |%6i      |%6i      |%6i    \n",TOTAL_A,TOTAL_B,TOTAL_C );
	printf("----------+------------+------------+-----------\n");
	printf(" 占用总量 |%6i      |%6i      |%6i    \n",g_UsingA,g_UsingB,g_UsingC);
	printf("----------+------------+------------+-----------\n");
	printf(" 剩余总量 |%6i      |%6i      |%6i    \n",TOTAL_A-g_UsingA,TOTAL_B-g_UsingB,TOTAL_C-g_UsingC);
	printf("================================================\n");
}

//根据用户输入初始化进程列表
void BuildPListFromUserInput()
{
	char fileName[MAX_STRLEN];
	FILE *pF = NULL;
	P_NODE *pN = &g_PList;
	
	g_UsingA = 0;
  g_UsingB = 0;
  g_UsingC = 0;
	
	//请求输入文件
	for(;;)
	{
		printf("请输入文件名：");
		scanf( "%s", fileName );
		pF = fopen( fileName, "r" );
		if( pF != NULL )
			break;
		printf("无法打开该文件，请确认输入的文件名是否正确。\n");
	}
	
	//从输入文件读取数据初始化进程列表
	while( !feof( pF ) )
	{
		
		P_NODE *pN1 = NewNode(" ",0,0,0);
		fscanf( pF, "%s %i %i %i", pN1->Name, &pN1->MaxA, &pN1->MaxB, &pN1->MaxC );
		InsertAt( pN, pN1 );
		pN = pN1;
	}
	
	fclose( pF );
}

void PrintUserMenu()
{
	printf("==================================\n");
	printf("           命令菜单               \n");
	printf("----------------------------------\n");
	printf("1|申请资源 2|显示资源分配表 0|退出\n");
	printf("==================================\n");
	printf("请选择：");
}

//根据用户的输入分配资源
//按银行家算法判断是否可以分配
void AllocateResource()
{
	char	name[MAX_STRLEN];
	int		a = 0;
	int		b = 0;
	int		c = 0;
	P_NODE *pN = NULL;
	
	printf("请输入进程名：");
	scanf("%s",name);
	pN = FindNodeByName( name );
	if( pN == NULL )
	{
		printf("资源分配失败。\n");
		printf("原因：不存在名为%s进程。\n",name);
		return;
	}
	
	printf("请输入申请A的数量：");
	scanf("%i",&a);
	if( (a + pN->UsingA) > pN->MaxA )
	{
		printf("资源分配失败。\n");
		printf("原因：本次申请的A + %s已占用的A > %s对A的最大需求量。\n",
			pN->Name,pN->Name);
		return;
	}
	if( (pN->MaxA - pN->UsingA) > (TOTAL_A - g_UsingA) )
	{
		printf("资源分配失败。\n");
		printf("原因：%s尚需要的A > 系统中A的剩余总量。\n",
			pN->Name);
		return;
	}
	
	printf("请输入申请B的数量：");
	scanf("%i",&b);
	if( (b + pN->UsingB) > pN->MaxB )
	{
		printf("资源分配失败。\n");
		printf("原因：本次申请的B + %s已占用的B > %s对B的最大需求量。\n",
			pN->Name,pN->Name);
		return;
	}
	if( (pN->MaxB - pN->UsingB) > (TOTAL_B - g_UsingB) )
	{
		printf("资源分配失败。\n");
		printf("原因：%s尚需要的B > 系统中B的剩余总量。\n",
			pN->Name);
		return;
	}
	
	printf("请输入申请C的数量：");
	scanf("%i",&c);
	if( (c + pN->UsingC) > pN->MaxC )
	{
		printf("资源分配失败。\n");
		printf("原因：本次申请的C + %s已占用的C > %s对C的最大需求量。\n",
			pN->Name,pN->Name);
		return;
	}
	if( (pN->MaxC - pN->UsingC) > (TOTAL_C - g_UsingC) )
	{
		printf("资源分配失败。\n");
		printf("原因：%s尚需要的C > 系统中C的剩余总量。\n",
			pN->Name);
		return;
	}
	
	printf("资源分配成功。\n");
	g_UsingA += a;
	g_UsingB += b;
	g_UsingC += c;
	pN->UsingA += a;
	pN->UsingB += b;
	pN->UsingC += c;
}

//显示菜单，请求用户选择，并执行
void ExecUserCommand()
{
	char command[MAX_STRLEN];
	
	for(;;)
	{
		PrintUserMenu();
		scanf("%s",command);
		switch( command[0] )
		{
			case '1':AllocateResource();break;
			case '2':PrintResourceTable();break;
			case '0':exit(0);
			default:printf("您输入的选项不存在，请重新选择。\n");
		}
	}
}

void main()
{
	BuildPListFromUserInput();
	PrintResourceTable();
	ExecUserCommand();
}