#include "template1_lib.cpp"

//Ӧ�ó��򿪷���Ա
class Application
{
  public:
	bool Step2()
	{
		cout << "myStep2" << endl;
		return true;
	}

	void Step4()
	{
		cout << "myStep4" << endl;
	}
};

int main()
{
	Library lib;
	Application app;

	lib.Step1();

	if (app.Step2())
	{
		lib.Step3();
	}

	for (int i = 0; i < 4; i++)
	{
		app.Step4();
	}

	lib.Step5();
}
