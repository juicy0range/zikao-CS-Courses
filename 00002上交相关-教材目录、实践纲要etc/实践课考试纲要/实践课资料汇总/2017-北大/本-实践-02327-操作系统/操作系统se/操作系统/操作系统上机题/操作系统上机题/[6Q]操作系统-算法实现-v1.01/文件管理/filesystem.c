#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef enum
{
	FALSE = 0,
	TRUE = !FALSE
}BOOL;
#define MAX_STRLEN	256

#define ROOT_NAME		"ROOT"

//文件类型枚举
typedef enum
{
	T_FILE,
	T_DIR,
	T_ROOT,
}TYPE;

//文件节点
typedef struct _file_node
{
	char Name[MAX_STRLEN];				//文件名
	TYPE Type;										//文件类型，目录，文件或者根
	struct _file_node *Father;		//父目录
	struct _file_node *Son;				//子目录
	struct _file_node *Next;			//右兄弟
	struct _file_node *Prev;			//左兄弟
}FILE_NODE;


//新建文件节点
FILE_NODE *NewFileNode( const char *name, TYPE t)
{
	FILE_NODE *pN = (FILE_NODE*)malloc(sizeof(FILE_NODE));
	//初始化数据
	pN->Next = NULL;
	pN->Prev = NULL;
	pN->Father = NULL;
	pN->Son = NULL;
	pN->Type = t;
	strcpy( pN->Name, name );

	return pN;
}

//销毁文件节点
void DelFileNode( FILE_NODE *pN )
{
	free( pN );
}

//孤立节点
//解除该结点兄弟的关系，同时保持兄弟之间的连续性
//解除该节点和父目录的关系，同时保持“父目录”和“父目录的子目录”之间连续性
//注意：这里没有解除和子节点(.Sub)的关系
void Isolate( FILE_NODE *pN )
{
	//如果该节点是其父目录的子目录的头节点
	//则该头节点指向变更
	if( pN->Father != NULL )
		if( pN->Father->Son == pN )
			pN->Father->Son = pN->Next;
	pN->Father = NULL;

	//解除和同级的关系
	if( pN->Prev != NULL )
		pN->Prev->Next = pN->Next;
	if( pN->Next != NULL )
		pN->Next->Prev = pN->Prev;
	pN->Prev = NULL;
	pN->Next = NULL;
}

//在*pP后插入*pN
//即将*pN作为*pP的右兄弟
void InsertNext( FILE_NODE *pP, FILE_NODE *pN )
{
	//建立同级间的关系
	pN->Next = pP->Next;
	if( pN->Next != NULL )
		pN->Next->Prev = pN;
	pN->Prev = pP;
	pP->Next = pN;

	//建立和父目录的关系
	pN->Father = pP->Father;
}

//在*pF中插入*pC
//即将*pC作为*pF的子节点
void InsertIn( FILE_NODE *pF, FILE_NODE *pC )
{
	if( pF->Son == NULL )
		pF->Son = pC;
	else
		InsertNext( pF->Son, pC );

	pC->Father = pF;
	//还可以考虑更改pC所有兄弟节点的父指针为pF，此处咱略
}

//沿同级节点查找
//根据提供的文件名和类型寻找节点
//成功返回该节点，失败返回NULL
FILE_NODE *FindFileNodeByNameType( const FILE_NODE *pN, const char *name, TYPE t )
{
	while( pN != NULL )
	{
		if( t == pN->Type && 0 == strcmp( pN->Name, name ) )
			break;
		pN = pN->Next;
	}
	return pN;
}


FILE_NODE g_Root;			//根目录
FILE_NODE *g_pCD = NULL;	//当前目录

//初始化根目录
void InitRoot()
{
	strcpy( g_Root.Name, ROOT_NAME );
	g_Root.Next = NULL;
	g_Root.Prev = NULL;
	g_Root.Father = NULL;
	g_Root.Son = NULL;
	g_Root.Type = T_ROOT;
	g_pCD = &g_Root;
}

//MD
void MakeDir( const char* name )
{
	FILE_NODE *pN = FindFileNodeByNameType( g_pCD->Son, name, T_DIR );
	
	if( pN != NULL )
	{
		printf("创建失败。原因：同名目录已经存在。\n");
		return;
	}
	
	pN = NewFileNode( name, T_DIR );
	InsertIn( g_pCD, pN );

	printf("创建成功。\n");
}

//RemoveDir的辅助函数，删除所有子目录节点
void DelAllSubs( FILE_NODE *pN )
{
	pN = pN->Son;

	while( pN != NULL)
	{
		FILE_NODE *pN1 = pN->Next;
		
		Isolate( pN );
		DelAllSubs( pN );
		DelFileNode( pN );
		
		pN = pN1;
	}
}

