#include <stdio.h>
#include <string.h>
#include <process.h>
#include <stdlib.h>
#include <conio.h>
#define null 0
#define len sizeof(struct jnote)

struct jcb
{
 int state;
 int num;
 int in;
 int run;
 int pri;
 int mem;
 int tape;
}job[50];

struct jnote
{
 int id;
 int in;
 int start;
 int run;
 int end;
 int pri;
 int size;
 int tape;
 int *maddr;
 struct jnote *next;
};

int rest=4,memory[101],*mh=memory,logo=0,fid=0;
struct jcb *p=job;
struct jnote *jh=null,*rp=null,*jp=null;


txt()/*从txt文件中作业流*/
{
 FILE *fp;
 char pt;
 int i,space=0,j=0,data[100],h,k,count;
 char str[10];
 for(i=0;i<100;i++) data[i]=-1;
 for(i=0;i<20;i++)
 {
  job[i].num=-1;
  job[i].tape=-1;
  job[i].state=-1;
 }
 i=0;
 fp=fopen("job.txt","r+");
 if(fp==NULL)
 {
	printf("Cann't the file\n");
	exit(0);
 }
 while((pt=getc(fp))!=EOF)
  {if(pt>='0'&&pt<='9')
	{
		str[i]=pt;
		i++;
		space=0;
	}
   else
	{
	  if(pt==' '||pt=='\n')
	  {if(space==1) continue;
	   else
		{str[i]='\0';
		 data[j]=atoi(str);
		 j++;
		 i=0;
		 space=1;
		}
	   }
	}
  }
 for(h=0,k=0;data[k]!=-1;k++,h++)
   {
	job[h].num=data[k];k++;
	job[h].in=data[k];k++;
	job[h].run=data[k];k++;
	job[h].pri=data[k];k++;
	job[h].mem=data[k];k++;
	job[h].tape=data[k];
   }
 if(job[h-1].tape==-1)
   {
	str[i]='\0';
	job[h-1].tape=atoi(str);
   }
 clrscr();
 for(i=0;job[i].num!=-1;i++);
 return(i);
}


rpend(start,run) /* 计算进程的结束时间 */
int start, run;
{
 int end=0;
 int i=start%100+run;
 end=(start/100+i/60)*100+i%60;
 return(end);
}


time_time(end,in)/*计算周转时间或计算剩余的运行时间*/
int in,end;
{int time;
 time=end/100*60+end%100-(in/100*60+in%100);
 return(time);
}

int *m_pd(int size)/*内存判断*/
{
 int *mp,*cp;
 int i=0;
 mp=cp=mh;
 while(*mp!=-1)
 {
	while(*cp==0)
	{
		cp++;i++;
	}
	if(i>=size)  return(mp);
	while(*cp==1) cp++;
	mp=cp;
  }
 return(null);
}


zy_div_free(mp,msize,tape,h)/*资源分配与释放*/
int *mp,msize,tape,h;
{int *cp,i=msize;
 cp=mp;
 if(h==1)
 {
   for(;i>0;i--)
    { *cp=1; cp++;}
   rest=rest-tape;
   return (1);
 }
 if(h==2)
 {
   for(;i>0;i--)
    { *cp=0;cp++; }
   rest=rest+tape;
   return (2);
 }
}



selectrp(plogo,time)/*选择当前运行进程*/
int plogo,time;
{
 struct jnote *newj;
 struct jnote *temp;
 if(jh==null&&rp==null)
  { p=job;
    for(;p->state==0;) p++;
    zy_div_free(mh,p->mem,p->tape,1);
    p->state=0;
    newj=(struct jnote *)malloc(len);
    rp=newj;
    rp->id=p->num; rp->in=p->in; rp->start=p->in; rp->run=p->run; rp->end=0;             rp->pri=p->pri;rp->size=p->mem; rp->tape=p->tape;
    rp->maddr=mh; rp->next=null;
    return (0);
  }
  else
	{if(jh!=null&&rp==null)
	 {
		rp=jh;
		jh=jh->next;
		rp->next=null;
		rp->start=time;
		if(plogo==2)  selectrp(plogo,time);
		else return (-1);
	 }
	 else
	      {	if(jh!=null&&rp!=null)
		 {
		   if(plogo==2)
		    {
			if(logo==0)
			  { if(jh->pri>rp->pri)
			       {
				   temp=jh;
				   jh=jh->next;
				   temp->next=rp;
				   printf("%d :  %d --- %d \n",rp->id,rp->start,temp->in);
				   rp->run-=time_time(temp->in,rp->start);
				   rp->start=0;
				   rp=temp;
				   rp->start=rp->in;
				   return (0);
			       }
			  }
			if(fid==1)
			  { if(jh->pri>rp->pri)
			      {
				temp=jh;
				jh=jh->next;
				temp->next=rp;
				rp=temp;
				rp->start=time;
				selectrp(plogo,time);
			      }
			    else
			     {	rp->start=time;
				return (0);
			     }
			  }
		     }
		    return (0);
		   }
		else
		     {	rp->start=time;
			return (0);
		     }
	       }
	}
  return (0);
}


