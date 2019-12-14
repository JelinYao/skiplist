 #C++ʵ�ֵ��������ݽṹ
Skip List����Ծ����һ��֧�ֿ��ٲ��ҵ����ݽṹ�����롢���Һ�ɾ������������ֻ��ҪO(log n)���������ʱ�临�Ӷȣ�����Ч�����������������ȶ���ƽ�������Ტ�ۣ�����ʵ�ֵ��Ѷ�Ҫ�Ⱥ�����򵥶��ˡ�

ʵ�ֵĹ����п������ϵĺܶ����ӣ���β��룬�еķ������������ĸ�ָ�롣����Ľṹ�ڲ��ҵ�ʱ��ֻ��Ҫ���Һ��������������ƶ��ͺ��ˣ��������ʵ�ֹ�����ֻ����һ��һά���顣�����������汾�ĵ�����
V1��skiplist_v2.h��ÿ������ڵ�������㼶ʹ�ö�̬���飬ֱ���õ�vector����������ˣ�
V2��skiplist_v1.h���鿴��levelDBԴ�룬�Բ�������������Ż���
V3��skiplist.h��ʹ�ö�̬�����ڴ淽ʽ�滻��vector������

![����ṹ](https://raw.githubusercontent.com/JelinYao/skiplist/master/img/insert.png "����ṹ")
##������������key
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
##�����ַ������͵�key
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