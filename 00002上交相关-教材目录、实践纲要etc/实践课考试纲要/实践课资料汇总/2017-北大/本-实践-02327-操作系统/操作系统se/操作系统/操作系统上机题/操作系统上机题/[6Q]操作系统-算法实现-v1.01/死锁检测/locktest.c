#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef enum
{
	FALSE = 0,
	TRUE = !FALSE
}BOOL;
#define MAX_STRLEN	256

#define MAX_VERTEX	20
#define MAX_PATH		20
#define PATH_END		-1

//顶点类型枚举
typedef enum
{
	Process,
	Resource
}V_TYPE;

//顶点信息结构
typedef struct _v_node
{
	char 		Name[MAX_STRLEN];	//进程或资源的名称
	int			Vertex;						//进程或资源对应的顶点号
	V_TYPE	Type;							//顶点类型，进程或者资源
	struct	_v_node *Next;
	struct	_v_node *Prev;
}V_NODE;

//新建顶点信息节点
V_NODE *NewNode( const char *name, V_TYPE vType )
{
	static int __vCount = 0;
	
	V_NODE *pN = (V_NODE *)malloc( sizeof( V_NODE) );
	pN->Next = NULL;
	pN->Prev = NULL;
	pN->Type = vType;
	pN->Vertex = __vCount++;
	strcpy( pN->Name, name );
	return pN;
}

//将*pN插入到*pP之后
void InsertAt( V_NODE *pP, V_NODE *pN )
{
	if( pP->Next != NULL )
		pP->Next->Prev = pN;
	pN->Next = pP->Next;
	
	pN->Prev = pP;
	pP->Next = pN;
}

//顶点信息链表
typedef V_NODE V_LIST;

V_LIST g_VList;//存储所有进程顶点和资源顶点

//根据提供的顶点号，返回顶点信息结构
//查找失败返回NULL
V_NODE * FindNodeByVertex( int v )
{
	V_NODE *pN = g_VList.Next;
	while( pN != NULL )
	{
		if( pN->Vertex == v )
			break;
		pN = pN->Next;
	}
	return pN;
}

//根据提供的顶点名称和类型，返回顶点信息结构
//查找失败返回NULL
V_NODE * FindNodeByNameType( const char *name, V_TYPE type )
{
	V_NODE *pN = g_VList.Next;
	while( pN != NULL )
	{
		if( type == pN->Type && 0 == strcmp(pN->Name, name ) )
			break;
		pN = pN->Next;
	}
	return pN;
}

int g_Graph[MAX_VERTEX][MAX_VERTEX];//资源分配图

//深度优先遍历资源分配图，试图得到一条从from到达to的路径path
//如果存在路径，则返回TRUE，否则返回FALSE
BOOL DFS( int from, int to, int *path)
{
	static int __sd = 0;							//递归的深度
	static BOOL __visted[MAX_VERTEX];	//访问标志
	
	int i = 0;
	
	//如果递归深度为零，说明本次函数调用为用户调用，需要清空所有访问标志
	//当递归深度大于零时，说明本次调用为自身递归，需要保留访问标志
	if( __sd++ == 0)
		for( i=0; i<MAX_VERTEX; i++)
			__visted[i] = FALSE;
	
	__visted[from] = 1;	//标记该点已经被访问
	*path = from;				//在路径中记录该点
		
	for( i = 0; i<MAX_VERTEX; i++ )
	{
		if( g_Graph[from][i] )
		{
			if( i == to )//如果该点就是to,则返回TRUE
			{
				*(path+1) = i;//在路径中记录终点
				*(path+2) = PATH_END;//设置路径结束标志
				__sd--;
				return TRUE;
			}
			else if(!__visted[i])//这点还没有被访问过，则访问该点
			{
				BOOL b = DFS(i, to ,path+1 );
				if( b == TRUE )//如果已经到达to，则返回TRUE，否则继续遍历
				{
					__sd--;
					return TRUE;
				}
			}
		}
	}
	
	*(path+2) = PATH_END;//设置路径结束标志
	__sd--;
	return FALSE;
}

