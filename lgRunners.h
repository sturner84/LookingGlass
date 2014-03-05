/**
 * @file Runners.h
 *
 * @brief Contains classes that allow users to run functions/methods in a
 * 	separate thread.
 *
 * The classes in this header take a callable object (function, method, class with
 * () operator) and stores it until it is run.  These classes are used to run
 * the callable entity in a separate thread (see CppThreadedSupport.h).
 *
 * Currently, there is support for callable entities with 0, 1, or 2 REFERENCE
 * parameters and for entities that return or do not return a value. If more flexibility
 * is required for a particular method, the user can extend the RunnableItem
 * class and create an appropriate constructor and () operator.
 *
 *  @attention This needs to be linked with the Boost library and the boost
 *  library should come after the CppTesting library (on the linker command).
 *  See CppThreadedSupport.h
 *  Linux/Unix: boost_system-mt
 *  			boost_thread-mt
 *
 * @author saturner
 * @date 2013-07-02
 */

//TODO use macros to generate multiple classes with multiple parameters?

#ifndef RUNNERS_H_
#define RUNNERS_H_

namespace cpptesting {

/**
 * @class RunnableItem Runnerst.h
 * @brief Abstract base class for items being run in a thread.
 *
 * This class allows an operation to be run in a thread. Derived classes should
 * implement operator() and, if returning a value from the operation, should
 * create appropriate methods for that.
 *
 */
class RunnableItem
{
public:
	/**
	 * @brief Runs the operation.
	 */
	virtual void operator()() = 0;


	/**
	 * @brief Does any clean up necessary
	 */
	virtual ~RunnableItem()
	{
		//nothing to do
	}
};



/**
 * @class FunctionRunner Runnerst.h
 * @brief Runs a function or static method. For use when running in
 * a separate thread.
 *
 * This class allows an operation to be run in a thread. This runs a function,
 * or static method.  It supposes the that function has no parameters
 * and a void return type.
 *
 * @tparam Callable A data type that can be called with the x() syntax.
 */
class FunctionRunner : public RunnableItem
{
public:
	typedef void (Callable)();
protected:
	/**
	 * Callable object being run in a thread.
	 */
	Callable& function;

public:
	/**
	 * @brief Stores a function, static method, or functor that will be run
	 * in a thread.
	 *
	 * @tparam f Function, static method or functor that will be called and run
	 * in the thread.  It should not require parameters.
	 */
	explicit FunctionRunner(Callable& f/*, Params &&... params*/): function(f)
	{
		//		//bind the parameters to the function.  Only up to 9!
		//		bindArguments(1, function, boost::forward<Params>(params)...);
	}


	/**
	 * @brief Does any clean up necessary
	 */
	virtual ~FunctionRunner()
	{
		//do nothing, let BaseRunner handle it.
	}


	/**
	 * @brief Runs the operation.
	 */
	virtual void operator()()
	{
		function();
	}
};


/**
 * @class FunctionReturnRunner Runnerst.h
 * @brief Runs a function or static method. For use when running in
 * a separate thread.
 *
 * This class allows an operation to be run in a thread. This runs a function,
 * or static method.  It supposes the that function has no parameters
 * but that it returns a value.
 *
 * @tparam ReturnType The type of data returned from the function of type
 * 	Callable.
 *
 * @tparam Callable A data type that can be called with the x() syntax.
 */
template <typename ReturnType/*, typename... Params*/>
class FunctionReturnRunner : public RunnableItem
{
public:
	typedef ReturnType (Callable)();
protected:
	/**
	 * Values being returned
	 */
	ReturnType& returnVal;
	/**
	 * Callable object being run in a thread.
	 */
	Callable& function;

public:
	/**
	 * @brief Stores a function or static method that will be run
	 * in a thread.
	 *
	 * @tparam r An object of the same type as the value to be returned. This
	 * value is likely to be changed by the operation.
	 *
	 * @tparam f Function, static method or functor that will be called and run
	 * in the thread.  It should not require parameters.
	 */
	explicit FunctionReturnRunner(ReturnType& r, Callable& f/*, Params&&... params*/):
	returnVal(r), function(f)
	{
		//		//bind the parameters to the function.  Only up to 9!
		//		bindArguments(1, function, boost::forward<Params>(params)...);
	}



	/**
	 * @brief Does any clean up necessary
	 */
	virtual ~FunctionReturnRunner()
	{
		//do nothing, let BaseRunner handle it.
	}


