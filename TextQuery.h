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

//������һ��QueryResult�����
class QueryResult;
class TextQuery
{
public:
	typedef vector<string>::size_type line_no;

	//���캯��
	TextQuery(ifstream&);

	//��ѯ����
	QueryResult query(const string&) const;

	//��ӡ��map
	void display_map();
	//~TextQuery();

private:
	shared_ptr<vector<string>> file;//�����ļ�

	//ÿ�����ʵ��������кŵļ��ϵ�ӳ��
	map < string, shared_ptr<set<line_no>>> wm;

	//�淶���ı�:ɾ�������Ų�ʹ��������Сд
	static string cleanup_str(const string&);
};


#endif // !TEXTQUERY_H
