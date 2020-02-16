#ifndef QUERY_H
#define QUERY_H
#include "TextQuery.h"
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

//����һ��������࣬����Ĳ�ѯ���ʹ������������г�Ա����private��
class Query_base
{
	friend class Query;
protected:
	using line_no = TextQuery::line_no;//����eval����
	virtual ~Query_base() = default;
private:
	//eval�����뵱ǰ��Queryƥ���QueryResult
	virtual QueryResult eval(const TextQuery&) const = 0;

	//rep�Ǳ�ʾ��ѯ��һ��string
	virtual std::string rep() const = 0;
};


//����һ������Query_base�̳���ϵ�Ľӿ���,��Query_base��֮��û�м̳й�ϵ
class Query
{
	//��Щ�������Ҫ���ʽ���shared_ptr�Ĺ��캯�������ú�����˽�е�
	friend Query operator~(const Query &);
	friend Query operator|(const Query&, const Query&);
	friend Query operator&(const Query&, const Query&);

public:
	Query(const std::string&);//����һ���µ�WordQuery

	//�ӿں��������ö�Ӧ��Query_base����
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

//��������
inline
std::ostream& operator<<(std::ostream &os, const Query &query)
{
	//Query::repͨ������Query_baseָ���rep()�����������
	return os << query.rep();
}

//WordQuery������
class WordQuery : public Query_base
{
private:
	friend class Query;//Queryʹ��WordQuery���캯��

	WordQuery(const std::string & s): query_word(s) {
		std::cout << "WordQuery(const std::string &)" << std::endl;
	}

	//������ࣺWordQuery���������м̳ж����Ĵ��麯��
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

	std::string query_word;//Ҫ���ҵĵ���

};

//������WordQuery�Ժ�Ϳ��Զ������string��Query�Ĺ��캯����
inline
Query::Query(const std::string &s) : q(new WordQuery(s)) {
	std::cout << "Query::Query(const std::string &)" << std::endl;
}//����һ��WordQuery


//NotQuery�༰~�����
class NotQuery : public Query_base
{
	friend Query operator~(const Query &);
private:
	NotQuery(const Query &q): query(q) {
		std::cout << "NotQuery(const Query &)" << std::endl;
	}

	//������ࣺNotQuery���������м̳ж����Ĵ��麯��
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

//BinaryQuery������࣬��������������������Ĳ�ѯ��������Ҫ�ĵ�����
class BinaryQuery : public Query_base {
protected:
	BinaryQuery(const Query &l, const Query &r, std::string s) :
		lhs(l), rhs(r), opSym(s) {
		std::cout << "BinaryQuery(const Query &, const Query &, std::string )" << std::endl;
	}

	// �����ࣺBinaryQuery������eval
	std::string rep() const override
	{
		std::cout << "BinaryQuery_rep" << std::endl;
		return "(" + lhs.rep() + " "+ opSym + " "+ rhs.rep() + ")";
		
	}

	Query lhs, rhs;    // �����Ҳ��������
	std::string opSym; // �����������
};

//AndQuery��
class AndQuery : public BinaryQuery
{
friend Query operator&(const Query&, const Query&);
private:
	AndQuery(const Query &left, const Query &right):
		BinaryQuery(left, right, "&") {
		std::cout << "AndQuery(const Query &, const Query &)" << std::endl;
	}

	//������ࣺAndQuery�̳���rep���Ҷ����������Ĵ��麯��
	QueryResult eval(const TextQuery&) const override;
};
  
inline Query operator&(const Query &lhs, const Query &rhs)
{
	return std::shared_ptr<Query_base>(new AndQuery(lhs, rhs));
}
//OrQuery��
class OrQuery : public BinaryQuery {
	friend Query operator|(const Query&, const Query&);
private:
	OrQuery(const Query &left, const Query &right) :
		BinaryQuery(left, right, "|") {
		std::cout << "OrQuery(const Query &, const Query &)" << std::endl;
	}

	//������ࣺOrQuery�̳���rep���Ҷ����������Ĵ��麯��
	QueryResult eval(const TextQuery&) const override;
};

inline Query operator|(const Query &lhs, const Query &rhs)
{
	return std::shared_ptr<Query_base>(new OrQuery(lhs, rhs));
}


//һЩ��������
std::ifstream& open_file(std::ifstream&, const std::string&);
TextQuery get_file(int, char**);
bool get_word(std::string&);
bool get_words(std::string&, std::string&);
std::ostream &print(std::ostream&, const QueryResult&);

#endif // !QUERY_H
