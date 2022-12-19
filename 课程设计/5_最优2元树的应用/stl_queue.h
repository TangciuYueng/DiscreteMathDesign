#ifndef _MY_STL_QUEUE
#define _MY_STL_QUEUE

#include "stl_list.h"

template<class T>
class Queue {
private:
	List<T> data;
public:
	Queue(){}
	inline bool empty() const {
		return data.empty();
	}
	inline int size() const {
		return data.Size;
	}
	void clear() {
		data.clear();
	}
	void emplace(const T& x) {
		data.emplace_back(x);
	}
	void pop() {
		data.pop_front();
	}
	T& front() {
		return data.getHead()->next->val;
	}
};
#endif // !_MY_STL_QUEUE