	/**
	 * @brief Runs the operation and stores the result.
	 */
	virtual void operator()()
	{
		//		std::cout << returnVal << "\n";
		//		std::cout.flush();
		returnVal = function();
		//		std::cout << returnVal << "\n";
		//		std::cout.flush();
	}


	/**
     * @brief Gets the return value from running operator()
     *
     * @return The value returned from the operation.
     */
    ReturnType getReturnValue()
	{
		return returnVal;
	}
};



/**
 * @class FunctionRunner Runnerst.h
 * @brief Runs a function, static method, or functor. For use when running in
 * a separate thread.
 *
 * This class allows an operation to be run in a thread. This runs a function,
 * static method, or functor.  It supposes the that function has no parameters
 * and a void return type.
 *
 * @tparam Callable A data type that can be called with the x() syntax.
 */
template <typename Callable> class FunctorRunner : public RunnableItem
{

protected:
	/**
	 * Callable object being run in a thread.
	 */
	Callable& function;

public:
	/**
	 * @brief Stores a function, static method, or functor that will be run
	 * in a thread.
	 *
	 * @tparam f Function, static method or functor that will be called and run
	 * in the thread.  It should not require parameters.
	 */
	explicit FunctorRunner(Callable& f): function(f)
	{
		//nothing to do
	}


	/**
	 * @brief Does any clean up necessary
	 */
	virtual ~FunctorRunner()
	{
		//do nothing, let BaseRunner handle it.
	}


	/**
	 * @brief Runs the operation.
	 */
	virtual void operator()()
	{
		function();
	}
};


/**
 * @class FunctionReturnRunner Runnerst.h
 * @brief Runs a function, static method, or functor. For use when running in
 * a separate thread.
 *
 * This class allows an operation to be run in a thread. This runs a function,
 * static method, or functor.  It supposes the that function has no parameters
 * but that it returns a value.
 *
 * @tparam ReturnType The type of data returned from the function of type
 * 	Callable.
 *
 * @tparam Callable A data type that can be called with the x() syntax.
 */
template <typename ReturnType, typename Callable>
class FunctorReturnRunner : public RunnableItem
{
protected:
	/**
	 * Value being returned
	 */
	ReturnType& returnVal;
	/**
	 * Callable object being run in a thread.
	 */
	Callable& function;

public:
	/**
	 * @brief Stores a function, static method, or functor that will be run
	 * in a thread.
	 *
	 * @tparam r An object of the same type as the value to be returned. This
	 * value is likely to be changed by the operation.
	 *
	 * @tparam f Function, static method or functor that will be called and run
	 * in the thread.  It should not require parameters.
	 */
	explicit FunctorReturnRunner(ReturnType& r, Callable& f):
	returnVal(r), function(f)
	{
		//nothing to do
	}



	/**
	 * @brief Does any clean up necessary
	 */
	virtual ~FunctorReturnRunner()
	{
		//do nothing, let BaseRunner handle it.
	}


	/**
	 * @brief Runs the operation and stores the result.
	 */
	virtual void operator()()
	{
		returnVal = function();
	}


	/**
     * @brief Gets the return value from running operator()
     *
     * @return The value returned from the operation.
     */
    ReturnType getReturnValue()
	{
		return returnVal;
	}
};



/**
 * @class MethodRunner Runnerst.h
 * @brief Runs a method (not static). For use when running in
 * a separate thread.
 *
 * This class allows an operation to be run in a thread. This runs a non-static
 * method.  It supposes the that method has no parameters and a void return
 * type.
 *
 * @tparam Object The object type on which to call the Callable method.
 *
 * @tparam Callable A method that can be called with the (object.*function)()
 * syntax.
 */
template <typename Object/*, typename... Params*/>
class MethodRunner: public RunnableItem
{
public:
	typedef void (Object::*Callable)();
protected:
	/**
	 * Callable method being run in a thread.
	 */
	Callable function;
	/**
	 * Object the method is being run on.
	 */
	Object& object;
public:
	/**
	 * @brief Stores a method that will be run
	 * in a thread.
	 *
	 * @tparam obj Object on which to call the method.
	 *
	 * @tparam f method that will be called and run in the thread.
	 * It should not require parameters.
	 */
	explicit MethodRunner(Object& obj, Callable f/*, Params&&... params*/):
	function(f), object(obj)
	{
		//		//bind the parameters to the function.  Only up to 8!
		//		//The first parameter is the object
		//		bindArguments(2, function, boost::forward<Params>(params)...);
	}


