#ifndef _MY_STL_BITSET
#define _MY_STL_BITSET

#define defaultSizeBitset 64
#include <assert.h>
#include <stdio.h>
const int intBit = 32;

//bitsetʵ��
class Bitset {
private:
	int* data;
	int maxSize;
	int capacity;
public:
	//���캯��
	Bitset(const int& size = defaultSizeBitset) {
		maxSize = size;
		capacity = maxSize / intBit + 1;
		data = new int[capacity];
		//��ʼ��ȫ��Ϊ��
		for (int i = 0; i < capacity; ++i)
			data[i] = 0;
		assert(data != NULL);
	}
	//��������
	~Bitset() {
		delete[] data;
	}
	//����nΪ���Ϊ1
	void set(const int& n) {
		int index = n / intBit;
		int pos = intBit - 1 - n % intBit;
		data[index] |= (1 << pos);
	}
	//����nΪ���Ϊ0
	void unset(const int& n) {
		int index = n / intBit;
		int pos = intBit - 1 - n % intBit;
		data[index] &= ~(1 << pos);
	}
	//��ѯ��nΪ״̬
	bool querry(const int& n) {
		int index = n / intBit;
		int pos = intBit - 1 - n % intBit;
		return data[index] & (1 << pos);
	}
	//����nΪ��ת
	void change(const int& n) {
		int index = n / intBit;
		int pos = intBit - 1 - n % intBit;
		data[index] ^= (1 << pos);
	}
	//��ѯ�ж��ٸ�1
	size_t count(const int& n) {
		size_t cnt = 0;
		for (int i = 0; i < capacity; ++i)
		{
			int temp = data[i];
			while (temp)
			{
				temp = temp & (temp - 1);//��ȥ���ұߵ�1
				++cnt;
			}
		}
		return cnt;
	}
	//ȫ��Ϊ�㷵��true
	bool none() {
		for (int i = 0; i < capacity; ++i)
			if (data[i])
				return false;
		return true;
	}
	//��һ��1����true
	bool any() {
		for (int i = 0; i < capacity; ++i)
			if (data[i])
				return true;
		return false;
	}
	//ȫ������1����true
	bool all() {
		for (int i = 0; i < capacity; ++i)
			if (data[i] != -1)//-1���ڴ���ȫ����1
				return false;
		return true;
	}
	//����[]
	inline const bool operator[](const int& n) {
		return querry(n);
	}
	//���¹涨��С
	void resize(const int sz) {
		if (sz < maxSize)
			return;

		maxSize = sz;
		capacity = maxSize / intBit + 1;
		//�ͷ�ԭ���Ŀռ�
		delete[] data;
		data = new int[capacity];
		//��ʼ��ȫ��Ϊ��
		for (int i = 0; i < capacity; ++i)
			data[i] = 0;
		assert(data != NULL);
	}
	//���ض�data����Ľӿ�
	int& get32bit(const int idx) {
		return data[idx];
	}
	//�������õ�bit����
	inline int size() const {
		return maxSize;
	}
	//���������ʵ�ʴ�С
	inline int arrSize() const {
		return capacity;
	}
};

/*
* �������ܣ�����������ת���ڴ��еĶ����������
* ���룺����ģ��T
* �����T�������ڴ��еĶ����Ʊ�ʾ
*/
template<class T>
void printf_bin(const T& num) {
	int i, k;
	unsigned char j;
	int len = sizeof(int);//����int��4���ֽ�
	//Ĭ�� С�ˣ����ֵĵ�λ���ڵ�ַ��λ
	//p��ָ��num����ĵ�4���ֽڵĵ�ַ(0,1,2,3)
	//ת��char�͵�ָ��
	unsigned char* p = (unsigned char*)&num + len - 1;

	for (i = 0; i < len; i++) { //���δ���4���ֽ�(32λ)
		//ȡÿ���ֽڵ��׵�ַ���Ӹ�λ����λ��p, p-1, p-2, p-3��
		//Ȼ��ȡֵ��������һ��8bit
		j = *(p - i);

		for (k = 7; k >= 0; k--) { //k���ε���7,6,5,4,3,2,1,0
			//��1����kλ��Ȼ���jȡ �������j����λ����1����true
			if (j & (1 << k))
				printf("1");
			//���j����λ��Ϊ0����false
			else
				printf("0");
		}
		printf(" ");//ÿ8λ��һ���ո�
	}
	printf("\n");
}
#endif // !_MY_STL_BITSET

