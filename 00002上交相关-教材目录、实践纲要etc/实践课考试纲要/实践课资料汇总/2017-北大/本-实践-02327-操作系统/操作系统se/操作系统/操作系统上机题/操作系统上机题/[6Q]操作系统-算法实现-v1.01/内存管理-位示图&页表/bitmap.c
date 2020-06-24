#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef enum
{
	FALSE = 0,
	TRUE = !FALSE
}BOOL;
#define MAX_STRLEN	256

#define PAGE_SIZE				1024	//内存页尺寸(Byte)
#define	WORD_SIZE 			32		//位图的字长
#define MAX_WORDS_COUNT	1024	//位图字数上限

#define BITMAP_TITLE		"位示图"

//位图结构
typedef struct
{
	int Bits[WORD_SIZE][MAX_WORDS_COUNT];
	int FreeBitsCount;
	int BitsCount;
}BITMAP;

BITMAP g_Bitmap;

int GetAt( int i )
{
	return ((int *)(g_Bitmap.Bits))[i];
}

void SetAt( int i, int bit)
{
	((int *)(g_Bitmap.Bits))[i] = bit;
}

//判断第i快内存是否未被占用
BOOL IsFree( int i )
{
	return (GetAt( i ) == 0);
}

//释放第i块内存
void Free( int i )
{
	if( !IsFree( i ) )
	{
		SetAt( i, 0);
		g_Bitmap.FreeBitsCount ++;
	}
}

//试图占用该第i块内存
//若该块已经被占用返回FALSE，占用成功返回TRUE
BOOL Allocate( int i )
{
	if( IsFree( i ) )
	{
		SetAt( i, 1);
		g_Bitmap.FreeBitsCount --;
		return TRUE;
	}
	return FALSE;
}

//从用户输入的文件读取信息，填充位图
void FillBitmapFromUserInput()
{
	char fileName[MAX_STRLEN];
	FILE *pF = NULL;
	
	g_Bitmap.BitsCount = 0;
	g_Bitmap.FreeBitsCount = 0;
	
	//请求文件名，并打开
	for(;;)
	{
		printf("请输入文件名：");
		scanf("%s",fileName);
		pF = fopen( fileName, "r" );
		if( NULL != pF )
			break;
		printf("无法打开该文件，请确认文件名是否正确。\n");
	}
	
	//从文件读取位图信息
	while( !feof( pF ) )
	{
		char c = fgetc(pF);
		switch( c )
		{
			case '1':
				SetAt(g_Bitmap.BitsCount,1);
				g_Bitmap.BitsCount++;
				break;
			case '0':
				SetAt(g_Bitmap.BitsCount,0);
				g_Bitmap.BitsCount++;
				g_Bitmap.FreeBitsCount++;
				break;
			default:break;
		}
		//防止溢出
		if( g_Bitmap.BitsCount == MAX_WORDS_COUNT*WORD_SIZE )
			break;
	}
	
	fclose( pF );
}

//屏幕打印位图
void PrintBitmap()
{
	int i = 0;
	int j = 0;
	
	for( i=0; i<WORD_SIZE; i++ )
		printf("=");
	printf("\n");
	
	for( i=0; i<(WORD_SIZE - sizeof(BITMAP_TITLE))/2; i++ )
		printf(" ");
	printf("%s\n",BITMAP_TITLE);
	
	
	
	for( i=0; i<WORD_SIZE; i++ )
		printf("-");
	printf("\n");
	
	for( i = 0; i<g_Bitmap.BitsCount; )
	{
		printf("%i",GetAt(i));
		
		if( ++i%WORD_SIZE == 0 || i == g_Bitmap.BitsCount)
		{
			printf("\n");
		}
	}
	
	for( i=0; i<WORD_SIZE; i++ )
		printf("-");
	printf("\n");
	
	printf("空闲块数：%i\n",g_Bitmap.FreeBitsCount);
	
	for( i=0; i<WORD_SIZE; i++ )
		printf("-");
	printf("\n");
	
	printf("块尺寸：%i Byte\n",PAGE_SIZE);
	
	for( i=0; i<WORD_SIZE; i++ )
		printf("-");
	printf("\n");
	
	printf("可用内存：%i Byte\n",PAGE_SIZE*g_Bitmap.FreeBitsCount);
	
	for( i=0; i<WORD_SIZE; i++ )
		printf("=");
	printf("\n");
	
}

//根据用户输入分配内存，并屏幕打印页表
void PrintPageTable( )
{
	int i = 0;
	int j = 0;
	int	mSize = 0;
	int pageCount = 0;
	
	printf("请输入需要分配的内存(Byte)：");
	scanf("%i",&mSize);
	
	if( mSize <= 0 )
	{
		printf("输入的数据无效。\n");
		return;
	}
	
	pageCount = (mSize%PAGE_SIZE == 0) ? (mSize/PAGE_SIZE) : (mSize/PAGE_SIZE + 1);
	
	if( pageCount > g_Bitmap.FreeBitsCount )
	{
		printf("无法分配足够内存。\n");
		return;
	}
		
	printf("本次共分配%i页内存，页表如下：\n",pageCount);
	printf("===========\n");
	printf("页号 | 块号\n");
	printf("-----------\n");
	i = 0;
	for( j = 0; j<g_Bitmap.BitsCount; j++)
	{
		if( Allocate(j) )
		{
			printf("%4i | %4i\n",i,j);
			if( ++i == pageCount )
				break;
		}
	}
	printf("===========\n");
}

//屏幕打印用户菜单
void PrintUserMenu()
{
	printf("==============================\n");
	printf("          命令菜单            \n");
	printf("------------------------------\n");
	printf("1|分配内存 2|显示位示图 0|退出\n");
	printf("==============================\n");
	printf("请选择：");
}

//接受用户命令，并执行
void ExecUserCommand()
{
	char command[MAX_STRLEN];//这里用字符串接受输入可以吃掉回车和其它无效字符，兼容性更好
	
	for(;;)
	{
		PrintUserMenu();
		scanf("%s",command);
		switch( command[0] )
		{
			case '1':PrintPageTable();break;
			case '2':PrintBitmap();break;
			case '0':exit(0);
			default:printf("您的输入的选项不存在,请重新选择。");break;
		}
	}
}

void main()
{
	FillBitmapFromUserInput();
	PrintBitmap();
	ExecUserCommand();
}