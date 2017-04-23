#pragma once

#include "CLog.h"
#include "CInput.h"

#include "CSystem.h"

#define _CMD_CMDLOG "cmdout"
#define _CMD_LOGLEVEL "loglev"
#define _CMD_LOGFILE "logfile"

void PassCMDLine(int argc, char** argv);