	/**
	 * @brief Does any clean up necessary
	 */
	virtual ~MethodRunner()
	{
		//do nothing, let BaseRunner handle it.
	}


	/**
	 * @brief Runs the operation.
	 */
	virtual void operator()()
	{
		//		boost::bind(function, boost::ref(object), 1);
		//		function();
		(object.*function)();
	}
};


/**
 * @class MethodRunner Runnerst.h
 * @brief Runs a method (not static). For use when running in
 * a separate thread.
 *
 * This class allows an operation to be run in a thread. This runs a non-static
 * method.  It supposes the that method has no parameters but that it returns
 * a value.
 *
 * @tparam ReturnType The type of data returned from the function of type
 * 	Callable.
 *
 * @tparam Object The object type on which to call the Callable method.
 *
 * @tparam Callable A method that can be called with the (object.*function)()
 * syntax.
 */
template <typename ReturnType, typename Object/*, typename... Params*/>
class MethodReturnRunner: public RunnableItem
{
public:
	typedef ReturnType (Object::*Callable)();
protected:
	/**
	 * Value returned from the method
	 */
	ReturnType& returnVal;
	/**
	 * Callable method being run in a thread.
	 */
	Callable function;
	/**
	 * Object the method is being run on.
	 */
	Object& object;
public:
	/**
	 * @brief Stores a method that will be run
	 * in a thread.
	 *
	 * @tparam r An object of the same type as the value to be returned. This
	 * value is likely to be changed by the operation.
	 *
	 * @tparam obj Object on which to call the method.
	 *
	 * @tparam f method that will be called and run in the thread.
	 * It should not require parameters.
	 */
	explicit MethodReturnRunner(ReturnType& r, Object& obj, Callable f/*, Params&&... params*/):
	returnVal(r), function(f), object(obj)
	{
		//		//bind the parameters to the function.  Only up to 8!
		//		//The first parameter is the object
		//		bindArguments(2, function, boost::forward<Params>(params)...);
	}


	/**
	 * @brief Does any clean up necessary
	 */
	virtual ~MethodReturnRunner()
	{
		//do nothing, let BaseRunner handle it.
	}


	/**
	 * @brief Runs the operation and stores the result.
	 */
	virtual void operator()()
	{
		returnVal = (object.*function)();
	}


	/**
     * @brief Gets the return value from running operator()
     *
     * @return The value returned from the operation.
     */
    ReturnType getReturnValue()
	{
		return returnVal;
	}
};

/**
 * @class FunctionRunner Runnerst.h
 * @brief Runs a function or static method. For use when running in
 * a separate thread.
 *
 * This class allows an operation to be run in a thread. This runs a function,
 * or static method.  It supposes the that function has one parameter
 * and a void return type.
 *
 * @tparam Callable A data type that can be called with the x() syntax.
 *
 * @tparam P1 Type of the parameter being passed to the Callable object.
 */
template <typename P1> class FunctionRunnerP1 : public RunnableItem
{
public:
	typedef void (Callable)(P1);
protected:
	Callable& function;
	P1 param1;
public:
//	/**
//	 * @brief Stores a function, static method, or functor that will be run
//	 * in a thread.
//	 *
//	 * @tparam f Function, static method or functor that will be called and run
//	 * in the thread.  It should not require parameters.
//	 *
//	 * @tparam p1 First parameter to the function.
//	 */
//	explicit FunctionRunnerP1(Callable& f, P1& p1): function(f), param1(boost::ref(p1))
//	{
//		//everything initialized
//	}

	/**
	 * @brief Stores a function, static method, or functor that will be run
	 * in a thread.
	 *
	 * @tparam f Function, static method or functor that will be called and run
	 * in the thread.  It should not require parameters.
	 *
	 * @tparam p1 First parameter to the function.
	 */
	explicit FunctionRunnerP1(Callable& f, P1 p1): function(f), param1(p1)
	{
		//everything initialized
	}

	/**
	 * @brief Does any clean up necessary
	 */
	virtual ~FunctionRunnerP1()
	{
		//do nothing, let BaseRunner handle it.
	}


	/**
	 * @brief Runs the operation.
	 */
	virtual void operator()()
	{
		function(param1);
	}
};

/**
 * @class FunctionReturnRunner Runnerst.h
 * @brief Runs a function or static method. For use when running in
 * a separate thread.
 *
 * This class allows an operation to be run in a thread. This runs a function
 * or static method.  It supposes the that function has one parameter
 * and that it returns a value.
 *
 * @tparam ReturnType The type of data returned from the function of type
 * 	Callable.
 *
 * @tparam Callable A data type that can be called with the x() syntax.
 *
 * @tparam P1 Type of the parameter being passed to the Callable object.
 */
template <typename ReturnType, typename P1>
class FunctionReturnRunnerP1 : public RunnableItem
{
public:
	typedef ReturnType (Callable)(P1);
protected:
	ReturnType& returnVal;
	Callable& function;
	P1 param1;

public:
//	/**
//	 * @brief Stores a function, static method, or functor that will be run
//	 * in a thread.
//	 *
//	 * @tparam r An object of the same type as the value to be returned. This
//	 * value is likely to be changed by the operation.
//	 *
//	 * @tparam f Function, static method or functor that will be called and run
//	 * in the thread.  It should not require parameters.
//	 *
//	 * @tparam p1 First parameter to the function.
//	 */
//	explicit FunctionReturnRunnerP1(ReturnType& r, Callable& f, P1& p1):
//	returnVal(r), function(f), param1(boost::ref(p1))
//	{
//		//everything initialized
//	}

