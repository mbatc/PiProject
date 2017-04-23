#include "main.h"

int main(int argc, char** argv)
{
	//Initialise the debug log
	PassCMDLine(argc,argv);

	_SYS.init(argc, argv);
	_SYS.run();

	return 0;
}

void PassCMDLine(int argc, char** argv)
{
	for(int i = 0; i < argc; i++)
	{
		string cmd = argv[i];
		int split_i = cmd.find_first_of("=");
		cmd[split_i] = '\0';

		string arg = &cmd.c_str()[split_i + 1];

		if(arg[0] == '"')
		{
			arg.back() = '\0';
			arg = &arg.c_str()[1];
		}

		if(!strcmp(cmd.c_str(), _CMD_LOGFILE))
		{
			_LOG.SetLogFile(arg.c_str());
		}
		if(!strcmp(cmd.c_str(), _CMD_CMDLOG))
		{
			if(!strcmp(arg.c_str(), "true"))
				_LOG.CMDLog(true);
			else if(!strcmp(arg.c_str(), "false"))
				_LOG.CMDLog(false);
			else
				_LOG.Log(0,DEBUGLOG_LEVEL_WARNING,"Invalid argument '%s' for command '%s'", arg.c_str(),cmd.c_str());
		}
		if(!strcmp(cmd.c_str(), _CMD_LOGLEVEL))
		{
			int level = _LOG.GetLLIndex(arg.c_str());
			if(level < 0)
			{
				_LOG.Log(0,DEBUGLOG_LEVEL_ERROR, "Invalid log level '%s' for command '%s'", arg.c_str(), cmd.c_str());
				level = DEBUGLOG_LEVEL_ALL;
			}
			_LOG.SetLogLevel(level);
		}
	}
}
