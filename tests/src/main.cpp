#include "Test.h"

void gameplayTest();
void engineTest();

int main()
{
	engineTest();
}

void engineTest()
{
	rfn::Test::testAll(true);
}

void gameplayTest()
{
	//SceneManager manager(L"../res/TestyTest.trpge");
	//manager.loadScene(L"malpoli");

	//bool run = true;
	//int choice = 0;
	//while (run)
	//{
	//	Scene s = manager.getCurrentScene();

	//	std::cout << "Scene : " << s.name.data() << std::endl;
	//	for (ustring s : s.description)
	//	{
	//		std::cout << s.data() << std::endl;
	//	}
	//	std::cout << std::endl;
	//	std::cout << "Choices : " << std::endl;
	//	int i = 0;
	//	for (ustring s : s.choiceDescriptions())
	//	{
	//		std::cout << "   (" << i << ") " << s.data() << std::endl;
	//		++i;
	//	}

	//	std::cin >> choice;
	//	if (choice == -1)
	//	{
	//		run = false;
	//	}
	//	else
	//	{
	//		manager.selectChoice(choice);
	//	}
	//}
}