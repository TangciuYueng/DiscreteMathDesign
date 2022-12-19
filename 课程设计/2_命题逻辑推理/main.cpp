#include <iostream>
using namespace std;

int main() {
	/*
		1.利用五个整型变量代表命题变元的真假值
		2.	A:营业员A偷了手表
			B:营业员B偷了手表
			C:作案不在营业时间
			D:B提供的证据正确
			E:货柜未上锁
		3.根据题目要求写出表达式
		(A || B) && (!A || C) && (!D || E) && (!C || D) && !E
		4.利用穷举法依次求取每项命题变元为真时为假时的表达式最终情况
	*/
	int A, B, C, D, E;
	for (A = 0; A <= 1; ++A)
		for (B = 0; B <= 1; ++B)
			for (C = 0; C <= 1; ++C)
				for (D = 0; D <= 1; ++D)
					for (E = 0; E <= 1; ++E)
						if ((A || B) && (!A || C) && (!D || E) && (!C || D) && !E)
							cout << "A = " << A << ", B = " << B << endl;
	cout << "请输入任意键继续..." << endl;
	//利用cin.get()函数等待用户输入再关闭窗口
	cin.get();
	return 0;
}