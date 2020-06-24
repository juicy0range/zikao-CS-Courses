#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef enum
{
	FALSE = 0,
	TRUE = !FALSE
}BOOL;
#define MAX_STRLEN	256

#define CLUSTER_SIZE			10								//每组含有的块数
#define BLOCK_SIZE				CLUSTER_SIZE + 1	//每块可容纳的字段数
#define MAX_JOB_BLOCKS		256								//每作业最大分配盘快量

typedef struct _job_node
{
	char	Name[MAX_STRLEN];					//作业名
	int		BlockCount;								//作业占用盘块总数
	int		BlockIds[MAX_JOB_BLOCKS];	//作业占用的盘块号
	struct _job_node *Next;
	struct _job_node *Prev;
}JOB_NODE;

//新建作业节点
JOB_NODE *NewJobNode( const char* name, const int *bIds, int count )
{
	JOB_NODE *pN = ( JOB_NODE * )malloc( sizeof( JOB_NODE ) );
	pN->Next = NULL;
	pN->Prev = NULL;
	strcpy( pN->Name, name );
	for( pN->BlockCount = 0; pN->BlockCount < count; pN->BlockCount++ )
		pN->BlockIds[ pN->BlockCount ] = bIds[ pN->BlockCount ];
	return pN;
}

//销毁作业节点
void DelJobNode( JOB_NODE *pN )
{
	free( pN );
}

//在*pP后插入*pN
void InsertJobNodeAt( JOB_NODE *pP, JOB_NODE *pN )
{
	pN->Next = pP->Next;
	if( pN->Next != NULL )
		pN->Next->Prev = pN;
	
	pN->Prev = pP;
	pP->Next = pN;
}

//孤立节点，将其前驱和后继直接相连
void IsolateJobNode( JOB_NODE *pN )
{
	if( pN->Next != NULL )
		pN->Next->Prev = pN->Prev;
	if( pN->Prev != NULL )
		pN->Prev->Next = pN->Next;
	
	pN->Next = NULL;
	pN->Prev = NULL;
}

//作业链
typedef JOB_NODE JOB_LIST;

JOB_LIST g_Jobs;//存放系统中所有作业

//根据作业名查找作业
//找到返回该作业节点的指针，否则返回NULL
JOB_NODE *FindJobByName( const char * name )
{
	JOB_NODE *pN = g_Jobs.Next;
	while( pN != NULL )
	{
		if( strcmp( pN->Name, name ) == 0 )
			break;
		pN = pN->Next;
	}
	return pN;
}

typedef struct _block_node
{
	int	Id;								//盘块号
	int Data[BLOCK_SIZE];	//如果是每组首块的话，Data[0]中存放下一组的盘块总数,其余存放下一组所有块的Id
												//而且如果下一组是末组，则Data[1]==0
	struct _block_node *Next;
	struct _block_node *Prev;
}BLOCK_NODE;

//建块节点
BLOCK_NODE *NewBlockNode( int id )
{
	BLOCK_NODE *pN = ( BLOCK_NODE * )malloc( sizeof( BLOCK_NODE ));
	pN->Id = id;
	pN->Data[0] = 0;
	pN->Next = NULL;
	pN->Prev = NULL;
	return pN;
}

//将*pS的Data复制给*pT
//返回值和pT相同
BLOCK_NODE * CopyData( BLOCK_NODE *pT ,const BLOCK_NODE *pS )
{
	int i = 0;
	for( i = 0; i< BLOCK_SIZE ; i++ )
		pT->Data[i] = pS->Data[i];
	
	return pT;
}

//销毁块节点
void DelBlockNode( BLOCK_NODE *pN )
{
	free( pN );
}

//在*pP后插入*pN
void InsertBlockNodeAt( BLOCK_NODE *pP, BLOCK_NODE *pN )
{
	pN->Next = pP->Next;
	if( pN->Next != NULL )
		pN->Next->Prev = pN;
	
	pP->Next = pN;
	pN->Prev = pP;
}

//孤立块节点，将其前驱和后继直接相连
void IsolateBlockNode( BLOCK_NODE *pN )
{
	if( pN->Next != NULL )
		pN->Next->Prev = pN->Prev;
	if( pN->Prev != NULL )
		pN->Prev->Next = pN->Next;
	
	pN->Next = NULL;
	pN->Prev = NULL;
}

//判断该块所指向的组是否为最后一组
BOOL IsLastCluster( BLOCK_NODE *pN )
{
	return pN->Data[1] == 0;
}