WORK(jlogo,plogo,count)
int jlogo,plogo,count;
{int k=count,sum=0,time,t;/*sum是周转时间之和*/
 struct jnote *cp,*p1,*newj;
 int *mp;
 selectrp(plogo,0);
 while(k>0)
 { p=job;
   do
   { if(p->state==0) p++;
     while(p->state==-1&&p->num!=-1&&((logo==0&&p->in<rpend(rp->start,rp->run))||(logo==1&&p->in<=rpend(rp->start,rp->run))))
     { mp=m_pd(p->mem);
       if(rest>=p->tape) t=1;
       else t=0;
       if(mp!=null&&t==1)
	{zy_div_free(mp,p->mem,p->tape,1);
	 p->state=0;
	 newj=(struct jnote *)malloc(len);
	 newj->id=p->num; newj->in=p->in; newj->start=0; newj->run=p->run; newj->end=0;
	 newj->pri=p->pri; newj->size=p->mem; newj->tape=p->tape; newj->maddr=mp; newj->next=null;
	 if(jh==null)
		{  jh=newj; jp=jh; }
	 else
		{if (jlogo==1) {jp->next=newj;jp=newj; }/*作业为FCFS*/
		 else /*作业为最短运行时间优先*/
		  { jp=cp=jh;
		    if(newj->run>=jp->run&&jp->next!=null)
		     { cp=jp; jp=jp->next; }
		    else
		     {	if(newj->run<jp->run)
			{  if(cp==jp)
			    {  newj->next=jp; jh=newj; }
			   else
			    {  cp->next=newj; newj->next=jp; }
			}
			else jp->next=newj;
		     }
		  }
		}
	 if (plogo==2)   selectrp(plogo,0);
	}
       p++;
     }
    }while(p->state==0);
    if (logo==0)
      { zy_div_free(rp->maddr,rp->size,rp->tape,2);
	logo=1;
      }
    else
      {
	rp->end=rpend(rp->start,rp->run); /*此时计算结束时间*/
	printf("%d :  %d --- %d \n",rp->id,rp->start,rp->end);
	sum+=time_time(rp->end,rp->in);/*计算周转时间总和*/
	if(k-1==0)
	{p1=rp;rp=rp->next;free(p1); break; }
	time=rp->end;
	p1=rp;
	rp=rp->next;
	free(p1);
	k--;
	fid=1;
	selectrp(plogo,time);
	fid=0;
	logo=0;
      }
  }
  printf("The average time :  %d \n",sum/count);
}

init()
{
 int i;
 for(i=0;job[i].num!=-1;i++) job[i].state=-1;
 logo=0;fid=0;
}

main()
{
 int i,count;
 count=txt();/*返回作业总数*/
 for(i=0;i<100;i++) memory[i]=0;memory[i]=-1;mh=memory;/*内存清0，处于未分配状态，最后一个用于标识*/
 printf("Job_Process\n\n");
 printf("FCFS_FCFS :\n");
 WORK(1,1,count);/*作业FCFS, 进程FCFS*/
 init();
 printf("\nSHORT_FCFS :\n");
 WORK(2,1,count);/*作业最短运行时间优先,进程FCFS*/
 printf("\n\nPlease press keybord to see the result of process is PRI ");
 getchar();
 clrscr();
 init();
 printf("Job_Process\n");
 printf("\nFCFS_PRI :\n");
 WORK(1,2,count);
 init();
 printf("\nSHORT_PRI :\n");
 WORK(2,2,count);
 getchar();
}
