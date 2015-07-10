#include "StrFormat.h"
#include <iostream>
#include <string>
#include <windows.h> /*仅为了输出彩色文字，不需要可以删除*/  
#include <iomanip>  
#include <fstream>
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

inline void inputWithTip(string tip, string& str){
	HANDLE outPutHandle = GetStdHandle(STD_OUTPUT_HANDLE);//获得控制台设备描述表的句柄
	SetConsoleTextAttribute(outPutHandle, FOREGROUND_GREEN);
	cout << tip << endl;
	SetConsoleTextAttribute(outPutHandle, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	getline(cin, str);
}

int main(){
	string  strfile = "", stroutfile = "", demostr_rulein = "", demostr_ruleout = "";

	inputWithTip("请输入源字符串所在文件", strfile);
	//inputWithTip("请输入源字符串", demostr);
	inputWithTip("请输入源字符串的规则（规律）", demostr_rulein);
	inputWithTip("请输入目标字符串的规则（规律）", demostr_ruleout);
	inputWithTip("请输入目标字符串文件地址", stroutfile);

	cout << endl << endl << endl;

	char buffer[256];
	ifstream myfile(strfile);
	ofstream outfile(stroutfile);
	while (!myfile.eof())
	{
		myfile.getline(buffer, 200);
		outfile << StringFormat::formatString(buffer, demostr_rulein, demostr_ruleout) << endl;
		cout << StringFormat::formatString(buffer, demostr_rulein, demostr_ruleout) << endl;
	}

	myfile.close();
	outfile.close();

	//cout << StringFormat::formatString(demostr, demostr_rulein, demostr_ruleout) << endl;
	system("pause");
	return 0;
}