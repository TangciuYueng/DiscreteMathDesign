#include <iostream>
#include "stl_priority_queue.h"
#include "stl_functional.h"
#include "stl_vector.h"
using namespace std;

//边的信息，数据成员包括两个顶点和权值
struct edge {
	int from, to;
	double val;
	//构造函数
	edge(int from = 0, int to = 0, double val = 0)
		:from(from), to(to), val(val) {}
};

//边节点，数据成员包括指向的顶点编号to、该边权值val
struct edgeNode {
	int to;
	double val;
	edgeNode* next;
	edgeNode(int to = 0, double val = 0, edgeNode* next = NULL)
		:to(to), val(val), next(next) {
	}
};

//重载比较，在优先队列中有用
bool operator< (const edge& a, const edge& b) {
	return a.val > b.val;
}
bool operator> (const edge& a, const edge& b) {
	return a.val < b.val;
}

//堆排序
void heapify(edge* nums, int start, int end)//[beg, end]
{
	int dad = start, son = 2 * dad + 1;
	while (son <= end)
	{
		//取大的那个儿子
		if ((son + 1) <= end && nums[son] > nums[son + 1])
			++son;
		if (nums[dad] < nums[son])//dad比son大，直接结束，因为要的是top最大
			return;
		//小的nums[dad]下沉到较大的子结点的位置
		edge t = nums[dad];
		nums[dad] = nums[son];
		nums[son] = t;

		dad = son;//迭代到更大的子结点
		son = dad * 2 + 1;
	}
}
void heapSort(edge* nums, int n)
{
	//初始化堆，从中间往前
	for (int i = (n - 1) / 2; i >= 0; --i)
		heapify(nums, i, n - 1);//下标从[i, n - 1]
	//堆排序，将已经排好的堆 top 放到最后（相当于出堆）
	for (int i = n - 1; i > 0; --i)//从后往前放top
	{
		edge t = nums[0];
		nums[0] = nums[i];
		nums[i] = t;

		heapify(nums, 0, i - 1);//放到最后了，重新堆化
	}
}



class MST {
private:
	//顶点数量，边数量
	int V, E;
	
	//答案数组大小
	int ansSize;

	//花费代价
	double cost;

	//边集数组
	edge* e;

	//答案输入
	edge* ans;

	//并查集数组
	int* fa;

	//邻接表数组
	edgeNode** edgeList;

	//选择的算法
	char op;

	/***************************************************************************
	函数名称：find
	功    能：并查集找祖先
	输入参数：int x，需要寻找祖先的顶点编号
	返 回 值：int，找到祖先的编号
	说    明：在找祖先的过程中进行路径压缩，加快下一次寻找的速度
	***************************************************************************/
	inline int find(int x) {
		while (fa[x] != x)
		{
			fa[x] = fa[fa[x]];
			x = fa[x];
		}
		return x;
	}

	/***************************************************************************
	函数名称：union_xy
	功    能：在并查集中将x，y合并入一个集合
	输入参数：int x, int y，两个顶点编号
	返 回 值：bool 真：合并成功，假：无需合并
	说    明：
	***************************************************************************/
	bool union_xy(int x, int y)
	{
		//寻根
		int root_x = find(x);
		int root_y = find(y);
		//判断根是否相同
		if (root_x == root_y)
			return false;
		//不相同才合并
		fa[root_x] = root_y;
		return true;
	}

	/***************************************************************************
	函数名称：is_connected
	功    能：判断x,y是否在同一个集合中
	输入参数：int x, int y，两个顶点编号
	返 回 值：bool 真：在同一个集合有相同祖先，假：不在同一个集合
	说    明：
	***************************************************************************/
	bool is_connected(int x, int y)
	{
		int root_x = find(x);
		int root_y = find(y);

		return root_x == root_y;
	}

