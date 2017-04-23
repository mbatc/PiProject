#include "Commands.h"

using namespace std;

vector<string> PassCMDLine(string in)
{
	vector<string> 	ret;
	vector<int>	index;

	int i = 0;
	i = in.find(":");

	while(i != std::string::npos)
	{
		index.push_back(i);
		in[i] = '\0';
		i = in.find(":",i+1);
	};

	ret.push_back(string(&in.c_str()[0]));
	for(int i = 0; i < index.size(); i++)
	{
		string arg = &in.c_str()[index[i] + 1];
		ret.push_back(arg);
	}

	return ret;
}
