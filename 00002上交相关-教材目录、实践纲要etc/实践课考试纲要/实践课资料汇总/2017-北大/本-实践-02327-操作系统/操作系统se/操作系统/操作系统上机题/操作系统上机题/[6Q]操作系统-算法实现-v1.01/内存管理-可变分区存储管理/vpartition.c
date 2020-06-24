#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef enum
{
	FALSE = 0,
	TRUE = !FALSE
}BOOL;
#define MAX_STRLEN	256


//可变分区表结点
typedef struct vp_node
{
	int StartAddress;
	int Capacity;
	BOOL IsFree;
	char User[MAX_STRLEN];
	struct vp_node *Next;
	struct vp_node *Prev;
}VP_NODE;

//创建并插入新结点，参数User为占用作业名，当User==NULL时认为该分区为Free
VP_NODE *NewNode( int sa, int c, char *user, VP_NODE *pNext ,VP_NODE *pPrev)
{
	VP_NODE *pN = (VP_NODE*)malloc( sizeof( VP_NODE ) );
	pN->StartAddress = sa;
	pN->Capacity = c;
	
	pN->Next = pNext;
	if( NULL != pNext )
		pNext->Prev = pN;
		
	pN->Prev = pPrev;
	if( NULL != pPrev )
		pPrev->Next = pN;
		
	if( user != NULL )
	{ 
		strcpy( pN->User,user);
		pN->IsFree = FALSE;
	}
	else
		pN->IsFree = TRUE;
		
	return pN;
}

//分离并销毁结点
void DelNode( VP_NODE *pN )
{
	if( pN->Prev != NULL )
		pN->Prev->Next = pN->Next;
	if( pN->Next != NULL )
		pN->Next->Prev = pN->Prev;
	free( pN );
}

//可变分区表
typedef VP_NODE VP_LIST;

VP_LIST g_VpList;

void BuildListFromUserInput()
{
	char fileName[MAX_STRLEN];
	VP_NODE *pN = &g_VpList;
	FILE *pf = NULL;
	
	for(;;)
	{
		printf("请输入文件名：");
		scanf("%s",fileName);
		
		pf = fopen(fileName,"r");
		
		if( NULL == pf )
			printf("打开文件失败，请确认输入是否正确。\n");
		else
			break;
	}
	
	while( !feof(pf) )
	{
		int sa;
		int c;
		fscanf( pf,"%i %i",&sa, &c);
		
		pN->Next = NewNode( sa, c, NULL, NULL, pN );
		pN = pN->Next;
	}
	fclose( pf );
}

//屏幕打印空闲分区表
void PrintFreeTable()
{
	VP_NODE *pN = g_VpList.Next;
	printf("===================\n");
	printf("    空闲分区表     \n");
	printf("-------------------\n");
	printf("起始地址 | 分区长度\n");
	while( NULL != pN )
	{
		if( pN->IsFree )
		printf("%8i | %8i\n",pN->StartAddress, pN->Capacity);
		pN=pN->Next;
	}	
	printf("===================\n");
}

//屏幕打印已分配分区表
void PrintUsedTable()
{
	VP_NODE *pN = g_VpList.Next;
	printf("==============================\n");
	printf("         已分配区表           \n");
	printf("------------------------------\n");
	printf("起始地址 | 分区长度 | 作业名称\n");
	while( NULL != pN )
	{
		if(!pN->IsFree)
			printf("%8i | %8i | %8s\n",pN->StartAddress, pN->Capacity,pN->User);
		pN=pN->Next;
	}	
	printf("===============================\n");
}

//屏幕打印内存分配表
void PrintMemoryTable()
{
	VP_NODE *pN = g_VpList.Next;
	printf("==============================\n");
	printf("         内存分配表           \n");
	printf("------------------------------\n");
	printf("起始地址 | 分区长度 | 分配情况\n");
	while( NULL != pN )
	{
		printf("%8i | %8i | %8s\n",pN->StartAddress, pN->Capacity,pN->IsFree?"空闲":pN->User);
		pN=pN->Next;
	}	
	printf("===============================\n");
}

