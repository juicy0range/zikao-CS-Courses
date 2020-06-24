#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef enum
{
	FALSE = 0,
	TRUE = !FALSE
}BOOL;
#define MAX_STRLEN			256
#define MAX_TRACK_COUNT	256
#define STR_TITLE				"磁盘调度-最短寻找时间优先"


typedef struct
{
	int		Track;
	BOOL	IsVisted;
	int		Order;
}REQUEST;

REQUEST g_Requests[MAX_TRACK_COUNT];
int			g_RequestsCount = 0;
int			g_StartTrack = 0;

//查找柱面，得到其索引*pIndex
//查找成功返回TRUE,查找失败返回FALSE
BOOL FindTrack( int track , int *pIndex)
{
	int i = 0;
	for( i=0; i<g_RequestsCount; i++ )
	{
		if( g_Requests[i].Track == track )
		{
			*pIndex = i;
			return TRUE;
		}
	}
	return FALSE;
}

//屏幕打印输入的柱面访问流
void PrintRequests()
{
	int i=0;
	for( i=0; i<g_RequestsCount; i++ )
		printf("====");
	printf("\n");
	
	for( i=0; i<(g_RequestsCount*4 - strlen("待访问柱面"))/2; i++ )
		printf(" ");
	printf("待访问柱面\n");
	
	for( i=0; i<g_RequestsCount; i++ )
		printf("----");
	printf("\n");
	
	for( i=0; i<g_RequestsCount; i++ )
		printf("%3i|",g_Requests[i].Track);
	printf("\n");
	
	for( i=0; i<g_RequestsCount; i++ )
		printf("----");
	printf("\n");
}

//从用户输入的文件读取柱面访问流，并要求用户输入当前柱面
void InitRequestsFromUserInput()
{
	char	fileName[MAX_STRLEN];
	int		i = 0;
	FILE	*fp = NULL;
	
	//请求输入文件名
	for(;;)
	{
		printf("请输入文件名：");
		scanf("%s",fileName);
		fp = fopen( fileName,"r");
		if( fp == NULL )
			printf("无法打开该文件，请重新输入。\n");
		else
			break;
	}
	
	//从文件读取柱面访问流
	g_RequestsCount = 0;
	while( !feof(fp) )
	{
		fscanf(fp,"%i",&(g_Requests[g_RequestsCount].Track));
		g_Requests[g_RequestsCount].IsVisted = FALSE;
		g_Requests[g_RequestsCount].Order = g_RequestsCount;
		g_RequestsCount ++;
	}
	
	//请求选择当前柱面
	PrintRequests();
	printf("请选择当前柱面：");
	scanf("%i", &g_StartTrack );
	while( !FindTrack(g_StartTrack,&i) )
	{
		printf("您输入的柱面不存在，请重新选择：");
		scanf("%i", &g_StartTrack );
	}
}

//柱面由小到大排序
void SortByTrack()
{
	int i = 0;
	int j = 1;
	REQUEST r;
	
	while( j>0 )
	{
		j = 0;
		for( i=1; i<g_RequestsCount; i++ )
		{
			if( g_Requests[i-1].Track >g_Requests[i].Track )
			{
				r = g_Requests[i];
				g_Requests[i] = g_Requests[i-1];
				g_Requests[i-1] = r;
				j++;
			}
		}
	}
}

//屏幕打印表头
void PrintHead()
{
	int i = 0;
	int titleAt = (g_RequestsCount*5 - strlen(STR_TITLE))/2;
	
	for( i=0; i<g_RequestsCount; i++ )
		printf("=====");
	printf("\n");
	
	for( i=0; i<titleAt; i++ )
		printf(" ");
	printf("%s\n",STR_TITLE);
	
	for( i=0; i<g_RequestsCount; i++ )
		printf("--+--");
	printf("\n");
	
	for( i=0; i<g_RequestsCount; i++ )
		printf("[%3i]",g_Requests[i].Track);
	printf("\n");
	
	for( i=0; i<g_RequestsCount; i++ )
		printf("--+--",g_Requests[i].Track);
	printf("\n");
}
//屏幕打印表尾
void PrintBottom()
{
	int len = g_RequestsCount*5;
	int i = 0;
	for( i=0; i<len; i++ )
		printf("=");
	printf("\n");
}

//屏幕打印表中的箭头
//iFrom和iTo分别是出发柱面的索引和结束柱面的索引
void PrintCursor(int iFrom, int iTo)
{
	int i = 0;
	if( iFrom < iTo )
	{
		for( i=0; i<iFrom; i++ )
			printf("     ");
		printf("  |--");
		for( i=iFrom; i<iTo-1; i++ )
			printf("-----");
		printf("->|");
	}
	else
	{
		for( i=0; i<iTo; i++ )
			printf("     ");
		printf("  |<-");
		for( i=iTo; i<iFrom-1; i++ )
			printf("-----");
		printf("--|");
	}
	printf("\n");
}

//寻找下一个没有被访问的柱面，得到其索引*pITo
//如果全部柱面都已经被访问，返回FALSE
//这里用最短寻找时间法
BOOL GetNext( int iFrom , int *pITo)
{
	//两个静态变量，分别记录向前和先后寻找的最远位置
	static int __front = -100;
	static int __next;
	
	if( __front == -100 )//如果__front==100说明是第一次调用该函数，则初始化
	{
		__front = iFrom - 1;
		__next = iFrom + 1;
	}
	
	if( __next == g_RequestsCount && __front<0 )
  	return FALSE;
  if( __next == g_RequestsCount )
  {
		*pITo = __front;
		__front--;
	}
 else if( __front<0 )
 {
 		*pITo = __next;
		__next++;
 }
 else
  *pITo = (g_Requests[iFrom].Track - g_Requests[__front].Track ) <(g_Requests[__next].Track - g_Requests[iFrom].Track ) ? __front-- : __next++;
  
 return TRUE;

}

void main()
{
	int iFrom = 3;
	int iTo = 0;
	
	InitRequestsFromUserInput();
	SortByTrack();
	PrintHead();
	
	FindTrack( g_StartTrack, &iFrom);
	g_Requests[iFrom].IsVisted = TRUE;
	while( GetNext( iFrom, &iTo) )
	{
		PrintCursor(iFrom,iTo);
		g_Requests[iTo].IsVisted = TRUE;
		iFrom = iTo;
	}
	PrintBottom();
}