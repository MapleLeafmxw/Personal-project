#pragma once
#include "BigOperation.h"



int main()
{
	//BigOperation a("-9223372036854775809");
	//BigOperation b("+9223372036854775806");
	//BigOperation c("965456");
	////BigOperation a("      +92236");
	////BigOperation b("00000000002");
	//cout << a.operator+(b) << endl;
	//cout << a.operator-(b) << endl;
	//BigOperation a("9223372036854775809");
	//BigOperation b("-2");
	//cout << a.operator-(b) << endl;
	//BigOperation a("-9223372036854775802");
	//BigOperation b("-9223372036854775802");
	//BigOperation c("2");
	//BigOperation d("56432189765431231654789");

	//cout << a.operator+(b) << endl;
	//cout << a.operator*(d) << endl;
	BigOperation a("-9223372036854775809");
	BigOperation b("33");
	cout << a.operator/(b) << endl;
	return 0;
}