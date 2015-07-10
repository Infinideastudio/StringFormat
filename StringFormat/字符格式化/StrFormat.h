#include <string>
#include <vector>
#include <map>
using std::vector;
using std::string;
using std::map;

class StringFormat{
private:
	//�ü��ַ��� ��start���ü�len�����ɹ����زü����ݣ�ʧ�ܷ���""
	static string strClip(string str, int start, int len){
		if (start < 0 || len < 0) return ""; //����Ĳ���
		if (len == 0) return ""; //���ü�0���ȣ���ֱ�ӷ���""
		if (str == "") return ""; //���ַ�������Ҫ����

		string retStr = "";
		for (int a = start; a != start + len; a++){
			retStr += str[a];
		}
		return retStr;
	}

	//Ѱ��ĳ�ַ�(��)��һ�γ��ֵ�λ�ã����ҳɹ�����λ�ã�ʧ�ܷ���-1
	static int findString(string str, string find_str, int pos_start = 0){
		if (str == "" || find_str == "") return -1;
		return str.find(find_str, pos_start);
	}
	
	//�õ�һ���ַ��������еı������俪ʼ���ŵ��ڴ��ַ����е�λ�ã�����һ��map<int, string>
	static map<int, string> getAllVar(string str){
		map<int, string> str_finds;
		string find_str = "";
		int index_last = 0;
		int fnd_l = 0, fnd_r = 0;
		do{
			fnd_l = findString(str, "{", index_last) + 1;
			fnd_r = findString(str, "}", index_last);

			find_str = strClip(str, fnd_l, fnd_r - fnd_l);

			if (fnd_l - 1 >= 0)
				str_finds[fnd_l - 1] = find_str;

			index_last = fnd_r + 1;

		} while (fnd_l > 0 && fnd_r - fnd_l >= 0);
		return str_finds;
	}
	
	//����һ����������������ַ�����λ�ã���������ַ�����Դ�ַ�����Ϊ����������һ��������Դ�ַ����е�����
	static string getVarInSrcStr(string str, string rule, int var_position, int var_position_end, int last_var_end, int& real_pos_end){
		if (var_position < 0) return "";

		if (var_position == 0 && var_position_end == rule.length())  //�����ַ���ֻ��һ������ѡ�������ַ�����
		{
			return str;
		}


		if (var_position == 0)  //��ͷ���й��򣬴�ͷ��ȡ�������һ���ַ���
		{
			int pos_len = findString(str, strClip(rule, var_position_end + 1, 1), real_pos_end);
			real_pos_end = pos_len - 1;
			return strClip(str, 0, pos_len);
		}
		

		if (var_position_end+1 == rule.length())  //��β�й��򣬴ӹ���ǰһ���ַ�����ʼ��ȡ������
		{
			//int pos_start = findString(str, strClip(rule, var_position - 1, 1), real_pos_end) + 1;
			string pos_start_temp = strClip(rule, last_var_end + 1, var_position - last_var_end - 1);    //���ڹ����ַ����н�ȡ����"{"��ǰ��һֱ��ȡ����һ��"}"�����ݣ�����"{","}"��
			int pos_start = findString(str, pos_start_temp, real_pos_end) + pos_start_temp.length();
			real_pos_end = str.length() - 1;
			return strClip(str, pos_start, str.length() - pos_start);
		}

		
		//�������
		string pos_start_temp = strClip(rule, last_var_end + 1, var_position - last_var_end - 1);    //���ڹ����ַ����н�ȡ����"{"��ǰ��һֱ��ȡ����һ��"}"�����ݣ�����"{","}"��
		int pos_start = findString(str, pos_start_temp, real_pos_end) + pos_start_temp.length();
		int pos_end = findString(str, strClip(rule, var_position_end + 1, 1), ((real_pos_end > pos_start) ? real_pos_end : pos_start)) - 1;
		real_pos_end = pos_end - 1;
		return strClip(str, pos_start, pos_end - pos_start + 1);

	}

	//��һ���ַ�����ָ�������滻����Ӧ����
	static string replaceStr(string str, int position_start, int position_end, string content){
		return strClip(str, 0, position_start) + content + strClip(str, position_end + 1, str.length() - position_end - 1);
	}

	//����һ��map��value��key
	static int mapFind(map<int, string> findmap, std::string content){
		for (map<int, string>::iterator it = findmap.begin(); it != findmap.end(); ++it){
			if (it->second == content)
				return it->first;
		}
		return -1;
	}
public:
	static string formatString(string str, string rule_str, string rule_dest){
		map<int,string> rule_str_vars = getAllVar(rule_str);
		map<int,string> rule_dest_vars = getAllVar(rule_dest);
		map<int,string>::iterator it2;
		map<int,string>::iterator itmp;
		bool notfirst = false;
		int last_result_position = 0;
		string result = rule_dest;

		for (map<int, string>::iterator it = rule_str_vars.begin(); it != rule_str_vars.end(); ++it){
			string content = getVarInSrcStr(str, rule_str, it->first, it->first + it->second.length() + 1, 
				notfirst ? itmp->first + itmp->second.length() + 1 : -1, last_result_position);
			//��ȡ�����������ʵ������
			if (it->second != ""){ //�������Ʊ�����Ϊ*���������μӼ���
				rule_dest_vars = getAllVar(result);  //��������λ�ã���������ԭ�ȵ���result��λ�ñ��ı�����ɵ�
				it2 = rule_dest_vars.find(mapFind(rule_dest_vars, it->second));
				result = replaceStr(result, it2->first, it2->first + it2->second.length() + 1, content);
			}
			if (notfirst){
				if (itmp != rule_str_vars.end()) itmp++;
			}
			else{
				itmp = rule_str_vars.begin();
				notfirst = true;
			}
			
		}
		return result;
	}
};