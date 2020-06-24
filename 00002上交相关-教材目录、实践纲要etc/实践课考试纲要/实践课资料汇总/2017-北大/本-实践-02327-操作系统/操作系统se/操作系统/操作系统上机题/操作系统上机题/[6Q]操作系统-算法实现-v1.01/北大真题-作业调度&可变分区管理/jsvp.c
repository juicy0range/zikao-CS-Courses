#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef enum
{
	FALSE = 0,
	TRUE = !FALSE
}BOOL;
#define MAX_STRLEN	256

#define MEMORY_SIZE 	100 //内存总量(K)
#define MEMORY_START	0 	//内存起始地址

//时间转换函数，将时间转换为分钟计数
int T2M( int i)
{
	return (i/100)*60 + i%100;
}
//时间转换函数，将分钟计数转换为时间
int M2T( int i)
{
	return (i/60)*100 + i%60;
}

//可变分区节点
typedef struct _vp_node
{
	int Start;					//起始地址
	int End;						//结束地址
	int Size;						//分区大小
	BOOL IsFree;				//是否可用
	struct _vp_node *Next;
	struct _vp_node *Prev;
}VP_NODE;

//新建可变分区节点
VP_NODE *NewVpNode( int start, int size, BOOL isFree )
{
	VP_NODE *pN = ( VP_NODE * )malloc( sizeof( VP_NODE ) );
	
	pN->Next = NULL;
	pN->Prev = NULL;
	pN->Start = start;
	pN->Size = size;
	pN->End = pN->Start + size;
	pN->IsFree = isFree;
	
	return pN;
}
//销毁可变分区节点
void DelVpNode( VP_NODE *pN )
{
	free( pN );
}

//在*pP后插入*pN
void InsertVpNodeAt( VP_NODE *pP, VP_NODE *pN )
{
	pN->Next = pP->Next;
	if( pN->Next != NULL )
		pN->Next->Prev = pN;
		
	pN->Prev = pP;
	pP->Next = pN;
}

//孤立节点，将其前驱后继直接相连
void IsolateVpNode( VP_NODE *pN )
{
	if( pN->Next != NULL )
		pN->Next->Prev = pN->Prev;
	if( pN->Prev != NULL )
		pN->Prev->Next = pN->Next;
	
	pN->Next = NULL;
	pN->Prev = NULL;
}

//可变分区链
typedef VP_NODE VP_LIST;

VP_LIST g_Vps;//存储系统中所有可变分区

//按照内存的大小初始化可变分区
void InitVps()
{
	g_Vps.Next = NULL;
	g_Vps.Prev = NULL;
	InsertVpNodeAt( &g_Vps, NewVpNode( MEMORY_START, MEMORY_SIZE,TRUE ) );
}

//根据起始地址查找可用分区节点
//成功返回该分区，失败返回NULL
VP_NODE *FindFreeVpByStart( int start )
{
	VP_NODE *pN = g_Vps.Next;
	
	while( pN )
	{
		if( pN->IsFree && pN->Start == start )
			break;
		pN = pN->Next;
	}
	return pN;
}

//根据结束地址查找可用分区节点
//成功返回该分区，失败返回NULL
VP_NODE *FindFreeVpByEnd( int end )
{
	VP_NODE *pN = g_Vps.Next;
	
	while( pN )
	{
		if( pN->IsFree && pN->End == end )
			break;
		pN = pN->Next;
	}
	return pN;
}

//释放分区
void FreeVp( VP_NODE *pN )
{
	VP_NODE *pN1 = FindFreeVpByEnd( pN->Start );//上邻分区
	VP_NODE *pN2 = FindFreeVpByStart( pN->End );//下邻分区
	
	if( pN1 != NULL )//与上邻融合
	{
		pN1->Size = pN1->Size + pN->Size;
		pN1->End = pN->End;
		IsolateVpNode( pN );
		DelVpNode( pN );
		pN = pN1;
	}
	if( pN2 != NULL )//与下邻居融合
	{
		pN->Size = pN->Size + pN2->Size;
		pN->End = pN2->End;
		IsolateVpNode( pN2 );
		DelVpNode( pN2 );
	}
	pN->IsFree = TRUE;
}

//作业节点
typedef struct _job_node
{
	char	Name[MAX_STRLEN];	//作业名
	int		RunTime;					//作业运行时间
	int		EnterBufTime;			//作业进入输入井的时间
	int		EnterMemTime;			//作业进入内存的时间
	int		StartTime;				//作业开始运行的时间
	int		FinishTime;				//作业完成的时间
	int		MemSize;					//作业占用内存大小
	BOOL	IsFinished;				//是否被完成
	VP_NODE *Vp;						//作业占用的内存可变分区
	struct _job_node *Next;
	struct _job_node *Prev;
}JOB_NODE;

