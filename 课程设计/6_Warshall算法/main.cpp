#include <iostream>
#include "stl_bitset.h"
#include "stl_hash_table.h"
#include "stl_string.h"
using namespace std;


class relation {
private:
	//矩阵
	bool** map;
	//biset的矩阵
	Bitset* bitMap;
	//矩阵行列
	int M;
	//二元关系的个数
	int N;
	//选择的算法
	char op;
	//哈希表记录集合中元素以及对应编号
	hashtable<String, int> hash;
	//记录集合中元素
	String* Set;
	
	//求传递闭包warshell算法，仿照floyd算法
	void _transition() {
		for (int k = 0; k < M; ++k)
			for (int i = 0; i < M; ++i)
				for (int j = 0; j < M; ++j)
					if (map[i][k] && map[k][j])
						map[i][j] = 1;
	}

	/***************************************************************************
	函数名称：transition
	功    能：利用Warshell算法求解传递闭包
	输入参数：
	返 回 值：
	说    明：
	***************************************************************************/
	void transition() {
		for (int i = 0; i < M; ++i)
			for (int j = 0; j < M; ++j)
			{
				if (i == j)
					continue;
				if (map[j][i])//存在j -> i的路径
					for (int k = 0; k < M; ++k)
						map[j][k] |=  map[i][k];//逻辑加，j -> i -> k
			}
				
	}

	/***************************************************************************
	函数名称：bit_transition
	功    能：存储关系矩阵改用位向量，同样利用Warshell算法求解
	输入参数：
	返 回 值：
	说    明：
	***************************************************************************/
	void bit_transition() {
		for(int i = 0; i < M; ++i)
			for (int j = 0; j < M ; ++j)
			{
				if (i == j)
					continue;
				if (bitMap[j].querry(i))//第j行逻辑加上第i行
				{
					int capacity = bitMap[j].arrSize();
					for (int k = 0; k < capacity; ++k)
						bitMap[j].get32bit(k) |= bitMap[i].get32bit(k);
				}
			}
	}
	
	/***************************************************************************
	函数名称：show
	功    能：展示求解得到的传递闭包、
	输入参数：
	返 回 值：
	说    明：
	***************************************************************************/
	void show() {
		//标记是否是第一个输出的关系对
		bool firstOutput = false;
		cout << "R的传递闭包(集合形式)为:" << endl;
		cout << "t(R) = {";
		if (op == '1' || op == '3')//用二维矩阵求解
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
		else if (op == '2')//由位向量求解
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
	函数名称：select
	功    能：用户选择对应的算法，并执行相应算法得到传递闭包结果
	输入参数：
	返 回 值：
	说    明：
	***************************************************************************/
	void select() {
		op = '\0';
		bool loop = true;
		while (loop)
		{
			cout << endl;
			cout << "请输入对应序号选择算法" << endl;
			cout << "1:warshell算法" << endl;
			cout << "2:利用bitset的warshell算法" << endl;
			cout << "3:退出" << endl;
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
					cout << "输入错误操作，请重新输入!" << endl;
					cin.clear();
					cin.ignore(65536, '\n');
					break;
			}
		}
	}

