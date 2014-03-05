/*
 * CppThreadedSupport.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: scturner
 */

#include "lgThreadedSupport.h"

namespace cpptesting {

//
//bool signalOccurred;
//int signalRaised;
//
//void SIGSEGVHandler(int signal)
//{
//	//stack overflow
////	std::cout << "signal " << signal << "\n";
////	std::cout.flush();
//	signalOccurred = true;
//	signalRaised = signal;
////	if (signal == SIGSEGV)
////	{
////
//////		std::cout << "signal occurred" << signal << "\n";
//////			std::cout.flush();
////	}
//}
//
//void resetSignalFlag()
//{
//	signalOccurred = false;
//	signalRaised = 0;
//}
//
//bool didSignalOccur()
//{
//	return signalOccurred;
//}
//
//bool getSignal()
//{
//	return signalOccurred;
//}

int invokeRunner(int timeOutMs, RunnableItem& run, ThreadRunnerException& except)
{
	//TODO add support for stack overflows
	//timeLock is locked as soon as runner is created
	//it is unlocked at the end of the () operator or in the destructor
	boost::timed_mutex timeLock;
	ThreadRunner runner(timeLock, run);

	if (timeOutMs < 0)
	{
		timeOutMs = 0;
	}

	boost::thread runnerThread(boost::ref(runner));

	int success;

	if (timeLock.timed_lock(boost::posix_time::millisec(timeOutMs)))
	{
		//TODO lock for this?
		success = runner.getStatus();
		except = runner.getException();

//		if (didSignalOccur())
//		{
//			success = success | THREAD_INVOKE_SIGNAL;
//			std::cout << "Success " << success << "\n";
//		}

//		std::cout << "Signal " << getSignal() << "\n";
//		std::cout.flush();
		timeLock.unlock();
	}
	else
	{
		runnerThread.interrupt();
		//fail
		success = THREAD_INVOKE_TIMEOUT;
	}

	return success;
}


int invokeRunner(int timeOutMs, RunnableItem& run)
{
	ThreadRunnerException e;
	return invokeRunner(timeOutMs, run, e);
}


}

