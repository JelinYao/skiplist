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
		node_ptr p = find_node(k);
		if (p != NULL)
		{
			v = p->value;
			return true;
		}
		return false;
	}

	void add(const K& k, const V& v)
	{
		//�Ȳ����Ƿ���ڣ����������
		node_ptr p = find_node(k);
		if (p != NULL)
		{
			p->value = v;
			return;
		}
		assert(header != NULL);
		int node_level = random_level();
		int level = node_level;
		if (current_level < level)
			current_level = level;
		node_ptr new_p = NULL;
		p = header;
		//ά���㼶
		while (true)
		{
			level--;
			//���ҵ�����λ�õ�ǰһ��Ԫ��
			while (p->level_arr[level] != NULL && p->level_arr[level]->key < k)
				p = p->level_arr[level];
			//���������л���תָ��
			if (new_p == NULL)
			{
				new_p = init_node(node_level, k, v);
				node_count++;
			}
			new_p->level_arr[level] = p->level_arr[level];
			p->level_arr[level] = new_p;
			//������һ��
			if (level == 0)
				break;
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
					//���ҵ��ˣ���������ָ��
					node_ptr rm = p->level_arr[i];
					p->level_arr[i] = rm->level_arr[i];
					if (i == 0)
					{//ɾ���ڵ�
						delete rm;
						node_count--;
						return true;
					}
					//��һ��ֻ�и�Ԫ�أ����������߶�
					if (p == header && p->level_arr[i] == NULL)
					{
						current_level--;
					}
					//����ɾ����һ��ڵ�
					break;
				}
				//��һ��û������ڵ㣬ǰ����һ�����
				if (p->level_arr[i]->key > k)
					break;
				//������ǰ�ƶ�����
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
				std::cout << p->level_arr[i]->key << "  ";// << ":" << p->level_arr[i]->value << "  ";
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
	node_ptr find_node(const K& k)
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
					break;
				setp++;
				p = p->level_arr[i];
			}
		}
		std::cout << "Cannot find key: " << k << ", step is: " << setp << std::endl;
		return NULL;
	}

	//���������--����
	int random_level()
	{
		int level = 1;
		while (((rand() % 10) / 10.0) < level_random_p && level < max_level)
		{
			level += 1;
		}
		return level;
	}
	//�ͷ��ڴ�ռ�
	void release()
	{
		if (header == NULL)
			return;
		//�ͷ��ӽڵ�
		node_ptr p = header->level_arr[0];
		while (p != NULL)
		{
			node_ptr next = p->level_arr[0];
			delete p;
			p = next;
		}
		//�ͷ�ͷ�ڵ�
		delete header;
		header = NULL;
	}

private:
	int current_level;//����ǰ����
	int max_level;//����������
	int node_count;//Ԫ�ظ���
	node_ptr header;//�����ͷ
};

/*
Skip List����Ծ����һ��֧�ֿ��ٲ��ҵ����ݽṹ�����롢���Һ�ɾ������������ֻ��ҪO(log n)���������ʱ�临�Ӷȣ�
����Ч�����������������ȶ���ƽ�������Ტ�ۣ�����ʵ�ֵ��Ѷ�Ҫ�Ⱥ�����򵥶��ˡ�
*/