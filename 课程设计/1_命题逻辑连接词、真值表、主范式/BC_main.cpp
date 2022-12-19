#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <vector>
using namespace std;

class logicAlgorithm {
private:
	//�������ȼ�
	map<char, int> priority;

	//�洢�����Ԫ
	map<char, int> proposition;

	//����ı��ʽ
	string formula;

	//�洢��ֵ���ֵ
	vector<bool> ans;

	//�洢�����Ƶ�ֵ
	vector<int> binary;

	/***************************************************************************
	�������ƣ�pow2
	��    �ܣ���2��n�η�
	���������
	�� �� ֵ��int
	˵    ��������ѭ����ȡ
	***************************************************************************/
	int pow2(const int& n) 
	{
		int ans = 1;
		for (int i = 0; i < n; ++i)
			ans *= 2;
		return ans;
	}

	/***************************************************************************
	�������ƣ�formulaCheck
	��    �ܣ����������ʽ�Ƿ�Ϸ�
	���������
	�� �� ֵ��bool��true��ʾ�Ϸ���false��ʾ���Ϸ�
	˵    �������Ƿ��зǷ��ַ���ȱ�����⡢ȱ�ٲ�����������ƥ��Ƚ����ж�
	***************************************************************************/
	bool formulaCheck()
	{		
		//������
		if (formula.length() == 0 || formula == "#")
			return false;
		bool prevIsPro = false;
		//��¼������
		int cnt = 0;

		for (size_t i = 0; i < formula.length(); ++i)
		{
			auto ch = formula[i];
			//�Ƿ��ַ������������Ҳ���������Ԫ
			if (!isalpha(ch) && !priority.count(ch))
				return false;
			//�����Ԫ
			if (isalpha(ch))
			{
				//���������Ԫ��һ��
				if (prevIsPro)//��һ��Ҳ������
					return false;
				//��¼����Ϊ����
				prevIsPro = true;
			}
			else
			{
				//�����ż���
				if (ch == '(')
					++cnt;
				//������ƥ��
				else if (ch == ')')
				{
					//������ȱ��
					if (cnt <= 0)
						return false;
					--cnt;
				}
				switch (ch)
				{
					//��Ԫ�����
					case '&':
					case '|':
					case '^':
					case '~':
						if (!prevIsPro && i > 0 && formula[i - 1] != ')')//�����������һ��
							return false;
						if (i < formula.length() - 1 && !isalpha(formula[i + 1]) && formula[i + 1] != '(')
							return false;//�жϺ����������
						if (i == formula.length() - 1 || i == 0)
							return false;
						break;
					//һԪ
					case '!':
						if (prevIsPro)//ǰ���������Ԫ
							return false;
						if (i == formula.length() - 1)//ȱ�������Ԫ
							return false;
						break;
					case '(':
						if (i == formula.length() - 1 || formula[i + 1] == ')')//����()�м�û�б��ʽ
							return false;
					default:
						break;
				}
				//���¼�¼��һ���Ƿ�����
				prevIsPro = false;
			}
		}
		return cnt == 0;
	}

	/***************************************************************************
	�������ƣ�getProposition
	��    �ܣ��ӱ��ʽ�м�¼����
	���������
	�� �� ֵ��
	˵    ����
	***************************************************************************/
	void getProposition()
	{
		int index = 0;
		for (auto& ch : formula)
		{
			if (isalpha(ch))//��һ��Ӣ����ĸ
			{
				//��������
				if (proposition.count(ch) == 0)
				{
					//�����ϣ��
					proposition.insert(make_pair(ch, index++));
				}
			}
			//�����ںϷ��ַ�
			else if (priority.count(ch) == 0)
			{
				cout << ch << "is undefined!" << endl;
				exit(2);
			}
		}
	}

	/***************************************************************************
	�������ƣ�toBinary
	��    �ܣ�������ʾ�Ķ����Ƽ�һ
	���������
	�� �� ֵ��
	˵    ����
	***************************************************************************/
	void toBinary()
	{
		//����ʹ��size_t�޷��ŵ�����
		//i == 0 ��--i����������ָ���
		for (size_t i = binary.size() - 1;; --i)
		{
			//���һ�����Ϊ1����
			if (binary[i] == 0)
			{
				binary[i] = 1;
				break;
			}
			//����1һֱ��Ϊ0
			else
			{
				binary[i] = 0;
			}
			//����һ������ѭ��
			if (i == 0)
				break;
		}
	}

	/***************************************************************************
	�������ƣ�calHelper
	��    �ܣ����㸨��������ȡջ��������һ���������
	���������stack<char>& opter, stack<int>& pvalue���洢������Ͳ�������ջ
	�� �� ֵ��
	˵    ����
	***************************************************************************/
	void calHelper(stack<char>& opter, stack<int>& pvalue)
	{
		//����ֵ��ջ��
		int temp1 = 0, temp2 = 0;
		switch (opter.top())
		{
			case '&':
				temp1 = pvalue.top();
				pvalue.pop();
				temp2 = pvalue.top();
				pvalue.pop();
				pvalue.emplace(temp1 && temp2);
				opter.pop();
				break;

			case '|':
				temp1 = pvalue.top();
				pvalue.pop();
				temp2 = pvalue.top();
				pvalue.pop();
				pvalue.emplace(temp1 || temp2);
				opter.pop();
				break;

			case '!':
				temp1 = pvalue.top();
				pvalue.pop();
				pvalue.emplace(!temp1);
				opter.pop();
				break;

			case '^':
				temp1 = pvalue.top();
				pvalue.pop();
				temp2 = pvalue.top();
				pvalue.pop();
				pvalue.emplace(!temp1 || temp2);
				opter.pop();
				break;

			case '~':
				temp1 = pvalue.top();
				pvalue.pop();
				temp2 = pvalue.top();
				pvalue.pop();
				pvalue.emplace((!temp1 || temp2) && (!temp2 || temp1));
				opter.pop();
				break;

			case '(':
			case'#':
				break;

			case ')':
				opter.pop();
				while (opter.top() != '(')
					calHelper(opter, pvalue);
				if (opter.top() == '(')
					opter.pop();
				break;
		}
	}

