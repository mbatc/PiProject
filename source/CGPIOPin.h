#pragma once

#include "CIO.h"
#include <string>


//PGIO pin file paths
#define GPIO_EXPORT "/sys/class/gpio/export"
#define GPIO_UNEXPORT "/sys/class/gpio/unexport"
#define GPIO_PATH "/sys/class/gpio/gpio"
#define GPIO_VALUE "/value"
#define GPIO_DIRECTION "/direction"

class CGPIOPin
{
public:
	CGPIOPin(string pin_num);
	~CGPIOPin();

	bool Export();
	bool Unexport();

	bool Init(std::string dir, std::string val);

	bool SetValue(std::string val);
	bool SetDirection(std::string val);

	std::string 	GetDir();

	std::string	getNum();

	std::string	GetValRawS();
	int		GetValRawI();
	bool		GetValRawB();

	std::string 	GetValS();
	int		GetValI();
	bool		GetValB();

private:
	bool		Set(string f, string val);

	bool 		m_bExported;
	std::string 	m_str_pin_num;

	CIO _io;
};
