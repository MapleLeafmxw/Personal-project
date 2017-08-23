#pragma once
#include "BigOperation.h"

BigOperation::BigOperation(INT64 value)
{
	_value = value;
	INT64 temp = _value;
	int count = 0;
	while (temp != 0)
	{
		++count;
		temp /= 10;
	}
	_pData.resize(count + 1);
	_pData[0] = '+';
	temp = _value;
	if (temp < 0)
	{
		_pData[0] = '-';
		temp = -temp;
	}
	for (int index = 0; index < count; index++)
	{
		_pData[count - index] = temp % 10 + '0';
		temp /= 10;
	}
}
BigOperation::BigOperation(const std::string& strData)
	:_value(0)
{
	if ("" == strData)
		return;
	const char* pData = strData.c_str();
	while (isspace(*pData))
		++pData;
	char symbol = *pData;
	if ('+' == *pData || '-' == *pData)
		++pData;
	else if (*pData >= '0' && *pData <= '9')
		symbol = '+';
	else
		return;
	while (*pData == '0')
		++pData;
	_pData.resize(strlen(pData) + 1);
	_pData[0] = symbol;
	size_t index = 1;
	while (*pData >= '0' && *pData <= '9')
	{
		_value = _value * 10 + *pData - '0';
		_pData[index++] = *pData;
		pData++;
	}
	if (1 != index)
		_pData.resize(index);
	if (symbol == '-')
		_value = -_value;

}
BigOperation::~BigOperation()
{}

bool BigOperation::IsINT64overflow()const
{

	string temp("+9223372036854775807");
	if (_pData[0] == '-')
		temp = "-9223372036854775808";
	if (_pData.size() < temp.size())
		return false;
	else if (_pData.size() == temp.size() && _pData <= temp)
	{
		return false;
	}	 
	return true;
}

ostream& operator<<(ostream& _cout, const BigOperation& s)
{
	if (!s.IsINT64overflow())//IsINT64overflow
	{
		_cout << s._value << endl;
	}
	else
	{
		char* str = (char*)s._pData.c_str();
		if (s._pData[0] == '+')
			str++;
		_cout << str << endl;
	}
	return _cout;
}

BigOperation BigOperation::operator+(const BigOperation& b)
{
	if (!IsINT64overflow() && !b.IsINT64overflow())
	{
		//两种情况，两数相加溢出和不溢出
		//1、异号。2、同号

		if (_pData[0] != b._pData[0])//异号
			return BigOperation(_value + b._value);
		else//同号
		{
			if (((_pData[0] == '+') && (MAX_INT64 - _value > b._value))
				|| ((_pData[0] == '-') && (MIN_INT64 - _value < b._value)))
			{
				return BigOperation(_value + b._value);
			}
		}
	}

	//至少一个溢出、其中一个溢出或者两个相加溢出

	if (_pData[0] == b._pData[0])//两个数为同号
	{
		return BigOperation(ADD(_pData, b._pData));
	}
	else
	{
		//两个数为异号 例:-12+11 相当于-12-(-11) 13+(-11)相当于13-11 123+(-11)相当于123-11 11+(-123)相当于-123-(-11) 
		//都是将绝对值小的数统一为绝对值大的数的符号，在让绝对值大数减去绝对值小数
		int leftsize = _pData.size();
		int rightsize = b._pData.size();
		string temp(b._pData);//b为常量不能改变故取个临时变量
		if (leftsize < rightsize || (leftsize == rightsize && strcmp(_pData.c_str() + 1, b._pData.c_str() + 1) < 0))
		{
			_pData[0] = temp[0];
			_pData.swap(temp);
		}
		else 
		{
			temp[0] = _pData[0];
		}
		return BigOperation(SUB(_pData, temp));
	}
	return BigOperation(_pData);
}
string BigOperation::ADD(string left, string right)
{
	int leftsize = left.size();
	int rightsize = right.size();
	if (leftsize < rightsize)
	{
		left.swap(right);
		std::swap(leftsize, rightsize);
	}
	string temp;
	temp.resize(leftsize + 1);//两个数或许最后有进位
	int tempsize = temp.size();
	int i = 1;
	temp[0] = left[0];
	char step = 0;//记录是否有进位
	for (; i < leftsize; i++)
	{
		char sum = 0;//个个位相加之和
		sum = left[leftsize - i] - '0'+ step;//这里减字符0，算出的是数字只不过这个数字存储在字符中
		if (i < rightsize)
		{
			sum += right[rightsize - i] - '0';
		}
		temp[tempsize - i] = sum % 10 + '0';
		step = sum/ 10;
	}
	temp[1] = step + '0';
	return temp;
}
BigOperation BigOperation::operator-(const BigOperation& b)
{
	if (!IsINT64overflow() && !b.IsINT64overflow())
	{
		if (_pData[0] == b._pData[0])//同号
		{
			return BigOperation(_value - b._value);
		}
		else//异号
		{
			if (((_pData[0] == '+') && (MAX_INT64 - b._value > _value))
				|| ((_pData[0] == '-') && (MIN_INT64 - b._value < _value)))
			{
				return BigOperation(_value + b._value);
			}
		}	
	}
	//至少一个溢出、其中一个溢出或者两个相减溢出
	if (b._pData[0] == _pData[0])
	{
		return BigOperation(SUB(_pData, b._pData));
	}
	else
	{
		// -3-2 相当于-3+(-2) 3-(-2)相当于3+2 所以都是以被减数的符号看齐然后让他们相加
		string temp(b._pData);
		temp[0] = _pData[0];
		return BigOperation(ADD(_pData, temp));
	}
	return BigOperation(_pData);
}
string BigOperation::SUB(string left, string right)
{
	int leftsize = left.size();
	int rightsize = right.size();
	char symbol = left[0];
	if (leftsize < rightsize || (leftsize == rightsize && left < right))
	{
		left.swap(right);
		std::swap(leftsize, rightsize);
		if (symbol == '+')
			symbol = '-';
		else
			symbol = '+';
	}
	string temp("0");
	temp.resize(leftsize);//这里不加1 是不考虑最后借位
	int i = 1;
	char step = 0;//是否有借位
	for (; i < leftsize; i++)
	{
		char sub = left[leftsize - i] - step - '0';
		if (i < rightsize)
		{
			sub = sub - (right[rightsize - i] - '0');
		}
		if (sub < 0)
		{
			step = 1;
			sub += 10;
		}
		temp[leftsize - i] = sub + '0';
	}
	temp[0] = symbol;
	return temp;
}
BigOperation BigOperation::operator*(const BigOperation& b)
{
	if (!IsINT64overflow() && !b.IsINT64overflow())//都没有越界
	{
		if (_pData[0] == b._pData[0])
		{
			if ((_pData[0] == '+' && (MAX_INT64 / _value >= b._value)) || 
				( _pData[0] == '-' && (MAX_INT64 / _value <= b._value)))
			{
				return BigOperation(_value * b._value);
			}
		}
		else
		{
			if ((_pData[0] == '+' && (MIN_INT64 / _value <= b._value)) ||
				(_pData[0] == '-' && (MIN_INT64 / _value >= b._value)))
			{
				return BigOperation(_value * b._value);
			}
		}
	}
	return BigOperation(MUL(_pData, b._pData));
}
string BigOperation::MUL(string left, string right)
{
	int leftsize = left.size();
	int rightsize = right.size();
	if (leftsize > rightsize)//让位数小的数放在第一位 方便以后乘的时候
	{
		left.swap(right);
		std::swap(leftsize, rightsize);
	}
	char symbol = left[0];
	if (left[0] != right[0])
		symbol = '-';
	string str;
	str.resize(leftsize + rightsize - 1, '0');
	str[0] = symbol;
	int strsize = str.size();
	int i = 1;
	int j = 1;
	char offset = 0;//每次个个位乘积的偏移如123*12 第一次2*123偏移为0 第二次1*123偏移为1
	char step = 0;//记录每次乘积是否有进位
	for (; i < leftsize; i++)
	{
		char sum = left[leftsize - i] - '0';
		if (sum == 0)//如果本次sum为0 就不必要乘了直接让偏移加1
		{
			offset++;
			continue;
		}
		for (j = 1; j < rightsize; j++)
		{
			char temp = sum * (right[rightsize - j] - '0') + step;//相乘完了记得要加后面一位的进位必须要赋给另外的值 否则sum就会改变
			temp += str[strsize - j - offset] - '0';//让乘完之后的结果记得加上此时temp所对应位置上的值
			str[strsize - j - offset] = temp % 10 + '0';//然后将sum赋值为temp所对应的哪一位
			step = temp / 10;
		}
		str[strsize - j - offset] += step;//记得要加上最后一次乘积的进位
		offset++;
	}
	return str;

}
BigOperation BigOperation::operator/(const BigOperation& b)
{
	if (!IsINT64overflow() && !b.IsINT64overflow())
	{
		return (_value / b._value);
	}
	return BigOperation(DIV(_pData, b._pData));
}