	/**
	 * @brief Stores a function, static method, or functor that will be run
	 * in a thread.
	 *
	 * @tparam r An object of the same type as the value to be returned. This
	 * value is likely to be changed by the operation.
	 *
	 * @tparam f Function, static method or functor that will be called and run
	 * in the thread.  It should not require parameters.
	 *
	 * @tparam p1 First parameter to the function.
	 */
	explicit FunctionReturnRunnerP1(ReturnType& r, Callable& f, P1 p1):
				returnVal(r), function(f), param1(p1)
	{
		//everything initialized
	}



	/**
	 * @brief Does any clean up necessary
	 */
	virtual ~FunctionReturnRunnerP1()
	{
		//do nothing, let BaseRunner handle it.
	}


	/**
	 * @brief Runs the operation and stores the result.
	 */
	virtual void operator()()
	{
		returnVal = function(param1);
	}


	/**
     * @brief Gets the return value from running operator()
     *
     * @return The value returned from the operation.
     */
    ReturnType getReturnValue()
	{
		return returnVal;
	}
};

/**
 * @class FunctionRunner Runnerst.h
 * @brief Runs a function, static method, or functor. For use when running in
 * a separate thread.
 *
 * This class allows an operation to be run in a thread. This runs a function,
 * static method, or functor.  It supposes the that function has one parameter
 * and a void return type.
 *
 * @tparam Callable A data type that can be called with the x() syntax.
 *
 * @tparam P1 Type of the parameter being passed to the Callable object.
 */
template <typename Callable, typename P1> class FunctorRunnerP1 : public RunnableItem
{
protected:
	Callable& function;
	P1 param1;
public:

	/**
	 * @brief Stores a function, static method, or functor that will be run
	 * in a thread.
	 *
	 * @tparam f Function, static method or functor that will be called and run
	 * in the thread.  It should not require parameters.
	 *
	 * @tparam p1 First parameter to the function.
	 */
	explicit FunctorRunnerP1(Callable& f, P1 p1): function(f), param1(p1)
	{
		//everything initialized
	}

	/**
	 * @brief Does any clean up necessary
	 */
	virtual ~FunctorRunnerP1()
	{
		//do nothing, let BaseRunner handle it.
	}


	/**
	 * @brief Runs the operation.
	 */
	virtual void operator()()
	{
		function(param1);
	}
};

/**
 * @class FunctionReturnRunner Runnerst.h
 * @brief Runs a function, static method, or functor. For use when running in
 * a separate thread.
 *
 * This class allows an operation to be run in a thread. This runs a function,
 * static method, or functor.  It supposes the that function has one parameter
 * and that it returns a value.
 *
 * @tparam ReturnType The type of data returned from the function of type
 * 	Callable.
 *
 * @tparam Callable A data type that can be called with the x() syntax.
 *
 * @tparam P1 Type of the parameter being passed to the Callable object.
 */
template <typename ReturnType, typename Callable, typename P1>
class FunctorReturnRunnerP1 : public RunnableItem
{
protected:
	ReturnType& returnVal;
	Callable& function;
	P1 param1;

public:
	/**
	 * @brief Stores a function, static method, or functor that will be run
	 * in a thread.
	 *
	 * @tparam r An object of the same type as the value to be returned. This
	 * value is likely to be changed by the operation.
	 *
	 * @tparam f Function, static method or functor that will be called and run
	 * in the thread.  It should not require parameters.
	 *
	 * @tparam p1 First parameter to the function.
	 */
	explicit FunctorReturnRunnerP1(ReturnType& r, Callable& f, P1 p1):
				returnVal(r), function(f), param1(p1)
	{
		//everything initialized
	}



