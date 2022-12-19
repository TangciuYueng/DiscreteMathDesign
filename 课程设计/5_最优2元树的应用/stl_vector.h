#ifndef _MY_STL_VECTOR
#define _MY_STL_VECTOR

#include <iostream>
const int defaultSize = 10;

template<class T>
class Vector {
	//迭代器类型
	typedef T* iterator;
private:
	//元素个数
	int Size;

	//实际容量
	int capacity;

	//内部数组
	T* data;

	//扩容函数
	void expand();
public:
	//构造函数
	Vector();
	Vector(int size);
	Vector(const Vector<T>& V);
	Vector(int size, const T& val);

	//析构函数
	~Vector();

	//返回元素个数
	inline int size() const {
		return Size;
	}

	//判断数组是否为空
	inline bool empty() const {
		return Size == 0;
	}

	//Vector迭代器
	class vector_iterator {
		friend class Vector<T>;
	private:
		iterator data;
	public:
		typedef T value_type;
		vector_iterator(iterator data) :data(data){}
		//重载*
		T& operator*() {
			return *data;
		}
		T* operator->() {
			return data;
		}
		//重载==
		inline bool operator==(const vector_iterator& it) const {
			return this->data == it.data;
		}
		//重载!=
		inline bool operator!=(const vector_iterator& it) const {
			return this->data != it.data;
		}
		//重载不等号
		inline bool operator<(const vector_iterator& it) const {
			return this->data < it.data;
		}
		inline bool operator>(const vector_iterator& it) const {
			return this->data > it.data;
		}
		inline bool operator<=(const vector_iterator& it) const {
			return this->data <= it.data;
		}
		inline bool operator>=(const vector_iterator& it) const {
			return this->data >= it.data;
		}
		//重载自加
		inline Vector<T>::vector_iterator& operator++()
		{
			if (data != NULL)
				++data;
			return *this;
		}
		//重载自减
		inline Vector<T>::vector_iterator& operator--()
		{
			if (data != NULL)
				--data;
			return *this;
		}
		//后置自加
		Vector<T>::vector_iterator operator++(int)
		{
			Vector<T>::vector_iterator temp = *this;
			++(*this);
			return temp;
		}
		//后置自减
		Vector<T>::vector_iterator operator--(int)
		{
			Vector<T>::vector_iterator temp = *this;
			--(*this);
			return temp;
		}
		//与数字相加
		friend Vector<T>::vector_iterator operator+(const vector_iterator& it,int num)
		{
			Vector<T>::vector_iterator temp = it;
			for (int i = 0; i < num; ++i)
				++temp;
			return temp;
		}
		//与数字相减
		friend Vector<T>::vector_iterator operator-(const vector_iterator& it,int num)
		{
			Vector<T>::vector_iterator temp = it;
			for (int i = 0; i < num; ++i)
				--temp;
			return temp;
		}
		//自加
		Vector<T>::vector_iterator operator+=(int num)
		{
			for (int i = 0; i < num; ++i)
				++(*this);
			return *this;
		}
		//自减
		Vector<T>::vector_iterator operator-=(int num)
		{
			for (int i = 0; i < num; ++i)
				--(*this);
			return *this;
		}
		//两个相减
		friend int operator-(const vector_iterator& it1, const vector_iterator& it2)
		{
			int ans = 0;
			Vector<T>::vector_iterator temp = it1;
			while (it2 != temp)
			{
				--temp;
				++ans;
			}
			return ans;
		}
	};

	//清空Vector
	void clear();

	//最后添加元素
	void emplace_back(const T& x);

	//最后删除元素
	void pop_back();

	//返回最后元素
	const T back() const {
		return data[Size - 1];
	}
	T& back() {
		return data[Size - 1];
	}
	
	//返回开头元素
	const T front() const {
		return data[0];
	}
	T& front() {
		return data[0];
	}
	
	//插入
	void insert(const Vector<T>::vector_iterator& it, const T& x);


	//删除
	void remove(const Vector<T>::vector_iterator& it);

	//迭代器begin
	inline Vector<T>::vector_iterator begin() {
		if (Size == 0)
			return NULL;
		return vector_iterator(data);
	}
	//迭代器end
	inline Vector<T>::vector_iterator end() {
		if (Size == 0)
			return NULL;
		return vector_iterator(data + Size);
	}

	//返回迭代器的查找
	typename Vector<T>::vector_iterator find(const T& x);

