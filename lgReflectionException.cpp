/*
 * @file ReflectionException.cpp
 *
 * @brief Exception for use with the reflection methods defined in
 * CppReflectionSupport.h
 *
 * @date Jul 10, 2013
 * @author saturner
 */
#include <exception>
#include <string>

#include "lgReflectionSupport.h"

namespace cpptesting {


ReflectionException::ReflectionException() throw()
{
	message = "An exception occurred using the reflection metadata.";
}

ReflectionException::ReflectionException(const std::string &msg) throw()
{
	message = msg;
}

ReflectionException::ReflectionException(const std::exception &e) throw()
{
	message = e.what();
}

ReflectionException& ReflectionException::operator= (const std::exception& e) throw()
{
	message = e.what();
	return *this;
}

ReflectionException::~ReflectionException() throw()
{
	//do nothing
}

const char* ReflectionException::what() const throw()
{
	return message.c_str();
}

}
