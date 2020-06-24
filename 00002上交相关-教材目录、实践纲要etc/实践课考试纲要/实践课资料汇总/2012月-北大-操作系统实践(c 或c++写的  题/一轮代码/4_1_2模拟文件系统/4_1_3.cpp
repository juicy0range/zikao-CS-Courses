#include "stdio.h"
#include "string.h"
#include "malloc.h"

#define maxStr 50
#define maxNum 10  //目录文件数，便于修改

struct NormalFile
{  
   char name[50]; /* 文件名 */
   int type;/*0代表目录文件，1代表普通文件*/
   int size;/*文件大小*/
};
struct DirectoryNode
{
   int tag;      /*0代表此节点空白，1代表有内容*/
   char name[50];
   int type;  /*0代表目录文件，1代表普通文件*/
   union
   {
      struct NormalFile *p_normFile; /*用于指向普通文件*/
      struct DirectoryFile *p_dirFile;/*用于指向目录文件*/
   }p_File;/*注意看后面的初始化代码*/
};
struct DirectoryFile
{/*目录文件由一个节点数组组成*/      	   
   struct DirectoryNode nodesArray[10];
};


struct DirectoryFile root;
struct DirectoryFile *curOpeDirFile; //curOpeDirFile 保存当前路径，便于 2创建文件，5 创建目录 6删除目录；其默认值为root; 可在cd命令中修改
struct DirectoryNode *curOpeDirNode; 

void init();
//
int cd(char *dirName);
//
int create(char *fileName,int fileLength); /*创建文件*/
void create_file();
int  searchFile(struct DirectoryFile *curDirFile,char *fileName);  //判断文件名唯一否
//
void delName();
int deleteFile(char *);/*删除文件*/
//
void lsall();/*显示所有目录与文件*/
void lsall_dirFile(struct DirectoryFile *curDirFile,int n);  //n用于 输出空格数
//
void md_dirName();
int serchDirName(struct DirectoryFile *curDirFile,char *dirName);
int md(char *dirName);/*创建目录*/
//
void rd_dirName();
int rd_dirEmpty(struct DirectoryNode *curDirNode);  //看此目录下是否空
int rd(char *dirName); /*删除目录*/
void rd_del(struct DirectoryFile  *curDirFile);
//
void new_line();       //吸收上一行的空白字符

//int searchDir_cd(struct DirectoryFile *curDirFile,char *dirName);  
//int searchFile_del(struct DirectoryFile *curDirFile,char *fileName);

void main()
{
   int sel;
   char dirName[maxStr];

   init();
   curOpeDirFile=&root;   //设置初始的当前路径   
   printf("\n模拟文件系统,实现下面6个命令面\n");
   printf("\n 按 1 改变目录(cd命令；2创建文件(create命令；3 删除文件(del命令；4 显示目录(lstall命令 ；5 创建目录(md命令；6 删除目录(rd命令; 7 退出系统\n");
   scanf("%d",&sel);
   while(sel!=7)
   {
	   switch(sel)
	   {
		   case 1:
		   {
			  printf("\n当前路径为:root\\ \n");
			  printf("\n请输入要跳到的目录名\n");
			  new_line();
			  gets(dirName);
			  cd(dirName);
			  break;
		   }
		   case 2:
		   {
              create_file();
			  break;
		   }
		   case 3:
		   {			  
			  delName();			  
			  break;
		   }
			case 4:
		   {
			  lsall();
			  break;
		   }
		   case 5:
		   {
              md_dirName();
			  break;
		   }
		   case 6:
		   {
			  rd_dirName();
			  break;
		   }	   
	   }
      printf("\n 按 1 改变目录(cd命令；2创建文件(create命令；3 删除文件(del命令；4 显示目录(lstall命令 ；5 创建目录(md命令；6 删除目录(rd命令; 7 退出系统\n");
	   scanf("%d",&sel);
   }   
}

/*初始化文件系统*/
void init()
{
     //int i,j;/*j参数可用于子函数调用*/
     int i;
	 
     struct DirectoryFile *curDirFile;
     struct NormalFile *newNormFile;
//     struct DirectoryNode *newNode;
     struct DirectoryFile *newDirFile;
     
     
     for (i=0;i<10;i++)     /*initial a directory*/
       root.nodesArray[i].tag=0;
     
     newDirFile=(struct DirectoryFile *)malloc(1100);   //目录A
     for(i=0;i<10;i++)
       newDirFile->nodesArray[i].tag=0;
      
      root.nodesArray[0].tag=1;            //代表有内容                         
      strcpy(root.nodesArray[0].name,"A");
      root.nodesArray[0].type=0;
      root.nodesArray[0].p_File.p_dirFile=newDirFile;
      
     newDirFile=(struct DirectoryFile *)malloc(1100);    //目录B
     for(i=0;i<10;i++)
       newDirFile->nodesArray[i].tag=0;
      
      root.nodesArray[1].tag=1;
      strcpy(root.nodesArray[1].name,"B");            
      root.nodesArray[1].type=0;                         
      root.nodesArray[1].p_File.p_dirFile=newDirFile; 

      
      newDirFile=(struct DirectoryFile *)malloc(1100);  //目录c
      for(i=0;i<10;i++)
       newDirFile->nodesArray[i].tag=0;
      
      root.nodesArray[2].tag=1;
      strcpy(root.nodesArray[2].name,"C");
      root.nodesArray[2].type=0;                //
      root.nodesArray[2].p_File.p_dirFile=newDirFile;
      
      newNormFile=(struct NormalFile *)malloc(108);    //普通文件F1
      strcpy((*newNormFile).name,"F1");
      (*newNormFile).type=1;
      (*newNormFile).size=500;
      printf("%s\n",(*newNormFile).name);
      
      root.nodesArray[3].tag=1;
      strcpy(root.nodesArray[3].name,(*newNormFile).name);
      root.nodesArray[3].type=(*newNormFile).type;
      root.nodesArray[3].p_File.p_normFile=newNormFile;
      
      newNormFile=(struct NormalFile *)malloc(108);   //普通文件F2
      strcpy((*newNormFile).name,"F2");
      (*newNormFile).type=1; 
      (*newNormFile).size=50;
      printf("%s\n",(*newNormFile).name);
      
      root.nodesArray[4].tag=1;
      strcpy(root.nodesArray[4].name,(*newNormFile).name);
      root.nodesArray[4].type=(*newNormFile).type;
      root.nodesArray[4].p_File.p_normFile=newNormFile;

      newNormFile=(struct NormalFile *)malloc(108);    //普通文件F3
      strcpy((*newNormFile).name,"F3");
      (*newNormFile).type=1;
      (*newNormFile).size=5;
      printf("%s\n",(*newNormFile).name);
      
      root.nodesArray[5].tag=1;
      strcpy(root.nodesArray[5].name,(*newNormFile).name);
      root.nodesArray[5].type=(*newNormFile).type;
      root.nodesArray[5].p_File.p_normFile=newNormFile;  
      
      curDirFile=root.nodesArray[1].p_File.p_dirFile;    //目录B的 指向下面目录文件的 指针
      newNormFile=(struct NormalFile *)malloc(108);      //普通文件F4
      strcpy((*newNormFile).name,"F4");
      (*newNormFile).type=1;
      (*newNormFile).size=123;
      printf("%s\n",(*newNormFile).name);
      
      curDirFile->nodesArray[0].tag=1;
      strcpy(curDirFile->nodesArray[0].name,(*newNormFile).name);
      curDirFile->nodesArray[0].type=(*newNormFile).type;
      curDirFile->nodesArray[0].p_File.p_normFile=newNormFile;
      printf("%s\n",curDirFile->nodesArray[0].p_File.p_normFile->name);

      curDirFile=root.nodesArray[2].p_File.p_dirFile;
      newDirFile=(struct DirectoryFile *)malloc(1100);   //目录文件D
      for (i=0;i<10;i++)
          newDirFile->nodesArray[i].tag=0;
          
      curDirFile->nodesArray[0].tag=1;
      strcpy(curDirFile->nodesArray[0].name,"D");
      curDirFile->nodesArray[0].type=0;
      curDirFile->nodesArray[0].p_File.p_dirFile=newDirFile;
      
      newNormFile=(struct NormalFile *)malloc(108);    //普通文件F5
      strcpy((*newNormFile).name,"F5");
      (*newNormFile).type=1;
      (*newNormFile).size=321;
      printf("%s\n",(*newNormFile).name);
      
      curDirFile->nodesArray[1].tag=1;
      strcpy(curDirFile->nodesArray[1].name,(*newNormFile).name);
      curDirFile->nodesArray[1].type=(*newNormFile).type;
      curDirFile->nodesArray[1].p_File.p_normFile=newNormFile;

     //这是在找D的 空目录文件指针，便于建立其下的（目录节点F6的FCB） 和 （目录节点F7的FCB）
      curDirFile=curDirFile->nodesArray[0].p_File.p_dirFile;  
      newNormFile=(struct NormalFile *)malloc(108);         //普通文件F6
      strcpy((*newNormFile).name,"F6");
      (*newNormFile).type=1;
      (*newNormFile).size=10;
      printf("%s\n",(*newNormFile).name);
      
      curDirFile->nodesArray[0].tag=1;
      strcpy(curDirFile->nodesArray[0].name,(*newNormFile).name);
      curDirFile->nodesArray[0].type=(*newNormFile).type;
      curDirFile->nodesArray[0].p_File.p_normFile=newNormFile;
      
      newNormFile=(struct NormalFile *)malloc(108);
      strcpy((*newNormFile).name,"F7");                 //普通文件F7
      (*newNormFile).type=1;
      (*newNormFile).size=70;
      printf("%s\n",(*newNormFile).name);
      
      curDirFile->nodesArray[1].tag=1;
      strcpy(curDirFile->nodesArray[1].name,(*newNormFile).name);
      curDirFile->nodesArray[1].type=(*newNormFile).type;
      curDirFile->nodesArray[1].p_File.p_normFile=newNormFile;	  
}

/*改变当前目录*/
int cd(char *dirName)
{	
    int i;  
	int dirFlag=0;
	for(i=0;i<maxNum;i++)
	{
		if(root.nodesArray[i].tag==0) {continue;}
		if(root.nodesArray[i].tag==1)
		{
		   if(root.nodesArray[i].type==1) {continue;}
		   else if(!strcmp(root.nodesArray[i].name,dirName) && root.nodesArray[i].type==0)
		   {
			    dirFlag=1;
				printf("\n工作目录已切换到 root:\\%s\n",dirName);
				curOpeDirFile=root.nodesArray[i].p_File.p_dirFile;  //保存下当前路径				
				break;
		   }		   
		}
	}	
	if(dirFlag==0)
	{
	   printf("\n%s下没有没有目录%s\n","root",dirName);
	}
	return 0;
}

//----------关于create命令
int create(char *fileName,int fileLength)
{
	int i;
	struct NormalFile *newNormFile;
	//为文件分配内存，并赋值
    newNormFile=(struct NormalFile *)malloc(108);
    strcpy(newNormFile->name,fileName);
    newNormFile->type=1;
    newNormFile->size=fileLength;	

    //找当前路径下空的FcB插入文件   
   
	for(i=0;i<maxNum;i++)
	{
	   if(curOpeDirFile->nodesArray[i].tag==0)
	   {
		  curOpeDirFile->nodesArray[i].tag=1;
		  strcpy(curOpeDirFile->nodesArray[i].name,newNormFile->name);
          curOpeDirFile->nodesArray[i].type=1;
		  curOpeDirFile->nodesArray[i].p_File.p_normFile=newNormFile;
		  break;
	   }
	} 
	printf("\n文件%s创建成功！\n",fileName);
	return 0;
}


//创建文件时，要保证文件名在当前路径下 唯一
void create_file()
{
	char fileName[maxStr];
	int fileFlag=0,fileLength;
	
	//---- 一 要保证文件名在当前路径下 唯一
    printf("\n请输入文件名\n");
	new_line();
	gets(fileName);    
    
	/*20130422 自己弄复杂了，题只要求 在当前路径下
	//------------找文件名的唯一和文件的插入位置（下边只完能插入 到root.nodesArray[i]中某个目录文件中）
	for(i=0;i<maxNum;i++)
	{
	   if(root.nodesArray[i].tag==1 && root.nodesArray[i].type==0)
	   {
	        curDirFile=root.nodesArray[i].p_File.p_dirFile;
		    //
            fileFlag=searchFile(curDirFile,dirName); 
		    if(fileFlag==0)   //表明此目录文件下：此文件名唯一
			{
			    break;
			} 
	   }
	}
	*/

    fileFlag=searchFile(curOpeDirFile,fileName);  //只在当前路径下看文件名是否唯一 
	while(fileFlag==1)
	{
	    printf("\n当前路径下已存在文件：%s\n",fileName);
        printf("\n请您重新输入 文件名\n");
		new_line();
		gets(fileName);
		fileFlag=searchFile(curOpeDirFile,fileName); 
	}	 
	//-------二
	printf("\n请输入此文件的长度\n");
	scanf("%d",&fileLength);
	create(fileName,fileLength);

}

//确保文件名在某一个目录下唯一：文件名存在，则fileFlag=1 返回；
int  searchFile(struct DirectoryFile *curDirFile,char *fileName)
{
	int fileFlag=0,i;
    //如搜
    for(i=0;i<maxNum;i++)
	{
	    if(curDirFile->nodesArray[i].tag==1 && curDirFile->nodesArray[i].type==1 && !strcmp(curDirFile->nodesArray[i].name,fileName))
		{
		   fileFlag=1;
           curOpeDirNode=&curDirFile->nodesArray[i];   //用于 删除文件操作
		   break;
		}
	}
    return fileFlag;
}


//------/*删除文件*/
void delName()
{
	int fileFlag=0;
    char fileName[maxStr];
	printf("\n请输入要删除的文件名\n");
	new_line();
	gets(fileName);
    fileFlag=searchFile(curOpeDirFile,fileName);
	if(fileFlag==1)
	{
	   deleteFile(fileName);
	}
	else
	{
	   printf("\n系统找不到文件%s\n",fileName);
	}

}
int deleteFile(char *fileName)
{

   free(curOpeDirNode->p_File.p_normFile);
   curOpeDirNode->tag=0;
   printf("\n成功删除文件%s！\n",fileName); 
   return 0;
}

//--------
void lsall()
{
    int i;
	struct DirectoryFile *curDirFile;
    printf("\n/  \n");
	for(i=0;i<maxNum;i++)
	{
		if(root.nodesArray[i].tag==0) {continue;}
		else if(root.nodesArray[i].tag==1)
		{
		    if(root.nodesArray[i].type==0)  //针对目录文件
			{
			   printf("%5s%5c\n",root.nodesArray[i].name,'d'); 
			   //看其下的目录文件
               curDirFile=root.nodesArray[i].p_File.p_dirFile;
               lsall_dirFile(curDirFile,5);
			}
			else if(root.nodesArray[i].type==1)  //普通文件
			{
			   printf("%5s%5c%5d\n",root.nodesArray[i].name,'f',root.nodesArray[i].p_File.p_normFile->size); 
			}
		}
	   
	}
}


void lsall_dirFile(struct DirectoryFile *curDirFile,int n)
{   
	int i,k;
	n=n+5;  
   	for(i=0;i<maxNum;i++)
	{
		if(curDirFile->nodesArray[i].tag==0) {continue;}
		else if(curDirFile->nodesArray[i].tag==1)
		{			
		    if(curDirFile->nodesArray[i].type==0)  //针对目录文件
			{
			   for(k=0;k<n;k++){printf(" ");}
			   printf("%s%5c\n",curDirFile->nodesArray[i].name,'d'); 
			   //看其下的目录文件			   
               lsall_dirFile(curDirFile->nodesArray[i].p_File.p_dirFile,n);
			}
			else if(curDirFile->nodesArray[i].type==1)  //普通文件
			{  			  
			   for( k=0;k<n;k++){printf(" ");}
			   printf("%s%5c%5d\n",curDirFile->nodesArray[i].name,'f',curDirFile->nodesArray[i].p_File.p_normFile->size); 
			}
		}
	   
	}
}


//
void md_dirName()
{
	char dirName[maxStr];
	int dirFlag=0;
    printf("\n请输入目录名\n");
	new_line();
	gets(dirName);   
	
    dirFlag=serchDirName(curOpeDirFile,dirName);
	if(dirFlag==1)
	{
	    printf("\n目录名：%s已存在\n",dirName);
	}
	else
	{
	    md(dirName);
		printf("\n目录%s创建成功！\n",dirName);
	}

}

int serchDirName(struct DirectoryFile *curDirFile,char *dirName)
{ 
    int dirFlag=0,i;
    //如搜
    for(i=0;i<maxNum;i++)
	{
	    if(curDirFile->nodesArray[i].tag==1 && curDirFile->nodesArray[i].type==0 && !strcmp(curDirFile->nodesArray[i].name,dirName))
		{
		   dirFlag=1;
           curOpeDirNode=&curDirFile->nodesArray[i];   //用于 删除目录操作
		   break;
		}
	}
    return dirFlag;
}

int md(char *dirName) /*创建目录*/
{
	int i;
	struct DirectoryFile *newDirFile;

    newDirFile=(struct DirectoryFile *)malloc(1100);  
	for(i=0;i<maxNum;i++)
	{ newDirFile->nodesArray[i].tag=0; }
	
    //找当前路径下空的FcB插入文件    
   
	for(i=0;i<maxNum;i++)
	{
	   if(curOpeDirFile->nodesArray[i].tag==0)
	   {
		  curOpeDirFile->nodesArray[i].tag=1;
		  strcpy(curOpeDirFile->nodesArray[i].name,dirName);
          curOpeDirFile->nodesArray[i].type=0;
		  curOpeDirFile->nodesArray[i].p_File.p_dirFile=newDirFile;
		  break;
	   }
	}
	
	return 0;
}
//
void rd_dirName()
{
    char dirName[maxStr];
	int dirFlag=0,dirEmpty=0,delFlag;
    printf("\n请输入要删除的 目录名\n");
	new_line();
	gets(dirName);   
	
    dirFlag=serchDirName(curOpeDirFile,dirName);
	if(dirFlag==0)
	{
	    printf("\n目录名：%s不存在\n",dirName);
	}
	else
	{
	    dirEmpty=rd_dirEmpty(curOpeDirNode);  //看当前目录下是否空
        if(dirEmpty==0)
		{
		   
			free(curOpeDirNode->p_File.p_dirFile);  //释放内存( 
			curOpeDirNode->tag=0;	
			printf("\n目录%s删除成功！\n",dirName);
		}
		else
		{
		   //当待删目录非空时，
		   printf("\n目录%s非空，确定要删除吗？按 1 删除.\n",dirName);
           scanf("%d",&delFlag);
           if(delFlag==1)
		   {              
              rd(dirName);  // 
		      printf("\n目录%s删除成功！\n",dirName);
		   }
		}

		
	}
}

//看此目录下是否空
int rd_dirEmpty(struct DirectoryNode *curDirNode)
{
     int dirEmpty=0;   //默认为空
	 int i;
	 for(i=0;i<maxNum;i++)
	 {
	     if(curDirNode->p_File.p_dirFile->nodesArray[i].tag==1)
		 {
		     dirEmpty=1;
			 break;
		 }
	 }
     return dirEmpty;
}

//
int rd(char *dirName)   /*删除目录*/
{
	rd_del(curOpeDirNode->p_File.p_dirFile);   //因题只让在它基础上该，所以就重写了个函数rd_del
    curOpeDirNode->tag=0;
	return 0;
}
//递归删除
void rd_del(struct DirectoryFile  *curDirFile)
{
   int i;
   for(i=0;i<maxNum;i++)
	{  	   
	   //tag=0;  代表就没有分配内存，当然也就不用释放
	   if(curDirFile->nodesArray[i].tag==1 && curDirFile->nodesArray[i].type==1)  //文件的FCB
		{
		    free(curDirFile->nodesArray[i].p_File.p_normFile);   
            curDirFile->nodesArray[i].tag=0;
		}
		else if(curDirFile->nodesArray[i].tag==1 && curDirFile->nodesArray[i].type==0)  //FCB下既有非空目录FCB，又有文件文件的FCB
		{
		    //
            rd_del(curDirFile->nodesArray[i].p_File.p_dirFile);   //因为最终就2种情况：要么FCB表空，要么下边为文件的FCB
			//
			curDirFile->nodesArray[i].tag=0;
		}
	} 
}

//-------------
void new_line()
{
    char ch;
	do
	{
	   ch=getchar();
	}while(ch!='\n');
}

/*int searchDir_cd(struct DirectoryFile *curDirFile,char *dirName)   //返回查找成功与否标志
{
	int i;
	int flag=0;
	for(i=0;i<maxNum;i++)
	{
		if(curDirFile->nodesArray[i].tag==0) {continue;}
		if(curDirFile->nodesArray[i].tag==1)
		{
		   if(curDirFile->nodesArray[i].type==1) {continue;}
		   else if(!strcmp(curDirFile->nodesArray[i].name,dirName) && curDirFile->nodesArray[i].type==0)
		   {
				flag=1;	
			    curOpeDirFile=curDirFile->nodesArray[i].p_File.p_dirFile;	
				break;
		   }
		   else
		   {
			    curDirFile=curDirFile->nodesArray[i].p_File.p_dirFile;
                flag=searchDir_cd(curDirFile,dirName);
		   }
		}
	}

	return flag;
}		
*/


/*20130422： 读不懂题，浪费时间精力，下边注释掉的就是 
int deleteFile(char *fileName)
{
    int i;
    int flag=0;       //删除成功与否标志
	struct DirectoryFile *curDirFile;  
  
	for(i=0;i<maxNum;i++)
	{	
		if(root.nodesArray[i].tag==0) {continue;}
		if(root.nodesArray[i].tag==1)  // FCB不空且为文件的FCB 且文件名--
		{
		   if(root.nodesArray[i].type==0) {continue;}
		   else if(root.nodesArray[i].type==1 && !strcmp(root.nodesArray[i].name,fileName))
		   {
				flag=1;	
				//找到后释放文件占的内存且 FCB表空
				free(root.nodesArray[i].p_File.p_normFile);
				root.nodesArray[i].tag=0;
                printf("\n成功删除文件%s！\n",fileName);
				break;
		   }
		   else
		   {
			    curDirFile=root.nodesArray[i].p_File.p_dirFile; 
                flag=searchFile_del(curDirFile,fileName);
		   }
		}
	}
    //这个flag用于 root下有无此文件的标识
	if(flag==0)
	{
	   printf("\n没有找到文件%s！\n",fileName);
	}
	
	return 0;
}

int searchFile_del(struct DirectoryFile *curDirFile,char *fileName)
{
    int i;
	int flag=0;
	for(i=0;i<maxNum;i++)
	{
		if(curDirFile->nodesArray[i].tag==0) {continue;}
		if(curDirFile->nodesArray[i].tag==1)
		{
		   if(curDirFile->nodesArray[i].type==0) {continue;}
		   else if(curDirFile->nodesArray[i].type==1 && !strcmp(curDirFile->nodesArray[i].name,fileName))
		   {
				flag=1;	
			    free(curDirFile->nodesArray[i].p_File.p_normFile);
				curDirFile->nodesArray[i].tag=0;
                printf("\n成功删除文件%s！\n",fileName);
				break;
		   }
		   else
		   {
			    curDirFile=curDirFile->nodesArray[i].p_File.p_dirFile;
                flag=searchFile_del(curDirFile,fileName);
		   }
		}
	}
    //这个标识用于curDirFile下：有无此文件
	if(flag==0)
	{
	   printf("\n没有找到文件%s！\n",fileName);
	}
	return flag;
}
*/