/*****************************************************\
4．编一C程序，它能对输入的一串整数（不多于1000个，以-9999为结束标记）到数组a中，再对a的元素进行直接插入排序（从小到大排序），输出排序结果和所用关键字比较次数。（输入时，两个相邻的整数用空格隔开)。
\*****************************************************/
#include "iostream.h"
#define  MAX 1000
int a[MAX] = {-9999};

int InsertSort(int n)
{
	int count= 1;
	int temp;
	int j;
	for(int i = 1;i< n ;i++)
	{
		if(a[i] < a[i-1] && count ++)
		{
			temp = a[i];
			j = i-1;
			do{
				a[j+1] = a[j];
				j--;
			}while (temp < a[j] && count ++);
			a[j+1] = temp;
		}
	}
	return count -1;
}
void main()
{
	cout<<"enter a"<<endl;

	int temp;
	cin>>temp;

	int n = 0;
	while(temp != -9999)
	{
		a[n++]= temp;
		cin>>temp;
	}
	
	int count = InsertSort(n);

	for(int i = 0;i < n; i++)
	{
		cout<<a[i]<<" ";
	}
	cout<<"\nthe num is "<<count<<endl;
}