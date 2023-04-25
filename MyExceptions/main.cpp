#include<iostream> 
using namespace std;

#define EXCEPTION_THEORT

void main()
{
	setlocale(LC_ALL, "");

#ifdef EXCEPTION_THEORT

	int a = 0;
	int b = 0;
	try
	{
		//throw 1;
		//throw "Exception happend";
		//throw std::exception("Object of std::exception");
		cout << a / b << endl;
	}
	catch (int e)
	{
		std::cerr << "Error# " << e << endl;
	}
	catch (const char* e)
	{
		std::cerr << e << endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << endl;
		//метод what - возвращает сообщение об ошибке
	}
	catch (...)
	{
		cout << "Somthing happend" << endl;
	}

#endif // DEBUG

}