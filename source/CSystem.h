#pragma once

#include "CIO.h"
#include "CGPIOManager.h"
#include "CPyManager.h"

#include <string.h>
#include <string>
#include <vector>

struct SPROGSTATE
{
	SPROGSTATE(std::string name, std::string heading)
		:
		m_name(name),
		m_heading(heading)
	{}

	bool isState(std::string state)const { return !strcmp(state.c_str(),m_name.c_str());}

	std::string m_heading;
	std::string m_name;
};

class CSystem
{
public:
	CSystem();
	~CSystem();

	void init(int argc = 0, char** argv = 0);
	int run();
private:
	int _main();

	void change_state(std::string state);

	void ProcessCMD(std::vector<std::string> cmdln);
	void PrintHelp(std::string arg);

	bool		m_bDoState;

	int		m_iState;
	CGPIOManager 	m_gpio;
	CPyManager*	m_python;
	CIO	 	m_io;

	const SPROGSTATE m_states[2] = {
        {SPROGSTATE("main"," --Main Menu-- ")},
        {SPROGSTATE("gpio"," --General Purpose I/O Menu-- ")}};
};

extern CSystem _SYS;
