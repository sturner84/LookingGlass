/**
 * @file CppThreadedSupport.h
 *
 * @brief Contains functions that allow users to run functions/methods in a
 * 	separate thread.
 *
 * 	These functions are convenience functions and try to simplify the process.
 * 	The invoke/invokeMethod functions assume that the function/method takes
 * 	no parameters. Functions/methods that do take parameters can be run by
 * 	creating a RunnableItem object (see Runners.h for the various classes
 * 	provided or create your own) and passing it to invokeRunner.
 *
 * 	See the Runner.h file for more details about creating RunnableItem classes.
 *
 * 	If a threads takes too long to run, an attempt to interrupt it will be made
 * 	and THREAD_INVOKE_TIMEOUT will be returned from the function.
 *
 *
 * 	Exceptions are catch by the methods and, if it is a standard exception, it
 * 	is stored and returned as a reference parameter.
 *
 *
 * @attention This needs to be linked with the Boost library and the boost
 *  library should come after the CppTesting library (on the linker command)
 *  Linux/Unix: boost_system-mt
 *  			boost_thread-mt
 *
 */

#ifndef CPPTHREADEDSUPPORT_H_
#define CPPTHREADEDSUPPORT_H_

#include <boost/thread.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/bind.hpp>
#include <csignal>

#include "lgRunners.h"
#include "lgThreadRunnerException.h"



namespace cpptesting {

//TODO try to handle stack overflows? Other SIGSEGV?
/**
 * @brief Shows that the thread ended successfully within the time limit.
 */
#define THREAD_INVOKE_SUCCESS 0
/**
 * @brief Shows that the thread did not end within the time limit.
 */
#define THREAD_INVOKE_TIMEOUT 1
/**
 * @brief Shows that the thread threw some sort of exception (maybe not
 * a standard exception). May be used with THREAD_INVOKE_STD_EXCEPTION.
 */
#define THREAD_INVOKE_EXCEPTION 2
/**
 * @brief Shows that the thread threw sa standard exception.
 * May be used with THREAD_INVOKE_EXCEPTION.
 */
#define THREAD_INVOKE_STD_EXCEPTION 4
/**
 * @brief Shows that the thread threw a signal.  Currently only SIGSEGV is
 * being caught (well, attempted to be caught).
 */
////#define THREAD_INVOKE_SIGNAL 8
//
////defines the type of function used for a signal handler
//typedef void (*SignalHandler)(int);
//
////TODO test with windows
//
///**
// * @brief Handler for the signals.  Currently only handles SIGSEGV.
// *
// * This should not be used directly.
// *
// * @param signal Signal thrown.
// */
//void SIGSEGVHandler(int signal);
//
///**
// * Resets the global flag showing that a signal occurred.
// *
// * This should not be used directly.
// */
//void resetSignalFlag();
//
///**
// * Determines if a signal has occurred since the last time resetSignalFlag was
// * called.
// *
// * This should not be used directly.
// *
// * @return true if a signal has occurred (that was caught).
// */
//bool didSignalOccur();
//
///**
// * Gets the signal raised since the last time resetSignalFlag was
// * called.
// *
// * This should not be used directly.
// *
// * @return the signal that was raised (if it was caught). If returns 0 if no
// * signal was raised.
// */
//int getSignal();
//


/**
 * @class ThreadRunner CppThreadedSupport.h
 *
 * @brief Utility class used to run a RunnableItem in a separate thread.
 *
 * This class runs a RunnableItem in a separate thread.  It attempts to
 * catch the exceptions thrown.
 */
class ThreadRunner
{
private:
	boost::timed_mutex& lock;
	RunnableItem& runner;
//	bool signalOccurred;
//	SignalHandler lastHandler;
	int status;
	ThreadRunnerException except;
//	int signalRaised;
public:
	/**
	 * @brief Creates a ThreadRunner using a shared mutex.
	 *
	 * @param tLock A shared mutex.  This mutex should not be locked (it will
	 * be locked as part of the constructor) and can be used by the called to
	 * determine if the thread is running too long.
	 *
	 * @param run The RunnableItem to be run in the new thread.
	 */
	explicit ThreadRunner(boost::timed_mutex& tLock, RunnableItem& run):
	lock(tLock), runner(run)
	{
		lock.lock();

		status = THREAD_INVOKE_SUCCESS;
//		signalOccurred = false;
//		lastHandler = NULL;
//		signalRaised = 0;
	}