	/**
	 * @brief Does any clean up necessary
	 */
	virtual ~FunctorReturnRunnerP1()
	{
		//do nothing, let BaseRunner handle it.
	}


	/**
	 * @brief Runs the operation and stores the result.
	 */
	virtual void operator()()
	{
		returnVal = function(param1);
	}


	/**
     * @brief Gets the return value from running operator()
     *
     * @return The value returned from the operation.
     */
    ReturnType getReturnValue()
	{
		return returnVal;
	}
};


/**
 * @class MethodRunner Runnerst.h
 * @brief Runs a method (not static). For use when running in
 * a separate thread.
 *
 * This class allows an operation to be run in a thread. This runs a non-static
 * method.  It supposes the that method has one parameter and a void return
 * type.
 *
 * @tparam Object The object type on which to call the Callable method.
 *
 * @tparam Callable A method that can be called with the (object.*function)()
 * syntax.
 *
 * @tparam P1 Type of the parameter being passed to the Callable object.
 */
template <typename Object, typename P1>
class MethodRunnerP1: public RunnableItem
{
public:
	typedef void (Object::*Callable)(P1);
protected:
	Callable function;
	Object& object;
	P1 param1;

public:
//	/**
//	 * @brief Stores a method that will be run
//	 * in a thread.
//	 *
//	 * @tparam obj Object on which to call the method.
//	 *
//	 * @tparam f method that will be called and run in the thread.
//	 * It should not require parameters.
//	 *
//	 * @tparam p1 First parameter to the function.
//	 */
//	explicit MethodRunnerP1(Object& obj, Callable& f, P1& p1):
//	function(f), object(obj), param1(boost::ref(p1))
//	{
//		//everything initialized
//	}

	/**
	 * @brief Stores a method that will be run
	 * in a thread.
	 *
	 * @tparam obj Object on which to call the method.
	 *
	 * @tparam f method that will be called and run in the thread.
	 * It should not require parameters.
	 *
	 * @tparam p1 First parameter to the function.
	 */
	explicit MethodRunnerP1(Object& obj, Callable f, P1 p1):
	function(f), object(obj), param1(p1)
	{
		//everything initialized
	}


	/**
	 * @brief Does any clean up necessary
	 */
	virtual ~MethodRunnerP1()
	{
		//do nothing, let BaseRunner handle it.
	}


	/**
	 * @brief Runs the operation.
	 */
	virtual void operator()()
	{
		(object.*function)(param1);
	}
};


/**
 * @class MethodRunner Runnerst.h
 * @brief Runs a method (not static). For use when running in
 * a separate thread.
 *
 * This class allows an operation to be run in a thread. This runs a non-static
 * method.  It supposes the that method has one parameter and that it returns
 * a value.
 *
 * @tparam ReturnType The type of data returned from the function of type
 * 	Callable.
 *
 * @tparam Object The object type on which to call the Callable method.
 *
 * @tparam Callable A method that can be called with the (object.*function)()
 * syntax.
 *
 * @tparam P1 Type of the parameter being passed to the Callable object.
 */
template <typename ReturnType, typename Object, typename P1>
class MethodReturnRunnerP1: public RunnableItem
{
public:
	typedef ReturnType (Object::*Callable)(P1);

protected:
	ReturnType& returnVal;
	Callable function;
	Object& object;
	P1 param1;

public:
//	/**
//	 * @brief Stores a method that will be run
//	 * in a thread.
//	 *
//	 * @tparam r An object of the same type as the value to be returned. This
//	 * value is likely to be changed by the operation.
//	 *
//	 * @tparam obj Object on which to call the method.
//	 *
//	 * @tparam f method that will be called and run in the thread.
//	 * It should not require parameters.
//	 *
//	 * @tparam p1 First parameter to the function.
//	 */
//	explicit MethodReturnRunnerP1(ReturnType& r, Object& obj, Callable& f, P1& p1):
//	returnVal(r), function(f), object(obj), param1(boost::ref(p1))
//	{
//		//everything initialized
//	}

