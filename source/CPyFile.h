#pragma once

#include <Python.h>
#include <string>

class CPyFile
{
public:
	CPyFile(std::string filename);
	~CPyFile();

	int Run(int argv = 0, char** argc = 0);

	std::string getName();
private:
	bool initialize();

	FILE*		m_pCpp_file;
	PyObject* 	m_pPy_file_obj;

	std::string	m_py_file_name;
};
