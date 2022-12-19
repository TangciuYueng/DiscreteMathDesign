#ifndef _MY_STL_STRING
#define _MY_STL_STRING
#include <iostream>
using namespace std;
//默认申请空间大小50个char
const int defaultSizeString = 50;

class String {
private:
	//指向尾零的位置，初始化时候注意Size = 1
	int Size;

	//实际容量
	int capacity;

	//数据数组
	char* data;

	//容量扩张defaultSizeString
	void expand();

	//模式串匹配得到next数组函数
	void getNext(const char* str, int* next, int len);
public:
	//空构造函数
	String();
	
	//赋值构造函数
	String(const String& str);

	//字符串构造函数
	String(const char* str);

	//大小为size的字符串构造函数
	String(const char* str, int size);
	
	//析构函数
	~String();

	//字符串长度
	int length()const {
		//减去的是尾零
		return Size - 1;
	}

	//计算字符串长度，与实例化对象无关，声明为静态成员函数
	static int strLen(const char* str);

	//重载=，Stirng
	String& operator=(const String& str);

	//重载=，字符串
	String& operator=(const char* str);

	//重载=，一个字符
	String& operator=(const char& ch);

	//重载<<
	friend ostream& operator<<(ostream& out, const String& str) {
		out << str.data;
		return out;
	}

	//重载>>
	friend istream& operator>>(istream& in,String& str) {
		//这里存在问题，输入太多字符该如何是好
		in >> str.data;
		str.Size = str.strLen(str.data) + 1;
		return in;
	}

	//重载比较函数
	friend int strCmp(const String& str1, const String& str2)
	{
		int i = 0;
		while (str1.data[i] && str2.data[i])
		{
			//出现了不同的，返回相同位置第一个减第二个
			if (str1.data[i] != str2.data[i])
				return str1.data[i] - str2.data[i];
			else
				++i;
		}
		
		if (str1.data[i])
			return str1.data[i];
		else
			return 0 - str2.data[i];
	}
	//重载< > <= >=
	friend bool operator<(const String& str1, const String& str2){
		return strCmp(str1, str2) < 0;
	}
	friend bool operator>(const String& str1, const String& str2){
		return strCmp(str1, str2) > 0;
	}
	friend bool operator<=(const String& str1, const String& str2){
		return strCmp(str1, str2) <= 0;
	}
	friend bool operator>=(const String& str1, const String& str2){
		return strCmp(str1, str2) >= 0;
	}

	//重载[]
	inline char& operator[](const int index) {
		return data[index];
	}
	inline char& operator[](const size_t index) {
		return data[index];
	}

	//重载==
	bool operator==(const String& str);
	bool operator==(const char* str);

	//重载+
	String operator+(const String& str);
	String operator+(const char* str);

	//重载+=
	String& operator+=(const String& str);
	String& operator+=(const char* str);
	String& operator+=(const char ch);

	//返回字符串类型
	char* c_str() {
		return data;
	}
	char* c_str() const{
		return data;
	} 

	//模式匹配
	int find(const char* str);
	int find(String& str);

	//删除所有的某个字符
	void erase(char ch);

	//翻转
	void reverse();

	//清空
	void clear();

	//删掉最后一个
	void pop_back() {
		//空的字符串
		if (Size == 1)
			return;
		--Size;
		data[Size - 1] = '\0';//之前尾零前的字符放上尾零
	}

	//获得最后一个字符
	char back() const {
		if (Size <= 1)
			return '\0';
		return data[Size - 2];
	}

};
//构造函数
String::String()
{
	Size = 1;
	capacity = defaultSizeString;
	data = new char[capacity];
	data[0] = '\0';
}
String::String(const String& str)
{
	Size = str.Size;
	capacity = (Size / defaultSizeString + 1) * defaultSizeString;
	data = new char[capacity];
	for (int i = 0; i < Size; ++i)
		data[i] = str.data[i];
}
String::String(const char* str)
{
	int size = 0;
	while (str[size] != '\0')
		++size;
	++size;//最后一个尾零

	Size = size;

	capacity = (Size / defaultSizeString + 1) * defaultSizeString;
	data = new char[capacity];
	for (int i = 0; i < Size; ++i)
		data[i] = str[i];

}
String::String(const char* str, int size)
{
	Size = size;
	capacity = (Size / defaultSizeString + 1) * defaultSizeString;
	data = new char[capacity];

	for (int i = 0; i < Size; ++i)
		data[i] = str[i];
}

//析构函数
String::~String() {
	delete[] data;
}

//字符串长度
int String::strLen(const char* str)
{
	int size = 0;
	while (str[size] != '\0')
		++size;
	return size;
}

//容量扩张defaultSizeString
void String::expand()
{
	capacity += defaultSizeString;
	char* newData = new char[capacity];
	for (int i = 0; i < Size && Size < capacity; ++i)
		newData[i] = data[i];
	delete[] data;
	data = newData;
}

//重载=
String& String::operator=(const String& str)
{
	Size = str.Size;
	capacity = (Size / defaultSizeString + 1) * defaultSizeString;
	delete[] data;
	data = new char[capacity];
	for (int i = 0; i < Size; ++i)
		data[i] = str.data[i];
	return *this;
}
String& String::operator=(const char* str)
{
	int size = 0;
	while (str[size] != '\0')
		++size;
	++size;//尾零

	Size = size;
	capacity = (Size / defaultSizeString + 1) * defaultSizeString;
	delete[] data;
	data = new char[capacity];
	for (int i = 0; i < Size; ++i)
		data[i] = str[i];
	return *this;
}
String& String::operator=(const char& ch)
{
	Size = 2;
	capacity = (Size / defaultSizeString + 1) * defaultSizeString;
	delete[] data;
	data = new char[capacity];
	data[0] = ch;
	data[1] = '\0';
	return *this;
}