//判断该块指向的组是否已满
BOOL IsClusterFull( BLOCK_NODE *pN )
{
	if( IsLastCluster( pN ) )
		return pN->Data[0] == ( CLUSTER_SIZE - 1 );
	else 
		return pN->Data[0] == CLUSTER_SIZE;
}

//块链
typedef BLOCK_NODE BLOCK_LIST;

BLOCK_LIST	g_HeadBlocks;			//用于存放磁盘上全部首块
BLOCK_NODE	g_SpecialBlock;		//内存中的专用块，内包括磁盘中第一组的所有块Id
int					g_FreeBlockCount;	//空闲块总数

//根据Id查找首块
//成功返回该首块的节点指针，否则返回NULL
BLOCK_NODE *FindHeadBlockById( int id )
{
	BLOCK_NODE *pN = g_HeadBlocks.Next;
	
	while( pN != NULL )
	{
		if( pN->Id == id )
			break;
		pN = pN->Next;
	}
	return pN;
}


//增加一个空闲快
void FreeOne( int id )
{
	//如果专用块指向的组尚有空闲，则直将该空闲块Id记录在该组
	if( !IsClusterFull( &g_SpecialBlock ) )
	{
		if( IsLastCluster (&g_SpecialBlock) )//注意如果是最后一组的情况,越过Data[0]标记位
		{
			g_SpecialBlock.Data[ g_SpecialBlock.Data[0] + 2 ] = id;
			g_SpecialBlock.Data[0]++;
		}
		else
		{
			g_SpecialBlock.Data[ g_SpecialBlock.Data[0] + 1 ] = id;
			g_SpecialBlock.Data[0]++;
		}
	}
	else//如果专用块指向的组已满，则将专用块数据写入新增的空闲块，并将此块作为新一组的首块,然后专用块指向新组
	{
		BLOCK_NODE *pN = NewBlockNode( id );
		CopyData( pN, &g_SpecialBlock );
		InsertBlockNodeAt( &g_HeadBlocks, pN );
		g_SpecialBlock.Data[0] = 1;
		g_SpecialBlock.Data[1] = id;
	}
}

//分配一个空闲块
//成功返回此块的Id，失败返回0
int AllocateOne( )
{
	int id = 0;
	if( g_SpecialBlock.Data[0] == 1 )//如果本组仅剩一块
		if( IsLastCluster( &g_SpecialBlock ) )//且如果是最后一组，则将仅剩的一块分配
		{
			id = g_SpecialBlock.Data[2];
			g_SpecialBlock.Data[0] = 0;
			return id;
		}
		else//且如果不是最后一组，则将此快数据备份到专用快，然后分配此块
		{
			BLOCK_NODE *pN = NULL;
			id = g_SpecialBlock.Data[1];
			pN = FindHeadBlockById( id );
			CopyData( &g_SpecialBlock, pN );
			IsolateBlockNode( pN );
			DelBlockNode( pN );
			return id;
		}
	else//如果本组还剩不只一块，直接分配
		if( IsLastCluster( &g_SpecialBlock ) )//注意最后一组的特殊情况
		{
			id = g_SpecialBlock.Data[ g_SpecialBlock.Data[0] + 1 ];
			g_SpecialBlock.Data[0] --;
			return id;
		}
		else
		{
			id = g_SpecialBlock.Data[ g_SpecialBlock.Data[0]];
			g_SpecialBlock.Data[0] --;
			return id;
		}			
}

//从用户输入的文件初始化专用快、首块链表、空闲块总数
void InitBlocksFromUserInput()
{
	char fileName[MAX_STRLEN];
	FILE *pF = NULL;
	
	//请求用户输入文件名
	for( ;; )
	{
		printf("请输入文件名：");
		scanf( "%s", fileName );
		pF = fopen( fileName, "r" );
		if( pF != NULL )
			break;
		printf("无法打开该文件，请检查输入的文件名是否正确。\n");
	}
	
	//从文件读取数据
	g_FreeBlockCount = 0;
	g_SpecialBlock.Data[1] = 0;//当前组为最后一组
	g_SpecialBlock.Data[0] = 0;//当前组含有0块
	g_HeadBlocks.Next = NULL;
	g_HeadBlocks.Prev = NULL;
	while( !feof( pF ) )
	{
		int id = 0;
		fscanf( pF, "%i",&id);
		FreeOne( id );
		g_FreeBlockCount ++;
	}
	fclose( pF );
}

