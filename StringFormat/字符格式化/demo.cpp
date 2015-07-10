#include "StrFormat.h"
#include <iostream>
#include <string>
#include <windows.h> /*��Ϊ�������ɫ���֣�����Ҫ����ɾ��*/  
#include <iomanip>  
#include <fstream>
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

inline void inputWithTip(string tip, string& str){
	HANDLE outPutHandle = GetStdHandle(STD_OUTPUT_HANDLE);//��ÿ���̨�豸������ľ��
	SetConsoleTextAttribute(outPutHandle, FOREGROUND_GREEN);
	cout << tip << endl;
	SetConsoleTextAttribute(outPutHandle, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	getline(cin, str);
}

int main(){
	string  strfile = "", stroutfile = "", demostr_rulein = "", demostr_ruleout = "";

	inputWithTip("������Դ�ַ��������ļ�", strfile);
	//inputWithTip("������Դ�ַ���", demostr);
	inputWithTip("������Դ�ַ����Ĺ��򣨹��ɣ�", demostr_rulein);
	inputWithTip("������Ŀ���ַ����Ĺ��򣨹��ɣ�", demostr_ruleout);
	inputWithTip("������Ŀ���ַ����ļ���ַ", stroutfile);

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