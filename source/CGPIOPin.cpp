#include "CGPIOPin.h"

using namespace std;

CGPIOPin::CGPIOPin(string num)
	:
	m_str_pin_num(num)
{
	Export();
}

CGPIOPin::~CGPIOPin()
{
	Unexport();
}

string CGPIOPin::getNum()
{
	return m_str_pin_num;
}

bool CGPIOPin::Init(string dir, string val)
{
	bool ret = true;
	if(!SetValue(val))
		ret = false;

	if(!SetDirection(val))
		ret = false;

	return ret;
}

bool CGPIOPin::Set(string f, string val)
{
	if(m_bExported)
	{
		string path = GPIO_PATH + m_str_pin_num + f;
		_io.OFStream(path, val);
	}
}

bool CGPIOPin::SetValue(string val)
{
	Set(GPIO_VALUE, val);
}

bool CGPIOPin::SetDirection(string dir)
{
	Set(GPIO_DIRECTION, dir);
}

bool CGPIOPin::Export()
{
	if(!m_bExported)
		_io.OFStream(GPIO_EXPORT, m_str_pin_num);
	m_bExported = true;

	return true;
}
bool CGPIOPin::Unexport()
{
	if(m_bExported)
		_io.OFStream(GPIO_UNEXPORT, m_str_pin_num);
	m_bExported = false;

	return true;
}

string CGPIOPin::GetDir()
{
	if(m_bExported)
	{
		string path = GPIO_PATH + m_str_pin_num + GPIO_DIRECTION;
		return _io.IFStream(path);
	}
	return ""; 
}

string CGPIOPin::GetValRawS()
{
	if(m_bExported)
	{
		string path = GPIO_PATH + m_str_pin_num + GPIO_VALUE;
		return _io.IFStream(path);
	}

	return "";
}

int CGPIOPin::GetValRawI()
{
	return atoi(GetValRawS().c_str());
}

bool CGPIOPin::GetValRawB()
{
	int i = atoi(GetValRawS().c_str());
	if(i == 0)
		return false;

	return true;
}

string CGPIOPin::GetValS()
{
	string s = GetValRawS();
	if(s != "0")
		s = "1";
	else
		s = "0";

	return s;
}

int CGPIOPin::GetValI()
{
	int i = GetValRawI();
	if(i > 1)
		i = 1;

	return i;
}

bool CGPIOPin::GetValB()
{
	return GetValRawB();
}
