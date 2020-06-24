#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef enum
{
	FALSE = 0,
	TRUE = !FALSE
}BOOL;
#define MAX_STRLEN				256
#define PAGE_FRAME_COUNT	3

//页节点
typedef struct page_node
{
	int Number;
	int LastTime;
	struct page_node *Next;
	struct page_node *Prev;
}PAGE_NODE;

//新建页节点,并接入到链中
PAGE_NODE *NewPage( int n, PAGE_NODE *pNext, PAGE_NODE *pPrev )
{
	PAGE_NODE *pNew = NULL;
	pNew = (PAGE_NODE*)malloc( sizeof(PAGE_NODE) );
	pNew->Number = n;
	pNew->LastTime = 0;
	pNew->Next = pNext;
	pNew->Prev = pPrev;
	if( pNew->Next != NULL )
		pNew->Next->Prev = pNew;
	if( pNew->Prev != NULL )
		pNew->Prev->Next = pNew;
	return pNew;
}

//从链中分离并销毁页节点
void DelPage( PAGE_NODE *pN )
{
	if( pN->Prev != NULL )
		pN->Prev->Next = pN->Next;
	if( pN->Next != NULL )
		pN->Next->Prev = pN->Prev;
	free( pN );
}

typedef PAGE_NODE PAGE_LIST;

PAGE_LIST g_PList;
int				g_PageCount = 0;

//寻找页，n为页编号，没找到返回NULL
PAGE_NODE *FindPage( int n )
{
	PAGE_NODE *pN = g_PList.Next;
	
	while( pN != NULL )
	{
		if( pN->Number == n )
			break;
		pN = pN->Next;
	}
	return pN;
}

//寻找最近最少使用页
PAGE_NODE *FindLruPage( )
{
	PAGE_NODE *pN = g_PList.Next;
	PAGE_NODE *pN1 = g_PList.Next;
	
	while( pN != NULL )
	{
		if( pN->LastTime > pN1->LastTime )
			pN1 = pN;
		pN = pN->Next;
	}
	return pN1;
}

//计时器
void Timmer()
{
	PAGE_NODE *pN = g_PList.Next;
	
	while( pN != NULL )
	{
		pN->LastTime ++;
		pN = pN->Next;
	}
}

void main()
{
	char fileName[MAX_STRLEN];
	FILE *pf = NULL;
	PAGE_NODE *pN = &g_PList;
	PAGE_NODE *pN1 = NULL;
	g_PList.Next = NULL;
	g_PList.Prev = NULL;
	
	//打开输入文件
	for(;;)
	{
		printf("请输入文件名：");
		scanf("%s",fileName);
		pf = fopen( fileName, "r" );
		if( NULL != pf )
			break;
		else
			printf("文件打开失败，请重新输入。\n");
	}
	
	printf("===============================================\n");
	printf("              LRU页面置换顺序表                \n");
	printf("-----------------------------------------------\n");
	printf("页面流 | 换入页号 | 换出页号 | 换出页未访问时间\n");
	//从输入文件读取数据
	while( !feof( pf ) )
	{
		int n;
		Timmer();
		
		fscanf( pf, "%i", &n );
		
		pN1 = FindPage(n);
		
		if( NULL != pN1 )//待访问页在内存中存在
		{
				printf("%6i | %8c | %8c | \n",n,'*','*');
				pN1->LastTime = 0;//更新最近使用时间为0
		}
		else if( g_PageCount < PAGE_FRAME_COUNT )//页框未满
		{
			pN = NewPage( n, pN->Next, pN );//新页进入内存
			g_PageCount ++;									//占用一个页框
			printf("%6i | %8i | %8c | \n",n,n,'*');
		}
		else//页框已满
		{
			pN1 = FindLruPage();//找到最近最少使用页
			printf("%6i | %8i | %8i | %16i\n",n,n,pN1->Number,pN1->LastTime);
			pN = NewPage( n, pN->Next, pN );//新页进入内存
			DelPage( pN1 );		//最近最少使用页换出内存
		}
			
	} 
	printf("===============================================\n");
}