	/**
	 * @brief Stores a method that will be run
	 * in a thread.
	 *
	 * @tparam r An object of the same type as the value to be returned. This
	 * value is likely to be changed by the operation.
	 *
	 * @tparam obj Object on which to call the method.
	 *
	 * @tparam f method that will be called and run in the thread.
	 * It should not require parameters.
	 *
	 * @tparam p1 First parameter to the function.
	 */
	explicit MethodReturnRunnerP1(ReturnType& r, Object& obj, Callable f, P1 p1):
	returnVal(r), function(f), object(obj), param1(p1)
	{
		//everything initialized
	}


	/**
	 * @brief Does any clean up necessary
	 */
	virtual ~MethodReturnRunnerP1()
	{
		//do nothing, let BaseRunner handle it.
	}

	/**
	 * @brief Runs the operation and store the result.
	 */
	virtual void operator()()
	{
		returnVal = (object.*function)(param1);
	}


	/**
     * @brief Gets the return value from running operator()
     *
     * @return The value returned from the operation.
     */
    ReturnType getReturnValue()
	{
		return returnVal;
	}
};

/**
 * @class FunctionRunner Runnerst.h
 * @brief Runs a function or static method. For use when running in
 * a separate thread.
 *
 * This class allows an operation to be run in a thread. This runs a function
 * or static method.  It supposes the that function has two parameters
 * and a void return type.
 *
 * @tparam Callable A data type that can be called with the x() syntax.
 *
 * @tparam P1 Type of the first parameter being passed to the Callable object.
 *
 * @tparam P2 Type of the second parameter being passed to the Callable object.
 */
template <typename P1, typename P2> class FunctionRunnerP2 : public RunnableItem
{
public:
	typedef void (Callable)(P1, P2);
protected:
	Callable& function;
	P1 param1;
	P2 param2;

public:

	/**
	 * @brief Stores a function, static method, or functor that will be run
	 * in a thread.
	 *
	 * @tparam f Function, static method or functor that will be called and run
	 * in the thread.  It should not require parameters.
	 *
	 * @tparam p1 First parameter to the function.
	 *
	 * @tparam p2 Second parameter to the function.
	 */
	explicit FunctionRunnerP2(Callable& f, P1 p1, P2 p2): function(f), param1(p1), param2(p2)
	{
		//everything initialized
	}


	/**
	 * @brief Does any clean up necessary
	 */
	virtual ~FunctionRunnerP2()
	{
		//do nothing, let BaseRunner handle it.
	}


	/**
	 * @brief Runs the operation.
	 */
	virtual void operator()()
	{
		function(param1, param2);
	}
};

/**
 * @class FunctionReturnRunner Runnerst.h
 * @brief Runs a function or static method. For use when running in
 * a separate thread.
 *
 * This class allows an operation to be run in a thread. This runs a function
 * or static method.  It supposes the that function has two parameters
 * and that it returns a value.
 *
 * @tparam ReturnType The type of data returned from the function of type
 * 	Callable.
 *
 * @tparam Callable A data type that can be called with the x() syntax.
 *
 * @tparam P1 Type of the first parameter being passed to the Callable object.
 *
 * @tparam P2 Type of the second parameter being passed to the Callable object.
 */
template <typename ReturnType, typename P1, typename P2>
class FunctionReturnRunnerP2 : public RunnableItem
{
public:
	typedef ReturnType (Callable)(P1, P2);
protected:
	ReturnType& returnVal;
	Callable& function;
	P1 param1;
	P2 param2;

public:

	/**
	 * @brief Stores a function, static method, or functor that will be run
	 * in a thread.
	 *
	 * @tparam r An object of the same type as the value to be returned. This
	 * value is likely to be changed by the operation.
	 *
	 * @tparam f Function, static method or functor that will be called and run
	 * in the thread.  It should not require parameters.
	 *
	 * @tparam p1 First parameter to the function.
	 *
	 * @tparam p2 Second parameter to the function.
	 */
	explicit FunctionReturnRunnerP2(ReturnType& r, Callable& f, P1 p1, P2 p2):
	returnVal(r), function(f), param1(p1), param2(p2)
	{
		//everything initialized
	}


	/**
	 * @brief Does any clean up necessary
	 */
	virtual ~FunctionReturnRunnerP2()
	{
		//do nothing, let BaseRunner handle it.
	}


	/**
	 * @brief Runs the operation and store the result.
	 */
	virtual void operator()()
	{
		returnVal = function(param1, param2);
	}


