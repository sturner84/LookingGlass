/*
 * ThreadRunnerException.h
 *
 *  Created on: Jul 2, 2013
 *      Author: Administrator
 */

#ifndef THREADRUNNEREXCEPTION_H_
#define THREADRUNNEREXCEPTION_H_

#include <string>
#include <exception>

namespace cpptesting {

class ThreadRunnerException : public std::exception
{
private:
	std::string whatStr;
public:
	ThreadRunnerException () throw();
	ThreadRunnerException (const char* what_arg) throw();
	ThreadRunnerException (const exception& ex) throw();
	std::exception& operator= (const std::exception& ex) throw();
	virtual ~ThreadRunnerException() throw();
	virtual const char* what() const throw();
};

}

#endif /* THREADRUNNEREXCEPTION_H_ */
