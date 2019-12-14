#pragma once
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <iostream>


//���㼶������Խ��Խ�ã�Խ�����Խ�쵫��ռ���ڴ�ҲԽ��
static const int stand_max_level = 24;


//��Ϊ�����ڲ�������Ҫ�жϴ�С���Զ���ļ�����K�������� == �� ��������
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
		init(stand_max_level);
	}

	~skiplist()
	{
		release();
	}

	bool get(const K& k, V& v)
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

	void set(const K& k, const V& v)
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
			new_node->set_level(i, prev[i]->get_level(i));
			prev[i]->set_level(i, new_node);
		}
		//�߶ȸ��º���Ҫ���ӵ�ͷ�ڵ�
		if (new_level > current_level)
		{
			for (int i = current_level; i < new_level; ++i)
				header->set_level(i, new_node);
			current_level = new_level;
		}
	}

	bool remove(const K& k)
	{
		assert(header != NULL);
		node_ptr p = header;
		for (int i = current_level - 1; i >= 0; --i)
		{
			node_ptr next = NULL;
			while ((next = p->get_level(i)) != NULL)
			{
				if (next->key == k)
				{
					//���ҵ��ˣ���������ָ��
					p->set_level(i, next->get_level(i));
					if (i == 0)
					{//ɾ���ڵ�
						delete next;
						node_count--;
						return true;
					}
					//��һ��ֻ�и�Ԫ�أ����������߶�
					if (p == header && p->get_level(i) == NULL)
					{
						current_level--;
					}
					//����ɾ����һ��ڵ�
					break;
				}
				//��һ��û������ڵ㣬ǰ����һ�����
				if (next->key > k)
					break;
				//������ǰ�ƶ�����
				p = next;
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
			node_ptr next = NULL;
			while ((next = p->get_level(i)) != NULL)
			{
				std::cout << next->key << "[" << next->value << "]  ";
				p = next;
			}
			std::cout << " " << std::endl;
		}
		std::cout << std::endl;
	}

protected:
	typedef unsigned char byte;
	typedef struct node {
		K key;
		V value;
		int level;
		byte* levels_buf;//�ڵ�ָ���������׵�ַ
		node* get_level(int index)
		{
			if (index < 0 || index >= level)
				return NULL;
			byte* dest = levels_buf + index * sizeof(node*);
			node* p = NULL;
			//������ĵ�ַ������ָ����ڴ���
			memcpy(&p, dest, sizeof(node*));
			return p;
		}
		bool set_level(int index, node* p_node)
		{
			if (index < 0 || index >= level)
				return false;
			byte* dest = levels_buf + index * sizeof(node*);
			unsigned int addr = (unsigned int)(void*)p_node;
			//��ָ���ַ���浽������
			memcpy(dest, &addr, sizeof(node*));
			return true;
		}

	}*node_ptr;

	node_ptr init_node(int level, const K& k, const V& v)
	{
		node_ptr p = new node;
		if (p != NULL)
		{
			p->key = k;
			p->value = v;
			int mem_size = level * sizeof(node_ptr);
			p->levels_buf = (byte*)malloc(mem_size);
			memset(p->levels_buf, 0, mem_size);
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
			node_ptr next = NULL;
			while ( (next = p->get_level(i)) != NULL)
			{
				if (next->key == k)
				{
					std::cout << "Find key: " << k << ", step is: " << setp << std::endl;
					return next;
				}
				if (next->key > k)
				{//�˲㼶�в����ڸ�key������һ����ȥ����
					break;
				}
				setp++;
				p = next;
			}
			//��¼�¸ýڵ�
			prev[i] = p;
		}
		//std::cout << "Cannot find key: " << k << ", step is: " << setp << std::endl;
		return NULL;
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
	//���������--����
	int random_level()
	{
		int level = 1;
		while (((rand() % 4) == 0) && level < max_level)
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
		node_ptr p = header;
		while (p != NULL)
		{
			node_ptr next = p->get_level(0);
			free(p->levels_buf);
			delete p;
			p = next;
		}
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