#include "TextQuery.h"
#include "make_plural.h"

#include <memory>
using std::shared_ptr;

#include <sstream>
using std::istringstream;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <utility>
using std::pair;

#include <iostream>
using std::cout;
using  std::endl;
using std::cin;
using std::ostream;
using std::getline;
using std::cerr;

#include <fstream>
using std::ifstream;

#include <cstddef>
using std::size_t;

#include <set>
using std::set;

#include <cctype>
#include <cstring>
using std::ispunct;
using std::tolower;
using std::strlen;

//���ͱ���
typedef map<string, shared_ptr<set<TextQuery::line_no>>> wmType;
typedef wmType::const_iterator wmIter;
typedef shared_ptr<set<TextQuery::line_no>> lineType;
typedef set<TextQuery::line_no>::const_iterator lineIter;

TextQuery::TextQuery(ifstream &is) : file(new vector<string>)
{
	string text;
	while (getline(is, text))//���ļ���ÿһ��
	{
		file->push_back(text);//��������ļ�
		int n = file->size() - 1;//��ǰ�к�

		istringstream line(text);//�����ı��ֽ�ɵ���
		string word;
		while (line >> word)
		{
			word = cleanup_str(word);

			//������ʲ���wm�У���֮Ϊ�±���wm�����һ��
			lineType& lines = wm[word];//lines��һ��shared_ptr
			
			//�����ǵ�һ�������������ʱ����ָ��Ϊ��
			if (!lines)
				lines.reset(new set<line_no>);//����һ���µ�set
			lines->insert(n);
		}

	}
}

string TextQuery::cleanup_str(const string &word)
{
	string ret;
	for (auto it = word.begin(); it != word.end(); ++it)
	{
		if (!ispunct(*it))
			ret += tolower(*it);

	}

	return ret;
}

QueryResult TextQuery::query(const string &sought) const
{
	//���δ�ҵ�sought��������һ��ָ���set��ָ��
	static lineType nodata(new set<line_no>);

	//ʹ��find�������±�����������ҵ��ʣ����⽫������ӵ�wm��
	wmIter loc = wm.find(cleanup_str(sought));
	if (loc == wm.end())
		return QueryResult(sought, nodata, file);
	else
		return QueryResult(sought, loc->second, file);
}

ostream &print(ostream & os, const QueryResult &qr)
{
	//����ҵ��˵��ʣ���ӡ���ִ��������г��ֵ�λ��
	os << qr.sought << " occurs " << qr.size() << " " << make_plural(qr.size(), "time", "s") << endl;

	//��ӡ���ʳ��ֵ�ÿһ��
	for (auto num : *qr.lines)
	{
		//�����0��ʼ���û�����������
		os << "\t(line " << num + 1 << ")" << *(qr.file->begin() + num) << endl;
	}

	return os;
}

void TextQuery::display_map()
{
	wmIter iter = wm.begin(), iter_end = wm.end();
	for (; iter != iter_end; ++iter)
	{
		cout << " word: " << iter->first << " {";
		lineType text_locs = iter->second;
		lineIter loc_iter = text_locs->begin(), loc_iter_end = text_locs->end();

		//��ӡ�����иõ��ʵ���
		while (loc_iter != loc_iter_end)
		{
			cout << *loc_iter;
			if (++loc_iter != loc_iter_end)
				cout << ", ";
		}

		cout << "}\n";
	}

	cout << endl;
}