	/***************************************************************************
	�������ƣ�calculate
	��    �ܣ�������ʽ��ֵ
	���������
	�� �� ֵ��int ����-1��ʾ�������
	˵    ����
	***************************************************************************/
	int calculate()
	{
		stack<char> opter;//������
		stack<int> pvalue;//����ֵ
		opter.push('#');
		formula += "#";
		for (auto& ch : formula)
		{
			if (isalpha(ch))
			{
				//��ch�ַ���Ӧ�������Ԫ��ֵ����ջ��
				pvalue.emplace(binary[proposition[ch]]);
			}
			else
			{
				//��ʱΪ�����
				auto temp = opter.top();
				//ջ�����ȼ�����Ҫ�ȼ���
				if (priority[temp] > priority[ch])
				{
					while (priority[temp] > priority[ch] && temp != '(')
					{
						calHelper(opter, pvalue);
						temp = opter.top();
						//������ɵ�����#
						if (temp == '#' && ch == '#')
							return pvalue.top();
					}
					opter.emplace(ch);
				}
				//��ǰ����������������ȼ���
				//ֱ����ջ
				else
					opter.emplace(ch);
			}
			
		}
		//�������
		if (pvalue.size())
			return pvalue.top();
		else
			return -1;
	}
public:
	logicAlgorithm()
	{
		//��������ȼ�
		priority['('] = 6;
		priority[')'] = 6;
		priority['!'] = 5;
		priority['&'] = 4;
		priority['|'] = 3;
		priority['^'] = 2;
		priority['~'] = 1;
		priority['#'] = 0;

	}

	/***************************************************************************
	�������ƣ�solution
	��    �ܣ�����ӿ�
	���������
	�� �� ֵ��
	˵    ����
	***************************************************************************/
	void solution() {
		cout << "***************************************\n";
		cout << "**                                   **\n";
		cout << "**         ��ӭ�����߼��������      **\n";
		cout << "**   (��������ֵ��,����ʽ,֧������)  **\n";
		cout << "**                                   **\n";
		cout << "**              ��!��ʾ��            **\n";
		cout << "**              ��&��ʾ��            **\n";
		cout << "**              ��|��ʾ��            **\n";
		cout << "**             ��^��ʾ�̺�           **\n";
		cout << "**             ��~��ʾ��ֵ           **\n";
		cout << "**                                   **\n";
		cout << "***************************************\n\n";
		cout << "Please enter a legitimate proposition formula�����ܰ����ո�: " << endl;
		char buffer[255] = { '\0' };
		while (1) {
			//�����ڿո����룬��׼����
			fgets(buffer, 255, stdin);
			formula = buffer;
			//ɾ�����пո�isspace��ʾ�հ��ַ������ո�\n��\t�ȵ�
			formula.erase(std::remove_if(formula.begin(), formula.end(), isspace), formula.end());
			//���Ϸ���
			if (formulaCheck() == false)
				cout << "���ⲻ�Ϸ������������룡" << endl;
			
			//����Ϸ�
			else
				break;
		}
		//�õ����ʽ�е������Ԫ
		getProposition();
		cout << "��ʽ���еı�������Ϊ��" << proposition.size() << endl << "�����ֵ�����£�" << endl;
		//������������Ԫ
		for (auto it = proposition.begin(); it != proposition.end(); ++it)
			cout << it->first << '\t';
		cout << endl << "�����ʽ��Ϊ��" << formula << endl;
		//��ֵ����С
		int ansSize = pow2(proposition.size());
		//�洢��ֵ���
		ans.resize(ansSize, false);
		//ת��Ϊ�����Ƶ���ֵ
		binary.resize(proposition.size(), 0);
		for (int i = 0; i < ansSize; ++i)
		{
			//�ȵõ�������
			if (i != 0)//һ��ʼ����0�������һ
				this->toBinary();
			//�����ǰ���������Ԫ��ֵ
			for (auto& val : binary)
				cout << val << '\t';
			//������
			ans[i] = static_cast<bool>(this->calculate());
			cout << ans[i] << endl;
		}
		int n_m = 0, n_M = 0;
		cout << "�����⹫ʽ������ȡ��ʽ��" << endl;
		for (size_t i = 0; i < ans.size(); ++i)
		{
			if (ans[i])
			{
				if (n_m == 0)
					cout << "m<" << i << ">";
				else
					cout << " \\/ m<" << i << "> ";
				n_m++;
			}
		}
		//û������ȡ��ʽ
		if (n_m == 0)
			cout << "0";

		cout << endl;
		cout << "�����⹫ʽ������ȡ��ʽ��" << endl;
		for (size_t i = 0; i < ans.size(); ++i)
		{
			if (!ans[i])
			{
				if (n_M == 0)
					cout << "M<" << i << ">";
				else
					cout << " /\\ M<" << i << "> ";
				n_M++;
			}
		}
		if (n_M == 0)
			cout << "0";

		cout << endl;
		cout << "�밴������˳�..." << endl;
		
		cin.get();
	}
};
int main() {
	logicAlgorithm la;
	la.solution();
	return 0;
}
