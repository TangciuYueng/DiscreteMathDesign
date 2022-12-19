#include <iostream>
#include "stl_priority_queue.h"
#include "stl_functional.h"
#include "stl_vector.h"
using namespace std;

//�ߵ���Ϣ�����ݳ�Ա�������������Ȩֵ
struct edge {
	int from, to;
	double val;
	//���캯��
	edge(int from = 0, int to = 0, double val = 0)
		:from(from), to(to), val(val) {}
};

//�߽ڵ㣬���ݳ�Ա����ָ��Ķ�����to���ñ�Ȩֵval
struct edgeNode {
	int to;
	double val;
	edgeNode* next;
	edgeNode(int to = 0, double val = 0, edgeNode* next = NULL)
		:to(to), val(val), next(next) {
	}
};

//���رȽϣ������ȶ���������
bool operator< (const edge& a, const edge& b) {
	return a.val > b.val;
}
bool operator> (const edge& a, const edge& b) {
	return a.val < b.val;
}

//������
void heapify(edge* nums, int start, int end)//[beg, end]
{
	int dad = start, son = 2 * dad + 1;
	while (son <= end)
	{
		//ȡ����Ǹ�����
		if ((son + 1) <= end && nums[son] > nums[son + 1])
			++son;
		if (nums[dad] < nums[son])//dad��son��ֱ�ӽ�������ΪҪ����top���
			return;
		//С��nums[dad]�³����ϴ���ӽ���λ��
		edge t = nums[dad];
		nums[dad] = nums[son];
		nums[son] = t;

		dad = son;//������������ӽ��
		son = dad * 2 + 1;
	}
}
void heapSort(edge* nums, int n)
{
	//��ʼ���ѣ����м���ǰ
	for (int i = (n - 1) / 2; i >= 0; --i)
		heapify(nums, i, n - 1);//�±��[i, n - 1]
	//�����򣬽��Ѿ��źõĶ� top �ŵ�����൱�ڳ��ѣ�
	for (int i = n - 1; i > 0; --i)//�Ӻ���ǰ��top
	{
		edge t = nums[0];
		nums[0] = nums[i];
		nums[i] = t;

		heapify(nums, 0, i - 1);//�ŵ�����ˣ����¶ѻ�
	}
}



class MST {
private:
	//����������������
	int V, E;
	
	//�������С
	int ansSize;

	//���Ѵ���
	double cost;

	//�߼�����
	edge* e;

	//������
	edge* ans;

	//���鼯����
	int* fa;

	//�ڽӱ�����
	edgeNode** edgeList;

	//ѡ����㷨
	char op;

	/***************************************************************************
	�������ƣ�find
	��    �ܣ����鼯������
	���������int x����ҪѰ�����ȵĶ�����
	�� �� ֵ��int���ҵ����ȵı��
	˵    �����������ȵĹ����н���·��ѹ�����ӿ���һ��Ѱ�ҵ��ٶ�
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
	�������ƣ�union_xy
	��    �ܣ��ڲ��鼯�н�x��y�ϲ���һ������
	���������int x, int y������������
	�� �� ֵ��bool �棺�ϲ��ɹ����٣�����ϲ�
	˵    ����
	***************************************************************************/
	bool union_xy(int x, int y)
	{
		//Ѱ��
		int root_x = find(x);
		int root_y = find(y);
		//�жϸ��Ƿ���ͬ
		if (root_x == root_y)
			return false;
		//����ͬ�źϲ�
		fa[root_x] = root_y;
		return true;
	}

	/***************************************************************************
	�������ƣ�is_connected
	��    �ܣ��ж�x,y�Ƿ���ͬһ��������
	���������int x, int y������������
	�� �� ֵ��bool �棺��ͬһ����������ͬ���ȣ��٣�����ͬһ������
	˵    ����
	***************************************************************************/
	bool is_connected(int x, int y)
	{
		int root_x = find(x);
		int root_y = find(y);

		return root_x == root_y;
	}