	//重载[]
	T& operator[](const int index) {
		return data[index];
	}

	//重载=
	Vector<T>& operator=(const Vector<T>& V);

	//重新设置大小
	void resize(int size);
	void resize(int size, const T& val);
};

//构造函数
template<class T>
Vector<T>::Vector()
{
	Size = 0;
	capacity = defaultSize;
	data = new T[capacity];
}

//指定大小构造函数
template<class T>
Vector<T>::Vector(int size)
{
	Size = size;
	capacity = (Size / defaultSize + 1) * defaultSize;
	Size = 0;//只是需要size个空间，并没有添加元素进来
	data = new T[capacity];
}

//复制构造函数
template<class T>
Vector<T>::Vector(const Vector<T>& V)
{
	Size = V.Size();
	capacity = (Size / defaultSize + 1) * defaultSize;
	data = new T[capacity];

	for (int i = 0; i < Size; ++i)
		data[i] = V.data[i];
}

//指定大小和元素值构造函数
template<class T>
Vector<T>::Vector(int size,const  T& val)
{
	Size = size;
	capacity = (Size / defaultSize + 1) * defaultSize;
	data = new T[capacity];
	for (int i = 0; i < Size; ++i)
		data[i] = val;
}

//析构函数
template<class T>
Vector<T>::~Vector()
{
	delete[] data;
}

//扩容函数
template<class T>
void Vector<T>::expand()
{
	capacity += defaultSize;
	T* newData = new T[capacity];

	for (int i = 0; i < Size && i < capacity; ++i)
		newData[i] = data[i];

	delete[] data;
	data = newData;
}

//清空Vector
template<class T>
void Vector<T>::clear()
{
	Size = 0;
	capacity = defaultSize;

	delete[] data;
	data = new T[capacity];
}

//在最后添加
template<class T>
void Vector<T>::emplace_back(const T& x)
{
	if (Size == capacity - 1)
		expand();
	data[Size++] = x;//在data[Size]的位置添加
}

//最后删除
template<class T>
void Vector<T>::pop_back()
{
	if (Size == 0)
		return;
	--Size;//直接不要最后那个空间
}

//利用迭代器插入
template<class T>
void Vector<T>::insert(const Vector<T>::vector_iterator& it, const T& x)
{
	int loc = it - begin();//获得要插入的下标
	if (loc < 0 || loc > Size + 1)
		return;

	if (Size == capacity - 1)//空间不够就扩容
		expand();

	for (int i = Size - 1; i >= loc; --i)//loc到Size-1的元素全部后移
		data[i + 1] = data[i];

	data[loc] = x;
	++Size;
}

//利用迭代器删除
template<class T>
void Vector<T>::remove(const Vector<T>::vector_iterator& it)
{
	if (it == NULL|| Size == 0)
		return;

	int loc = it - begin();
	if (loc < 0 || loc >= Size)
		return;

	//往前移，覆盖掉loc位置
	for (int i = loc; i < Size - 1; ++i)
		data[i] = data[i + 1];

	--Size;
}

//重载=
template<class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& V)
{
	Size = V.size();
	capacity = V.capacity;
	delete[] data;
	data = new T[capacity];
	for (int i = 0; i < Size && i < capacity; ++i)
		data[i] = V.data[i];
	return *this;
}

//重新设置大小
template<class T>
void Vector<T>::resize(int size)
{
	if (capacity < size)
	{
		capacity = (size / defaultSize + 1) * defaultSize;
		T* newData = new T[capacity];
		for (int i = 0; i < Size; ++i)
			newData[i] = data[i];
		delete[] data;
		data = newData;
	}
}

//给出元素填充，重新设置大小
template<class T>
void Vector<T>::resize(int size,const T& val)
{
	if (capacity < size)
	{
		capacity = (size / defaultSize + 1) * defaultSize;
		T* newData = new T[capacity];
		for (int i = 0; i < Size; ++i)
			newData[i] = val;//将值全部设为val
		delete[] data;
		data = newData;
		Size = size;
	}
	else//够大就直接填充
	{
		for (int i = 0; i < size; ++i)
			data[i] = val;//将值全部设为val
		Size = size;
	}	
}

//寻找值为x的，返回迭代器
template<class T>
typename Vector<T>::vector_iterator Vector<T>::find(const T& x)
{
	for (auto it = begin(); it != end(); ++it)
		if (*it == x)
			return it;
	return NULL;
}
#endif 