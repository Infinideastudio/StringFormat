#include <string>
#include <vector>
#include <map>
using std::vector;
using std::string;
using std::map;

class StringFormat{
private:
	//裁剪字符串 从start处裁剪len长，成功返回裁剪内容，失败返回""
	static string strClip(string str, int start, int len){
		if (start < 0 || len < 0) return ""; //错误的参数
		if (len == 0) return ""; //若裁剪0长度，则直接返回""
		if (str == "") return ""; //空字符串不需要剪裁

		string retStr = "";
		for (int a = start; a != start + len; a++){
			retStr += str[a];
		}
		return retStr;
	}

	//寻找某字符(串)第一次出现的位置，查找成功返回位置，失败返回-1
	static int findString(string str, string find_str, int pos_start = 0){
		if (str == "" || find_str == "") return -1;
		return str.find(find_str, pos_start);
	}
	
	//得到一个字符串中所有的变量及其开始符号的在此字符串中的位置，返回一个map<int, string>
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
	
	//给出一个变量所在其规则字符串的位置，及其规则字符串，源字符串作为参数，返回一个变量在源字符串中的内容
	static string getVarInSrcStr(string str, string rule, int var_position, int var_position_end, int last_var_end, int& real_pos_end){
		if (var_position < 0) return "";

		if (var_position == 0 && var_position_end == rule.length())  //规则字符串只有一个规则（选中整个字符串）
		{
			return str;
		}


		if (var_position == 0)  //开头就有规则，从头截取到规则后一个字符处
		{
			int pos_len = findString(str, strClip(rule, var_position_end + 1, 1), real_pos_end);
			real_pos_end = pos_len - 1;
			return strClip(str, 0, pos_len);
		}
		

		if (var_position_end+1 == rule.length())  //结尾有规则，从规则前一个字符处开始截取到结束
		{
			//int pos_start = findString(str, strClip(rule, var_position - 1, 1), real_pos_end) + 1;
			string pos_start_temp = strClip(rule, last_var_end + 1, var_position - last_var_end - 1);    //先在规则字符串中截取，从"{"的前面一直截取到上一个"}"的内容（不含"{","}"）
			int pos_start = findString(str, pos_start_temp, real_pos_end) + pos_start_temp.length();
			real_pos_end = str.length() - 1;
			return strClip(str, pos_start, str.length() - pos_start);
		}

		
		//正常情况
		string pos_start_temp = strClip(rule, last_var_end + 1, var_position - last_var_end - 1);    //先在规则字符串中截取，从"{"的前面一直截取到上一个"}"的内容（不含"{","}"）
		int pos_start = findString(str, pos_start_temp, real_pos_end) + pos_start_temp.length();
		int pos_end = findString(str, strClip(rule, var_position_end + 1, 1), ((real_pos_end > pos_start) ? real_pos_end : pos_start)) - 1;
		real_pos_end = pos_end - 1;
		return strClip(str, pos_start, pos_end - pos_start + 1);

	}

	//将一个字符串中指定区域替换成相应内容
	static string replaceStr(string str, int position_start, int position_end, string content){
		return strClip(str, 0, position_start) + content + strClip(str, position_end + 1, str.length() - position_end - 1);
	}

	//根据一个map的value找key
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
			//获取变量所代表的实际内容
			if (it->second != ""){ //将空名称变量作为*看待，不参加计算
				rule_dest_vars = getAllVar(result);  //重新生成位置，这是由于原先的在result的位置被改变所造成的
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