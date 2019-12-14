// Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "skiplist.h"
#include <string>

int TestIntSkipList();
int TestStrSkipList();
int main()
{
	TestStrSkipList();
    std::cout << "Hello World!\n"; 
	_CrtDumpMemoryLeaks();
	system("pause");
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

int TestIntSkipList()
{
	skiplist<int, int> sp;
	for (int i = 10; i < 40; ++i)
	{
		sp.set(i, i * 2);
	}

	sp.print();
	sp.set(50, 501);
	sp.print();
	sp.set(51, 502);
	sp.print();
	sp.set(52, 503);
	sp.print();
	sp.set(53, 503);
	sp.print();
	sp.set(54, 503);
	sp.print();

	sp.set(200, 1222);
	sp.set(56, 445);
	sp.set(73, 354);
	sp.set(7, 8);
	sp.set(10, 8);
	sp.set(66, 8);
	sp.set(77, 8);
	sp.print();
	sp.remove(10);
	sp.print();
	sp.remove(66);
	sp.print();
	sp.remove(77);
	sp.set(7, 108);
	sp.print();
	sp.set(56, 445);
	sp.set(73, 354);
	sp.set(7, 8);
	sp.set(200, 1024);
	sp.print();


	int find_value = -1;
	bool ret = sp.get(23, find_value);
	ret = sp.get(467, find_value);
	ret = sp.get(3, find_value);
	sp.get(6, find_value);
	return 0;
}

int TestStrSkipList()
{
	skiplist<std::string, std::string> sl;
	char key_buffer[32], val_buffer[32];
	for (int i = 0; i < 20; ++i)
	{
		sprintf_s(key_buffer, "key_%d", i);
		sprintf_s(val_buffer, "value_%d", i);
		sl.set(std::string(key_buffer), std::string(val_buffer));
	}
	sl.print();
	std::string val;
	sl.get("key_11", val);
	sl.remove("key_13");
	sl.print();
	return 0;
}