	/**
	 * @brief Destructor that unlocks the mutex and restores the signal handler
	 * if needed.
	 */
	virtual ~ThreadRunner()
	{
		lock.unlock();
		//restore last handler if not done already
//		if (lastHandler)
//		{
//			signal(SIGSEGV, lastHandler);
//		}
	}

	/**
	 * @brief Runs the RunnableItem in a new thread.
	 */
	void operator () ()
	{

		try
		{
//			lastHandler = signal(SIGSEGV, SIGSEGVHandler);
			//            	if (lastHandler == SIG_ERR)
			//            	{
			//            		std::cout << "SIG_ERR " << lastHandler << "\n";
			//            		std::cout.flush();
			//            	}
			//            	signal(SIGABRT, SIGSEGVHandler);
			//            	signal(SIGFPE, SIGSEGVHandler);
			//            	signal(SIGILL, SIGSEGVHandler);
			//            	signal(SIGINT, SIGSEGVHandler);
			//            	signal(SIGTERM, SIGSEGVHandler);
//			resetSignalFlag();
			status = THREAD_INVOKE_SUCCESS;
			runner();
		}
		catch (std::exception& e)
		{
			status = THREAD_INVOKE_EXCEPTION | THREAD_INVOKE_STD_EXCEPTION;
//			std::cout << e.what() << "\n";
//			std::cout.flush();
			except = e;
//			std::cout << except.what() << "\n";
//			std::cout.flush();
		}
		catch (...)
		{
			status = THREAD_INVOKE_EXCEPTION;
			//don't know what kind of exception happened
		}
		lock.unlock();
//		if (lastHandler)
//		{
//			signal(SIGSEGV, lastHandler);
//			lastHandler = NULL;
//		}

//		std::cout << "Thread signal " << getSignal() << "\n";
//		std::cout.flush();
//		signalOccurred = didSignalOccur();
//		if (signalOccurred)
//		{
//			status = THREAD_INVOKE_SIGNAL;
//			signalRaised = getSignal();
//		}
	}

	/**
	 * @brief Determines if the RunnableItem completed successfully.
	 *
	 * @return true if the thread completed successfully.
	 */
	bool wasSuccessful()
	{
		return status == THREAD_INVOKE_SUCCESS;
	}


//	/**
//	 * @brief Determines if the RunnableItem caused a signal to be raised.
//	 *
//	 * @return true if the thread raised a signal.
//	 */
//	bool didSignalOccur()
//	{
//		return status == THREAD_INVOKE_SIGNAL;
//	}


	/**
	 * @brief Determines if the RunnableItem threw an exception.
	 *
	 * @return true if the thread threw an exception (standard or otherwise).
	 */
	bool didExceptionOccur()
	{
		return (status & THREAD_INVOKE_EXCEPTION) != 0;
	}


	/**
	 * @brief Determines if the RunnableItem threw a standard exception.
	 *
	 * @return true if the thread threw a standard exception.
	 */
	bool didStdExceptionOccur()
	{
		return (status & THREAD_INVOKE_STD_EXCEPTION) != 0;
	}


	/**
	 * @brief Gets the standard exception thrown.  This is only valid if
	 * didStdExceptionOccur returns true.
	 *
	 * @return The standard exception that was thrown.
	 */
	std::exception& getException()
	{
		return except;
	}

//	int getSignal()
//	{
//		return signalRaised;
//	}