//新建作业节点
//新建的作业节点，默认为未完成，未分配内存
JOB_NODE *NewJobNode( const char *name, int et, int rt, int ms )
{
	JOB_NODE *pN = ( JOB_NODE * )malloc( sizeof( JOB_NODE ) );
	pN->Next = NULL;
	pN->Prev = NULL;
	pN->RunTime = rt;
	pN->EnterBufTime = et;
	pN->MemSize = ms;
	pN->IsFinished = FALSE;
	pN->Vp = NULL;
	pN->StartTime = 0;
	pN->EnterMemTime = 0;
	strcpy( pN->Name, name );
	return pN;
}

//销毁作业节点
void DelJobNode( JOB_NODE *pN )
{
	free( pN );
}

//在*pP之后插入*pN
void InsertJobNodeAt( JOB_NODE *pP, JOB_NODE *pN )
{
	pN->Next = pP->Next;
	if( pN->Next != NULL )
		pN->Next->Prev = pN;
	
	pN->Prev = pP;
	pP->Next = pN;
}

//孤立节点，将其前驱后继直接相连
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

JOB_LIST	g_OuterJobs;			//等待进入输入井的作业
JOB_LIST	g_BufJobs;				//输入井中的作业
JOB_LIST	g_MemJobs;				//内存中的作业
JOB_LIST	g_FinishedJobs;		//结束的作业
int				g_Time;						//系统时间

//未进入输入井的作业按进入输入井的时间排序
void SortOuterJobsByEnterBufTime( )
{
	JOB_NODE *pN = NULL;
	
	int f = 0;
	
	for(;;)
	{
		pN = g_OuterJobs.Next;
		f = 0;
		while( pN->Next != NULL )
		{
			if( pN->Next->EnterBufTime < pN->EnterBufTime )
			{
				JOB_NODE *pN1 = pN->Next;
				IsolateJobNode( pN );
				InsertJobNodeAt( pN1, pN );
				f++;
				continue;
			}
			pN = pN->Next;
		}
		if( f == 0 )
			break;
	}	
}


//输入井中的作业按照运行时间排序，短作业在前
void SortBufJobsByRunTime()
{
	JOB_NODE *pN = NULL;
	
	int f = 0;
	
	for(;;)
	{
		pN = g_BufJobs.Next;
		f = 0;
		while( pN->Next != NULL )
		{
			if( pN->Next->RunTime < pN->RunTime )
			{
				JOB_NODE *pN1 = pN->Next;
				IsolateJobNode( pN );
				InsertJobNodeAt( pN1, pN );
				f++;
				continue;
			}
			pN = pN->Next;
		}
		if( f == 0 )
			break;
	}
}
//根据用户的输入文件，初始化g_OuterJobs
void BuildOuterJobsFromUserInput()
{
	char fileName[MAX_STRLEN];
	FILE *pF = NULL;
	JOB_NODE *pJN = NULL;
	
	for(;;)
	{
		printf("请输入文件名：");
		scanf("%s",fileName);
		pF = fopen( fileName, "r" );
		if( pF != NULL )
			break;
		printf("无法打开该文件，请检查输入的文件名是否正确。");
	}
	
	g_OuterJobs.Next = NULL;
	g_OuterJobs.Prev = NULL;
	pJN = &g_OuterJobs;
	while( !feof( pF ) )
	{
		char jobName[MAX_STRLEN];
		int et,rt,ms;
		fscanf( pF, "%s %i %i %i", jobName,&et, &rt, &ms );
		InsertJobNodeAt( pJN, NewJobNode( jobName, T2M(et), rt, ms ) );
		pJN = pJN->Next;
	}
	fclose( pF );
}

//测试当前内存是否有满足该作业的运行
BOOL TestAllocateMemForJob( JOB_NODE * pJN )
{
	VP_NODE *pVN = g_Vps.Next;
	
	while( pVN != NULL )
	{
		if( pVN->Size >= pJN->MemSize && pVN->IsFree == TRUE )
			return TRUE;
		
		pVN = pVN->Next;
	}
	return FALSE;
}
//为作业分配内存
BOOL AllocateMemForJob( JOB_NODE * pJN )
{
	VP_NODE *pVN = g_Vps.Next;
	
	while( pVN != NULL )
	{
		if( pVN->Size == pJN->MemSize && pVN->IsFree == TRUE )
		{
			pJN->Vp = pVN;
			pVN->IsFree = FALSE;
			return TRUE;
		}
		if( pVN->Size > pJN->MemSize && pVN->IsFree == TRUE )
		{
			VP_NODE *pVN1 = NewVpNode( pVN->Start + pJN->MemSize , pVN->Size - pJN->MemSize,TRUE );
			pVN->End = pVN1->Start;
			pVN->Size = pJN->MemSize;
			InsertVpNodeAt( pVN, pVN1 );
			pJN->Vp = pVN;
			pVN->IsFree = FALSE;
			return TRUE;
		}
		pVN = pVN->Next;
	}
	return FALSE;
}

