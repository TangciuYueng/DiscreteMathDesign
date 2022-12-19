#include <iostream>
#include "stl_bitset.h"
#include "stl_hash_table.h"
#include "stl_string.h"
using namespace std;


class relation {
private:
	//����
	bool** map;
	//biset�ľ���
	Bitset* bitMap;
	//��������
	int M;
	//��Ԫ��ϵ�ĸ���
	int N;
	//ѡ����㷨
	char op;
	//��ϣ���¼������Ԫ���Լ���Ӧ���
	hashtable<String, int> hash;
	//��¼������Ԫ��
	String* Set;
	
	//�󴫵ݱհ�warshell�㷨������floyd�㷨
	void _transition() {
		for (int k = 0; k < M; ++k)
			for (int i = 0; i < M; ++i)
				for (int j = 0; j < M; ++j)
					if (map[i][k] && map[k][j])
						map[i][j] = 1;
	}

	/***************************************************************************
	�������ƣ�transition
	��    �ܣ�����Warshell�㷨��⴫�ݱհ�
	���������
	�� �� ֵ��
	˵    ����
	***************************************************************************/
	void transition() {
		for (int i = 0; i < M; ++i)
			for (int j = 0; j < M; ++j)
			{
				if (i == j)
					continue;
				if (map[j][i])//����j -> i��·��
					for (int k = 0; k < M; ++k)
						map[j][k] |=  map[i][k];//�߼��ӣ�j -> i -> k
			}
				
	}

	/***************************************************************************
	�������ƣ�bit_transition
	��    �ܣ��洢��ϵ�������λ������ͬ������Warshell�㷨���
	���������
	�� �� ֵ��
	˵    ����
	***************************************************************************/
	void bit_transition() {
		for(int i = 0; i < M; ++i)
			for (int j = 0; j < M ; ++j)
			{
				if (i == j)
					continue;
				if (bitMap[j].querry(i))//��j���߼����ϵ�i��
				{
					int capacity = bitMap[j].arrSize();
					for (int k = 0; k < capacity; ++k)
						bitMap[j].get32bit(k) |= bitMap[i].get32bit(k);
				}
			}
	}
	
	/***************************************************************************
	�������ƣ�show
	��    �ܣ�չʾ���õ��Ĵ��ݱհ���
	���������
	�� �� ֵ��
	˵    ����
	***************************************************************************/
	void show() {
		//����Ƿ��ǵ�һ������Ĺ�ϵ��
		bool firstOutput = false;
		cout << "R�Ĵ��ݱհ�(������ʽ)Ϊ:" << endl;
		cout << "t(R) = {";
		if (op == '1' || op == '3')//�ö�ά�������
		{
			for (int i = 0; i < M; ++i)
				for (int j = 0; j < M; ++j)
					if (map[i][j])
					{
						if (firstOutput)
							cout << ",";
						else
							firstOutput = true;
						cout << "<" << Set[i] << "," << Set[j] << ">";
					}
		}
		else if (op == '2')//��λ�������
		{
			for (int i = 0; i < M; ++i)
			{
				for (int j = 0; j < M; ++j)
					if (bitMap[i].querry(j))
					{
						if (firstOutput)
							cout << ",";
						else
							firstOutput = true;
						cout << "<" << Set[i] << "," << Set[j] << ">";
					}
			}
		}
		cout << "}" << endl;
		
	}

	/***************************************************************************
	�������ƣ�select
	��    �ܣ��û�ѡ���Ӧ���㷨����ִ����Ӧ�㷨�õ����ݱհ����
	���������
	�� �� ֵ��
	˵    ����
	***************************************************************************/
	void select() {
		op = '\0';
		bool loop = true;
		while (loop)
		{
			cout << endl;
			cout << "�������Ӧ���ѡ���㷨" << endl;
			cout << "1:warshell�㷨" << endl;
			cout << "2:����bitset��warshell�㷨" << endl;
			cout << "3:�˳�" << endl;
			cin >> op;
			switch (op) {
				case '1':
					this->transition();
					this->show();
					break;
				case '2':
					this->bit_transition();
					this->show();
					break;
				case '3':
					loop = false;
					break;
				default:
					cout << "����������������������!" << endl;
					cin.clear();
					cin.ignore(65536, '\n');
					break;
			}
		}
	}

