#include "stdio.h"
#include "string.h"
#include "malloc.h"
struct NormalFile
{  
   char name[50]; /* 文件名 */
   int type;/*0代表目录文件，1代表普通文件*/
   int size;/*文件大小*/
};
struct DirectoryNode
{
   int tag;/*0代表此节点空白，1代表有内容*/
   char name[50];
   int type;/*0代表目录文件，1代表普通文件*/
   union
   {
      struct NormalFile *p_normFile; /*用于指向普通文件*/
      struct DirectoryFile *p_dirFile;/*用于指向目录文件*/
   }p_File;/*注意看后面的初始化代码*/
};
struct DirectoryFile
{/*目录文件由一个节点数组组成*/	
   //nodes/*此处不明其义，不知为何*/
   struct DirectoryNode nodesArray[10];
};
/*初始化文件系统*/
void init()
{
     int i,j;/*j参数可用于子函数调用*/
     struct DirectoryFile root;
     struct NormalFile *newNormFile;
     struct DirectoryNode *newNode;
     struct DirectoryFile *newDirFile;
     struct DirectoryFile *curDirFile;
     
     for (i=0;i<10;i++)/*initial a directory*/
       root.nodesArray[i].tag=0;
     
     newDirFile=(struct DirectoryFile *)malloc(1100);
     for(i=0;i<10;i++)
       newDirFile->nodesArray[i].tag=0;
      
      root.nodesArray[0].tag=1;
      strcpy(root.nodesArray[0].name,"A");
      root.nodesArray[0].type=0;
      root.nodesArray[0].p_File.p_dirFile=newDirFile;
      
     newDirFile=(struct DirectoryFile *)malloc(1100);
     for(i=0;i<10;i++)
       newDirFile->nodesArray[i].tag=0;
      
      root.nodesArray[1].tag=1;
      strcpy(root.nodesArray[0].name,"B");
      root.nodesArray[1].type=0;
      root.nodesArray[1].p_File.p_dirFile=newDirFile;

      
     newDirFile=(struct DirectoryFile *)malloc(1100);
     for(i=0;i<10;i++)
       newDirFile->nodesArray[i].tag=0;
      
      root.nodesArray[2].tag=1;
      strcpy(root.nodesArray[0].name,"C");
      root.nodesArray[2].type=0;
      root.nodesArray[2].p_File.p_dirFile=newDirFile;
      
      newNormFile=(struct NormalFile *)malloc(108);
      strcpy((*newNormFile).name,"F1");
      (*newNormFile).type=1;
      (*newNormFile).size=500;
      printf("%s\n",(*newNormFile).name);
      
      root.nodesArray[3].tag=1;
      strcpy(root.nodesArray[3].name,(*newNormFile).name);
      root.nodesArray[3].type=(*newNormFile).type;
      root.nodesArray[3].p_File.p_normFile=newNormFile;
      
      newNormFile=(struct NormalFile *)malloc(108);
      strcpy((*newNormFile).name,"F2");
      (*newNormFile).type=1;
      (*newNormFile).size=50;
      printf("%s\n",(*newNormFile).name);
      
      root.nodesArray[4].tag=1;
      strcpy(root.nodesArray[3].name,(*newNormFile).name);
      root.nodesArray[4].type=(*newNormFile).type;
      root.nodesArray[4].p_File.p_normFile=newNormFile;

      newNormFile=(struct NormalFile *)malloc(108);
      strcpy((*newNormFile).name,"F3");
      (*newNormFile).type=1;
      (*newNormFile).size=5;
      printf("%s\n",(*newNormFile).name);
      
      root.nodesArray[5].tag=1;
      strcpy(root.nodesArray[5].name,(*newNormFile).name);
      root.nodesArray[5].type=(*newNormFile).type;
      root.nodesArray[5].p_File.p_normFile=newNormFile;  
      
      curDirFile=root.nodesArray[1].p_File.p_dirFile;
      newNormFile=(struct NormalFile *)malloc(108);
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
      newDirFile=(struct DirectoryFile *)malloc(1100);
      for (i=0;i<10;i++)
          newDirFile->nodesArray[i].tag=0;
          
      curDirFile->nodesArray[0].tag=1;
      strcpy(curDirFile->nodesArray[0].name,"D");
      curDirFile->nodesArray[0].type=0;
      curDirFile->nodesArray[0].p_File.p_dirFile=newDirFile;
      
      newNormFile=(struct NormalFile *)malloc(108);
      strcpy((*newNormFile).name,"F5");
      (*newNormFile).type=1;
      (*newNormFile).size=321;
      printf("%s\n",(*newNormFile).name);
      
      curDirFile->nodesArray[1].tag=1;
      strcpy(curDirFile->nodesArray[1].name,(*newNormFile).name);
      curDirFile->nodesArray[1].type=(*newNormFile).type;
      curDirFile->nodesArray[1].p_File.p_normFile=newNormFile;

      curDirFile=curDirFile->nodesArray[0].p_File.p_dirFile;
      newNormFile=(struct NormalFile *)malloc(108);
      strcpy((*newNormFile).name,"F6");
      (*newNormFile).type=1;
      (*newNormFile).size=10;
      printf("%s\n",(*newNormFile).name);
      
      curDirFile->nodesArray[0].tag=1;
      strcpy(curDirFile->nodesArray[0].name,(*newNormFile).name);
      curDirFile->nodesArray[0].type=(*newNormFile).type;
      curDirFile->nodesArray[0].p_File.p_normFile=newNormFile;
      
      newNormFile=(struct NormalFile *)malloc(108);
      strcpy((*newNormFile).name,"F7");
      (*newNormFile).type=1;
      (*newNormFile).size=70;
      printf("%s\n",(*newNormFile).name);
      
      curDirFile->nodesArray[1].tag=1;
      strcpy(curDirFile->nodesArray[1].name,(*newNormFile).name);
      curDirFile->nodesArray[1].type=(*newNormFile).type;
      curDirFile->nodesArray[1].p_File.p_normFile=newNormFile;
}

int cd(char *){return 0;}/*改变当前目录*/
int create(char *,int){return 0;}/*创建文件*/
int delete(char *){return 0;}/*删除文件*/
void lsall(){}/*显示所有目录与文件*/
int md(char *){return 0;}/*创建文件*/
int rd(char *){return 0;}/*删除目录*/

void main()
{

}