	/**
     * @brief Gets the return value from running operator()
     *
     * @return The value returned from the operation.
     */
    ReturnType getReturnValue()
	{
		return returnVal;
	}
};

/**
 * @class FunctionRunner Runnerst.h
 * @brief Runs a function, static method, or functor. For use when running in
 * a separate thread.
 *
 * This class allows an operation to be run in a thread. This runs a function,
 * static method, or functor.  It supposes the that function has two parameters
 * and a void return type.
 *
 * @tparam Callable A data type that can be called with the x() syntax.
 *
 * @tparam P1 Type of the first parameter being passed to the Callable object.
 *
 * @tparam P2 Type of the second parameter being passed to the Callable object.
 */
template <typename Callable, typename P1, typename P2> class FunctorRunnerP2 : public RunnableItem
{
protected:
	Callable& function;
	P1 param1;
	P2 param2;

public:
	/**
	 * @brief Stores a function, static method, or functor that will be run
	 * in a thread.
	 *
	 * @tparam f Function, static method or functor that will be called and run
	 * in the thread.  It should not require parameters.
	 *
	 * @tparam p1 First parameter to the function.
	 *
	 * @tparam p2 Second parameter to the function.
	 */
	explicit FunctorRunnerP2(Callable& f, P1 p1, P2 p2): function(f), param1(p1), param2(p2)
	{
		//everything initialized
	}


	/**
	 * @brief Does any clean up necessary
	 */
	virtual ~FunctorRunnerP2()
	{
		//do nothing, let BaseRunner handle it.
	}


	/**
	 * @brief Runs the operation.
	 */
	virtual void operator()()
	{
		function(param1, param2);
	}
};

/**
 * @class FunctionReturnRunner Runnerst.h
 * @brief Runs a function, static method, or functor. For use when running in
 * a separate thread.
 *
 * This class allows an operation to be run in a thread. This runs a function,
 * static method, or functor.  It supposes the that function has two parameters
 * and that it returns a value.
 *
 * @tparam ReturnType The type of data returned from the function of type
 * 	Callable.
 *
 * @tparam Callable A data type that can be called with the x() syntax.
 *
 * @tparam P1 Type of the first parameter being passed to the Callable object.
 *
 * @tparam P2 Type of the second parameter being passed to the Callable object.
 */
template <typename ReturnType, typename Callable, typename P1, typename P2>
class FunctorReturnRunnerP2 : public RunnableItem
{
protected:
	ReturnType& returnVal;
	Callable& function;
	P1 param1;
	P2 param2;

public:

	/**
	 * @brief Stores a function, static method, or functor that will be run
	 * in a thread.
	 *
	 * @tparam r An object of the same type as the value to be returned. This
	 * value is likely to be changed by the operation.
	 *
	 * @tparam f Function, static method or functor that will be called and run
	 * in the thread.  It should not require parameters.
	 *
	 * @tparam p1 First parameter to the function.
	 *
	 * @tparam p2 Second parameter to the function.
	 */
	explicit FunctorReturnRunnerP2(ReturnType& r, Callable& f, P1 p1, P2 p2):
	returnVal(r), function(f), param1(p1), param2(p2)
	{
		//everything initialized
	}


	/**
	 * @brief Does any clean up necessary
	 */
	virtual ~FunctorReturnRunnerP2()
	{
		//do nothing, let BaseRunner handle it.
	}


	/**
	 * @brief Runs the operation and store the result.
	 */
	virtual void operator()()
	{
		returnVal = function(param1, param2);
	}


	/**
     * @brief Gets the return value from running operator()
     *
     * @return The value returned from the operation.
     */
    ReturnType getReturnValue()
	{
		return returnVal;
	}
};



/**
 * @class MethodRunner Runnerst.h
 * @brief Runs a method (not static). For use when running in
 * a separate thread.
 *
 * This class allows an operation to be run in a thread. This runs a non-static
 * method.  It supposes the that method has two parameters and a void return
 * type.
 *
 * @tparam Object The object type on which to call the Callable method.
 *
 * @tparam Callable A method that can be called with the (object.*function)()
 * syntax.
 *
 * @tparam P1 Type of the first parameter being passed to the Callable object.
 *
 * @tparam P2 Type of the second parameter being passed to the Callable object.
 */
template <typename Object, typename P1, typename P2>
class MethodRunnerP2: public RunnableItem
{
public:
	typedef void (Object::*Callable)(P1, P2);
protected:
	Callable function;
	Object& object;
	P1 param1;
	P2 param2;
public:
//	/**
//	 * @brief Stores a method that will be run
//	 * in a thread.
//	 *
//	 * @tparam obj Object on which to call the method.
//	 *
//	 * @tparam f method that will be called and run in the thread.
//	 * It should not require parameters.
//	 *
//	 * @tparam p1 First parameter to the function.
//	 *
//	 * @tparam p2 Second parameter to the function.
//	  */
//	explicit MethodRunnerP2(Object& obj, Callable& f, P1& p1, P2& p2):
//	function(f), object(obj), param1(boost::ref(p1)), param2(boost::ref(p2))
//	{
//		//everything initialized
//	}

