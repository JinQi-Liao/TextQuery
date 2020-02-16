#ifndef TEXTQUERY_H
#define TEXTQUERY_H
#include <memory>
using std::shared_ptr;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <set>
using std::set;

#include <fstream>
using std::ifstream;

#include "QueryResult.h"

//先声明一下QueryResult这个类
class QueryResult;
class TextQuery
{
public:
	typedef vector<string>::size_type line_no;

	//构造函数
	TextQuery(ifstream&);

	//查询操作
	QueryResult query(const string&) const;

	//打印出map
	void display_map();
	//~TextQuery();

private:
	shared_ptr<vector<string>> file;//输入文件

	//每个单词到它所在行号的集合的映射
	map < string, shared_ptr<set<line_no>>> wm;

	//规范化文本:删除标点符号并使所有内容小写
	static string cleanup_str(const string&);
};


#endif // !TEXTQUERY_H