	/**
	 * @brief Gets the signal raised.  This is only valid if
	 * didSignalOccur returns true.
	 *
	 * @return The signal raised.
	 */
	int getStatus()
	{
		return status;
	}
};


/**
 * @brief Runs a RunnableItem in a new thread and tries to impose a time limit
 * on it.  If a standard exception is thrown, except has the value of the
 * exception.
 *
 * @param timeOutMs Time limit on the thread in milliseconds
 * @param run RunnableItem to run in a thread
 * @param except Exception thrown (if any)
 *
 * @return This returns THREAD_INVOKE_SUCCESS (on success) or a combination
 * of any of the following:
 * 		THREAD_INVOKE_TIMEOUT (thread took too long)
 *		THREAD_INVOKE_EXCEPTION (thread threw an exception)
 *		THREAD_INVOKE_STD_EXCEPTION (thread threw a standard exception)
 *		THREAD_INVOKE_SIGNAL (thread raised a signal)
 */
int invokeRunner(int timeOutMs, RunnableItem& run, ThreadRunnerException& except);


/**
 * @brief Runs a RunnableItem in a new thread and tries to impose a time limit
 * on it.  This is the same as invokeRunner(int, RunnableItem&, std:exception&)
 * except that it throws away the exception.
 *
 * @param timeOutMs Time limit on the thread in milliseconds
 * @param run RunnableItem to run in a thread
 *
 * @return This returns THREAD_INVOKE_SUCCESS (on success) or a combination
 * of any of the following:
 * 		THREAD_INVOKE_TIMEOUT (thread took too long)
 *		THREAD_INVOKE_EXCEPTION (thread threw an exception)
 *		THREAD_INVOKE_STD_EXCEPTION (thread threw a standard exception)
 *		THREAD_INVOKE_SIGNAL (thread raised a signal)
 */
int invokeRunner(int timeOutMs, RunnableItem& run);




/**
 * @brief Runs a method on an object.  Assumes that you have a pointer to an
 * object.
 *
 * @param timeOutMs Time limit on the thread in milliseconds
 * @tparam obj Pointer to the object on which the method will be called.
 * @tparam method Method to call. Assumes that is has
 * no parameters.
 *
 * @return This returns THREAD_INVOKE_SUCCESS (on success) or a combination
 * of any of the following:
 * 		THREAD_INVOKE_TIMEOUT (thread took too long)
 *		THREAD_INVOKE_EXCEPTION (thread threw an exception)
 *		THREAD_INVOKE_STD_EXCEPTION (thread threw a standard exception)
 *		THREAD_INVOKE_SIGNAL (thread raised a signal)
 */
template <typename Pointer, typename Callable/*, typename... Params*/>
int invokeMethod(int timeOutMs, Pointer* obj, Callable method/*, Params &&... params*/)
{

	MethodRunner<Pointer> runner(*obj, method/*, boost::forward<Params>(params)...*/);
	return invokeRunner(timeOutMs, runner);
}

/**
 * @brief Runs a method on an object.  A
 *
 * @param timeOutMs Time limit on the thread in milliseconds
 * @tparam obj Object on which the method will be called.
 * @tparam method Method to call. Assumes that is has
 * no parameters.
 *
 * @return This returns THREAD_INVOKE_SUCCESS (on success) or a combination
 * of any of the following:
 * 		THREAD_INVOKE_TIMEOUT (thread took too long)
 *		THREAD_INVOKE_EXCEPTION (thread threw an exception)
 *		THREAD_INVOKE_STD_EXCEPTION (thread threw a standard exception)
 *		THREAD_INVOKE_SIGNAL (thread raised a signal)
 */
template <typename Object, typename Callable/*, typename... Params*/>
int invokeMethod(int timeOutMs, Object& obj, Callable method/*, Params &&... params*/)
{
	MethodRunner<Object> runner(obj, method/*, boost::forward<Params>(params)...*/);
	return invokeRunner(timeOutMs, runner);
}

/**
 * @brief Runs a method on an object.  Assumes that you have a pointer to an
 * object.
 *
 * @param timeOutMs Time limit on the thread in milliseconds
 * @tparam returnT Object to store the result of the method call.  The object
 * should be assignable.
 * @tparam obj Pointer to the object on which the method will be called.
 * @tparam method Method to call. Assumes that is has
 * no parameters.
 *
 * @return This returns THREAD_INVOKE_SUCCESS (on success) or a combination
 * of any of the following:
 * 		THREAD_INVOKE_TIMEOUT (thread took too long)
 *		THREAD_INVOKE_EXCEPTION (thread threw an exception)
 *		THREAD_INVOKE_STD_EXCEPTION (thread threw a standard exception)
 *		THREAD_INVOKE_SIGNAL (thread raised a signal)
 */
template <typename ReturnType, typename Pointer, typename Callable/*, typename... Params*/>
int invokeMethod(int timeOutMs, ReturnType& returnT, Pointer* obj,
		Callable method/*, Params &&... params*/)
{
	MethodReturnRunner<ReturnType, Pointer> runner(returnT, *obj,
			method/*, boost::forward<Params>(params)...*/);
	int success = invokeRunner(timeOutMs, runner);

	returnT = runner.getReturnValue();
	return success;
}

/**
 * @brief Runs a method on an object.
 *
 * @param timeOutMs Time limit on the thread in milliseconds
 * @tparam returnT Object to store the result of the method call.  The object
 * should be assignable.
 * @tparam obj Object on which the method will be called.
 * @tparam method Method to call. Assumes that is has
 * no parameters.
 *
 * @return This returns THREAD_INVOKE_SUCCESS (on success) or a combination
 * of any of the following:
 * 		THREAD_INVOKE_TIMEOUT (thread took too long)
 *		THREAD_INVOKE_EXCEPTION (thread threw an exception)
 *		THREAD_INVOKE_STD_EXCEPTION (thread threw a standard exception)
 *		THREAD_INVOKE_SIGNAL (thread raised a signal)
 */
template <typename ReturnType, typename Object, typename Callable/*, typename... Params*/>
int invokeMethod(int timeOutMs, ReturnType& returnT, Object& obj,
		Callable method/*, Params &&... params*/)
{
	MethodReturnRunner<ReturnType, Object> runner(returnT, obj,
			method/*, boost::forward<Params>(params)...*/);
	int success = invokeRunner(timeOutMs, runner);

	returnT = runner.getReturnValue();
	return success;
}

/**
 * @brief Runs a function, static method or functor.
 *
 * @param timeOutMs Time limit on the thread in milliseconds
  @tparam function Function/static method/functor to call. Assumes that is has
 * no parameters.
 *
 * @return This returns THREAD_INVOKE_SUCCESS (on success) or a combination
 * of any of the following:
 * 		THREAD_INVOKE_TIMEOUT (thread took too long)
 *		THREAD_INVOKE_EXCEPTION (thread threw an exception)
 *		THREAD_INVOKE_STD_EXCEPTION (thread threw a standard exception)
 *		THREAD_INVOKE_SIGNAL (thread raised a signal)
 */
template <typename Callable/*, typename... Params*/>
int invoke(int timeOutMs, Callable& function/*, Params &&... params*/)
{
	FunctorRunner<Callable> runner(function);
	return invokeRunner(timeOutMs, runner);
}

/**
 * @brief Runs a function, static method or functor.
 *
 * @param timeOutMs Time limit on the thread in milliseconds
 * @tparam returnT Object to store the result of the method call.  The object
 * should be assignable.
 * @tparam function Function/static method/functor to call. Assumes that is has
 * no parameters.
 *
 * @return This returns THREAD_INVOKE_SUCCESS (on success) or a combination
 * of any of the following:
 * 		THREAD_INVOKE_TIMEOUT (thread took too long)
 *		THREAD_INVOKE_EXCEPTION (thread threw an exception)
 *		THREAD_INVOKE_STD_EXCEPTION (thread threw a standard exception)
 *		THREAD_INVOKE_SIGNAL (thread raised a signal)
 */
template <typename ReturnType, typename Callable/*, typename... Params*/>
int invoke(int timeOutMs, ReturnType& returnT, Callable& function/*, Params &&... params*/)
{
	//	std::cout << "start " << returnT << "\n";
	//	std::cout.flush();
	FunctorReturnRunner<ReturnType, Callable> runner(returnT, function/*, boost::forward<Params>(params)...*/);
	int success = invokeRunner(timeOutMs, runner);

	returnT = runner.getReturnValue();
	//	std::cout << "start " << returnT << "\n";
	//	std::cout.flush();
	return success;
}



//use boost's threads to run tests
//http://www.boost.org/doc/libs/1_53_0/doc/html/thread.html
//invoke a function/method call with a time limit
//  allow messages
//   or default message





}

#endif /*CPPTHREADEDSUPPORT_H_*/
