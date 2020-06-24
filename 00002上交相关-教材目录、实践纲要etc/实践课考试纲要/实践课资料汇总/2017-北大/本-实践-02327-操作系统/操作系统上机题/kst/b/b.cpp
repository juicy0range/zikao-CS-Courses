# include<iostream.h>
class test{
   private:
	int num;
   public:
	test();
	int getint(){return num;}
	~test();
};
test::test()
{
num=0;
}
test::~test()
{
	cout<<"Destructor is active"<<endl;
}
void main()
{
	test x[3];
	cout<<"Exiting main"<<endl;
    cin.get();
}