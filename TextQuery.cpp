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

//类型别名
typedef map<string, shared_ptr<set<TextQuery::line_no>>> wmType;
typedef wmType::const_iterator wmIter;
typedef shared_ptr<set<TextQuery::line_no>> lineType;
typedef set<TextQuery::line_no>::const_iterator lineIter;

TextQuery::TextQuery(ifstream &is) : file(new vector<string>)
{
	string text;
	while (getline(is, text))//对文件的每一行
	{
		file->push_back(text);//保存此行文件
		int n = file->size() - 1;//当前行号

		istringstream line(text);//将行文本分解成单词
		string word;
		while (line >> word)
		{
			word = cleanup_str(word);

			//如果单词不在wm中，以之为下标在wm中添加一项
			lineType& lines = wm[word];//lines是一个shared_ptr
			
			//在我们第一次遇到这个单词时，此指针为空
			if (!lines)
				lines.reset(new set<line_no>);//分配一个新的set
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
	//如果未找到sought，将返回一个指向此set的指针
	static lineType nodata(new set<line_no>);

	//使用find而不是下标运算符来查找单词，避免将单词添加到wm中
	wmIter loc = wm.find(cleanup_str(sought));
	if (loc == wm.end())
		return QueryResult(sought, nodata, file);
	else
		return QueryResult(sought, loc->second, file);
}

ostream &print(ostream & os, const QueryResult &qr)
{
	//如果找到了单词，打印出现次数和所有出现的位置
	os << qr.sought << " occurs " << qr.size() << " " << make_plural(qr.size(), "time", "s") << endl;

	//打印单词出现的每一行
	for (auto num : *qr.lines)
	{
		//避免从0开始给用户带来的困惑
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

		//打印出所有该单词的行
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