	/***************************************************************************
	函数名称：getVE
	功    能：得到顶点和边的数量，并选择算法，申请相应数组空间
	输入参数：
	返 回 值：
	说    明：
	***************************************************************************/
	void getVE() {
		cout << "请输入所求图的顶点数目和边的而数目(以空格分隔各个数，输入两个0结束):";
		//错误判断
		while (1) {
			cin >> V >> E;
			if (cin.fail() || V < 0 || E < 0)
			{
				cout << "请输入非负整数!" << endl;
				cin.clear();
				cin.ignore(65536, '\n');
			}
			else
				break;
		}
		//没有顶点或没有边
		if (V == 0 || E == 0)
			return;
		if (E < V - 1)
		{
			cout << "边数过少，不能生成最小生成树!" << endl;
			return;
		}

		//选择算法
		cout << "请选择生成给最小树的方法" << endl;
		cout << "1.prim" << endl;
		cout << "2.kruskal" << endl;
		while (1) {
			cin >> op;
			if (op == '1' || op == '2')
				break;
			else
			{
				cout << "输入错误，请重新输入!" << endl;
				cin.clear();
				cin.ignore(65536, '\n');
			} 
		}
		//选择prim算法构建邻接表
		if (op == '1')
		{
			edgeList = new(nothrow) edgeNode * [V];
			if (edgeList == NULL)
			{
				cerr << "申请空间失败 " << endl;
				exit(1);
			}
			//初始化
			for (int i = 0; i < V; ++i)
				edgeList[i] = NULL;
		}
		//选择kruskal算法构建并查集和边集数组
		else 
		{
			//边集数组申请空间
			e = new(nothrow) edge[E];
			if (e == NULL)
			{
				cerr << "申请空间失败 " << endl;
				exit(1);
			}
			//并查集数组申请空间
			fa = new(nothrow) int[V];
			if (fa == NULL)
			{
				cerr << "申请空间失败 " << endl;
				exit(1);
			}
			//初始化fa数组
			for (int i = 0; i < V; ++i)
				fa[i] = i;
		}
		//不管选择什么算法都要为答案数组申请空间
		//答案数组同样申请空间
		ans = new(nothrow) edge[E];
		if (ans == NULL)
		{
			cerr << "申请空间失败 " << endl;
			exit(1);
		}

	}

	/***************************************************************************
	函数名称：show
	功    能：展示答案数组，求解结果
	输入参数：
	返 回 值：
	说    明：
	***************************************************************************/
	void show()
	{
		for (int i = 0; i < ansSize; ++i)
		{
			cout << "最小耗费是:" << ans[i].from + 1 << "和" << ans[i].to + 1 << endl;
		}
	}

	/***************************************************************************
	函数名称：addEdge
	功    能：用户输入添加边的信息
	输入参数：
	返 回 值：
	说    明：需要进行一系列输入错误的判断
				非法字符、顶点编号超出范围、权值负数
			根据所选择的算法对相应的数据结构信息添加
	***************************************************************************/
	void addEdge() {
		int from = 0, to = 0;
		double val = 0;
		cout << "请输入两条边的结点序号以及他们的权值(以空格分隔各个数):" << endl;
		for (int i = 0; i < E;)
		{
			cin >> from >> to >> val;
			if (cin.fail())
			{
				cout << "输入非法字符，请输入正整数!" << endl;
				cin.clear();
				cin.ignore(65536, '\n');
			}
			else if (from <= 0 || from > V || to <= 0 || to > V)
			{
				cout << "节点序号不在范围内，请重新输入!" << endl;
				cin.clear();
				cin.ignore(65536, '\n');
			}
			else if (val < 0)
			{
				cout << "权值不能为负数，请输入正整数!" << endl;
				cin.clear();
				cin.ignore(65536, '\n');
			}
			else
			{
				if (op == '1')//prim算法
				{
					--from, --to;
					//存入邻接表，无向边，正反都要存
					if (edgeList[from] == NULL)
						edgeList[from] = new edgeNode(to, val);
					else
					{
						edgeNode* cur = edgeList[from];
						while (cur && cur->to != to)
							cur = cur->next;

						//出循环 到最后了， 找到相同的
						if (cur)//相同的边
						{
							if (cur->val > val)
								cur->val = val;//取最小的存入
						}
						else//到最后没找到相同的边，插入头头
						{
							edgeNode* newNode = new edgeNode(to, val, edgeList[from]);
							edgeList[from] = newNode;
						}


					}
					
					if (edgeList[to] == NULL)
						edgeList[to] =  new edgeNode(from, val);
					else
					{
						edgeNode* cur = edgeList[to];
						while (cur && cur->to != from)
							cur = cur->next;
						//出循环 到最后了， 找到相同的
						if (cur)//相同的边
						{
							if (cur->val > val)
								cur->val = val;//取最小的存入
						}
						else//到最后没找到相同的边，插入头头
						{
							edgeNode* newNode = new edgeNode(from, val, edgeList[to]);
							edgeList[to] = newNode;
						}

					}
					++i;//让循环继续
				}
				else//kruskal算法
				{
					//直接存入边集数组，无所谓有误重复，最后排序取前面权值小的
					e[i++] = edge(from - 1, to - 1, val);
				}
			}
		}
	}

