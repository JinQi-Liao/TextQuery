#include "Query.h"
#include "TextQuery.h"
#include <memory>
using std::shared_ptr;

#include <set>
using std::set;

#include <algorithm>
using std::set_intersection;

#include <iostream>
using std::ostream;

#include <cstddef>
using std::size_t;

#include <iterator>
using std::inserter;

#include <vector>
using std::vector;

#include <string>
using std::string;

//主要是定义不同派生类的eval函数，其是整个查询系统的核心

//返回运算对象结果set中不存在的行
QueryResult NotQuery::eval(const TextQuery& text) const
{
	std::cout << "NotQuery_eval" << std::endl;

	//通过query运算对象对eval进行虚调用
	QueryResult result = query.eval(text);

	//开始时结果为空
	shared_ptr<set<line_no>> ret_lines = std::make_shared<set<line_no>>();

	//必须在运算对象出现的所以行中进行迭代
	QueryResult::line_it beg = result.begin(), end = result.end();

	//对于输入文件中的每一行，如果该行不在result当中，则将其添加到ret_lines中
	QueryResult::line_no sz = result.get_file()->size();
	for (size_t n = 0; n != sz; ++n)
	{
		//如果我们还没有处理完result的所以行
		//检查当前行是否存在
		if (beg == end || *beg != n)
			ret_lines->insert(n);//如果不在result中，则添加这一行
		else if (beg != end)
			++beg;//否则继承获取result的下一行
	}

	return QueryResult(rep(), ret_lines, result.get_file());
}

//返回运算对象查询结果set的交集
QueryResult AndQuery::eval(const TextQuery& text) const
{	
	std::cout << "AndQuery_eval" << std::endl;

	//通过query运算对象对eval进行虚调用
	QueryResult left = lhs.eval(text), right = rhs.eval(text);

	//开始时结果为空
	shared_ptr<set<line_no>> ret_lines = std::make_shared<set<line_no>>();

	//将两个范围的交集写入一个目的迭代器中
	//本次调用的目的迭代器向ret_lines中添加元素
	set_intersection(left.begin(), left.end(), right.begin(), right.end(), inserter(*ret_lines, ret_lines->begin()));

	return QueryResult(rep(), ret_lines, left.get_file());
}

//返回运算对象查询结果set的并集
QueryResult OrQuery::eval(const TextQuery& text) const
{
	std::cout << "OrQuery_eval" << std::endl;

	//通过query运算对象对eval进行虚调用
	QueryResult left = lhs.eval(text), right = rhs.eval(text);

	//将左侧运算对象的行号拷贝到结果set中
	shared_ptr<set<line_no>> ret_lines = std::make_shared<set<line_no>>(left.begin(), left.end());

	//插入右侧运算对象所得的行号
	ret_lines->insert(right.begin(), right.end());

	return QueryResult(rep(), ret_lines, left.get_file());

}
