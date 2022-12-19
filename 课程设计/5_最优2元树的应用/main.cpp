#include <iostream>
#include "stl_queue.h"
#include "stl_priority_queue.h"
using namespace std;


//树节点
//使用静态链表储存
struct TreeNode {
	double val;
	int left, right;
	TreeNode(const double val = 0, int left = -1, int right = -1)
		:val(val), left(left), right(right) {}
};

bool operator>(const TreeNode& a, const TreeNode& b) {
	return a.val < b.val;
}
bool operator<(const TreeNode& a, const TreeNode& b) {
	return a.val > b.val;
}

//树节点与下标一起存入优先队列
struct Node {
	TreeNode data;
	int index;
	Node(TreeNode data = TreeNode(), int index = -1)
		:data(data), index(index){}
};

//重载不等号，利用优先队列
bool operator<(const Node& a, const Node& b) {
	return a.data < b.data;
}
bool operator>(const Node& a, const Node& b) {
	return a.data > b.data;
}

//哈夫曼树
class HuffmanTree {
private:
	//树的根结点
	int root;

	//结点个数
	int N;

	//哈夫曼树的静态数组存储
	TreeNode* tree;

	//存放哈夫曼编码
	char* s;

	//优先队列
	Priority_queue<Node, Vector<Node>, Greater<Node>> pq;

	/***************************************************************************
	函数名称：getN
	功    能：得到顶点个数，并申请存储哈夫曼树的空间，申请存储哈夫曼编码的空间
	输入参数：
	返 回 值：
	说    明：
	***************************************************************************/
	void getN()
	{
		cout << "请输入节点个数(必须是正整数): ";
		while (1)
		{
			cin >> N;
			if (cin.fail() || N <= 0)
			{
				cout << "输入有误，请重新输入!" << endl;
				cin.clear();
				cin.ignore(65536, '\n');
			}
			else
				break;
		}
		//申请哈夫曼树数组存储空间，N个叶子节点共需要2*N - 1个节点
		tree = new(nothrow) TreeNode[2 * N];
		if (tree == NULL)
		{
			cerr << "申请空间失败!" << endl;
			exit(1);
		}
		//申请哈夫曼编码空间
		s = new(nothrow) char[2 * N];
		if (s == NULL)
		{
			cerr << "申请空间失败!" << endl;
			exit(1);
		}
	}

	/***************************************************************************
	函数名称：getNums
	功    能：得到叶子结点的权值
	输入参数：
	返 回 值：
	说    明：
	***************************************************************************/
	void getNums()
	{
		cout << "请输入节点(以空格分隔): ";
		double temp = 0;
		for (int i = 0; i < N; )
		{
			cin >> temp;
			if (cin.fail() || temp <= 0)//非法字符
			{
				cout << "输入有误，请重新输入!" << endl;
				cin.clear();
				cin.ignore(65536, '\n');
			}
			else
			{
				tree[i].val = temp;
				pq.emplace(Node{ tree[i], i });
				++i;
			}
		}
	}

	/***************************************************************************
	函数名称：buildHuffmanTree
	功    能：构建哈夫曼树
	输入参数：
	返 回 值：
	说    明：
	***************************************************************************/
	void buildHuffmanTree()
	{
		Node temp1, temp2;
		int i = N;
		while (pq.size() > 1)
		{
			//取前两个最小的
			temp1 = pq.top();
			pq.pop();
			temp2 = pq.top();
			pq.pop();

			//两个最小的合成一棵树
			tree[i].val = temp1.data.val + temp2.data.val;
			tree[i].left = temp1.index;
			tree[i].right = temp2.index;

			//两个节点合成的树重新进入队列
			pq.emplace(Node(tree[i], i));

			//更新root值
			root = i;
			//下标移动
			++i;
		}
	}

	/***************************************************************************
	函数名称：preOrder
	功    能：前序遍历得到哈夫曼编码
	输入参数：int cur, bool from, int idx
			当前结点的下标，作为左孩子还是右孩子，哈夫曼编码数组的下标
	返 回 值：
	说    明：
	***************************************************************************/
	void preOrder(int cur, bool from, int idx)
	{
		//递归结束，到没有的节点
		if (cur == -1)
			return;

		//当前结点作为左孩子，添加0；右孩子，添加1
		if (from)
			s[idx] = '0';
		else
			s[idx] = '1';

		//到了叶子结点
		if (tree[cur].left == -1 && tree[cur].right == -1)
		{
			//最后添加尾零
			s[idx + 1] = '\0';
			//设置占位八个空格，左对齐
			cout.width(8);
			cout.setf(ios::left);
			cout << tree[cur].val << "：" << s << endl;
			return;//结束
		}
		//进入递归
		preOrder(tree[cur].left, true, idx + 1);
		preOrder(tree[cur].right, false, idx + 1);

	}

	/***************************************************************************
	函数名称：bfs
	功    能：层序遍历哈夫曼树
	输入参数：
	返 回 值：
	说    明：
	***************************************************************************/
	void bfs()
	{
		//没有建树
		if (root == -1)
			return;

		Queue<int> q;
		q.emplace(root);
		int idx = -1;
		//利用队列暂存储要遍历的结点的下标
		while (!q.empty())
		{
			int Size = q.size();
			for (int i = 0; i < Size; ++i)
			{
				idx = q.front();
				q.pop();

				cout << tree[idx].val << ' ';
				//将孩子的下标加入队列
				if (tree[idx].left != -1)
					q.emplace(tree[idx].left);

				if (tree[idx].right != -1)
					q.emplace(tree[idx].right);
			}
			cout << endl;

		}

	}

public:
	/***************************************************************************
	函数名称：~HuffmanTree
	功    能：析构函数，释放申请的动态空间
	输入参数：
	返 回 值：
	说    明：
	***************************************************************************/
	~HuffmanTree() {
		//释放申请的空间
		delete[] tree;
		delete[] s;
	}
	/***************************************************************************
	函数名称：HuffmanTree
	功    能：构造函数，初始化类的的数据成员
	输入参数：
	返 回 值：
	说    明：
	***************************************************************************/
	HuffmanTree() {
		root = -1;
		N = 0;
		tree = NULL;
		s = NULL;
	}

	/***************************************************************************
	函数名称：getHfmCode
	功    能：对外接口，用户输入序列，构建哈夫曼编码
	输入参数：
	返 回 值：
	说    明：
	***************************************************************************/
	void getHfmCode()
	{
		getN();
		getNums();
		buildHuffmanTree();
		preOrder(tree[root].left, true, 0);
		preOrder(tree[root].right, false, 0);
	}
};

int main() {
	HuffmanTree hft;
	hft.getHfmCode();
	cout << "请输入任意键继续..." << endl;
	cin.clear();
	cin.ignore(65536, '\n');
	cin.get();
	return 0;
}