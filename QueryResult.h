#ifndef QUERYRESULT_H
#define QUERYRESULT_H
#include <memory>
using std::shared_ptr;

#include <string>
using std::string;

#include <vector>
using std::vector;


#include <set>
using std::set;

#include <iostream>
using std::ostream;

class QueryResult
{
	//友元函数
	friend ostream& print(ostream&, const QueryResult&);
public:
	typedef vector<string>::size_type line_no;
	typedef set<line_no>::const_iterator line_it;

	//构造函数
	QueryResult(string s, shared_ptr<set<line_no>> p, shared_ptr<vector<string>> f):
		sought(s), lines(p), file(f) {}

	//相关操作
	set<line_no>::size_type size() const { return lines->size(); }
	line_it begin() const { return lines->begin(); }
	line_it end() const { return lines->end(); }

	//返回文件
	shared_ptr<vector<string>> get_file() { return file; }


	//~QueryResult();

private:
	string sought;//查询的单词
	shared_ptr<set<line_no>> lines; //出现的行号
	shared_ptr<vector<string>> file;//输入文件
};

//函数声明
ostream& print(ostream&, const QueryResult&);

//QueryResult::~QueryResult()
//{
//}

#endif // !QUERYRESULT_H
