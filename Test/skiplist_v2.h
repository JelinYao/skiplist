#pragma once
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <iostream>
#include <vector>

//standard from redis
static const double level_random_p = 1 / 4.0;
static const int stand_max_level = 32;
template<class K, class V>
class skiplist
{
public:
	skiplist()
		: current_level(0)
		, max_level(0)
		, node_count(0)
		, header(NULL)
	{

	}

	~skiplist()
	{
		release();
	}

	bool init(int max_level)
	{
		if (header != NULL)
		{
			//re init
			return false;
		}
		this->max_level = max_level;
		srand((unsigned int)time(NULL));
		header = init_node(max_level, {}, {});
		return header != NULL;
	}

	bool find(const K& k, V& v)
	{
		node_ptr prev[stand_max_level] = {0};
		node_ptr p = find_node(k, prev);
		if (p != NULL)
		{
			v = p->value;
			return true;
		}
		return false;
	}

	void add(const K& k, const V& v)
	{
		//先查找是否存在，存在则更新
		node_ptr prev[stand_max_level] = { 0 };
		node_ptr p = find_node(k, prev);
		if (p != NULL)
		{
			p->value = v;
			return;
		}
		//不存在，插入到跳表中
		int new_level = random_level();
		node_ptr new_node = init_node(new_level, k, v);
		node_count++;
		for (int i = 0; i < current_level && i < new_level; ++i)
		{
			new_node->level_arr[i] = prev[i]->level_arr[i];
			prev[i]->level_arr[i] = new_node;
		}
		//高度更新后，需要连接到头节点
		if (new_level > current_level)
		{
			for (int i = current_level; i < new_level; ++i)
				header->level_arr[i] = new_node;
			current_level = new_level;
			header->level = current_level;
		}
	}

	bool remove(const K& k)
	{
		assert(header != NULL);
		node_ptr p = header;
		for (int i = current_level - 1; i >= 0; --i)
		{
			while (p->level_arr[i] != NULL)
			{
				if (p->level_arr[i]->key == k)
				{
					//查找到了，更新链表指针
					node_ptr rm = p->level_arr[i];
					p->level_arr[i] = rm->level_arr[i];
					if (i == 0)
					{//删除节点
						delete rm;
						node_count--;
						return true;
					}
					//这一层只有该元素，则更新跳表高度
					if (p == header && p->level_arr[i] == NULL)
					{
						current_level--;
					}
					//继续删除下一层节点
					break;
				}
				//这一层没有这个节点，前往下一层查找
				if (p->level_arr[i]->key > k)
					break;
				//继续往前移动查找
				p = p->level_arr[i];
			}
		}
		return false;
	}

	void print()
	{
		assert(header != NULL);
		std::cout << "\r\nSkiplist header: " << current_level << std::endl;
		for (int i = current_level-1; i >=0; --i)
		{
			std::cout << "Print line " << i + 1 <<": ";
			node_ptr p = header;
			while (p->level_arr[i] != NULL)
			{
				std::cout << p->level_arr[i]->key << "[" << p->level_arr[i]->value << "]  ";
				p = p->level_arr[i];
			}
			std::cout << " " << std::endl;
		}
		std::cout << std::endl;
	}

protected:
	typedef struct node {
		K key;
		V value;
		int level;
		std::vector<node*> level_arr;
	}*node_ptr;

	node_ptr init_node(int level, const K& k, const V& v)
	{
		node_ptr p = new node;
		if (p != NULL)
		{
			p->key = k;
			p->value = v;
			int mem_size = level * sizeof(node_ptr);
			p->level_arr.resize(level, NULL);
			p->level = level;
		}
		return p;
	}
	node_ptr find_node(const K& k, node_ptr* prev)
	{
		assert(header != NULL);
		if (node_count < 1)
			return NULL;
		node_ptr p = header;
		int setp = 0;
		for (int i = current_level - 1; i >= 0; --i)
		{
			setp++;
			while (p->level_arr[i] != NULL)
			{
				if (p->level_arr[i]->key == k)
				{
					std::cout << "Find key: " << k << ", step is: " << setp << std::endl;
					return p->level_arr[i];
				}
				if (p->level_arr[i]->key > k)
				{//此层级中不存在该key，到下一层中去查找
					break;
				}
				setp++;
				p = p->level_arr[i];
			}
			//记录下该节点
			prev[i] = p;
		}
		//std::cout << "Cannot find key: " << k << ", step is: " << setp << std::endl;
		return NULL;
	}

	//返回随机数--层数
	int random_level()
	{
		int level = 1;
		while (((rand() % 10) / 10.0) < level_random_p && level < max_level)
		{
			level += 1;
		}
		return level;
	}
	//释放内存空间
	void release()
	{
		if (header == NULL)
			return;
		//释放子节点
		node_ptr p = header->level_arr[0];
		while (p != NULL)
		{
			node_ptr next = p->level_arr[0];
			delete p;
			p = next;
		}
		//释放头节点
		delete header;
		header = NULL;
	}

private:
	int current_level;//跳表当前层数
	int max_level;//跳表最大层数
	int node_count;//元素个数
	node_ptr header;//跳表表头
};

/*
Skip List（跳跃表）是一种支持快速查找的数据结构，插入、查找和删除操作都仅仅只需要O(log n)对数级别的时间复杂度，
它的效率甚至可以与红黑树等二叉平衡树相提并论，而且实现的难度要比红黑树简单多了。
*/