	/***************************************************************************
	�������ƣ�init_aggregation
	��    �ܣ���ʼ�����ϣ��û����뼯��A��Ԫ�صĸ���
			������Ԫ�ص����֣���������Ӧ�ռ�
	���������
	�� �� ֵ��
	˵    ����
	***************************************************************************/
	void init_aggregation()
	{
		cout << "�����뼯�� A �е�Ԫ�ظ���(������)�����س���������һ�" << endl;
		while (1)
		{
			cin >> M;
			if (cin.fail() || M <= 0)
			{
				cout << "������������������!" << endl;
				cin.clear();
				cin.ignore(65536, '\n');
			}
			else
				break;
		}
		//Ϊ��������ռ�
		Set = new(nothrow) String[M];
		if (Set == NULL)
		{
			cerr << "����ռ�ʧ��" << endl;
			exit(1);
		}
		cout << "���������뼯��A�е�" << M;
		cout << "��Ԫ��(���磺abcd......�����ĸ�ʽ)�����س���������һ�" << endl;
		//���뼯����Ԫ�ص�����
		for (int i = 0; i < M; ++i)
		{
			cin >> Set[i];
			if (hash.find(Set[i]) == NULL)
				hash.insert(make_pair(Set[i], i));
			else
			{
				cout << "���������и�Ԫ�أ�����������!" << endl;
				--i;
			}
		}
		//Ϊ��ϵ����ռ�
		map = new(nothrow) bool* [M];
		if (map == NULL)
		{
			cerr << "����ռ�ʧ��" << endl;
			exit(1);
		}
		for (int i = 0; i < M; ++i) {
			map[i] = new(nothrow) bool[M];
			if (map[i] == NULL) {
				cerr << "����ռ�ʧ��" << endl;
				exit(1);
			}
		}
		//��ʼ��map
		for (int i = 0; i < M; ++i)
			for (int j = 0; j < M; ++j)
				map[i][j] = false;
		//bitset���ô�С
		bitMap = new(nothrow) Bitset[M];
		if (bitMap == NULL)
		{
			cerr << "����ռ�ʧ��" << endl;
			exit(1);
		}
		for (int i = 0; i < M; ++i)
			bitMap[i].resize(M);
	}

	/***************************************************************************
	�������ƣ�init_BinaryRelation
	��    �ܣ������Ԫ��ϵ
	���������
	�� �� ֵ��
	˵    ������Ҫ�ж�������Ĺ�ϵ���е�Ԫ���Ƿ����ڼ���A��
	***************************************************************************/
	void init_BinaryRelation()
	{
		//�����Ԫ��ϵ��������
		int Max = M * M;
		cout << "�������Ԫ��ϵ R �е�Ԫ�ظ���(������)�����س���������һ�" << endl;
		while (1)
		{
			cin >> N;
			if (cin.fail() || N < 0 || N > Max)
			{
				cout << "�����뷶Χ��[0," << Max << "]" << "��������" << endl;
				cin.clear();
				cin.ignore(65536, '\n');
			}
			else
				break;
		}
		cout << "���������� R �е�" << N << "��Ԫ�أ�һ����һ��Ԫ��" << endl;
		cout << "(���磺" << endl << "a b" << endl;
		cout << "b c" << endl;
		cout << "c d" << endl;
		cout << "......" << endl;
		cout << "�����ĸ�ʽ)�����س���������һ�" << endl;

		String temp1, temp2;
		for (int i = 0; i < N; ++i)
		{
			cin >> temp1 >> temp2;
			auto res1 = hash.find(temp1);
			auto res2 = hash.find(temp2);
			if (res1 == NULL)//�Ҳ������Ԫ��
			{
				cout << "������û��" << temp1 << "���Ԫ�أ�����������!" << endl;
				--i;
			}
			else if (res2 == NULL)
			{
				cout << "������û��" << temp2 << "���Ԫ�أ�����������!" << endl;
				--i;
			}
			else//����Ԫ�ض����ڼ�����
			{
				if (map[res1->kv.second][res2->kv.second])//�Ѿ�ʻ���������ϵ
				{
					cout << "�Ѿ����������ϵ������������!" << endl;
					--i;
				}
				else//����ϵ�����ϵ����
				{
					bitMap[res1->kv.second].set(res2->kv.second);
					map[res1->kv.second][res2->kv.second] = 1;
				}
			}
		}
	}
public:
	/***************************************************************************
	�������ƣ�relation
	��    �ܣ����캯��
	���������
	�� �� ֵ��
	˵    ����
	***************************************************************************/
	relation() {
		map = NULL;
		bitMap = NULL;
		M = 0;
		N = 0;
		op = '\0';
		Set = NULL;
	}
	/***************************************************************************
	�������ƣ�~relation
	��    �ܣ������������ͷ�����Ķ�̬�ռ�
	���������
	�� �� ֵ��
	˵    ����
	***************************************************************************/
	~relation() {
		for (int i = 0; i < M; ++i)
			delete[] map[i];

		delete[] map;
		delete[] bitMap;
		delete[] Set;
	}

	/***************************************************************************
	�������ƣ�solution
	��    �ܣ�����ӿڣ���ʼ�������û�ѡ���㷨ִ����Ӧ������⴫�ݱհ�
	���������
	�� �� ֵ��
	˵    ����
	***************************************************************************/
	void solution()
	{
		init_aggregation();
		init_BinaryRelation();
		select();
	}
};

int main() {
	relation r;
	r.solution();

	cout << "���������������..." << endl;
	cin.clear();
	cin.ignore(65536, '\n');
	cin.get();
	return 0;
}