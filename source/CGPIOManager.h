#pragma once

#include "CGPIOPin.h"
#include <vector>

class CGPIOManager
{
public:
	CGPIOManager();
	~CGPIOManager();

	bool addPin(string num);
	bool remPin(string num);

	void ClearPins();

	CGPIOPin* getPin(string num);
private:
	std::vector<CGPIOPin*> m_ppPin;
};
