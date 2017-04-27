#include "CSystem.h"
#include "CLog.h"
#include "CMDHelp.h"
#include "Commands.h"

CSystem _SYS;

CSystem::CSystem(){}

CSystem::~CSystem()
{}

void CSystem::init(int argc, char** argv)
{
	_LOG.Log(this,DEBUGLOG_LEVEL_INFO,"Entered CSystem::init()");
	_LOG.Log(this,DEBUGLOG_LEVEL_INFO,"Initialising System Class");
	_LOG.Log(this,DEBUGLOG_LEVEL_INFO,"CSystem::init() returned");
	change_state("main");
	m_python = new CPyManager(argv[0]);
	m_python->PYInit();
}

int CSystem::run()
{
	m_python->AddScript("py/getGoogleImage.py");
	m_python->RunScript("py/getGoogleImage.py");
	m_python->RemScript("py/getGoogleImage.py");

	_LOG.Log(this,DEBUGLOG_LEVEL_INFO,"Entered CSystem::_run()");
	do
	{
		m_bDoState = true;
		_main();
	}while(m_iState >= 0);
	_LOG.Log(this,DEBUGLOG_LEVEL_INFO,"CSystem::run() returned 0");

	m_python->PYFinalize();
	delete m_python;
	m_python = 0;

	return 0;
}

int CSystem::_main()
{
	_LOG.Log(this,DEBUGLOG_LEVEL_INFO,"Entered CSystem::_main()");

	m_io.Print(m_states[m_iState].m_heading.c_str());
	m_io.Print("\n");
	do
	{
		m_io.Print("\t%s >", m_states[m_iState].m_name.c_str());
		string in = m_io.GetCMDLine();
		std::vector<std::string> cmd = PassCMDLine(in);
		ProcessCMD(cmd);
	}while(m_bDoState);

	_LOG.Log(this,DEBUGLOG_LEVEL_INFO,"CSystem::_main() returned 0");
	return 0;
}

void CSystem::change_state(std::string state)
{
	for(int i = 0; i < sizeof(m_states)/sizeof(SPROGSTATE); i++)
	{
		if(m_states[i].isState(state))
		{
			_LOG.Log(this,DEBUGLOG_LEVEL_INFO, "Program State changed: %s", state.c_str());
			m_iState = i;
			m_bDoState = false;
			return;
		}
	}
	if(!strcmp(state.c_str(), _CMD_EXIT))
	{
		m_iState = -1;
		m_bDoState = false;
		return;
	}
	_LOG.Log(this, DEBUGLOG_LEVEL_ERROR, "Unknown program state: %s", state.c_str());
}

