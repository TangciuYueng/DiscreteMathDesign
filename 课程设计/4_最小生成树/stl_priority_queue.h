#ifndef _MY_STL_PRIORITY_QUEUE
#define _MY_STL_PRIORITY_QUEUE

#include <iostream>
using namespace std;
#include "stl_heap.h"
#include "stl_functional.h"
template<class T,  class Seq = Vector<T>, class Compare = Less<T>>
class Priority_queue {
private:
	Seq data;
	Compare cmp;
public:
	//构造函数
	Priority_queue() :data(), cmp() {}
	Priority_queue(const Compare& cmp) :data(), cmp(cmp) {}
	/*迭代器
	template<class Iter, class T>
	Priority_queue(Iter beg, Iter end, Compare cmp = Less<T>()) :data(beg, end){
		//makeHeap(beg, end, cmp);
	}*/
	//析构函数
	~Priority_queue() {}
	//判断是否为空
	bool empty() const {
		return data.empty();
	}
	//返回大小
	int size() const {
		return data.size();
	}
	//顶
	const T top() const{
		return data.front();
	}
	//加入元素
	void emplace(const T& x)
	{
		data.emplace_back(x);
		pushHeap(data.begin(), data.end());
	}
	//弹出元素
	void pop()
	{
		popHeap(data.begin(), data.end());
		data.pop_back();
	}
	void showData()
	{
		for (auto it = data.begin(); it != data.end(); ++it)
			cout << *it << " ";
		cout << endl;
	}
};

#endif // !_MY_STL_PRIORITY_QUEUE
