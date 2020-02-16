# TextQuery
A simple text query program: support word query, logical non - query, logical or query, logic and query

一、整个继承体系的结构图：（参考class inhertance system.png）
1.TextQuery：是文本查询类，该类读入给定的文件并构建一个查找图。
	query操作：接受一个string实参，返回一个QueryResult对象，该QueryResult对象表示string出现的行数。
2.QueryResult：是查询结果类，该类保存一个query操作的结果


Query程序接口操作
3.Query：是接口类，用于将继承的层次关系隐藏于接口类中，指向Query_base派生类的对象
Query q(s)：将Query对象q绑定到一个存放string s的新的WordQuery对象上
q1 & q2：返回一个Query对象，该Query对象绑定到一个存放q1和q2的新AndQuery对象上
q1 | q2：返回一个Query对象，该Query对象绑定到一个存放q1和q2的新OrQuery对象上
~q：返回一个Query对象，该Query对象绑定到一个存放q的新NotQuery对象上
用Query类封装了Query_base指针，管理实际查询处理用到的不同Query类型对象。

Query程序实现类
4.Query_base：查询类的抽象基类：
5.WordQuery：Query_base的派生类，用于查找一个给定的单词
6.NotQuery：Query_base的派生类，查询结果是Query运算对象没有出现的行的集合
7.BinaryQuery：Query_base派生出来的另一个抽象基类，表示有两个运算对象的查询
8.OrQuery：BinaryQuery的派生类，返回它的两个运算对象分别出现的行的并集
9.AndQuery：BinaryQuery的派生类，返回它的两个运算对象分别出现的行的交集

二、文件解释
1.text.cpp和data.txt是测试文件；
2.TextQuery.h,QueryResult.h,Query.h是相应类的头文件
3.make_plural.h、get_print.cpp是辅助文件