	/**
	 * @brief Stores a method that will be run
	 * in a thread.
	 *
	 * @tparam obj Object on which to call the method.
	 *
	 * @tparam f method that will be called and run in the thread.
	 * It should not require parameters.
	 *
	 * @tparam p1 First parameter to the function.
	 *
	 * @tparam p2 Second parameter to the function.
	  */
	explicit MethodRunnerP2(Object& obj, Callable f, P1 p1, P2 p2):
	function(f), object(obj), param1(p1), param2(p2)
	{
		//everything initialized
	}


	/**
	 * @brief Does any clean up necessary
	 */
	virtual ~MethodRunnerP2()
	{
		//do nothing, let BaseRunner handle it.
	}


	/**
	 * @brief Runs the operation.
	 */
	virtual void operator()()
	{
		(object.*function)(param1, param2);
	}
};

/**
 * @class MethodRunner Runnerst.h
 * @brief Runs a method (not static). For use when running in
 * a separate thread.
 *
 * This class allows an operation to be run in a thread. This runs a non-static
 * method.  It supposes the that method has two parameters and that it returns
 * a value.
 *
 * @tparam ReturnType The type of data returned from the function of type
 * 	Callable.
 *
 * @tparam Object The object type on which to call the Callable method.
 *
 * @tparam Callable A method that can be called with the (object.*function)()
 * syntax.
 *
 * @tparam P1 Type of the first parameter being passed to the Callable object.
 *
 * @tparam P2 Type of the second parameter being passed to the Callable object.
 */
template <typename ReturnType, typename Object, typename P1, typename P2>
class MethodReturnRunnerP2: public RunnableItem
{
public:
	typedef ReturnType (Object::*Callable)(P1, P2);
protected:
	ReturnType& returnVal;
	Callable function;
	Object& object;
	P1 param1;
	P2 param2;
public:
//	/**
//	 * @brief Stores a method that will be run
//	 * in a thread.
//	 *
//	 * @tparam r An object of the same type as the value to be returned. This
//	 * value is likely to be changed by the operation.
//	 *
//	 * @tparam obj Object on which to call the method.
//	 *
//	 * @tparam f method that will be called and run in the thread.
//	 * It should not require parameters.
//	 *
//	 * @tparam p1 First parameter to the function.
//	 *
//	 * @tparam p2 Second parameter to the function.
//	 */
//	explicit MethodReturnRunnerP2(ReturnType& r, Object& obj, Callable& f, P1& p1, P2& p2):
//	returnVal(r), function(f), object(obj), param1(boost::ref(p1)), param2(boost::ref(p2))
//	{
//		//everything initialized
//	}

	/**
	 * @brief Stores a method that will be run
	 * in a thread.
	 *
	 * @tparam r An object of the same type as the value to be returned. This
	 * value is likely to be changed by the operation.
	 *
	 * @tparam obj Object on which to call the method.
	 *
	 * @tparam f method that will be called and run in the thread.
	 * It should not require parameters.
	 *
	 * @tparam p1 First parameter to the function.
	 *
	 * @tparam p2 Second parameter to the function.
	 */
	explicit MethodReturnRunnerP2(ReturnType& r, Object& obj, Callable f, P1 p1, P2 p2):
	returnVal(r), function(f), object(obj), param1(p1), param2(p2)
	{
		//everything initialized
	}


	/**
	 * @brief Does any clean up necessary
	 */
	virtual ~MethodReturnRunnerP2()
	{
		//do nothing, let BaseRunner handle it.
	}


	/**
	 * @brief Runs the operation and store the result.
	 */
	virtual void operator()()
	{
		returnVal = (object.*function)(param1, param2);
	}



	/**
     * @brief Gets the return value from running operator()
     *
     * @return The value returned from the operation.
     */
    ReturnType getReturnValue()
	{
		return returnVal;
	}
};







}


#endif /* RUNNERS_H_ */
