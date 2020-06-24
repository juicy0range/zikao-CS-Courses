#include <stdio.h>
#include <stdlib.h>
#include <iomanip.h>
#include <string.h>
#include <iostream.h>
#include <math.h>
//银行家算法
const int MAXJOB=20;//最大作业数量
const int MAXA=10;//定义A类资源的数量
const int MAX=20;//资源名字长度

typedef struct node1{
	int ResourceA;
	int Reamin_A;
}BANK;
typedef struct node2{
	char JobName[MAX];
	int  ResourceA;
	int  Need_A;
}JOB;
BANK temp,Bank;
JOB Job[MAXJOB];
int Quantity;//总共作业数量

void Init(){
	Bank.ResourceA=MAXA;
	Bank.Reamin_A=MAXA;

	temp.ResourceA=MAXA;
	temp.Reamin_A=MAXA;
	for(int i=0;i<MAXJOB;i++){
		strcpy(Job[i].JobName,"");
		Job[i].ResourceA=0;
		Job[i].Need_A=0;
	}
}
//新加入作业
void AddJob(){
	char jobname[MAX];
	int i,Flag=0,t;//Flag用于表示作业名称是否已经存在
	int resourcea=0;
	if(Quantity<MAXJOB){
		cout<<"请输入作业名称："<<endl;
		cin>>jobname;
		for(i=0;i<Quantity;i++){
			if(strcmp(Job[i].JobName,jobname)==0){
				Flag=1;
				break;
			}
		}
		if(Flag){
			cout<<"对不起，新加入的作业名称已经存在！"<<endl;
			return;
		}
		else{
			cout<<"本作业所需A类资源为："<<endl;
			cin>>resourcea;
			t=0;//用于判断虚分配是否出现错误
			if(resourcea>Bank.Reamin_A){
				cout<<"错误，该作业所需A类资源大于银行所剩A类资源！"<<endl;
				t=1;
			}
			if(t!=1){
				strcpy(Job[Quantity].JobName,jobname);
				Job[Quantity].Need_A=resourcea;
				Quantity++;
				cout<<"新加入作业成功！"<<endl;
			}
			else{
				cout<<"新加入作业失败！"<<endl;
			}
		}
	}
}
//为作业申请资源
void Apply(){
	char jobname[MAX];
	int i,j,p=-1,t;
	int resourcea;
	int minA;
	cout<<"请输入要申请资源的作业名称："<<endl;
	cin>>jobname;
	for(i=0;i<Quantity;i++){
		if(strcmp(Job[i].JobName,jobname)==0){
			p=i;
			break;

		}
	}
	if(p!=-1){
		cout<<"该作业要申请A类资源数量："<<endl;
		cin>>resourcea;
		t=0;//用于
		if(resourcea>Bank.Reamin_A||resourcea>Job[p].Need_A){
			cout<<"错误，该作业所需A类资源大于银行所剩A类资源或该作业还需资源数量！"<<endl;
			t=1;
		}
		if(t!=1){
			temp.Reamin_A=temp.Reamin_A-resourcea;//虚分配
			Job[p].Need_A-=resourcea;
			Job[p].ResourceA+=resourcea;
			minA=MAXA;
			for(j=0;j<Quantity;j++){
				if(Job[j].Need_A<minA){
					minA=Job[j].Need_A;
				}
			}
			if(temp.Reamin_A>=minA){
				Bank.Reamin_A-=resourcea;
				cout<<"为作业申请资源成功！"<<endl;

			}
			else{
				temp.Reamin_A+=resourcea;//虚分配
				
				Job[p].Need_A+=resourcea;
				Job[p].ResourceA-=resourcea;
				cout<<"所需资源会破坏银行家算法！"<<endl;
				cout<<"资源分配不成功！"<<endl;
			}
		}
		else{
			cout<<"该作业申请资源不成功！"<<endl;
		}
	}
	else{
		cout<<"您输入要申请资源的作业名称不存在！"<<endl;
	}
}
//撤消作业
void Reclaim(){
	char jobname[MAX];
	int i;
	int p=-1;
	cout<<"请输入要撤消的作业名称："<<endl;
	cin>>jobname;
	for(i=0;i<Quantity;i++){
		if(strcmp(Job[i].JobName,jobname)==0){
			p=i;
			break;
		}
	}
	if(p!=-1){
		Bank.Reamin_A+=Job[p].ResourceA;
		temp.Reamin_A+=Job[p].ResourceA;
		for(i=p;i<Quantity-1;i++){
			Job[i]=Job[i+1];
		}
		strcpy(Job[Quantity-1].JobName,"");
		Job[Quantity-1].ResourceA=0;
		Job[Quantity-1].Need_A=0;
		Quantity--;
		cout<<"撤消作业成功！"<<endl;
	}
	else{
		cout<<"您要撤消的作业不存在！"<<endl;
	}

}
//查看资源
void View(){
	int i;
	cout<<"银行家资源分配情况(剩余资源/总共资源)："<<endl;
	if(Quantity>0){
		cout<<"A类资源："<<Bank.Reamin_A<<" / "<<Bank.ResourceA<<endl;
		cout<<"作业占用资源情况："<<endl;
		for(i=0;i<Quantity;i++){
			cout<<"作业名称:"<<"  "<<Job[i].JobName<<endl;
			cout<<"A类资源(已分配资源/需要资源):"<<"  "<<Job[i].ResourceA<<"/"<<Job[i].Need_A<<endl;
			cout<<endl;
		}
	}
	else{
		cout<<"当前系统中没有作业！"<<endl;
	}
}
void main(){
	int Menu,Exit1=-1;
	while(1){
		cout<<"--------------------------------------"<<endl;
		cout<<"银行家算法模拟程序！"<<endl;
		cout<<"--------------------------------------"<<endl;
		cout<<"1.初始化数据"<<endl;
		cout<<"2.加入作业"<<endl;
		cout<<"3.为作业申请资源"<<endl;
		cout<<"4.撤消作业"<<endl;
		cout<<"5.查看资源分配情况"<<endl;
		cout<<"6.退出"<<endl;
		cin>>Menu;
		if(Menu>0&&Menu<7){
			switch(Menu){
				case 1:
					Init();
					break;
				case 2:
					AddJob();
					break;
				case 3:
					Apply();
					break;
				case 4:
					Reclaim();
					break;
				case 5:
					View();
					break;
				case 6:
					Exit1=1;
					break;
			}
		}
		if(Exit1==1){
			cout<<"感谢您使用本系统！"<<endl;
			break;
		}
	}

}