//最差适应分配
VP_NODE *WorstFitAllocate( char *jobName,int size )
{
	VP_NODE *pMax = NULL;
	VP_NODE *pN = g_VpList.Next;
	VP_NODE *pNew = NULL;
	
	//找到最大的空闲分区
	while( NULL != pN )
	{
		if( pN->IsFree && ( NULL == pMax || pMax->Capacity < pN->Capacity ) )
			pMax = pN;
		pN=pN->Next;
	}
	
	if( NULL==pMax || pMax->Capacity < size )
		return NULL;
	
	pNew = NewNode( pMax->StartAddress,
		size,
		jobName,
		pMax,
		pMax->Prev);
		
	pMax->Capacity -= size;
	pMax->StartAddress += size;
	
	if( 0 == pMax->Capacity )
		DelNode( pMax );
		
	return pNew;
}

//回收分区
void Free( VP_NODE *pN )
{
	VP_NODE *pN1 = g_VpList.Next;
	
	pN->IsFree = TRUE;
	strcpy(pN->User,"\0");
	
	//寻找上邻，合并
	while( pN1 != NULL )
	{
		if( pN1->IsFree && (pN1->StartAddress + pN1->Capacity ) == pN->StartAddress )
		{
			pN1->Capacity += pN->Capacity;
			DelNode( pN );
			pN = pN1;
			break;
		}
		pN1 = pN1->Next;
	}
	
	//寻找下邻，合并
	pN1 = g_VpList.Next;
	while( pN1 != NULL )
	{
		if( pN1->IsFree && (pN->StartAddress + pN->Capacity) == pN1->StartAddress )
		{
			pN->Capacity += pN1->Capacity;
			DelNode( pN1 );
			break;
		}
		pN1 = pN1->Next;
	}
}

//根据用户输入，为作业分配内存
void NewJob()
{
	char jobName[MAX_STRLEN];
	int size = 0;
	VP_NODE *pN = NULL;
	
	printf("请输入作业名：");
	scanf("%s",jobName);
	printf("请输入所需空间大小：");
	scanf("%i",&size);
	
	pN = WorstFitAllocate( jobName, size );
	if( NULL == pN )
		printf("分配失败!");
	else
	{
		printf("==============================\n");
		printf("           分配成功           \n");
		printf("------------------------------\n");
		printf("起始地址 | 分区长度 | 作业名称\n");
		printf("%8i | %8i | %8s\n",
			pN->StartAddress,
			pN->Capacity,
			pN->User);
		printf("==============================\n");
	}
}

//根据用户输入，撤销作业，回收内存
void DelJob( )
{
	char jobName[MAX_STRLEN];
	VP_NODE *pN = g_VpList.Next;
	
	printf("请输入作业名：");
	scanf("%s",jobName);
	
	//找到要撤销的作业
	while( NULL != pN )
	{
		if( 0 == strcmp(pN->User, jobName) )
			break;
		pN=pN->Next;
	}
	
	if( NULL==pN )
		printf("对不起您要撤销的作业不存在。\n");
	else
	{
		Free( pN );
		printf("作业撤销成功。\n");
	}
}

void main()
{
	char command[MAX_STRLEN];
	BuildListFromUserInput();
	PrintFreeTable();
	for(;;)
	{
		printf("============================================================================\n");
		printf("                                   命令菜单                                 \n");
		printf("----------------------------------------------------------------------------\n");
		printf("1|申请空间  2|撤销作业  3|显示空闲表  4|显示已分配表  5|查看全部内存  0|退出\n");
		printf("============================================================================\n");
		printf("请选择：");
		scanf("%s",command);
		switch( command[0] )
		{
			case '1':NewJob();break;
			case '2':DelJob();break;
			case '3':PrintFreeTable();break;
			case '4':PrintUsedTable();break;
			case '5':PrintMemoryTable();break;
			case '0':exit(0);
			default:printf("您输入的选项不存在，请重新选择。\n");
		}
	}
}