	/***************************************************************************
	函数名称：init_aggregation
	功    能：初始化集合，用户输入集合A中元素的个数
			集合中元素的名字，并申请相应空间
	输入参数：
	返 回 值：
	说    明：
	***************************************************************************/
	void init_aggregation()
	{
		cout << "请输入集合 A 中的元素个数(正整数)，按回车键输入下一项：" << endl;
		while (1)
		{
			cin >> M;
			if (cin.fail() || M <= 0)
			{
				cout << "输入有误，请重新输入!" << endl;
				cin.clear();
				cin.ignore(65536, '\n');
			}
			else
				break;
		}
		//为集合申请空间
		Set = new(nothrow) String[M];
		if (Set == NULL)
		{
			cerr << "申请空间失败" << endl;
			exit(1);
		}
		cout << "请依次输入集合A中的" << M;
		cout << "个元素(形如：abcd......这样的格式)，按回车键输入下一项：" << endl;
		//输入集合中元素的名字
		for (int i = 0; i < M; ++i)
		{
			cin >> Set[i];
			if (hash.find(Set[i]) == NULL)
				hash.insert(make_pair(Set[i], i));
			else
			{
				cout << "集合中已有该元素，请重新输入!" << endl;
				--i;
			}
		}
		//为关系申请空间
		map = new(nothrow) bool* [M];
		if (map == NULL)
		{
			cerr << "申请空间失败" << endl;
			exit(1);
		}
		for (int i = 0; i < M; ++i) {
			map[i] = new(nothrow) bool[M];
			if (map[i] == NULL) {
				cerr << "申请空间失败" << endl;
				exit(1);
			}
		}
		//初始化map
		for (int i = 0; i < M; ++i)
			for (int j = 0; j < M; ++j)
				map[i][j] = false;
		//bitset设置大小
		bitMap = new(nothrow) Bitset[M];
		if (bitMap == NULL)
		{
			cerr << "申请空间失败" << endl;
			exit(1);
		}
		for (int i = 0; i < M; ++i)
			bitMap[i].resize(M);
	}

	/***************************************************************************
	函数名称：init_BinaryRelation
	功    能：输入二元关系
	输入参数：
	返 回 值：
	说    明：需要判断所输入的关系对中的元素是否都属于集合A中
	***************************************************************************/
	void init_BinaryRelation()
	{
		//计算二元关系个数上限
		int Max = M * M;
		cout << "请输入二元关系 R 中的元素个数(正整数)，按回车键输入下一项：" << endl;
		while (1)
		{
			cin >> N;
			if (cin.fail() || N < 0 || N > Max)
			{
				cout << "请输入范围在[0," << Max << "]" << "的正整数" << endl;
				cin.clear();
				cin.ignore(65536, '\n');
			}
			else
				break;
		}
		cout << "请依次输入 R 中的" << N << "个元素，一行是一个元素" << endl;
		cout << "(形如：" << endl << "a b" << endl;
		cout << "b c" << endl;
		cout << "c d" << endl;
		cout << "......" << endl;
		cout << "这样的格式)，按回车键输入下一项：" << endl;

		String temp1, temp2;
		for (int i = 0; i < N; ++i)
		{
			cin >> temp1 >> temp2;
			auto res1 = hash.find(temp1);
			auto res2 = hash.find(temp2);
			if (res1 == NULL)//找不到这个元素
			{
				cout << "集合中没有" << temp1 << "这个元素，请重新输入!" << endl;
				--i;
			}
			else if (res2 == NULL)
			{
				cout << "集合中没有" << temp2 << "这个元素，请重新输入!" << endl;
				--i;
			}
			else//两个元素都属于集合中
			{
				if (map[res1->kv.second][res2->kv.second])//已经驶入了这个关系
				{
					cout << "已经输入这个关系，请重新输入!" << endl;
					--i;
				}
				else//将关系记入关系矩阵
				{
					bitMap[res1->kv.second].set(res2->kv.second);
					map[res1->kv.second][res2->kv.second] = 1;
				}
			}
		}
	}
public:
	/***************************************************************************
	函数名称：relation
	功    能：构造函数
	输入参数：
	返 回 值：
	说    明：
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
	函数名称：~relation
	功    能：析构函数，释放申请的动态空间
	输入参数：
	返 回 值：
	说    明：
	***************************************************************************/
	~relation() {
		for (int i = 0; i < M; ++i)
			delete[] map[i];

		delete[] map;
		delete[] bitMap;
		delete[] Set;
	}

	/***************************************************************************
	函数名称：solution
	功    能：对外接口，初始化并由用户选择算法执行相应函数求解传递闭包
	输入参数：
	返 回 值：
	说    明：
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

	cout << "请输入任意键继续..." << endl;
	cin.clear();
	cin.ignore(65536, '\n');
	cin.get();
	return 0;
}