	/***************************************************************************
	�������ƣ�getVE
	��    �ܣ��õ�����ͱߵ���������ѡ���㷨��������Ӧ����ռ�
	���������
	�� �� ֵ��
	˵    ����
	***************************************************************************/
	void getVE() {
		cout << "����������ͼ�Ķ�����Ŀ�ͱߵĶ���Ŀ(�Կո�ָ�����������������0����):";
		//�����ж�
		while (1) {
			cin >> V >> E;
			if (cin.fail() || V < 0 || E < 0)
			{
				cout << "������Ǹ�����!" << endl;
				cin.clear();
				cin.ignore(65536, '\n');
			}
			else
				break;
		}
		//û�ж����û�б�
		if (V == 0 || E == 0)
			return;
		if (E < V - 1)
		{
			cout << "�������٣�����������С������!" << endl;
			return;
		}

		//ѡ���㷨
		cout << "��ѡ�����ɸ���С���ķ���" << endl;
		cout << "1.prim" << endl;
		cout << "2.kruskal" << endl;
		while (1) {
			cin >> op;
			if (op == '1' || op == '2')
				break;
			else
			{
				cout << "�����������������!" << endl;
				cin.clear();
				cin.ignore(65536, '\n');
			} 
		}
		//ѡ��prim�㷨�����ڽӱ�
		if (op == '1')
		{
			edgeList = new(nothrow) edgeNode * [V];
			if (edgeList == NULL)
			{
				cerr << "����ռ�ʧ�� " << endl;
				exit(1);
			}
			//��ʼ��
			for (int i = 0; i < V; ++i)
				edgeList[i] = NULL;
		}
		//ѡ��kruskal�㷨�������鼯�ͱ߼�����
		else 
		{
			//�߼���������ռ�
			e = new(nothrow) edge[E];
			if (e == NULL)
			{
				cerr << "����ռ�ʧ�� " << endl;
				exit(1);
			}
			//���鼯��������ռ�
			fa = new(nothrow) int[V];
			if (fa == NULL)
			{
				cerr << "����ռ�ʧ�� " << endl;
				exit(1);
			}
			//��ʼ��fa����
			for (int i = 0; i < V; ++i)
				fa[i] = i;
		}
		//����ѡ��ʲô�㷨��ҪΪ����������ռ�
		//������ͬ������ռ�
		ans = new(nothrow) edge[E];
		if (ans == NULL)
		{
			cerr << "����ռ�ʧ�� " << endl;
			exit(1);
		}

	}

	/***************************************************************************
	�������ƣ�show
	��    �ܣ�չʾ�����飬�����
	���������
	�� �� ֵ��
	˵    ����
	***************************************************************************/
	void show()
	{
		for (int i = 0; i < ansSize; ++i)
		{
			cout << "��С�ķ���:" << ans[i].from + 1 << "��" << ans[i].to + 1 << endl;
		}
	}

	/***************************************************************************
	�������ƣ�addEdge
	��    �ܣ��û�������ӱߵ���Ϣ
	���������
	�� �� ֵ��
	˵    ������Ҫ����һϵ�����������ж�
				�Ƿ��ַ��������ų�����Χ��Ȩֵ����
			������ѡ����㷨����Ӧ�����ݽṹ��Ϣ���
	***************************************************************************/
	void addEdge() {
		int from = 0, to = 0;
		double val = 0;
		cout << "�����������ߵĽ������Լ����ǵ�Ȩֵ(�Կո�ָ�������):" << endl;
		for (int i = 0; i < E;)
		{
			cin >> from >> to >> val;
			if (cin.fail())
			{
				cout << "����Ƿ��ַ���������������!" << endl;
				cin.clear();
				cin.ignore(65536, '\n');
			}
			else if (from <= 0 || from > V || to <= 0 || to > V)
			{
				cout << "�ڵ���Ų��ڷ�Χ�ڣ�����������!" << endl;
				cin.clear();
				cin.ignore(65536, '\n');
			}
			else if (val < 0)
			{
				cout << "Ȩֵ����Ϊ������������������!" << endl;
				cin.clear();
				cin.ignore(65536, '\n');
			}
			else
			{
				if (op == '1')//prim�㷨
				{
					--from, --to;
					//�����ڽӱ�����ߣ�������Ҫ��
					if (edgeList[from] == NULL)
						edgeList[from] = new edgeNode(to, val);
					else
					{
						edgeNode* cur = edgeList[from];
						while (cur && cur->to != to)
							cur = cur->next;

						//��ѭ�� ������ˣ� �ҵ���ͬ��
						if (cur)//��ͬ�ı�
						{
							if (cur->val > val)
								cur->val = val;//ȡ��С�Ĵ���
						}
						else//�����û�ҵ���ͬ�ıߣ�����ͷͷ
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
						//��ѭ�� ������ˣ� �ҵ���ͬ��
						if (cur)//��ͬ�ı�
						{
							if (cur->val > val)
								cur->val = val;//ȡ��С�Ĵ���
						}
						else//�����û�ҵ���ͬ�ıߣ�����ͷͷ
						{
							edgeNode* newNode = new edgeNode(from, val, edgeList[to]);
							edgeList[to] = newNode;
						}

					}
					++i;//��ѭ������
				}
				else//kruskal�㷨
				{
					//ֱ�Ӵ���߼����飬����ν�����ظ����������ȡǰ��ȨֵС��
					e[i++] = edge(from - 1, to - 1, val);
				}
			}
		}
	}

