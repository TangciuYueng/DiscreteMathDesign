#ifndef _MY_STL_BITSET
#define _MY_STL_BITSET

#define defaultSizeBitset 64
#include <assert.h>
#include <stdio.h>
const int intBit = 32;

//bitset实现
class Bitset {
private:
	int* data;
	int maxSize;
	int capacity;
public:
	//构造函数
	Bitset(const int& size = defaultSizeBitset) {
		maxSize = size;
		capacity = maxSize / intBit + 1;
		data = new int[capacity];
		//初始化全部为零
		for (int i = 0; i < capacity; ++i)
			data[i] = 0;
		assert(data != NULL);
	}
	//析构函数
	~Bitset() {
		delete[] data;
	}
	//将第n为标记为1
	void set(const int& n) {
		int index = n / intBit;
		int pos = intBit - 1 - n % intBit;
		data[index] |= (1 << pos);
	}
	//将第n为标记为0
	void unset(const int& n) {
		int index = n / intBit;
		int pos = intBit - 1 - n % intBit;
		data[index] &= ~(1 << pos);
	}
	//查询第n为状态
	bool querry(const int& n) {
		int index = n / intBit;
		int pos = intBit - 1 - n % intBit;
		return data[index] & (1 << pos);
	}
	//将第n为翻转
	void change(const int& n) {
		int index = n / intBit;
		int pos = intBit - 1 - n % intBit;
		data[index] ^= (1 << pos);
	}
	//查询有多少个1
	size_t count(const int& n) {
		size_t cnt = 0;
		for (int i = 0; i < capacity; ++i)
		{
			int temp = data[i];
			while (temp)
			{
				temp = temp & (temp - 1);//消去最右边的1
				++cnt;
			}
		}
		return cnt;
	}
	//全部为零返回true
	bool none() {
		for (int i = 0; i < capacity; ++i)
			if (data[i])
				return false;
		return true;
	}
	//有一个1返回true
	bool any() {
		for (int i = 0; i < capacity; ++i)
			if (data[i])
				return true;
		return false;
	}
	//全部都是1返回true
	bool all() {
		for (int i = 0; i < capacity; ++i)
			if (data[i] != -1)//-1在内存中全都是1
				return false;
		return true;
	}
	//重载[]
	inline const bool operator[](const int& n) {
		return querry(n);
	}
	//重新规定大小
	void resize(const int sz) {
		if (sz < maxSize)
			return;

		maxSize = sz;
		capacity = maxSize / intBit + 1;
		//释放原来的空间
		delete[] data;
		data = new int[capacity];
		//初始化全部为零
		for (int i = 0; i < capacity; ++i)
			data[i] = 0;
		assert(data != NULL);
	}
	//返回对data数组的接口
	int& get32bit(const int idx) {
		return data[idx];
	}
	//返回设置的bit个数
	inline int size() const {
		return maxSize;
	}
	//返回数组的实际大小
	inline int arrSize() const {
		return capacity;
	}
};

/*
* 函数功能：将数据类型转成内存中的二进制码输出
* 输入：类型模板T
* 输出：T类型在内存中的二进制表示
*/
template<class T>
void printf_bin(const T& num) {
	int i, k;
	unsigned char j;
	int len = sizeof(int);//现在int有4个字节
	//默认 小端，数字的低位放在地址低位
	//p先指向num后面的第4个字节的地址(0,1,2,3)
	//转成char型的指针
	unsigned char* p = (unsigned char*)&num + len - 1;

	for (i = 0; i < len; i++) { //依次处理4个字节(32位)
		//取每个字节的首地址，从高位到低位：p, p-1, p-2, p-3；
		//然后取值，正好是一个8bit
		j = *(p - i);

		for (k = 7; k >= 0; k--) { //k依次等于7,6,5,4,3,2,1,0
			//把1左移k位，然后和j取 并，如果j的这位上是1，则true
			if (j & (1 << k))
				printf("1");
			//如果j的这位上为0，则false
			else
				printf("0");
		}
		printf(" ");//每8位加一个空格
	}
	printf("\n");
}
#endif // !_MY_STL_BITSET

