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

//��Ҫ�Ƕ��岻ͬ�������eval����������������ѯϵͳ�ĺ���

//�������������set�в����ڵ���
QueryResult NotQuery::eval(const TextQuery& text) const
{
	std::cout << "NotQuery_eval" << std::endl;

	//ͨ��query��������eval���������
	QueryResult result = query.eval(text);

	//��ʼʱ���Ϊ��
	shared_ptr<set<line_no>> ret_lines = std::make_shared<set<line_no>>();

	//���������������ֵ��������н��е���
	QueryResult::line_it beg = result.begin(), end = result.end();

	//���������ļ��е�ÿһ�У�������в���result���У�������ӵ�ret_lines��
	QueryResult::line_no sz = result.get_file()->size();
	for (size_t n = 0; n != sz; ++n)
	{
		//������ǻ�û�д�����result��������
		//��鵱ǰ���Ƿ����
		if (beg == end || *beg != n)
			ret_lines->insert(n);//�������result�У��������һ��
		else if (beg != end)
			++beg;//����̳л�ȡresult����һ��
	}

	return QueryResult(rep(), ret_lines, result.get_file());
}

//������������ѯ���set�Ľ���
QueryResult AndQuery::eval(const TextQuery& text) const
{	
	std::cout << "AndQuery_eval" << std::endl;

	//ͨ��query��������eval���������
	QueryResult left = lhs.eval(text), right = rhs.eval(text);

	//��ʼʱ���Ϊ��
	shared_ptr<set<line_no>> ret_lines = std::make_shared<set<line_no>>();

	//��������Χ�Ľ���д��һ��Ŀ�ĵ�������
	//���ε��õ�Ŀ�ĵ�������ret_lines�����Ԫ��
	set_intersection(left.begin(), left.end(), right.begin(), right.end(), inserter(*ret_lines, ret_lines->begin()));

	return QueryResult(rep(), ret_lines, left.get_file());
}

//������������ѯ���set�Ĳ���
QueryResult OrQuery::eval(const TextQuery& text) const
{
	std::cout << "OrQuery_eval" << std::endl;

	//ͨ��query��������eval���������
	QueryResult left = lhs.eval(text), right = rhs.eval(text);

	//��������������кſ��������set��
	shared_ptr<set<line_no>> ret_lines = std::make_shared<set<line_no>>(left.begin(), left.end());

	//�����Ҳ�����������õ��к�
	ret_lines->insert(right.begin(), right.end());

	return QueryResult(rep(), ret_lines, left.get_file());

}
