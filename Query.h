#ifndef QUERY_H
#define QUERY_H
#include "TextQuery.h"
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

//这是一个抽象基类，具体的查询类型从中派生，所有成员都是private的
class Query_base
{
	friend class Query;
protected:
	using line_no = TextQuery::line_no;//用于eval函数
	virtual ~Query_base() = default;
private:
	//eval返回与当前的Query匹配的QueryResult
	virtual QueryResult eval(const TextQuery&) const = 0;

	//rep是表示查询的一个string
	virtual std::string rep() const = 0;
};


//这是一个管理Query_base继承体系的接口类,与Query_base类之间没有继承关系
class Query
{
	//这些运算符需要访问接受shared_ptr的构造函数，而该函数是私有的
	friend Query operator~(const Query &);
	friend Query operator|(const Query&, const Query&);
	friend Query operator&(const Query&, const Query&);

public:
	Query(const std::string&);//构建一个新的WordQuery

	//接口函数：调用对应的Query_base操作
	QueryResult eval(const TextQuery &t) const
	{
		std::cout << "Query_eval" << std::endl;
		return q->eval(t);
	}

	std::string rep() const 
	{
		std::cout << "Query_rep" << std::endl;
		return q->rep(); 
	}
private:
	std::shared_ptr<Query_base>q;
	Query(std::shared_ptr<Query_base> query): q(query) {
		std::cout << "Query(std::shared_ptr<Query_base> )" << std::endl;
	}

};

//输出运算符
inline
std::ostream& operator<<(std::ostream &os, const Query &query)
{
	//Query::rep通过它的Query_base指针对rep()进行了虚调用
	return os << query.rep();
}

//WordQuery派生类
class WordQuery : public Query_base
{
private:
	friend class Query;//Query使用WordQuery构造函数

	WordQuery(const std::string & s): query_word(s) {
		std::cout << "WordQuery(const std::string &)" << std::endl;
	}

	//具体的类：WordQuery将定义所有继承而来的纯虚函数
	QueryResult eval(const TextQuery &t) const override
	{
		std::cout << "WordQuery_eval" << std::endl;
		return t.query(query_word);
	}

	std::string rep() const override
	{
		std::cout << "WordQuery_rep" << std::endl;
		return query_word;
	}

	std::string query_word;//要查找的单词

};

//定义了WordQuery以后就可以定义接受string的Query的构造函数了
inline
Query::Query(const std::string &s) : q(new WordQuery(s)) {
	std::cout << "Query::Query(const std::string &)" << std::endl;
}//分配一个WordQuery


//NotQuery类及~运算符
class NotQuery : public Query_base
{
	friend Query operator~(const Query &);
private:
	NotQuery(const Query &q): query(q) {
		std::cout << "NotQuery(const Query &)" << std::endl;
	}

	//具体的类：NotQuery将定义所有继承而来的纯虚函数
	std::string rep() const override
	{
		std::cout << "NotQuery_rep" << std::endl;
		return "~(" + query.rep() + ")"; 
	}

	QueryResult eval(const TextQuery&) const override;

	Query query;

};

inline Query operator~(const Query &operand)
{
	return std::shared_ptr<Query_base>(new NotQuery(operand));
}

//BinaryQuery抽象基类，它保存操作两个运算对象的查询类型所需要的的数据
class BinaryQuery : public Query_base {
protected:
	BinaryQuery(const Query &l, const Query &r, std::string s) :
		lhs(l), rhs(r), opSym(s) {
		std::cout << "BinaryQuery(const Query &, const Query &, std::string )" << std::endl;
	}

	// 抽象类：BinaryQuery不定义eval
	std::string rep() const override
	{
		std::cout << "BinaryQuery_rep" << std::endl;
		return "(" + lhs.rep() + " "+ opSym + " "+ rhs.rep() + ")";
		
	}

	Query lhs, rhs;    // 左侧和右侧运算对象
	std::string opSym; // 运算符的名字
};

//AndQuery类
class AndQuery : public BinaryQuery
{
friend Query operator&(const Query&, const Query&);
private:
	AndQuery(const Query &left, const Query &right):
		BinaryQuery(left, right, "&") {
		std::cout << "AndQuery(const Query &, const Query &)" << std::endl;
	}

	//具体的类：AndQuery继承了rep并且定义了其他的纯虚函数
	QueryResult eval(const TextQuery&) const override;
};
  
inline Query operator&(const Query &lhs, const Query &rhs)
{
	return std::shared_ptr<Query_base>(new AndQuery(lhs, rhs));
}
//OrQuery类
class OrQuery : public BinaryQuery {
	friend Query operator|(const Query&, const Query&);
private:
	OrQuery(const Query &left, const Query &right) :
		BinaryQuery(left, right, "|") {
		std::cout << "OrQuery(const Query &, const Query &)" << std::endl;
	}

	//具体的类：OrQuery继承了rep并且定义了其他的纯虚函数
	QueryResult eval(const TextQuery&) const override;
};

inline Query operator|(const Query &lhs, const Query &rhs)
{
	return std::shared_ptr<Query_base>(new OrQuery(lhs, rhs));
}


//一些辅助函数
std::ifstream& open_file(std::ifstream&, const std::string&);
TextQuery get_file(int, char**);
bool get_word(std::string&);
bool get_words(std::string&, std::string&);
std::ostream &print(std::ostream&, const QueryResult&);

#endif // !QUERY_H
