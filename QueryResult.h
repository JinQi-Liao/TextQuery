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
	//��Ԫ����
	friend ostream& print(ostream&, const QueryResult&);
public:
	typedef vector<string>::size_type line_no;
	typedef set<line_no>::const_iterator line_it;

	//���캯��
	QueryResult(string s, shared_ptr<set<line_no>> p, shared_ptr<vector<string>> f):
		sought(s), lines(p), file(f) {}

	//��ز���
	set<line_no>::size_type size() const { return lines->size(); }
	line_it begin() const { return lines->begin(); }
	line_it end() const { return lines->end(); }

	//�����ļ�
	shared_ptr<vector<string>> get_file() { return file; }


	//~QueryResult();

private:
	string sought;//��ѯ�ĵ���
	shared_ptr<set<line_no>> lines; //���ֵ��к�
	shared_ptr<vector<string>> file;//�����ļ�
};

//��������
ostream& print(ostream&, const QueryResult&);

//QueryResult::~QueryResult()
//{
//}

#endif // !QUERYRESULT_H