//从输入井中找到第一个可以在当前内存状况下运行的程序
JOB_NODE *FindExecutableJobFromBuf( )
{
	JOB_NODE *pN = g_BufJobs.Next;
	
	while( pN != NULL )
	{
		if( TestAllocateMemForJob( pN ) )
			return pN;
		pN = pN->Next;
	}
	return NULL;
}

//作业进入输入井
void EnterBuf( JOB_NODE *pJN )
{
	JOB_NODE *pJN1 = &g_BufJobs;
	IsolateJobNode( pJN );
	while( pJN1->Next != NULL )
		pJN1 = pJN1->Next;
	InsertJobNodeAt( pJN1,pJN );
}

//作业进入内存
void EnterMem( JOB_NODE *pJN )
{
	JOB_NODE *pJN1 = &g_MemJobs;
	IsolateJobNode( pJN );
	pJN->EnterMemTime = g_Time;
	
	//如果此时内存中没有作业，则进入内存即开始处理该作业
	if( pJN1->Next == NULL )
		pJN->StartTime = g_Time;
	
	//接到内存队列尾部
	while( pJN1->Next != NULL )
		pJN1 = pJN1->Next;
	InsertJobNodeAt( pJN1,pJN );

	//分配内存分区
	AllocateMemForJob( pJN );
}

//查看内存中作业总数
int CountMemJobs( )
{
	int i=0;
	JOB_NODE *pJN = g_MemJobs.Next;
	
	while( pJN != NULL )
	{
		i++;
		pJN = pJN->Next;
	}
	return i;
}

//完成作业
void FinishJob( JOB_NODE *pJN )
{
	JOB_NODE *pJN1 = &g_FinishedJobs;
	IsolateJobNode( pJN );
	pJN->FinishTime = g_Time;

	//插入到完成队列尾
	while( pJN1->Next != NULL )
		pJN1 = pJN1->Next;
	InsertJobNodeAt( pJN1,pJN );

	//释放内存分区
	FreeVp( pJN->Vp );
	pJN->Vp = NULL;
}

//执行作业
void DoJobs()
{
	//如果内存中第一个作业的运行时间满，则结束第一个作业，开始处理第二个作业
	if( g_MemJobs.Next != NULL &&  g_Time == (g_MemJobs.Next->StartTime + g_MemJobs.Next->RunTime))
	{
		FinishJob( g_MemJobs.Next );//结束第一个作业
		if( g_MemJobs.Next != NULL )//开始处理第二个作业
			g_MemJobs.Next->StartTime = g_Time;
	}
}

//屏幕打印作业调度表
void PrintFinishedJobs()
{
	JOB_NODE *pJN = g_FinishedJobs.Next;
	int totalTime = 0;
	int jobCount = 0;
	
	printf("=======================================================================\n");
	printf("                            作业调度表                                 \n");
	printf("-----------------------------------------------------------------------\n");
	printf("作业名 | 进入输入井 | 进入内存 | 开始执行 | 完成 | 运行时长 | 周转时长\n");
	
	while( pJN != NULL )
	{
		printf("%6s | %10i | %8i | %8i | %4i | %8i | %8i\n",
			pJN->Name,
			M2T( pJN->EnterBufTime ),
			M2T( pJN->EnterMemTime ),
			M2T( pJN->StartTime ),
			M2T( pJN->FinishTime ),
			pJN->RunTime,
			pJN->FinishTime - pJN->EnterBufTime);

		totalTime += (pJN->FinishTime - pJN->EnterBufTime);
		jobCount ++;
		pJN = pJN->Next;
	}
	printf("-----------------------------------------------------------------------\n");
	printf("平均周转时长（分钟）| %i\n",totalTime/jobCount );
	printf("=======================================================================\n");
}


void main()
{
	
	InitVps();
	BuildOuterJobsFromUserInput();
	SortOuterJobsByEnterBufTime();
	
	for(g_Time=0;g_Time<24*60; g_Time++)//计时
	{
		//执行内存中的作业
		DoJobs();

		//如果等候进入输入井的作业不为空，且某个作业进入输入井的时间到，则该作业进入输入井
		if( g_OuterJobs.Next != NULL && g_Time == g_OuterJobs.Next->EnterBufTime )
		{
			EnterBuf( g_OuterJobs.Next );
			//井中作业重新排序，短作业在前
			SortBufJobsByRunTime();
		}
		
		//输入井不为空，且内存中的作业数少于2，则作业从输入井进入内存
		if( g_BufJobs.Next != NULL && CountMemJobs() < 2 )
		{
			JOB_NODE *pN = FindExecutableJobFromBuf();
			if( pN != NULL)
				EnterMem( pN );
		}
	}

	PrintFinishedJobs();
}