//重载==
bool String::operator==(const String& str)
{
	if (Size != str.Size)
		return false;
	for (int i = 0; i < Size; ++i)
		if (data[i] != str.data[i])
			return false;
	return true;
}
bool String::operator==(const char* str)
{
	int size = 0;
	while (str[size] != '\0')
		++size;
	++size;//尾零

	if (Size != size)
		return false;

	for (int i = 0; i < Size; ++i)
		if (data[i] != str[i])
			return false;
	return true;
}

//重载+
String String::operator+(const String& str)
{
	String temp(str.data, str.Size + Size);

	//包括str的尾零
	//注意data[Size]是尾零
	for (int i = 0; i <= str.Size; ++i)
		temp.data[Size + i - 1] = str.data[i];

	return temp;
}
String String::operator+(const char* str)
{
	int size = 0;
	while (str[size] != '\0')
		++size;
	++size;
	String temp(str, Size + size);
	
	
	//包括str的尾零
	//注意data[Size]是尾零
	for (int i = 0; i <= size; ++i)
		temp.data[Size + i - 1] = str[i];

	return temp;
}

//重载+=
String& String::operator+=(const String& str)
{
	if (Size + str.Size > capacity)
	{
		capacity = ((str.Size + Size) / defaultSizeString + 1) * defaultSizeString;
		char* newData = new char[capacity];
		for (int i = 0; i < Size; ++i)
			newData[i] = data[i];
		delete[] data;
		data = newData;
	}
	//包括str的尾零
	//注意data[Size - 1]是尾零
	for (int i = 0; i < str.Size; ++i)
		data[Size + i - 1] = str.data[i];

	Size += str.Size - 1;
	return *this;
}
String& String::operator+=(const char* str)
{
	int size = 0;
	while (str[size] != '\0')
		++size;
	++size;

	if (Size + size > capacity)
	{
		capacity = ((size + Size) / defaultSizeString + 1) * defaultSizeString;
		char* newData = new char[capacity];
		for (int i = 0; i < Size; ++i)
			newData[i] = data[i];
		delete[] data;
		data = newData;
	}
	//包括str的尾零
	//注意data[Size - 1]是尾零
	for (int i = 0; i < size; ++i)
		data[Size + i - 1] = str[i];

	Size += size - 1;
	return *this;
}
String& String::operator+=(const char ch)
{
	if (Size + 1 > capacity)
	{
		capacity = ((1 + Size) / defaultSizeString + 1) * defaultSizeString;
		char* newData = new char[capacity];
		for (int i = 0; i < Size; ++i)
			newData[i] = data[i];
		delete[] data;
		data = newData;
	}
	//包括str的尾零
	//注意data[Size - 1]是尾零
	data[Size - 1] = ch;
	data[Size] = '\0';
	++Size;
	return *this;
}

//模式串匹配
void String::getNext(const char* str, int* next, int len)
{
	next[0] = -1;
	next[1] = 0;
	int i = 2;//从2开始
	int cn = 0;//cn表示next[i - 1]的信息

	while (i < len)
	{
		if (str[i - 1] == str[cn])//可以继续相等
			next[i++] = ++cn;
		else if (cn > 0)//不可以继续相等
			cn = next[cn];
		else
			next[i++] = 0;
	}
}
int String::find(String& str)
{
	
	if (Size < str.Size || Size == 1 || str.Size == 1)
		return -1;
	int i1 = 0, i2 = 0, len1 = Size - 1, len2 = str.Size - 1;

	int* next = new int[len2 + 1];
	getNext(str.data, next, len2);

	while (i1 < len1 && i2 < len2)
	{
		if (data[i1] == str.data[i2])
			++i1, ++i2;
		else if (i2 != 0)
			i2 = next[i2];
		else
			++i1;
	}
	delete[] next;
	return i2 == len2 ? i1 - i2 : -1;
}
int String::find(const char* str)
{
	int len2 = 0, len1 = Size - 1;
	while (str[len2] != '\0')
		++len2;

	if (len1 < len2 || len1 == 0 || len2 == 0)
		return -1;

	int* next = new int[len2 + 1];
	getNext(str, next, len2);

	int i1 = 0, i2 = 0;
	while (i1 < len1 && i2 < len2)
	{
		if (data[i1] == str[i2])
			++i1, ++i2;
		else if (i2 != 0)
			i2 = next[i2];
		else
			++i1;
	}
	delete[] next;
	return i2 == len2 ? i1 - i2 : -1;
}

//删除所有值为ch的
void String::erase(char ch)
{
	int i = 0, k = 0;
	while(i < Size)
	{
		if (ch != data[i])//data[i]不等于了才赋值并且新数组的指针k++
			data[k++] = data[i];
		++i;
	}
	Size -= i - k;
}

//翻转
void String::reverse()
{
	int len = Size - 1;
	for (int i = 0; i < len / 2; ++i)
	{
		char t = data[i];
		data[i] = data[len - i - 1];
		data[len - i - 1] = t;
	}
}

//清空
void String::clear()
{
	//删掉原来的
	delete[] data;
	//重新设置
	Size = 1;
	capacity = defaultSizeString;
	data = new char[capacity];
	data[0] = '\0';
}
#endif // !_MY_STL_STRING