//根据用户的输入添加作业，分配磁盘
void Allocate( )
{
	char	jobName[MAX_STRLEN];
	int 	blockCount = 0;
	int		ids[MAX_JOB_BLOCKS];
	int		i = 0;
	JOB_NODE *pJN = NULL;
	
	printf("请输入作业名：");
	scanf("%s",jobName);
	
	printf("请输入分配的块数：");
	scanf("%i",&blockCount);
	if( blockCount > g_FreeBlockCount )
	{
		printf("分配失败，原因：磁盘空间不足。\n");
		return;
	}
	else
		g_FreeBlockCount -= blockCount;
	
	pJN = FindJobByName( jobName );
	
	for( i = 0; i < blockCount; i++ )
		ids[i] = AllocateOne();
	
	if( pJN != NULL )
	{
		for( i = 0; i<blockCount; i++ )
			pJN->BlockIds[ pJN->BlockCount++ ];
		printf("分配成功，向作业%s追加了%i个磁盘块。\n", pJN->Name, blockCount);
	}
	else
	{
		pJN = NewJobNode( jobName, ids, blockCount );
		InsertJobNodeAt( &g_Jobs, pJN );
		printf("分配成功，新建作业%s，并为其分配了%i个磁盘块。\n",pJN->Name, blockCount);
	}	
	
}

//撤销作业
void FinishJob()
{
	char jobName[MAX_STRLEN];
	JOB_NODE *pJN = NULL;
	
	printf("请输入作业名：");
	scanf("%s",jobName);
	
	pJN = FindJobByName( jobName );
	if( pJN == NULL )
		printf("撤销失败，输入的作业名不存在。\n");
	else
	{
		printf("撤销成功，共释放%i个磁盘块。\n",pJN->BlockCount);
		
		g_FreeBlockCount += g_FreeBlockCount;
		
		while( pJN->BlockCount-- > 0 )
			FreeOne( pJN->BlockIds[pJN->BlockCount] );
			
		IsolateJobNode( pJN );
		DelJobNode( pJN );
	}
	
}

//屏幕打印各个工作获得的磁盘块情况
void PrintJobsTable()
{
	JOB_NODE *pJN = g_Jobs.Next;
	int i = 0;
	printf("===============================================================\n");
	printf("                           块分配表                            \n");
	printf("---------------------------------------------------------------\n");
	while( pJN != NULL )
	{
		printf("%6s |",pJN->Name);
		for( i = 0; i< pJN->BlockCount; i++ )
			printf("[%i] ", pJN->BlockIds[i]);
		printf("\n");
		pJN = pJN->Next;
	}
	printf("===============================================================\n");
}

//屏幕打印各组的空闲快
void PrintClustersTable()
{
	BLOCK_NODE *pBN = g_HeadBlocks.Next;
	int i = 0;
	int j = 0;
	
	printf("===============================================================\n");
	printf("                           空闲块表                            \n");
	printf("---------------------------------------------------------------\n");
	printf("%4i组 | ", ++j);
	if( IsLastCluster( &g_SpecialBlock ) )
		for( i = 0; i< g_SpecialBlock.Data[0]; i++ )
			printf("[%i] ", g_SpecialBlock.Data[i+2]);
	else
		for( i = 0; i< g_SpecialBlock.Data[0]; i++ )
			printf("[%i] ", g_SpecialBlock.Data[i+1]);
	printf("\n");
	
	while( pBN != NULL )
	{
		printf("%4i组 | ", ++j);
		if( IsLastCluster( pBN ) )
			for( i = 0; i< pBN->Data[0]; i++ )
				printf("[%i] ", pBN->Data[i+2]);
		else
			for( i = 0; i< pBN->Data[0]; i++ )
				printf("[%i] ", pBN->Data[i+1]);
		printf("\n");
		
		pBN = pBN->Next;
	}
	printf("===============================================================\n");
	
}

//屏幕打印用户菜单
void PrintUserMenu()
{
	printf("===============================================================\n");
	printf("                           命令菜单                            \n");
	printf("---------------------------------------------------------------\n");
	printf("1|申请磁盘空间 2|撤销作业 3|查看块分配表 4|查看空闲块表 0|退出 \n");
	printf("===============================================================\n");
	printf("请选择：");
}

//执行用户输入的命令
void ExecUserCommand()
{
	char command[MAX_STRLEN];
	for(;;)
	{
		PrintUserMenu();
		scanf("%s",command);
		switch( command[0] )
		{
			case '0':exit(0);
			case '1':Allocate();break;
			case '2':FinishJob();break;
			case '3':PrintJobsTable();break;
			case '4':PrintClustersTable();break;
			default:printf("您输入的选项不存在，请重新选择。\n");
		}
	}
}

void main()
{
	InitBlocksFromUserInput();
	ExecUserCommand();
}