	/***************************************************************************
	�������ƣ�kruskal
	��    �ܣ�����kruskal�㷨������С������
	���������
	�� �� ֵ��int ���������������ѵĴ��ۣ���������ͨ�򷵻�-1
	˵    ����
	***************************************************************************/
	double kruskal()
	{
		//�߼������С��������
		heapSort(e, E);

		//�������¼�±꣬˳���¼��ͨ�ıߵ�����
		int idx = 0;

		for (int i = 0; i < E; ++i)
		{
			//�Ѿ���ͬһ�������оͲ�����ӱߣ�����ɻ�
			if (is_connected(e[i].from, e[i].to))
				continue;
			//��¼����
			cost += e[i].val;
			//�ϲ���ͬһ������
			union_xy(e[i].from, e[i].to);
			//���������
			ans[idx++] = e[i];
			//���ɹ���n - 1����
			if (idx == V - 1)
				break;
		}
		//��¼������Ĵ�С
		ansSize = idx;
		//���ػ��ѵĴ���
		if (idx == V - 1)
			return cost;
		else
			return -1;
	}

	/***************************************************************************
	�������ƣ�prim
	��    �ܣ�����prim�㷨������С������
	���������
	�� �� ֵ��int ���������������ѵĴ��ۣ���������ͨ�򷵻�-1
	˵    ����
	***************************************************************************/
	double prim(int start = 0)
	{
		//���븨������ռ�
		//vis��¼�����Ƿ���ʹ�
		//dis��¼��start���㵽���ඥ�㻨����С����
		bool* vis = new(nothrow) bool[V];
		if (vis == NULL)
		{
			cerr << "����ռ�ʧ�� " << endl;
			exit(1);
		}
		double* dis = new(nothrow) double[V];
		if (dis == NULL)
		{
			cerr << "����ռ�ʧ�� " << endl;
			exit(1);
		}
		//��ʼֵ
		for (int i = 0; i < V; ++i)
		{
			vis[i] = false;
			dis[i] = INT_MAX;
		}
		//�������ȶ����Ż�
		Priority_queue<edge> q;
		q.emplace(edge(start, start, 0));
		edge temp;
		//��¼��С�������Ķ�����
		int cnt = 0;
		
		while (!q.empty())
		{
			//ȡ����С����
			temp = q.top();
			q.pop();
			//�������˾�����
			if (vis[temp.to])
				continue;
			//û�������ͼ����
			if(temp.from != temp.to)
				ans[ansSize++] = temp;
			//��Ǳ�������
			vis[temp.to] = true;
			//��¼��С�������Ķ�����
			++cnt;
			//���´���
			cost += temp.val;

			//��temp.to��Χ�Ҷ̵ı�
			edgeNode* cur = edgeList[temp.to];
			while (cur)
			{
				//û�б����������Ҵ�start��cur->to��temp.to��cur->to��
				if (vis[cur->to] == false && cur->val < dis[cur->to])
				{
					dis[cur->to] = cur->val;
					q.emplace(edge(temp.to, cur->to, cur->val));
				}
				//��һ��
				cur = cur->next;
			}
		}//end of while

		//����ͨ
		if (cnt == V)
			return cost;
		else
			return -1;
	}

public:
	/***************************************************************************
	�������ƣ�~MST
	��    �ܣ������������ͷŶ�̬����Ŀռ�
	���������
	�� �� ֵ��
	˵    ����
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
	�������ƣ�MST
	��    �ܣ����캯������ʼ�����ݳ�Ա
	���������
	�� �� ֵ��
	˵    ����
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
	�������ƣ�getMST
	��    �ܣ�����ӿڣ����붥��ͱߵ������ͱ�Ȩֵ���û�ѡ�����õ���С������
	���������
	�� �� ֵ��
	˵    ����
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
		//����-1˵������ͨ
		if (res != -1)
			show();
		else
			cout << "����������С������" << endl;
	}
};

int main() {
	MST mst;
	mst.getMST();

	cout << "�밴���������..." << endl;
	cin.ignore(65535, '\n');
	cin.get();

	return 0;
}
