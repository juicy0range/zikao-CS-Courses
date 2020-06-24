/***************************************************************\
8．编一C程序，它能读入6个字符串（每个串长度都不大于80，可能含空格字符），依次判断前5个串是否为第6个串的子串，若是则输出YES；否则输出NO。
\***************************************************************/
#include "string.h"
#include "stdio.h"
int substr(char *str1,char *str2)
{
	int n = strlen(str1);
	int m = strlen(str2);
	int j,k;
	for(int i = 0;i<=n-m;i++)
	{
		j = i;
		k = 0;
		while (str1[j] == str2[k] && k<m) {
			k++;j++;
		}
		if(k == m)
			return 1;
	}
	return 0;
}
void main()
{

	char *str[6];
	printf("please enter a num\n");
	int i = 0;
	for(i = 0 ;i<6;i++)
	{
		printf("%d:",i+1);
		str[i] = new char[80];
		gets(str[i]);
	}
	for(i = 0 ;i < 5;i++)
	{
		if(substr(str[5],str[i]))
		{
			printf("%d is Yes",i+1);
		}
		else
		{
			printf("%d is NO",i+1);
		}
	}
	for(i = 0 ;i<6;i++)
	{
		delete[] (str[i]);
	}

}