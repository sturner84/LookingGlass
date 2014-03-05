/*
 * ThreadRunnerException.cpp
 *
 *  Created on: Jul 2, 2013
 *      Author: Administrator
 */

#include "lgThreadRunnerException.h"

namespace cpptesting {

ThreadRunnerException::ThreadRunnerException () throw()
{
	whatStr = "Unknown ThreadRunnerException";
}

ThreadRunnerException::ThreadRunnerException (const char* what_arg) throw()
{
	whatStr = what_arg;
}

ThreadRunnerException::ThreadRunnerException (const exception& ex) throw()
{
	whatStr = ex.what();
}

std::exception& ThreadRunnerException::operator= (const std::exception& ex) throw()
{
	whatStr = ex.what();
	return *this;
}

ThreadRunnerException::~ThreadRunnerException() throw()
{
	//do nothing
}

const char* ThreadRunnerException::what() const throw()
{
	return whatStr.c_str();
}

}
