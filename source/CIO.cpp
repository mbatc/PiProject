#include "CIO.h"
#include "CLog.h"


#include <stdarg.h>
#include <iostream>
#include <fstream>

using namespace std;

CIO::CIO()
{}

CIO::~CIO()
{}

void CIO::Print(string format, ...)
{
	va_list args;
	va_start(args,format);

	_out.Print(format, args);

	va_end(args);
}

void CIO::PrintF(string file, string format, ...)
{
	va_list args;
	va_start(args,format);

	_out.PrintF(file, format, args);

	va_end(args);
}

string CIO::GetCMDLine()
{
	return _in.GetCMDLine();
}

void CIO::OFStream(string file, string format, ...)
{
	ofstream file_ostream(file.c_str());
	if(file_ostream < 0)
	{
		_LOG.Log(this,DEBUGLOG_LEVEL_WARNING, "Unable to open file output stream: %s", file.c_str());
		return;
	}


	file_ostream << format;
	file_ostream.close();
}

std::string CIO::IFStream(string file)
{
	string ret;

	ifstream file_istream(file.c_str());
	if(file_istream < 0)
	{
		_LOG.Log(this,DEBUGLOG_LEVEL_WARNING, "Unable to open file input stream: %s", file.c_str());
		return "";
	}

	file_istream >> ret;
	file_istream.close();
	return ret; 
}

void CIO::OpenFile(string file, const char* t)
{
	_out.OpenFile(file, t);
}

void CIO::CloseFile(string file)
{
	_out.CloseFile(file);
}

void CIO::CloseAllFiles()
{
	_out.CloseAllFiles();
}
