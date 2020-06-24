# include<iostream.h>
class pair{
	  char c1,c2;
public:
	pair(char b){c1=1+(c2=b);}
	unsigned where_am_I(){return((unsigned)this);}
	void print(){cout<<c1<<" "+c2<<"\n";}
};
void main()
{
	pair a('A'),b('B'),c('C');
	a.print();
	cout<<"is at"<<a.where_am_I()<<"\n";
	b.print();
	cout<<"is at"<<b.where_am_I()<<"\n";
	c.print();
	cout<<"is at"<<c.where_am_I()<<"\n";
	cin.get();
}