void CSystem::ProcessCMD(std::vector<std::string> cmdln)
{
	//LOG PROCESSNIG COMMAND
	string t = "";
	for(int i =0 ; i < cmdln.size(); i++)
	{
		t+=cmdln[i] + " ";
	}
	_LOG.Log(this, DEBUGLOG_LEVEL_ERROR, "ProccessCMD, state: %s | cmdln: %s", m_states[m_iState].m_name.c_str(), t.c_str());

	if(cmdln.size() <= 0)
		return;

	if(!strcmp(m_states[m_iState].m_name.c_str(),"gpio"))
	{
		if(!strcmp(cmdln[0].c_str(),_CMD_ADD))
		{
			if(cmdln.size() != 2)
			{
				PrintHelp("gpio_add");
				return;
			}
			//Add new gpio pin handle
			m_io.Print("Adding GPIO-Pin handle ['%s']\n", cmdln[1].c_str());
			if(m_gpio.addPin(cmdln[1]))
			{
				m_io.Print("Successfully retrieved GPIO-Pin handle [%p,'%s']\n",
					m_gpio.getPin(cmdln[1]),
					cmdln[1].c_str());
			}
			else
			{
				m_io.Print("GPIO-Pin handle already exists [%p, '%s']\n",
					m_gpio.getPin(cmdln[1]),
					cmdln[1].c_str());
			}
		}
		else if(!strcmp(cmdln[0].c_str(),_CMD_STATUS))
		{
			if(cmdln.size() != 2)
			{
				PrintHelp("gpio_status");
				return;
			}

			CGPIOPin* _pin = m_gpio.getPin(cmdln[1]);
			if(!_pin)
			{
				m_io.Print("GPIO-Pin handle ['%s'] has not been initialised", cmdln[1].c_str());
			}

			std::string active = "is active";
			if(!_pin->HandleExists())
				active = "is not active";

			m_io.Print("GPIO-Pin handle[%p,'%s'] %s\n",_pin,cmdln[1].c_str(), active.c_str());
		}
		else if(!strcmp(cmdln[0].c_str(),_CMD_REMOVE))
		{
			if(cmdln.size() != 2)
			{
				PrintHelp("gpio_remove");
				return;
			}
			//Remove gpio pin handle
			m_io.Print("Removing GPIO-Pin handle [%p,'%s']\n",
				m_gpio.getPin(cmdln[1]),
				cmdln[1].c_str());

			if(!m_gpio.remPin(cmdln[1]))
				m_io.Print("Unable to remove GPIO-Pin handle (does not exist\n");
		}
		else if(!strcmp(cmdln[0].c_str(),_CMD_SET))
		{
			if(cmdln.size() != 4)
			{
				PrintHelp("gpio_set");
				return;
			}

			string pin_num = cmdln[1];
			string type = cmdln[2];
			string val = cmdln[3];
			CGPIOPin* _pin = m_gpio.getPin(pin_num);

			if(!_pin)
			{
				m_io.Print("Handle for pin '%s' does not exist\n", pin_num.c_str());
			}

			if(!strcmp(type.c_str(),_CMD_VALUE))
			{
				std::vector<std::string> cmd;
				cmd.push_back(_CMD_GET);
				cmd.push_back(pin_num);
				cmd.push_back("value");

				_pin->SetValue(val);
				ProcessCMD(cmd);
			}
			else if(!strcmp(type.c_str(), _CMD_DIR))
			{
				std::vector<std::string> cmd;
                                cmd.push_back(_CMD_GET);
                                cmd.push_back(pin_num);
                                cmd.push_back("dir");

				if(!strcmp(val.c_str(), "in"))
				{
					_pin->SetDirection("in");
					ProcessCMD(cmd);
				}
				else if(!strcmp(val.c_str(), "out"))
				{
					_pin->SetDirection("out");
					ProcessCMD(cmd);
				}
				else
				{
					m_io.Print("Invalid GPIO-Pin direction: %s\n",val.c_str());
					PrintHelp("gpio_set");
				}
			}
			else
			{
				m_io.Print("Unknown GPIO-Pin variable '%s'\n", type.c_str());
			}
		}
		else if(!strcmp(cmdln[0].c_str(),_CMD_GET))
		{
			if(cmdln.size() != 3)
			{
				PrintHelp("gpio_get");
				return;
			}

			string pin_num = cmdln[1];
			string type = cmdln[2];

			if(!strcmp(type.c_str(),_CMD_VALUE))
			{
				m_io.Print("GPIO-Pin handle[%p,'%s'].value_raw: %s\n",
					m_gpio.getPin(pin_num), pin_num.c_str(),
					m_gpio.getPin(pin_num)->GetValRawS().c_str());

				m_io.Print("GPIO-Pin handle[%p,'%s'].value: %s\n",
                                        m_gpio.getPin(pin_num), pin_num.c_str(),
                                        m_gpio.getPin(pin_num)->GetValS().c_str());
			}
			else if(!strcmp(type.c_str(), _CMD_DIR))
			{
                                m_io.Print("GPIO-Pin handle[%p,'%s'].direction: %s\n",
                                        m_gpio.getPin(pin_num), pin_num.c_str(),
                                        m_gpio.getPin(pin_num)->GetDir().c_str());
			}
			else
			{
				m_io.Print("Unknown GPIO-Pin variable '%s'\n", type.c_str());
			}
		}
		else if(!strcmp(cmdln[0].c_str(),_CMD_EXIT))
		{
			change_state("main");
		}
	}
	else if(!strcmp(m_states[m_iState].m_name.c_str(), "main"))
	{
		if(!strcmp(cmdln[0].c_str(),_CMD_GPIO))
		{
			change_state(_CMD_GPIO);
		}
		else if(!strcmp(cmdln[0].c_str(), _CMD_EXIT))
		{
			change_state(_CMD_EXIT);
		}
	}

	if(!strcmp(cmdln[0].c_str(),_CMD_HELP))
		PrintHelp(m_states[m_iState].m_name);
}

void CSystem::PrintHelp(string arg)
{
	bool bGpio = !strcmp(arg.c_str(), _CMD_GPIO);
	bool bMain = !strcmp(arg.c_str(), _CMD_MAIN);

	//GPIO Manager help
	if(!strcmp(arg.c_str(), "gpio_add") || bGpio)
	{
		m_io.Print(_HELP_GPIOADD);
	}
	if(!strcmp(arg.c_str(), "gpio_remove") || bGpio)
	{
		m_io.Print(_HELP_GPIOREMOVE);
	}
	if(!strcmp(arg.c_str(), "gpio_set") || bGpio)
	{
		m_io.Print(_HELP_GPIOSET);
	}
	if(!strcmp(arg.c_str(), "gpio_get") || bGpio)
	{
		m_io.Print(_HELP_GPIOGET);
	}
	if(!strcmp(arg.c_str(), "gpio_exit") || bGpio)
	{
		m_io.Print(_HELP_GPIOEXIT);
	}
	if(!strcmp(arg.c_str(), "gpio_help") || bGpio)
	{
		m_io.Print(_HELP_GPIOHELP);
	}

	//Main Menu Help
	if(!strcmp(arg.c_str(), "main_gpio") || bMain)
	{
		m_io.Print(_HELP_MAINGPIO);
	}
	if(!strcmp(arg.c_str(), "main_exit") || bMain)
	{
		m_io.Print(_HELP_MAINEXIT);
	}
	if(!strcmp(arg.c_str(), "main_help") || bMain)
	{
		m_io.Print(_HELP_MAINHELP);
	}
}
