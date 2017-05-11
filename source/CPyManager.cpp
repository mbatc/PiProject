#include "CPyManager.h"
#include "CPyFile.h"

#include "CLog.h"

#include <Python.h>

CPyManager::CPyManager(std::string prog_name)
:
	m_program_str(""),
	m_program(L"")
{
	m_program_str = prog_name;
}

CPyManager::~CPyManager()
{
	PYFinalize();
}

int CPyManager::PYInit()
{
	if(Py_IsInitialized())
	{
		_LOG.Log(this,DEBUGLOG_LEVEL_INFO,"[PYTHON]CPyManager already initialised");
		return 0;
	}

	m_program.resize(m_program_str.size() + 1);
	mbstowcs(&m_program[0], m_program_str.c_str(), m_program_str.size());

	if(!m_program.size())
	{
		_LOG.Log(this, DEBUGLOG_LEVEL_ERROR, "[PYTHON]CPyManager: Cannot decode program name (%s)"
			,m_program_str.c_str());
		return 1;
	}

	_LOG.Log(this, DEBUGLOG_LEVEL_INFO, "[PYTHON]Decoded Program name: %s",m_program.c_str());

	Py_SetProgramName(&m_program[0]);
	Py_Initialize();

	return 0;
}

int CPyManager::PYFinalize()
{
	if(!Py_IsInitialized())
	{
		_LOG.Log(this,DEBUGLOG_LEVEL_INFO,"CPyManager finalize(): not initialised");
		return 1;
	}

	Py_Finalize();
	return 0;
}

bool CPyManager::AddScript(std::string file)
{
	if(!Py_IsInitialized())
	{
		_LOG.Log(this, DEBUGLOG_LEVEL_ERROR, "Cannot add python script(%s): CPyManager not initialized", 
			file.c_str());
		return false;
	}

	for(int i = 0; i < m_pyFile.size(); i++)
	{
		if(!strcmp(m_pyFile[i].getName().c_str(), file.c_str()))
		{
			_LOG.Log(this, DEBUGLOG_LEVEL_INFO, "Python script already loaded (%s)",
				file.c_str());
			return true;
		}
	}

	m_pyFile.push_back(CPyFile(file));
	return true;
}

bool CPyManager::RunScript(std::string file)
{
	if(!Py_IsInitialized())
        {
                _LOG.Log(this, DEBUGLOG_LEVEL_ERROR, "Cannot run python script(%s): CPyManager not initialized",
                        file.c_str());
                return false;
        }

	for(int i = 0; i < m_pyFile.size(); i++)
	{
		if(!strcmp(m_pyFile[i].getName().c_str(), file.c_str()))
		{
			_LOG.Log(this, DEBUGLOG_LEVEL_INFO, "Running python script: %s", 
				file.c_str());
			if(!m_pyFile[i].Run())
				return false;
			return true;
		}
	}
	_LOG.Log(this, DEBUGLOG_LEVEL_WARNING, "Failed to run Python script (%s): has not been added", 
		file.c_str());
	return false;
}

bool CPyManager::RemScript(std::string file)
{
	 if(!Py_IsInitialized())
        {
                _LOG.Log(this, DEBUGLOG_LEVEL_ERROR, "Cannot remove python script(%s): CPyManager not initialized",
                        file.c_str());
                return false;
        }


	for(int i = 0; i<m_pyFile.size(); i++)
	{
		if(!strcmp(m_pyFile[i].getName().c_str(), file.c_str()))
		{
			m_pyFile.erase(m_pyFile.cbegin() + i);
			return true;
		}
	}

	return false;
}

int CPyManager::numScripts()
{
	return m_pyFile.size();
}

std::string CPyManager::getName(int i)
{
	if(i < 0 || i >= m_pyFile.size())
		return std::string("Invalid Index");
	return m_pyFile[i].getName();
}
