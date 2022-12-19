#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <vector>
using namespace std;

class logicAlgorithm {
private:
	//运算优先级
	map<char, int> priority;

	//存储命题变元
	map<char, int> proposition;

	//输入的表达式
	string formula;

	//存储真值表的值
	vector<bool> ans;

	//存储二进制的值
	vector<int> binary;

	/***************************************************************************
	函数名称：pow2
	功    能：求2的n次方
	输入参数：
	返 回 值：int
	说    明：利用循环求取
	***************************************************************************/
	int pow2(const int& n) 
	{
		int ans = 1;
		for (int i = 0; i < n; ++i)
			ans *= 2;
		return ans;
	}

	/***************************************************************************
	函数名称：formulaCheck
	功    能：检查输入表达式是否合法
	输入参数：
	返 回 值：bool，true表示合法，false表示不合法
	说    明：对是否有非法字符、缺少命题、缺少操作符、括号匹配等进行判断
	***************************************************************************/
	bool formulaCheck()
	{		
		//空输入
		if (formula.length() == 0 || formula == "#")
			return false;
		bool prevIsPro = false;
		//记录左括号
		int cnt = 0;

		for (size_t i = 0; i < formula.length(); ++i)
		{
			auto ch = formula[i];
			//非法字符，不是运算符也不是命题变元
			if (!isalpha(ch) && !priority.count(ch))
				return false;
			//命题变元
			if (isalpha(ch))
			{
				//两个命题变元在一起
				if (prevIsPro)//上一个也是命题
					return false;
				//记录本次为命题
				prevIsPro = true;
			}
			else
			{
				//左括号计数
				if (ch == '(')
					++cnt;
				//右括号匹配
				else if (ch == ')')
				{
					//左括号缺少
					if (cnt <= 0)
						return false;
					--cnt;
				}
				switch (ch)
				{
					//二元运算符
					case '&':
					case '|':
					case '^':
					case '~':
						if (!prevIsPro && i > 0 && formula[i - 1] != ')')//两个运算符在一起
							return false;
						if (i < formula.length() - 1 && !isalpha(formula[i + 1]) && formula[i + 1] != '(')
							return false;//判断后置命题变项
						if (i == formula.length() - 1 || i == 0)
							return false;
						break;
					//一元
					case '!':
						if (prevIsPro)//前面是命题变元
							return false;
						if (i == formula.length() - 1)//缺少命题变元
							return false;
						break;
					case '(':
						if (i == formula.length() - 1 || formula[i + 1] == ')')//出现()中间没有表达式
							return false;
					default:
						break;
				}
				//更新记录上一个是否命题
				prevIsPro = false;
			}
		}
		return cnt == 0;
	}

	/***************************************************************************
	函数名称：getProposition
	功    能：从表达式中记录命题
	输入参数：
	返 回 值：
	说    明：
	***************************************************************************/
	void getProposition()
	{
		int index = 0;
		for (auto& ch : formula)
		{
			if (isalpha(ch))//是一个英文字母
			{
				//还不存在
				if (proposition.count(ch) == 0)
				{
					//插入哈希表
					proposition.insert(make_pair(ch, index++));
				}
			}
			//不属于合法字符
			else if (priority.count(ch) == 0)
			{
				cout << ch << "is undefined!" << endl;
				exit(2);
			}
		}
	}

	/***************************************************************************
	函数名称：toBinary
	功    能：向量表示的二进制加一
	输入参数：
	返 回 值：
	说    明：
	***************************************************************************/
	void toBinary()
	{
		//这里使用size_t无符号的整形
		//i == 0 再--i，并不会出现负数
		for (size_t i = binary.size() - 1;; --i)
		{
			//最后一个零变为1结束
			if (binary[i] == 0)
			{
				binary[i] = 1;
				break;
			}
			//遇到1一直变为0
			else
			{
				binary[i] = 0;
			}
			//到第一个结束循环
			if (i == 0)
				break;
		}
	}

