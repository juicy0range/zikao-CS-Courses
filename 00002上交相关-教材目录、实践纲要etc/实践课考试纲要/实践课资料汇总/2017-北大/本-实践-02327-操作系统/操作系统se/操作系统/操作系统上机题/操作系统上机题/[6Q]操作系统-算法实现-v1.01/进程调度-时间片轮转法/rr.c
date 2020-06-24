#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef enum
{
	FALSE = 0,
	TRUE = !FALSE
}BOOL;
#define MAX_STRLEN	256

#define TIME_SLICE	3		//时间片长度

typedef struct _process_node
{
	char		Name[MAX_STRLEN];	//进程名
	int			SchemeRunTime;		//预计运行时间
	int			RunTime;					//已运行时间
	int			WaitTime;					//已等待时间
	BOOL		IsFinished;				//是否完成
	struct _process_node *Next;
	struct _process_node *Prev;
}P_NODE;

//新建节点
P_NODE *NewPNode( const char *name, int srt )
{
	P_NODE *pN = (P_NODE*)malloc( sizeof(P_NODE) );
	pN->Next = NULL;
	pN->Prev = NULL;
	pN->IsFinished = FALSE;
	pN->RunTime = 0;
	pN->WaitTime = 0;
	pN->SchemeRunTime = srt;
	strcpy( pN->Name, name );
	return pN;
}

//销毁节点
void DelPNode( P_NODE *pN )
{
	free( pN );
}

//分离节点
//解除该节点同前序、后继的关系，将前序后继直接相连
void Isolate( P_NODE *pN )
{
	if( pN->Next != NULL )
		pN->Next->Prev = pN->Prev;
		
	if( pN->Prev != NULL )
		pN->Prev->Next = pN->Next;
}

//在*pP后插入*pN
void InsertAt( P_NODE *pP, P_NODE *pN )
{
	pN->Next = pP->Next;
	if( pN->Next != NULL )
		pN->Next->Prev = pN;

	pN->Prev = pP;
	pP->Next = pN;
}

//进程链表
typedef P_NODE P_LIST;

P_LIST g_PList;//存放系统中全部进程

//初始化就绪进程队列
void InitQuene()
{
	g_PList.Next = NULL;
	g_PList.Prev = NULL;
}

//入队
void EnQuene( P_NODE *pN )
{
	P_NODE *pRear = &g_PList;
	
	while( pRear->Next != NULL )
		pRear = pRear->Next;
	
	InsertAt( pRear, pN );
}

//出队
//当队列空时返回NULL
P_NODE *DeQuene( )
{
	P_NODE *pN = g_PList.Next;
	
	if( pN != NULL )
		Isolate( g_PList.Next );
	
	return pN;
}


//根据用户提供的输入文件，初始化进程链表
void BuildPListFromUserInput()
{
	char fileName[MAX_STRLEN];
	char procName[MAX_STRLEN];
	int  srt = 0;
	FILE *pF = NULL;
	
	//请求用户输入
	for( ;; )
	{
		printf("请输入文件名:");
		scanf("%s", fileName );
		pF = fopen( fileName, "r");
		if( pF != NULL )
			break;
		printf("无法打开该文件，请确认输入的文件名是否正确。\n");
	}
	
	//读取文件
	InitQuene();
	while( !feof( pF ) )
	{
		fscanf( pF,"%s %i",procName, &srt );
		EnQuene( NewPNode( procName, srt ) );
	}	
	fclose( pF );
}

void PrintScheduingTable()
{
	int			totalWaitTime = 0;
	int			totalProc     = 0;
	int			timeslice			= 0; //每次实际分配的时间
	P_NODE *pN = DeQuene();
	
	printf("==============================================================\n");
	printf("                    进程调度表-时间片轮转法                   \n");
	printf("--------------------------------------------------------------\n");
	printf("进程名 | 本次运行时间 | 累计运行时间 | 累计等待时间 | 是否完成\n");
	
	while( pN != NULL )
	{
		if( g_PList.Next == NULL )//如果是最后一个进程,则分配该进程所需剩余时间
			timeslice =  pN->SchemeRunTime - pN->RunTime;
		else//如果剩余运行时间小于时间片，则本次只分配所需剩余时间
			timeslice = (pN->SchemeRunTime - pN->RunTime) < TIME_SLICE ? (pN->SchemeRunTime - pN->RunTime) : TIME_SLICE;
		
		pN->RunTime += timeslice;//更新运行时间
		
		if( pN->RunTime >= pN->SchemeRunTime )
			pN->IsFinished = TRUE;
		
		printf("%6s | %12i | %12i | %12i | %8s\n",pN->Name, timeslice, pN->RunTime, pN->WaitTime,pN->IsFinished?"完成":"未完成" );
		
		
		{ //更新队列中余下进程的等待时间
			P_NODE *pN1 = g_PList.Next;
			while( pN1 != NULL )
			{
				pN1->WaitTime += timeslice;
				pN1 = pN1->Next;
			}
		}
		
		//若未完成进入队尾,已完成就不再入队
		if( !pN->IsFinished )
			EnQuene(pN);
		else
		{
			totalWaitTime += pN->WaitTime;
			totalProc ++;
			DelPNode( pN );
		}
		
		//下一个进程出队
		pN = DeQuene();
	}
	
	printf("--------------------------------------------------------------\n");
	printf("每进程平均累计等待时间 | %i\n",totalWaitTime/totalProc);
	printf("==============================================================\n");
}

void main()
{
	BuildPListFromUserInput();
	PrintScheduingTable();
}