	/***************************************************************************
	函数名称：kruskal
	功    能：利用kruskal算法生成最小生成树
	输入参数：
	返 回 值：int ，返回生成树花费的代价，若不能连通则返回-1
	说    明：
	***************************************************************************/
	double kruskal()
	{
		//边集数组从小到大排序
		heapSort(e, E);

		//答案数组记录下标，顺便记录连通的边的数量
		int idx = 0;

		for (int i = 0; i < E; ++i)
		{
			//已经在同一个集合中就不能添加边，否则成环
			if (is_connected(e[i].from, e[i].to))
				continue;
			//记录代价
			cost += e[i].val;
			//合并到同一集合中
			union_xy(e[i].from, e[i].to);
			//加入答案数组
			ans[idx++] = e[i];
			//生成够了n - 1条边
			if (idx == V - 1)
				break;
		}
		//记录答案数组的大小
		ansSize = idx;
		//返回花费的代价
		if (idx == V - 1)
			return cost;
		else
			return -1;
	}

	/***************************************************************************
	函数名称：prim
	功    能：利用prim算法生成最小生成树
	输入参数：
	返 回 值：int ，返回生成树花费的代价，若不能连通则返回-1
	说    明：
	***************************************************************************/
	double prim(int start = 0)
	{
		//申请辅助数组空间
		//vis记录顶点是否访问过
		//dis记录从start顶点到其余顶点花费最小代价
		bool* vis = new(nothrow) bool[V];
		if (vis == NULL)
		{
			cerr << "申请空间失败 " << endl;
			exit(1);
		}
		double* dis = new(nothrow) double[V];
		if (dis == NULL)
		{
			cerr << "申请空间失败 " << endl;
			exit(1);
		}
		//初始值
		for (int i = 0; i < V; ++i)
		{
			vis[i] = false;
			dis[i] = INT_MAX;
		}
		//利用优先队列优化
		Priority_queue<edge> q;
		q.emplace(edge(start, start, 0));
		edge temp;
		//记录最小生成树的顶点数
		int cnt = 0;
		
		while (!q.empty())
		{
			//取出最小距离
			temp = q.top();
			q.pop();
			//遍历过了就跳过
			if (vis[temp.to])
				continue;
			//没遍历过就加入答案
			if(temp.from != temp.to)
				ans[ansSize++] = temp;
			//标记遍历过的
			vis[temp.to] = true;
			//记录最小生成树的顶点数
			++cnt;
			//更新代价
			cost += temp.val;

			//从temp.to周围找短的边
			edgeNode* cur = edgeList[temp.to];
			while (cur)
			{
				//没有遍历过，并且从start到cur->to比temp.to到cur->to长
				if (vis[cur->to] == false && cur->val < dis[cur->to])
				{
					dis[cur->to] = cur->val;
					q.emplace(edge(temp.to, cur->to, cur->val));
				}
				//下一个
				cur = cur->next;
			}
		}//end of while

		//都连通
		if (cnt == V)
			return cost;
		else
			return -1;
	}

public:
	/***************************************************************************
	函数名称：~MST
	功    能：析构函数，释放动态申请的空间
	输入参数：
	返 回 值：
	说    明：
	***************************************************************************/
	~MST() {
		delete[] fa;
		delete[] ans;
		delete[] e;
		if (op == '1')
		{
			edgeNode* cur = NULL, * temp = NULL;
			for (int i = 0; i < V; ++i)
			{
				cur = edgeList[i];;
				while (cur)
				{
					temp = cur->next;
					delete cur;
					cur = temp;
				}
			}
			delete[] edgeList;
		}
		
	}
	/***************************************************************************
	函数名称：MST
	功    能：构造函数，初始化数据成员
	输入参数：
	返 回 值：
	说    明：
	***************************************************************************/
	MST()
	{
		V = 0;
		E = 0;
		ansSize = 0;
		cost = 0;
		e = NULL;
		ans = NULL;
		fa = NULL;
		edgeList = NULL;
		op = '0';
	}

	/***************************************************************************
	函数名称：getMST
	功    能：对外接口，输入顶点和边的数量和边权值，用户选择求解得到最小生成树
	输入参数：
	返 回 值：
	说    明：
	***************************************************************************/
	void getMST() {
		getVE();
		if (V == 0 || E == 0)
			return;
		if (E < V - 1)
			return;
		addEdge();
		double res = -1;
		if (op == '1')
			res = prim();
		else
			res = kruskal();
		//返回-1说明不连通
		if (res != -1)
			show();
		else
			cout << "不能生成最小生成树" << endl;
	}
};

int main() {
	MST mst;
	mst.getMST();

	cout << "请按任意键继续..." << endl;
	cin.ignore(65535, '\n');
	cin.get();

	return 0;
}
