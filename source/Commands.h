#include <string> 
#include <vector>
//Command Line Keywords

#define _CMD_GPIO 	"gpio"
#define _CMD_ADD 	"add"
#define _CMD_REMOVE	"rem"
#define _CMD_CLEAR	"clear"
#define _CMD_SET	"set"
#define _CMD_STATUS	"status"
#define _CMD_VALUE	"value"
#define _CMD_DIR	"dir"
#define _CMD_GET	"get"
#define _CMD_HELP	"help"
#define _CMD_MAIN	"main"
#define _CMD_EXIT	"exit"

std::vector<std::string> PassCMDLine(std::string in);