	/***************************************************************************
	函数名称：calHelper
	功    能：计算辅助函数，取栈顶两个或一个命题计算
	输入参数：stack<char>& opter, stack<int>& pvalue，存储运算符和操作数的栈
	返 回 值：
	说    明：
	***************************************************************************/
	void calHelper(stack<char>& opter, stack<int>& pvalue)
	{
		//计算值的栈顶
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
	函数名称：calculate
	功    能：计算表达式的值
	输入参数：
	返 回 值：int 返回-1表示计算出错
	说    明：
	***************************************************************************/
	int calculate()
	{
		stack<char> opter;//操作符
		stack<int> pvalue;//命题值
		opter.push('#');
		formula += "#";
		for (auto& ch : formula)
		{
			if (isalpha(ch))
			{
				//将ch字符对应的命题变元的值加入栈中
				pvalue.emplace(binary[proposition[ch]]);
			}
			else
			{
				//此时为运算符
				auto temp = opter.top();
				//栈顶优先级大，需要先计算
				if (priority[temp] > priority[ch])
				{
					while (priority[temp] > priority[ch] && temp != '(')
					{
						calHelper(opter, pvalue);
						temp = opter.top();
						//计算完成到最后的#
						if (temp == '#' && ch == '#')
							return pvalue.top();
					}
					opter.emplace(ch);
				}
				//当前遍历到的运算符优先级大
				//直接入栈
				else
					opter.emplace(ch);
			}
			
		}
		//计算出错
		if (pvalue.size())
			return pvalue.top();
		else
			return -1;
	}
public:
	logicAlgorithm()
	{
		//运算符优先级
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
	函数名称：solution
	功    能：对外接口
	输入参数：
	返 回 值：
	说    明：
	***************************************************************************/
	void solution() {
		cout << "***************************************\n";
		cout << "**                                   **\n";
		cout << "**         欢迎进入逻辑运算软件      **\n";
		cout << "**   (可运算真值表,主范式,支持括号)  **\n";
		cout << "**                                   **\n";
		cout << "**              用!表示非            **\n";
		cout << "**              用&表示与            **\n";
		cout << "**              用|表示或            **\n";
		cout << "**             用^表示蕴含           **\n";
		cout << "**             用~表示等值           **\n";
		cout << "**                                   **\n";
		cout << "***************************************\n\n";
		cout << "Please enter a legitimate proposition formula（不能包含空格）: " << endl;
		char buffer[255] = { '\0' };
		while (1) {
			//可用于空格输入，标准输入
			fgets(buffer, 255, stdin);
			formula = buffer;
			//删除所有空格，isspace表示空白字符包括空格、\n、\t等等
			formula.erase(std::remove_if(formula.begin(), formula.end(), isspace), formula.end());
			//检查合法性
			if (formulaCheck() == false)
				cout << "命题不合法！请重新输入！" << endl;
			
			//输入合法
			else
				break;
		}
		//得到表达式中的命题变元
		getProposition();
		cout << "该式子中的变量个数为：" << proposition.size() << endl << "输出真值表如下：" << endl;
		//输出所有命题变元
		for (auto it = proposition.begin(); it != proposition.end(); ++it)
			cout << it->first << '\t';
		cout << endl << "读入的式子为：" << formula << endl;
		//真值表大大小
		int ansSize = pow2(proposition.size());
		//存储真值表答案
		ans.resize(ansSize, false);
		//转换为二进制的真值
		binary.resize(proposition.size(), 0);
		for (int i = 0; i < ansSize; ++i)
		{
			//先得到二进制
			if (i != 0)//一开始都是0，无需加一
				this->toBinary();
			//输出当前各个命题变元的值
			for (auto& val : binary)
				cout << val << '\t';
			//计算结果
			ans[i] = static_cast<bool>(this->calculate());
			cout << ans[i] << endl;
		}
		int n_m = 0, n_M = 0;
		cout << "该命题公式的主析取范式：" << endl;
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
		//没有主析取范式
		if (n_m == 0)
			cout << "0";

		cout << endl;
		cout << "该命题公式的主合取范式：" << endl;
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
		cout << "请按任意键退出..." << endl;
		
		cin.get();
	}
};
int main() {
	logicAlgorithm la;
	la.solution();
	return 0;
}
