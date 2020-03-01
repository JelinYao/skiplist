# C++实现的跳表数据结构类
Skip List（跳跃表）是一种支持快速查找的数据结构，插入、查找和删除操作都仅仅只需要O(log n)对数级别的时间复杂度，它的效率甚至可以与红黑树等二叉平衡树相提并论，而且实现的难度要比红黑树简单多了。

实现的过程中看了网上的很多例子，层次不齐，有的放了上下左右四个指针。跳表的结构在查找的时候只需要向右和向下两个方向移动就好了，因此我在实现过程中只放了一个一维数组。经历了三个版本的迭代。
1：skiplist_v2.h，每个跳表节点中跳表层级使用动态数组，直接用的vector容器，最简单了；
2：skiplist_v1.h，查看了levelDB源码，对插入操作进行了优化；
3：skiplist.h，使用动态申请内存方式替换掉vector容器。

![跳表结构](https://raw.githubusercontent.com/JelinYao/skiplist/master/img/insert.png "跳表结构")
##测试整形数据key
```cpp
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

 
```
![](https://raw.githubusercontent.com/JelinYao/skiplist/master/img/int.png)
##测试字符串类型的key
```cpp
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
```
![](https://raw.githubusercontent.com/JelinYao/skiplist/master/img/str.png)