string BigOperation::DIV(string left, string right)
{
	char symbol = '+';
	if (left[0] != right[0])
		symbol = '-';
	int leftsize = left.size();
	int rightsize = right.size();
	if (leftsize < rightsize || (leftsize == rightsize && strcmp(left.c_str() + 1, right.c_str() + 1) < 0))
	{
		return string("0");
	}
	if (right == "+1" || right == "-1")
	{
		left[0] = symbol;
		return left;
	}
	if (leftsize == rightsize && (strcmp(left.c_str() + 1, right.c_str() + 1) == 0))
	{
		if (symbol == '+')
			return string("+1");
		else
			return string("-1");
	}
	string str;
	str.append(1, symbol);
	char* pLeft = (char*)left.c_str() + 1;
	char* pRigt = (char*)right.c_str() + 1;
	size_t RightSize = rightsize - 1;
	size_t DataSize = rightsize - 1;
	while (pLeft[DataSize - 1] != '\0')
	{
		if (*pLeft == '0')
		{
			pLeft++;
			str.append(1, '0');
			continue;
		}
		if (!IsLeftBig(pLeft, DataSize, pRigt, RightSize))
		{
			DataSize++;
			str.append(1, '0');
		}
		else
		{
			str.append(1, SubLoop(pLeft, DataSize, pRigt, RightSize));
			DataSize++;
		}
	}
	return str;
}

bool BigOperation::IsLeftBig(char* Left, size_t LSize, char* Right, size_t RSize)
{
	if (LSize < RSize || (LSize == RSize && strcmp(Left, Right) < 0))
		return false;
	return true;
}

char BigOperation::SubLoop(char*& Left, size_t& DataSize, char* Right, size_t RSize)
{
	char count = '0';//计算一共减了多少次也就是商了几
	while (IsLeftBig(Left, DataSize, Right, RSize))
	{
		for (size_t i = 1; i <= DataSize; ++i)//直接逐位相减
		{
			char Res = Left[DataSize - i] - '0';
			if (i <= RSize)
			{
				Res = Res - (Right[RSize - i] - '0');
			}
			if (Res < 0)
			{
				Res += 10;
				Left[DataSize - i - 1] -= 1;
			}
			Left[DataSize - i] = Res + '0';
		}
		while (*Left == '0' && DataSize > 0)
		{
			Left++;
			DataSize--;
		}
		count++;
	}
	return count;
}
