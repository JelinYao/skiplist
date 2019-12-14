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
		//�Ȳ����Ƿ���ڣ����������
		node_ptr prev[stand_max_level] = { 0 };
		node_ptr p = find_node(k, prev);
		if (p != NULL)
		{
			p->value = v;
			return;
		}
		//�����ڣ����뵽������
		int new_level = random_level();
		node_ptr new_node = init_node(new_level, k, v);
		node_count++;
		for (int i = 0; i < current_level && i < new_level; ++i)
		{
			new_node->level_arr[i] = prev[i]->level_arr[i];
			prev[i]->level_arr[i] = new_node;
		}
		//�߶ȸ��º���Ҫ���ӵ�ͷ�ڵ�
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
				{//�˲㼶�в����ڸ�key������һ����ȥ����
					break;
				}
				setp++;
				p = p->level_arr[i];
			}
			//��¼�¸ýڵ�
			prev[i] = p;
		}
		//std::cout << "Cannot find key: " << k << ", step is: " << setp << std::endl;
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