#include <iostream>
#include <string.h>

class Expression{
public:
	virtual Expression* diff(std::string var) = 0;
	virtual void print() = 0;
};

class Number : public Expression
{
	double value;
public:
	Number(double x): value(x) {}
	Expression* diff(std::string var){
		return new Number(0);
	}
	void print(){
		std::cout << value;
	}
};

class Variable : public Expression{
	std::string value;
public:
	Variable(std::string x): value(x) {}
	Expression* diff(std::string var){
		if(value == var)
			return new Number(1);
		else
			return new Number(0);
	}
	void print(){
		std::cout << value;
	}
};

class Add : public Expression{
	Expression* left, * right;
public:
	Add(Expression* l, Expression* r) : left(l), right(r)  {}
	Expression* diff(std::string var){
		return new Add(left->diff(var), right->diff(var));
	}
	void print(){
		std::cout << "(";
		left->print();
		std::cout << ")+(";
		right->print();
		std::cout << ")";
	}
};

class Sub : public Expression{
	Expression* left, * right;
public:
	Sub(Expression* l, Expression* r) : left(l), right(r)  {}
	Expression* diff(std::string var){
		return new Sub(left->diff(var), right->diff(var));
	}
	void print(){
			std::cout << "(";
			left->print();
			std::cout << ")-(";
			right->print();
			std::cout << ")";
	}
};

class Mul : public Expression{
	Expression* left, * right;
public:
	Mul(Expression* l, Expression* r) : left(l), right(r) {}
	Expression* diff(std::string var){
		return new Add(new Mul(left->diff(var), right), new Mul(left, right->diff(var)) );
	}
	void print(){
		std::cout << "(";
		left->print();
		std::cout << ")*(";
		right->print();
		std::cout << ")";
	}
};

class Div : public Expression{
	Expression* left, * right;
public:
	Div(Expression* l, Expression* r) : left(l), right(r) {}
	Expression* diff(std::string var){
		return new Div(new Sub(new Mul(left->diff(var), right), new Mul(left, right->diff(var))), new Mul(right, right));
	}
	void print()
	{
		std::cout << "(";
		left->print();
		std::cout << ")/(";
		right->print();
		std::cout << ")";
	}
};


class Ln : public Expression{
protected:
	Expression* x;
public:
	Ln(Expression* v) : x(v) {}
	Expression* diff(std::string var){
		return new Div(new Number(1), x);
	}
	void print(){
		std::cout << "ln(";
		x->print();
		std::cout << ')';
	}
};

class Log : public Ln{
	double a;
public:
	Log(double n, Expression* v) : a(n), Ln(v) {}
	Expression* diff(std::string var){
		return new Div(new Number(1), new Mul(x, new Ln(new Number(a))));
	}
	void print(){
		std::cout << "log_" << a << '(';
		x->print();
		std::cout << ')';
	}
};

class Pow : public Expression{
	Expression* x;
	Expression* y;
public:
	void print(){
		std::cout << "(";
		x->print();
		std::cout << ")^(";
		y->print();
		std::cout << ")";
	}
	Expression* diff(std::string var){
		return new Mul(new Mul(x->diff(var), y), new Mul(x, new Sub(y, new Number(1))));
	}
};

Expression* parse(std::string s){
	int count = 0;
	for (auto i = s.begin(); i < s.end(); i++){
		if (*i == '(') count++;
		else if (*i == ')') count--;
		if (count == 1){
			if (*i == '+'){
				return new Add(parse(s.substr(1, (i - s.begin() - 1))), parse(s.substr((i - s.begin()) + 1, (s.end() - i - 2))));
			}
			else if (*i == '-'){
				return new Sub(parse(s.substr(1, (i - s.begin() - 1))), parse(s.substr((i - s.begin()) + 1, (s.end() - i - 2))));
			}
			else if (*i == '*'){
				return new Mul(parse(s.substr(1, (i - s.begin() - 1))), parse(s.substr((i - s.begin()) + 1, (s.end() - i - 2))));
			}
			else if (*i == '/'){
				return new Div(parse(s.substr(1, (i - s.begin() - 1))), parse(s.substr((i - s.begin()) + 1, (s.end() - i - 2))));
			}
			else if (isdigit(*i)){
				auto j = i;
				for (; *j != ')'; j++);
				std::string tmp = s.substr(i - s.begin(), j - s.begin() - 1);
				return new Number(std::stod(tmp));
				i += tmp.size();
			}
			else if (isalpha(*i)){	
				if (!strncmp(s.c_str(), "(ln_", 4))
				{
					for (; *i != '('; i++);
					std::string tmp2 = s.substr(i - s.begin(), s.end() - i - 1);
					return new Ln(parse(tmp2));
					i += tmp2.size() + 3;
				}
				else if (!strncmp(s.c_str(), "(log_", 5))
				{
					auto j = i;
					for (; *j != '('; j++);
					std::string tmp = s.substr(i - s.begin()+4, j - s.begin() - 5);
					i = j;
					std::string tmp1 = s.substr(i - s.begin(), s.end() - i - 1);
					return new Log(std::stod(tmp), parse(tmp1));
					i += tmp.size();
				}
				else
				{
					auto j = i;
					for (; *j != ')'; j++);
					std::string tmp = s.substr(i - s.begin(), j - s.begin() - 1);
					return new Variable(tmp);
					i += tmp.size();
				}
			}
		}
	}
}


int main()
{
	std::string var;
	std::cin >> var;
	std::string in;
	std::cin >> in;
	Expression* e = new Log(3,new Add(new Number(1), new Variable("x")));
	Expression* t = new Add(new Variable("x"), new Variable("y"));
	Expression* de = t->diff(var);
	e->print();
	std::cout << "\n";
	t->print();
	std::cout << "\n";
	de->print();
	std::cout << "\n";
	Expression* test = parse(std::string(in));
	test->print();
	std::cout << '\n';
	test->diff(var)->print();
	std::cout << "\n";
	delete e;
	delete t;
	delete de;
	delete test;
}