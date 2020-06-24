#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef enum
{
	FALSE = 0,
	TRUE = !FALSE
}BOOL;
#define MAX_STRLEN	256

//作业节点
typedef struct _job_node
{
	char		Name[MAX_STRLEN];
	int			EnterTime;				//作业进入输入井的时间，是从0:00起计算的分钟数
	int			SchemeRunTime;		//作业预计运行时间
	int			IsFinished;				//作业是否完成
	int			WaitTime;					//作业等待时间
	double	Rate;							//作业响应比
	struct _job_node	*Next;
	struct _job_node	*Prev;
}JOB_NODE;

//新建作业节点
JOB_NODE *NewJobNode( const char *name, int et, int srt )
{
	JOB_NODE *pN = (JOB_NODE*)malloc( sizeof( JOB_NODE ) );
	pN->EnterTime = et;
	pN->SchemeRunTime = srt;
	pN->IsFinished = FALSE;
	pN->Rate = 1;
	pN->WaitTime = 0;
	strcpy( pN->Name, name );
	pN->Next = NULL;
	pN->Prev = NULL;
}

//在*pP后插入*pN
void InsertAt( JOB_NODE *pP, JOB_NODE *pN )
{
	pN->Next = pP->Next;
	if( pN->Next != NULL )
		pN->Next->Prev = pN;
		
	pP->Next = pN;
	pN->Prev = pP;
}

//将该结点从链中独立出来
void Isolate( JOB_NODE *pN )
{
	if( pN->Next!= NULL )
		pN->Next->Prev = pN->Prev;
	if( pN->Prev != NULL )
		pN->Prev->Next = pN->Next;
		
	pN->Next = NULL;
	pN->Prev = NULL;
}

//销毁节点
void DelJobNode( JOB_NODE *pN )
{
	free( pN );
}

//作业链表
typedef JOB_NODE JOB_LIST;

JOB_LIST	g_JList;					//存放输入井中的所有作业
int				g_Time = 0;				//系统时间，是从0:00起计算的分钟数
int				g_StartTime = 0;	//系统开始调度作业的时间，是从0:00起计算的分钟数


//按照最高响应比优先的原则找到下一份未处理作业
//如果作业都被完成则返回NULL
JOB_NODE *FindNextJob()
{
	JOB_NODE *pN = g_JList.Next;
	JOB_NODE *pMaxRate = NULL;
	
	while( pN != NULL )
	{
		if( (!pN->IsFinished) && ( pMaxRate == NULL ||( pN->Rate > pMaxRate->Rate ) ))
		{
			pMaxRate = pN;
		}
		pN = pN->Next;
	}
	return pMaxRate;
}

//运行作业
//运行结束后，更新系统时间
void DoJobAndUpdateTime( JOB_NODE *pN )
{
	//完成工作
	pN->IsFinished = TRUE;
	
	//更新系统时间
	g_Time += pN->SchemeRunTime;
}

//更新每个作业的等待时间和响应比
void UpdateJobs()
{
	JOB_NODE *pN = g_JList.Next;
	
	while( pN != NULL )
	{
		if( ! pN->IsFinished )
		{
			pN->WaitTime = g_Time - pN->EnterTime;
			pN->Rate = (pN->WaitTime + pN->SchemeRunTime)/pN->SchemeRunTime;
		}
		pN = pN->Next;
	}
}

//根据用户输入的文件，建立作业链
void BuildJListByUserInput()
{
	char 			fileName[MAX_STRLEN];
	FILE 			*pF = NULL;
	JOB_NODE	*pN = NULL;
	char			jobName[MAX_STRLEN];
	int  			srt = 0;
	int  			et = 0;
	
	//请求用户输入
	for(;;)
	{
		printf("请输入文件名：");
		scanf("%s",fileName);
		pF = fopen( fileName, "r");
		if( pF != NULL )
			break;
		printf("无法打开该文件，请确认文件名是否正确。\n");
	}
	
	//从文件读取数据初始化作业链
	g_JList.Next = NULL;
	g_JList.Prev = NULL;
	pN = &g_JList;
	while( !feof( pF ) )
	{
		fscanf( pF,"%s %i %i",jobName,&et,&srt);
		InsertAt( pN, NewJobNode( jobName, et, srt ));
		pN = pN->Next;
		if( et > g_StartTime )//最后一个作业进入输入井的时刻，为系统开始调度的时刻
			g_StartTime = et;
	}
	
	g_Time = g_StartTime;
	//初始化各各个作业的等待时间和响应比
	UpdateJobs();
	
	fclose( pF );
}

//调度作业，同时屏幕打印调度表
void Schemduling()
{
	JOB_NODE *pN = FindNextJob();
	int jobCount = 0;
	int totalTime = 0;
	
	printf("==========================================================================\n");
	printf("                         作业调度表-最高响应比优先                        \n");
	printf("--------------------------------------------------------------------------\n");
	printf("作业名称 | 进入时间 | 开始时间 | 结束时间 | 等待时长 | 运行时长 | 周转时长\n");
	while( pN != NULL )
	{
		printf("%8s | %8i | %8i | ",pN->Name, pN->EnterTime, g_Time);
		DoJobAndUpdateTime( pN );
		printf("%8i | %8i | %8i | %8i\n",g_Time, pN->WaitTime, pN->SchemeRunTime, g_Time - pN->EnterTime, g_Time - pN->EnterTime);
		totalTime += (g_Time - pN->EnterTime);
		jobCount ++;
		UpdateJobs();
		pN = FindNextJob();
	}
	printf("--------------------------------------------------------------------------\n");
	printf("平均周转时长 | %i\n",totalTime / jobCount);
	printf("==========================================================================\n");
}

void main()
{
	BuildJListByUserInput();
	Schemduling();
}




