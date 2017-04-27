#include "CGPIOPin.h"

#include "CLog.h"

using namespace std;

CGPIOPin::CGPIOPin(string num)
	:
	m_str_pin_num(num),
	m_bExported(false)
{
	int r = Export();
	int level = 0;
	std::string msg = "CGPIOPin Export()";
	switch(r)
	{
	case 0:
		level = DEBUGLOG_LEVEL_INFO;
		msg += "succeeded";
		break;
	case 1:
		level = DEBUGLOG_LEVEL_WARNING;
		msg += "failed - Handle already exists";
		break;
	case -1:
		level = DEBUGLOG_LEVEL_ERROR;
		msg += "failed - Unable to retrieve handle (check file permissions of application)";
		break;
	}

	_LOG.Log(this, level, msg.c_str());
	Init("out","0");
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
		_io.Print("%s > %s\n",val.c_str(),path.c_str());
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

int CGPIOPin::Export()
{
	_io.Print("%s > %s\n",m_str_pin_num.c_str(),GPIO_EXPORT);

	if(!m_bExported)
		_io.OFStream(GPIO_EXPORT, m_str_pin_num);
	else
		return 1;

	m_bExported = HandleExists();
	if(m_bExported == false)
		return -1;

	return 0;
}

int CGPIOPin::Unexport()
{
	if(m_bExported)
		_io.OFStream(GPIO_UNEXPORT, m_str_pin_num);
	else
		return 1;

	m_bExported = HandleExists();
	if(m_bExported)
		return -1;

	return 0;
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

bool CGPIOPin::HandleExists()
{
	std::string name = GPIO_PATH + m_str_pin_num;
	if(FILE*file = fopen(name.c_str(), "r"))
	{
		fclose(file);
		return true;
	}

	return false;
}
