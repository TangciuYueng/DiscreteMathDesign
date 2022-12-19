#include <iostream>
#include "stl_queue.h"
#include "stl_priority_queue.h"
using namespace std;


//���ڵ�
//ʹ�þ�̬������
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

//���ڵ����±�һ��������ȶ���
struct Node {
	TreeNode data;
	int index;
	Node(TreeNode data = TreeNode(), int index = -1)
		:data(data), index(index){}
};

//���ز��Ⱥţ��������ȶ���
bool operator<(const Node& a, const Node& b) {
	return a.data < b.data;
}
bool operator>(const Node& a, const Node& b) {
	return a.data > b.data;
}

//��������
class HuffmanTree {
private:
	//���ĸ����
	int root;

	//������
	int N;

	//���������ľ�̬����洢
	TreeNode* tree;

	//��Ź���������
	char* s;

	//���ȶ���
	Priority_queue<Node, Vector<Node>, Greater<Node>> pq;

	/***************************************************************************
	�������ƣ�getN
	��    �ܣ��õ����������������洢���������Ŀռ䣬����洢����������Ŀռ�
	���������
	�� �� ֵ��
	˵    ����
	***************************************************************************/
	void getN()
	{
		cout << "������ڵ����(������������): ";
		while (1)
		{
			cin >> N;
			if (cin.fail() || N <= 0)
			{
				cout << "������������������!" << endl;
				cin.clear();
				cin.ignore(65536, '\n');
			}
			else
				break;
		}
		//���������������洢�ռ䣬N��Ҷ�ӽڵ㹲��Ҫ2*N - 1���ڵ�
		tree = new(nothrow) TreeNode[2 * N];
		if (tree == NULL)
		{
			cerr << "����ռ�ʧ��!" << endl;
			exit(1);
		}
		//�������������ռ�
		s = new(nothrow) char[2 * N];
		if (s == NULL)
		{
			cerr << "����ռ�ʧ��!" << endl;
			exit(1);
		}
	}

	/***************************************************************************
	�������ƣ�getNums
	��    �ܣ��õ�Ҷ�ӽ���Ȩֵ
	���������
	�� �� ֵ��
	˵    ����
	***************************************************************************/
	void getNums()
	{
		cout << "������ڵ�(�Կո�ָ�): ";
		double temp = 0;
		for (int i = 0; i < N; )
		{
			cin >> temp;
			if (cin.fail() || temp <= 0)//�Ƿ��ַ�
			{
				cout << "������������������!" << endl;
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
	�������ƣ�buildHuffmanTree
	��    �ܣ�������������
	���������
	�� �� ֵ��
	˵    ����
	***************************************************************************/
	void buildHuffmanTree()
	{
		Node temp1, temp2;
		int i = N;
		while (pq.size() > 1)
		{
			//ȡǰ������С��
			temp1 = pq.top();
			pq.pop();
			temp2 = pq.top();
			pq.pop();

			//������С�ĺϳ�һ����
			tree[i].val = temp1.data.val + temp2.data.val;
			tree[i].left = temp1.index;
			tree[i].right = temp2.index;

			//�����ڵ�ϳɵ������½������
			pq.emplace(Node(tree[i], i));

			//����rootֵ
			root = i;
			//�±��ƶ�
			++i;
		}
	}

	/***************************************************************************
	�������ƣ�preOrder
	��    �ܣ�ǰ������õ�����������
	���������int cur, bool from, int idx
			��ǰ�����±꣬��Ϊ���ӻ����Һ��ӣ�����������������±�
	�� �� ֵ��
	˵    ����
	***************************************************************************/
	void preOrder(int cur, bool from, int idx)
	{
		//�ݹ��������û�еĽڵ�
		if (cur == -1)
			return;

		//��ǰ�����Ϊ���ӣ����0���Һ��ӣ����1
		if (from)
			s[idx] = '0';
		else
			s[idx] = '1';

		//����Ҷ�ӽ��
		if (tree[cur].left == -1 && tree[cur].right == -1)
		{
			//������β��
			s[idx + 1] = '\0';
			//����ռλ�˸��ո������
			cout.width(8);
			cout.setf(ios::left);
			cout << tree[cur].val << "��" << s << endl;
			return;//����
		}
		//����ݹ�
		preOrder(tree[cur].left, true, idx + 1);
		preOrder(tree[cur].right, false, idx + 1);

	}

	/***************************************************************************
	�������ƣ�bfs
	��    �ܣ����������������
	���������
	�� �� ֵ��
	˵    ����
	***************************************************************************/
	void bfs()
	{
		//û�н���
		if (root == -1)
			return;

		Queue<int> q;
		q.emplace(root);
		int idx = -1;
		//���ö����ݴ洢Ҫ�����Ľ����±�
		while (!q.empty())
		{
			int Size = q.size();
			for (int i = 0; i < Size; ++i)
			{
				idx = q.front();
				q.pop();

				cout << tree[idx].val << ' ';
				//�����ӵ��±�������
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
	�������ƣ�~HuffmanTree
	��    �ܣ������������ͷ�����Ķ�̬�ռ�
	���������
	�� �� ֵ��
	˵    ����
	***************************************************************************/
	~HuffmanTree() {
		//�ͷ�����Ŀռ�
		delete[] tree;
		delete[] s;
	}
	/***************************************************************************
	�������ƣ�HuffmanTree
	��    �ܣ����캯������ʼ����ĵ����ݳ�Ա
	���������
	�� �� ֵ��
	˵    ����
	***************************************************************************/
	HuffmanTree() {
		root = -1;
		N = 0;
		tree = NULL;
		s = NULL;
	}

	/***************************************************************************
	�������ƣ�getHfmCode
	��    �ܣ�����ӿڣ��û��������У���������������
	���������
	�� �� ֵ��
	˵    ����
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
	cout << "���������������..." << endl;
	cin.clear();
	cin.ignore(65536, '\n');
	cin.get();
	return 0;
}