//RD
void RemoveDir( const char* name )
{
	char command[MAX_STRLEN];
	FILE_NODE *pN = FindFileNodeByNameType( g_pCD->Son, name, T_DIR );
	
	if( pN == NULL )//文件名不存在
	{
		printf("删除失败。原因：指定目录不存在。\n");
		return;
	}
	else if ( pN->Son == NULL )//不含有子目录，直接删除
	{
		Isolate( pN );
		DelFileNode( pN );
		printf("删除成功。\n");
		return;
	}
	
	for(;;)
	{
		printf("提示：试图删除的目录含有子目录，确认删除？<y/n>\n");
		scanf( "%s", command );
		strupr( command );
		switch( command[0] )
		{
			case 'Y':
				{
					Isolate( pN );
					DelAllSubs( pN );
					DelFileNode( pN );
					printf("删除成功。\n");
					return;
				}
			case 'N':
				{
					printf("删除操作被用户取消。\n");
					return;
				}
			default:break;
		}
	}	
}

//MF
void CreateFile( const char* name )
{
	FILE_NODE *pN = FindFileNodeByNameType( g_pCD->Son, name, T_FILE );
	
	if( pN != NULL )
	{
		printf("创建失败。原因：同名文件已经存在。\n");
		return;
	}
	
	pN = NewFileNode( name, T_FILE );
	InsertIn( g_pCD,pN );

	printf("创建成功。\n");
}

//DEL
void DeleteFile( const char* name )
{
	FILE_NODE *pN = FindFileNodeByNameType( g_pCD->Son, name, T_FILE );
	
	if( pN == NULL )
	{
		printf("删除失败。原因：指定文件不存在。\n");
		return;
	}

	Isolate( pN );
	DelFileNode( pN );
	printf("删除成功。\n");
	return;
}

//CD
void CurrentDir( const char* name )
{
	FILE_NODE *pN = FindFileNodeByNameType( g_pCD->Son, name, T_DIR );
	if( pN != NULL)
	{
		g_pCD = pN;
		return;
	}
	else if( 0 == strcmp("..", name ) )
	{
		if( g_pCD->Type != T_ROOT )
			g_pCD = g_pCD->Father;
		return;
	}else if( 0 == strcmp("\\", name ) )
	{
		g_pCD = &g_Root;
		return;
	}
	printf("目录转移失败。原因：不存在指定目录。\n");
}

//得到路径
char* GetPath( FILE_NODE *pCD,char *path )
{	
	char path2[MAX_STRLEN] = "";
	FILE_NODE *pN = pCD;

	while( pN->Type != T_ROOT )
	{
		sprintf( path,"%s\\%s", pN->Name, path2 );
		strcpy( path2,path );
		pN = pN->Father;
	}
	sprintf( path,"%s\\%s", g_Root.Name, path2 );
	return path;
}

//DIR
void Dir( FILE_NODE *pCD )
{
	int i = 0;
	char path[MAX_STRLEN];
	int pathLen = strlen( GetPath( pCD, path ) );
	FILE_NODE *pN = pCD->Son;
	
	while( pN != NULL )
	{
		for( i = 0; i != pathLen; i++ )
			printf(" ");
		if( pN->Type == T_FILE )
		{
			printf("|-%s\n",pN->Name );
		}
		else
		{
			printf("|-[+]%s\n",pN->Name );
			Dir( pN );
		}
		pN = pN->Next;
	}
}

//屏幕打印帮助信息
void PrintHelp()
{	
	printf("============================\n");
	printf("     虚拟文件管理系统       \n");
	printf("----------------------------\n");
	printf("   命令 |     功能          \n");
	printf(" CD     | 更改当前目录      \n");
	printf(" DIR    | 显示文件和子目录  \n");
	printf(" DEL    | 删除文件          \n");
	printf(" EXIT   | 退出系统          \n");
	printf(" MD     | 创建目录          \n");
	printf(" MF     | 创建文件          \n");
	printf(" HELP   | 显示帮助功能      \n");
	printf(" RD     | 删除目录          \n");
	printf("============================\n");

}

main()
{
	char command[MAX_STRLEN];
	char parameter[MAX_STRLEN];
	char path[MAX_STRLEN];
	
	InitRoot();
	PrintHelp();

	for(;;)
	{
		printf( "%s>",GetPath( g_pCD,path ) );
		scanf( "%s", command);
		strupr( command );
		if( 0 == strcmp("CD", command ) )
		{
			scanf( "%s", parameter);
			CurrentDir( parameter );
		}else	if( 0 == strcmp("CD..", command ) )
		{
			CurrentDir( ".." );
		}else if( 0 == strcmp("CD\\", command ) )
		{
			CurrentDir( "\\" );
		}
		else if( 0 == strcmp("DIR", command ) )
			Dir( g_pCD );
		else if( 0 == strcmp("MD", command ) )
		{
			scanf( "%s", parameter);
			MakeDir( parameter );
		}
		else if( 0 == strcmp("RD", command ) )
		{
			scanf( "%s", parameter);
			RemoveDir( parameter );
		}
		else if( 0 == strcmp("MF", command ) )
		{
			scanf( "%s", parameter);
			CreateFile( parameter );
		}
		else if( 0 == strcmp("DEL", command ) )
		{
			scanf( "%s", parameter);
			DeleteFile( parameter );
		}
		else if( 0 == strcmp("EXIT", command ) )
			exit(0);
		else if( 0 == strcmp("HELP", command ) )
			PrintHelp();
		else
			printf("%s不是有效的命令名。\n",command);
	}
}