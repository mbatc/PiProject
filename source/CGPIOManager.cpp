#include "CGPIOManager.h"
#include "CGPIOPin.h"

#include <string.h>

CGPIOManager::CGPIOManager()
{
}

CGPIOManager::~CGPIOManager()
{
	ClearPins();
}

void CGPIOManager::ClearPins()
{
	for(int i = 0; i < m_ppPin.size(); i++)
	{
		delete m_ppPin[i];
	}
	m_ppPin.clear();
}

bool CGPIOManager::addPin(string num)
{
	for(int i = 0; i < m_ppPin.size(); i++)
	{
		if(!strcmp(m_ppPin[i]->getNum().c_str(), num.c_str()))
			return false;
	}

	CGPIOPin* add = new CGPIOPin(num);
	if(!add->HandleExists())
	{
		delete add;
		return false;
	}

	m_ppPin.push_back(add);
	return true;
}

bool CGPIOManager::remPin(string num)
{
	for(int i = 0; i < m_ppPin.size(); i++)
	{
		if(!strcmp(m_ppPin[i]->getNum().c_str(), num.c_str()))
		{
			m_ppPin[i]->Unexport();
			delete m_ppPin[i];
			m_ppPin.erase(m_ppPin.cbegin()+i);
			i--;
			return true;
		}
	}
	return false;
}

CGPIOPin* CGPIOManager::getPin(string num)
{
	for(int i = 0; i < m_ppPin.size(); i++)
	{
		if(!strcmp(m_ppPin[i]->getNum().c_str(), num.c_str()))
			return m_ppPin[i];
	}

	return 0;
}