//请求资源分配表文件，根据文件内容建立资源到进程的出边
void _1_BuildGraphFromUserInput()
{
	char fileName[MAX_STRLEN];
	char proName[MAX_STRLEN];
	char resName[MAX_STRLEN];
	FILE *pF = NULL;
	V_NODE *pN = &g_VList;
	
	for( ;; )
	{
		printf("请输入资源分配表文件:");
		scanf( "%s",fileName );
		pF = fopen( fileName, "r");
		if( pF != NULL )
			break;
		printf("无法打开该文件，请确认输入的文件名是否正确。\n");
	}
	
	printf("================\n");
	printf("   资源分配表   \n");
	printf("----------------\n");
	printf("资源名 | 进程名 \n");
	printf("----------------\n");
	while( !feof( pF ) )
	{
		int from = 0;
		int to = 0;
		V_NODE *pN1 = NULL;
		
		fscanf(pF, "%s %s", resName, proName);
		printf("%6s | %6s\n",resName, proName);
		
		//如果顶点信息表中不包括该资源则插入该资源
		pN1 = FindNodeByNameType( resName, Resource );
		if( NULL == pN1 )
		{
			pN1 = NewNode( resName, Resource );
			InsertAt( pN, pN1);
			pN = pN->Next;
		}
		from = pN1->Vertex;
		
		//如果顶点信息表中不包括该进程则插入该进程
		pN1 = FindNodeByNameType( proName, Process );
		if( NULL == pN1 )
		{
			pN1 = NewNode( proName, Process );
			InsertAt( pN, pN1);
			pN = pN->Next;
		}
		to = pN1->Vertex;
		
		//在资源分配图上连通这两个顶点
		g_Graph[from][to] = 1;
	}
	printf("================\n");
	fclose( pF );
}

//请求进程等待表文件，根据文件内容建立进程到资源的出边
void _2_BuildGraphFromUserInput()
{
	char fileName[MAX_STRLEN];
	char proName[MAX_STRLEN];
	char resName[MAX_STRLEN];
	FILE *pF = NULL;
	V_NODE *pN = &g_VList;
	
	for( ;; )
	{
		printf("请输入进程等待表文件:");
		scanf( "%s",fileName );
		pF = fopen( fileName, "r");
		if( pF != NULL )
			break;
		printf("无法打开该文件，请确认输入的文件名是否正确。\n");
	}
	
	printf("================\n");
	printf("   进程等待表   \n");
	printf("----------------\n");
	printf("进程名 | 资源名 \n");
	printf("----------------\n");
	
	while( !feof( pF ) )
	{
		int from = 0;
		int to = 0;
		V_NODE *pN1 = NULL;
		
		fscanf( pF, "%s %s",proName, resName);
		printf("%6s | %6s\n",proName, resName);
		
		//如果顶点信息表中不包括该进程则插入该进程
		pN1 = FindNodeByNameType( proName, Process );
		if( NULL == pN1 )
		{
			pN1 = NewNode( proName, Process );
			InsertAt( pN, pN1);
			pN = pN->Next;
		}
		from = pN1->Vertex;
		
		//如果顶点信息表中不包括该资源则插入该资源
		pN1 = FindNodeByNameType( resName, Resource );
		if( NULL == pN1 )
		{
			pN1 = NewNode( resName, Resource );
			InsertAt( pN, pN1);
			pN = pN->Next;
		}
		to = pN1->Vertex;
		
		//在资源分配图上连通这两个顶点
		g_Graph[from][to] = 1;
	}
	
	printf("================\n");
	fclose( pF );
}

//根据用户输入，建立资源分配图
void BuildGraphFromUserInput()
{
	int i = 0;
	int j = 0;
	for( j=0; j<MAX_VERTEX; j++ )
		for( i=0; i<MAX_VERTEX; i++)
			g_Graph[i][j] = 0;
	
	g_VList.Next = NULL;
	g_VList.Prev = NULL;
	
	_1_BuildGraphFromUserInput();
	_2_BuildGraphFromUserInput();
}

//屏幕打印路径
void PrintPath( int *path )
{
	int i = 0;
	V_NODE *pN = NULL;
	printf("======================================================================\n");
	printf("                          死锁环路示意图                              \n");
	printf("----------------------------------------------------------------------\n");
	for( i=0; path[i]!= PATH_END; i++)
	{
		pN = FindNodeByVertex(path[i]);
		if( pN->Type == Process )
			if( path[i+1] == PATH_END )
				printf("(%s)",pN->Name);
			else
				printf("(%s)-w->",pN->Name);
		else
			if( path[i+1] == PATH_END )
					printf("[%s]",pN->Name);
			else
				printf("[%s]-h->",pN->Name);
	}
	printf("\n");
	printf("----------------------------------------------------------------------\n");
	printf("符号释义:|  ()进程  |  []资源  |  -w->等待  |  -h->被占用             \n");
	printf("======================================================================\n");
}

void main()
{
	int i = 0;
	int path[MAX_PATH];
		
	BuildGraphFromUserInput();
	
	//从每个顶点出发，试图找到环路
	for( i = 0; i<MAX_VERTEX; i++ )
	{
		//如果从i出发可以到达i，则说明存在环路
		if( DFS(i,i,path) )
		{
			printf("存在死锁,死锁环路如下：\n");
			PrintPath( path );
			return;
		}
	}
	
	printf("测试完毕，不存在死锁。");
}