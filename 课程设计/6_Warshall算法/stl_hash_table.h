#ifndef _MY_STL_HASH_TABLE
#define _MY_STL_HASH_TABLE


#include "stl_string.h"
#include <string>
using namespace std;

//28个素数作为hashtable的size
const int primeNum = 28;
static const unsigned long primeList[primeNum] =
{
  53ul,         97ul,         193ul,       389ul,       769ul,
  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
  1610612741ul, 3221225473ul, 4294967291ul
};
//返回大于n的最小素数，扩容的时候需要
inline unsigned long getNextPrime(unsigned long num)
{
	if (num == 4294967291ul)//最后一位数字了没有下一位了
		return num;
	//二分查找
	int l = 0, r = primeNum - 1;
	while (l < r)
	{
		int mid = (l + r) >> 1;//右移除以二
		if (primeList[mid] <= num)
			l = mid;
		else
			r = mid - 1;
	}
	//返回该数字的下一位
	return primeList[l + 1];
}

//哈希表中的结点
template<class K, class V>
struct hashNode {
	pair<K, V> kv;
	hashNode<K, V>* next;
	hashNode(const pair<K, V>& _kv, hashNode<K, V>* _next = NULL)
		:kv(_kv), next(_next) {}
};

template<class K>
struct hashfunc {
	size_t operator()(const K& key) {
		return key;
	}
};

template<>
struct hashfunc<String> {
	size_t operator()(const String& str) {
		size_t hash = 0;
		size_t Size = str.length() + 1;
		for (size_t i = 0; i < Size; ++i) {
			hash *= 31;
			hash += str.c_str()[i];
		}
		return hash;
	}
};

template<>
struct hashfunc<string> {
	size_t operator()(const string& str) {
		size_t hash = 0;
		for (size_t i = 0; i < str.length() + 1; ++i) {
			hash *= 31;
			hash += str[i];
		}
		return hash;
	}
};

template<class K, class V, class Func = hashfunc<K>>
class hashtable {
	typedef hashNode<K, V> Node;
public:
	//构造函数
	hashtable();
	hashtable(const size_t sz);
	//析构函数
	~hashtable();
	//查找
	Node* find(const K& key);
	//插入
	void insert(const pair<K, V>& p);
	//删除
	void erase(const K& key);
	//判断是否为空
	bool empty() const {
		return Size == 0;
	}
	//返回元素个数
	bool size() const {
		return Size;
	}
private:
	//扩张函数
	void expand();

	Node** table = NULL;
	size_t Size = 0;
	size_t capacity = 0;
	Func hf;
};

//构造函数
template<class K, class V, class Func>
hashtable<K, V, Func>::hashtable() {
	capacity = 53ul;
	table = new(nothrow) Node * [capacity];
	if (table == NULL)
		exit(1);
	for (size_t i = 0; i < capacity; ++i)
		table[i] = NULL;
}

template<class K, class V, class Func>
hashtable<K, V, Func>::hashtable(const size_t sz) {
	capacity = getNextPrime(sz);//实际获得sz的下一个素数
	table = new(nothrow) Node * [capacity];
	if (table == NULL)
		exit(1);
	for (size_t i = 0; i < capacity; ++i)
		table[i] = NULL;
}

//析构函数
template<class K, class V, class Func>
hashtable<K, V, Func>::~hashtable() {
	Node* cur = NULL, *temp = NULL;
	for (size_t i = 0; i < capacity; ++i)
	{
		cur = table[i];
		while (cur) {
			temp = cur->next;
			delete cur;
			cur = temp;
		}
	}
	delete[] table;
}

//查找函数
template<class K, class V, class Func>
hashNode<K, V>* hashtable<K, V, Func>::find(const K& key) {
	size_t index = hf(key) % capacity;
	Node* cur = table[index];
	while (cur) {
		if (cur->kv.first == key)
			break;
		cur = cur->next;
	}
	return cur;
}

//删除函数
template<class K, class V, class Func>
void hashtable<K, V, Func>::erase(const K& key) {
	size_t index = hf(key) % capacity;
	Node* cur = table[index], *prev = NULL;
	while (cur) {
		if (cur->kv.first == key)
			break;
		prev = cur;
		cur = cur->next;
	}

	if (cur == NULL)//没有找到要删除的
		return;

	if (prev == NULL)//要删除的是第一个
	{
		table[index] = cur->next;
		delete cur;
	}
	else
	{
		prev = cur->next;
		delete cur;
	}
	--Size;
}

//扩张函数
template<class K, class V, class Func>
void hashtable<K, V, Func>::expand() {
	size_t newCapacity = getNextPrime(capacity);
	Node** newTable = new(nothrow) Node * [newCapacity];
	if (newTable == NULL)
		exit(1);
	for (size_t i = 0; i < newCapacity; ++i)
		newTable[i] = NULL;

	for (size_t i = 0; i < capacity; ++i)
	{
		Node* cur = table[i];

		while (cur) {
			size_t index = hf(cur->kv.first) % capacity;
			//插头
			cur->next = newTable[index];
			newTable[index] = cur;
		}
	}
	delete[] table;
	table = newTable;
	capacity = newCapacity;
}

//插入函数
template<class K, class V, class Func>
void hashtable<K, V, Func>::insert(const pair<K, V>& p) {
	if (Size * 100 / capacity > 70)
		expand();

	size_t index = hf(p.first) % capacity;
	Node* newNode = new Node(p, table[index]);
	table[index] = newNode;

	++Size;
}


#endif // !_MY_STL_HASH_TABLE
