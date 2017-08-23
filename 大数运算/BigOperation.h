#pragma once

#include <iostream>
using namespace std;
#include <string>

typedef long long INT64;
#define MAX_INT64 9223372036854775807
#define MIN_INT64 -9223372036854775806

class BigOperation
{
public:
	BigOperation(INT64 value = 0);
	BigOperation(const std::string& strData);
	~BigOperation();
public:
	bool IsINT64overflow()const;
	BigOperation operator+(const BigOperation& b);
	string ADD(string left, string right);
	BigOperation operator-(const BigOperation& b);
	string SUB(string left, string right);
	BigOperation operator*(const BigOperation& b);
	string MUL(string left, string right);
	BigOperation operator/(const BigOperation& b);
	string DIV(string left, string right);
	bool IsLeftBig(char* Left, size_t LSize, char* Right, size_t RSize);
	char SubLoop(char*& Left, size_t& DataSize, char* Right, size_t RSize);

	friend ostream& operator<<(ostream& _cout, const BigOperation& s);
	
private:
	string _pData;
	INT64 _value;
};