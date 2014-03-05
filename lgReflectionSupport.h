#ifndef CPPREFLECTIONSUPPORT_H_
#define CPPREFLECTIONSUPPORT_H_


#include <cpgf/gmetaclass.h>
#include <cpgf/gmetacommon.h>
#include <cpgf/gpp.h>

#include <map>
#include <string>
#include <list>
#include <exception>

#include "lgReflectedData.h"
#include "lgReflectedClass.h"

//Rhino Javascript for Java https://developer.mozilla.org/en-US/docs/Rhino/Download_Rhino?redirectlocale=en-US&redirectslug=RhinoDownload
//compile metagen - change com to org
//GScopedPointer for pointer manipulation
//doxygen http://www.stack.nl/~dimitri/doxygen/download.html using 1.8.4
//
// place cpgf include files in project path
// include library
// get doxygen and put in metagen folders

//TODO modify cpgf config file to exclude meta directory or this could get ugly

//TODO delete xml, meta directories, refresh eclipse? to get them to compile
//actions
// create doxygen config file - can use a standard one?
//		May have to modify the output directory - assumes that the build is
//		happening in the Debug/Release directory
// create a cpgf config file - use a standard one?
//pre build steps
// create doxygen files
// create cpgf meta files

// TODO prebuild step:
// cd ..; rm -f -r -v ./meta; rm -f -r -v ./xml; doxygen defaultconfig.doxyfile; java -cp "../cpgf/tools/metagen/js.jar" -jar "../cpgf/tools/metagen/metagen.jar" --config metagen_config.js --xml xml/index.xml;cd Debug
// use ant file
//

//
// extend cfpg to store a signature of the datatype??
//	updating Metagen to supply types
//		fields - done
//		methods - done
//		constructors - done
//		properties - ?
//		operators - methods and operators?
//


namespace cpptesting {



class ReflectionException : public std::exception
{
private:
	std::string message;
public:
	ReflectionException() throw();
	ReflectionException(const std::string &msg) throw();
	ReflectionException(const std::exception &e) throw();
	ReflectionException& operator= (const std::exception& e) throw();
	virtual ~ReflectionException() throw();
	virtual const char* what() const throw();
};


//TODO
//methods to ??????
//	create a default config file for doxygen
//  create cpgf config file
//  create cpgf metadata
//


class LookingGlass
{
public:
	friend class ReflectedObject;

	enum MAIN_TYPE {
		NO_MAIN,
		MAIN_NO_PARAM,
		MAIN_ARGS,
		MAIN_ARGS_ENV,
		MAIN_ARGS_ENV_OTHER
	};
private:
	LookingGlass();


	static const ReflectedMethod * getMain(std::string newMainName);
	static const ReflectedMethod * getMain(MAIN_TYPE mainType, std::string newMainName);

	enum INVOKE_RESULT
	{
		INVOKE_SUCCESS,
		INVOKE_BAD_CLASS,
		INVOKE_BAD_METHOD,
		INVOKE_BAD_RETURN,
		INVOKE_NOT_ACCESSIBLE,
	};

#define REF_INVOKE(N, unused) \
		template <typename ReturnType> \
		static INVOKE_RESULT invokeInternal(const ReflectedObject * object, \
				std::string signature, \
				bool useReturn, ReturnType & returnVal GPP_COMMA_IF(N) \
				GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p));

	GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE

//	template <typename ReturnType>
//	static INVOKE_RESULT invokeInternalTest(const ReflectedObject * object,
//				std::string signature,
//				bool useReturn, ReturnType & returnVal);


	// functions
	//TODO for testing add set similar functions method
	//test methods for each of these
	//findClosestFunctions
public:

	/**
	 * Invokes a method with the specified parameters and returns the result
	 *  (through the returnVal parameter).
	 *  The signature for this function is:
	 *  	bool invokeReturn(ReflectedObjectPtr object,
	 *  		string functionSignature, <ReturnType> returnVal,
	 *  		GVariant p0, ...)
	 *  		Most types can be used for GVariant
	 *
	 * @param object Pointer to the object to call the method on
	 * @param methodSignature Method to call (i.e. int foo(double x, char y))
	 * @param returnVal Value returned from the method Undefined if success = false
	 * @param p<1-28> Parameter for the function.  Currently up to 28 parameters are
	 * 		permitted.
	 *
	 * @return True is the method was successfully called.
	 * 		If the method was called but the return value could not be converted
	 * 		to the ReturnType, then this is false.
	 */
#define REF_INVOKE(N, unused) \
		template <typename ReturnType> \
		static bool invokeReturn(ReflectedObjectPtr object, \
				std::string methodSignature, \
				ReturnType & returnVal GPP_COMMA_IF(N) \
				GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p));

	GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE

	/**
	 * Invokes a method with the specified parameters and returns the result
	 *  (through the returnVal parameter).
	 *  The signature for this function is:
	 *  	bool invokeReturn(ReflectedObjectPtr object,
	 *  		string functionSignature, <ReturnType> returnVal,
	 *  		GVariant p0, ...)
	 *  		Most types can be used for GVariant
	 *
	 * @param object Pointer to the object to call the method on
	 * @param methodSignature Method to call (i.e. int foo(double x, char y))
	 * @param p<1-28> Parameter for the function.  Currently up to 28 parameters are
	 * 		permitted.
	 *
	 * @return True is the function was successfully called.
	 */
#define REF_INVOKE(N, unused) \
		static bool invoke(ReflectedObjectPtr object, \
				std::string functionSignature \
				GPP_COMMA_IF(N) \
				GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p));

	GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE




	/**
	 * Invokes a function with the specified parameters and returns the result
	 *  (through the returnVal parameter).
	 *  The signature for this function is:
	 *  	bool invokeFunctionReturn(string functionSignature, <ReturnType> returnVal,
	 *  		GVariant p0, ...)
	 *  		Most types can be used for GVariant
	 *
	 * @param functionSignature Function to call (i.e. int foo(double x, char y))
	 * @param returnVal Value returned from the function Undefined if success = false
	 * @param p<1-28> Parameter for the function.  Currently up to 28 parameters are
	 * 		permitted.
	 *
	 * @return True is the function was successfully called.
	 * 		If the function was called but the return value could not be converted
	 * 		to the ReturnType, then this is false.
	 */
#define REF_INVOKE(N, unused) \
		template <typename ReturnType> \
		static bool invokeReturn(std::string functionSignature, \
				ReturnType & returnVal GPP_COMMA_IF(N) \
				GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p));

	GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE

	/**
	 * Invokes a function with the specified parameters. No value is returned
	 * 	from the function invoked.The signature for this function is:
	 *  	bool invokeFunction(string functionSignature,
	 *  		GVariant p0, ...)
	 *  		Most types can be used for GVariant
	 *
	 * @param functionSignature Function to call (i.e. int foo(double x, char y))
	 * @param p<1-28> Parameter for the function.  Currently up to 28 parameters are
	 * 		permitted.
	 *
	 * @return True is the function was successfully called.
	 * 		If the function was called but the return value could not be converted
	 * 		to the ReturnType, then this is false.
	 */
#define REF_INVOKE(N, unused) \
		static bool invoke(std::string functionSignature \
				GPP_COMMA_IF(N) \
				GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p));

	GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE


	//	/**
	//	 * Invokes a method with the specified parameters and returns the result
	//	 *  (through the returnVal parameter).
	//	 *  The signature for this function is:
	//	 *  	bool invokeFunctionReturn(string functionSignature, <ReturnType> returnVal,
	//	 *  		GVariant p0, ...)
	//	 *  		Most types can be used for GVariant
	//	 *
	//	 * @param functionSignature Function to call (i.e. int foo(double x, char y))
	//	 * @param returnVal Value returned from the function Undefined if success = false
	//	 * @param p<1-28> Parameter for the function.  Currently up to 28 parameters are
	//	 * 		permitted.
	//	 *
	//	 * @return True is the function was successfully called.
	//	 * 		If the function was called but the return value could not be converted
	//	 * 		to the ReturnType, then this is false.
	//	 */
/*	//	#define REF_INVOKE(N, unused) \
	//		template <typename ReturnType> \
	//		static bool invokeMethodReturn(std::string className, \
	//				std::string methodSignature, \
	//				ReturnType & returnVal GPP_COMMA_IF(N) \
	//				GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p));
	//
	//		GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)
	//
	//	#undef REF_INVOKE
	//

*/


	/**
	 * Determines if a main function exists
	 *
	 * @param newMainName The name of the main function after reflection. By
	 * default, that name is "__student_main".
	 *
	 * @returns one of the following MAIN_TYPE values
	 * 		NO_MAIN - no main function found
			MAIN_NO_PARAM - if the function is defined as "int main()"
			MAIN_ARGS - if the function is defined as "int main(int, char * *)"
			MAIN_ARGS_ENV - if the function is defined as
				"int main(int, char * *, char * *)"
			MAIN_ARGS_ENV_OTHER - if the function is defined as
				"int main(int, char * *, char * *, char * *)"
	 */
	static MAIN_TYPE doesMainExist(std::string newMainName);


	/**
	 * Invokes the main function
	 *
	 * @param newMainName The name of the main function after reflection. By
	 * 	default, that name is "__student_main".
	 *
	 * @return The value returned from main.
	 */
	static int invokeMain(std::string newMainName);

	/**
	 * Invokes the main function
	 *
	 * @param newMainName The name of the main function after reflection. By
	 * 	default, that name is "__student_main".
	 * @param argc Argument count to be sent to main
	 * @param argv Arguments to be sent to main
	 *
	 * @return The value returned from main.
	 */
	static int invokeMain(std::string newMainName, int argc, char** argv);

	/**
	 * Invokes the main function
	 *
	 * @param newMainName The name of the main function after reflection. By
	 * 	default, that name is "__student_main".
	 * @param argc Argument count to be sent to main
	 * @param argv Arguments to be sent to main
	 * @param env Environment paths for main
	 *
	 * @return The value returned from main.
	 */
	static int invokeMain(std::string newMainName, int argc, char** argv, char** env);

	/**
	 * Invokes the main function
	 *
	 * @param newMainName The name of the main function after reflection. By
	 * 	default, that name is "__student_main".
	 * @param argc Argument count to be sent to main
	 * @param argv Arguments to be sent to main
	 * @param env Environment paths for main
	 * @param other Other data sent to main (on macs)
	 *
	 * @return The value returned from main.
	 */
	static int invokeMain(std::string newMainName, int argc, char** argv, char** env, char** other);

	/**
	 * Invokes the main function
	 *
	 * @param newMainName The name of the main function after reflection. By
	 * 	default, that name is "__student_main".
	 * @param output Output from the main function as written to cout.
	 *
	 * @return The value returned from main.
	 */
	static int invokeMain(std::string newMainName, std::string &output);

	/**
	 * Invokes the main function
	 *
	 * @param newMainName The name of the main function after reflection. By
	 * 	default, that name is "__student_main".
	 * @param output Output from the main function as written to cout.
	 * @param argc Argument count to be sent to main
	 * @param argv Arguments to be sent to main
	 *
	 * @return The value returned from main.
	 */
	static int invokeMain(std::string newMainName, std::string &output, int argc, char** argv);

	/**
	 * Invokes the main function
	 *
	 * @param newMainName The name of the main function after reflection. By
	 * 	default, that name is "__student_main".
	 * @param output Output from the main function as written to cout.
	 * @param argc Argument count to be sent to main
	 * @param argv Arguments to be sent to main
	 * @param env Environment paths for main
	 *
	 * @return The value returned from main.
	 */
	static int invokeMain(std::string newMainName, std::string &output, int argc, char** argv,
			char** env);

	/**
	 * Invokes the main function
	 *
	 * @param newMainName The name of the main function after reflection. By
	 * 	default, that name is "__student_main".
	 * @param output Output from the main function as written to cout.
	 * @param argc Argument count to be sent to main
	 * @param argv Arguments to be sent to main
	 * @param env Environment paths for main
	 * @param other Other data sent to main (on macs)
	 *
	 * @return The value returned from main.
	 */
	static int invokeMain(std::string newMainName, std::string &output, int argc, char** argv,
			char** env, char** other);

	/**
	 * Invokes the main function
	 *
	 * @param newMainName The name of the main function after reflection. By
	 * 	default, that name is "__student_main".
	 * @param output Output from the main function as written to cout.
	 * @param input Input to the main function as read from cin
	 *
	 * @return The value returned from main.
	 */
	static int invokeMain(std::string newMainName, std::string &output, std::string input);

	/**
	 * Invokes the main function
	 *
	 * @param newMainName The name of the main function after reflection. By
	 * 	default, that name is "__student_main".
	 * @param output Output from the main function as written to cout.
	 * @param input Input to the main function as read from cin
	 * @param argc Argument count to be sent to main
	 * @param argv Arguments to be sent to main
	 *
	 * @return The value returned from main.
	 */
	static int invokeMain(std::string newMainName, std::string &output,  std::string input, int argc, char** argv);

	/**
	 * Invokes the main function
	 *
	 * @param newMainName The name of the main function after reflection. By
	 * 	default, that name is "__student_main".
	 * @param output Output from the main function as written to cout.
	 * @param input Input to the main function as read from cin
	 * @param argc Argument count to be sent to main
	 * @param argv Arguments to be sent to main
	 * @param env Environment paths for main
	 *
	 * @return The value returned from main.
	 */
	static int invokeMain(std::string newMainName, std::string &output,  std::string input, int argc, char** argv,
			char** env);
	/**
	 * Invokes the main function
	 *
	 * @param newMainName The name of the main function after reflection. By
	 * 	default, that name is "__student_main".
	 * @param output Output from the main function as written to cout.
	 * @param input Input to the main function as read from cin
	 * @param argc Argument count to be sent to main
	 * @param argv Arguments to be sent to main
	 * @param env Environment paths for main
	 * @param other Other data sent to main (on macs)
	 *
	 * @return The value returned from main.
	 */
	static int invokeMain(std::string newMainName, std::string &output,  std::string input, int argc, char** argv,
			char** env, char** other);

	/**
	 * Gets a global variable with the specified signature.
	 *
	 * NOTE: This returns a copy of the value and changes to the value will not
	 * change the variable.  To change the variable use setGlobalVariable or:
	 * 	Get a ReflectedData object (ReflectedData::getDataInstance())
	 * 	Get the variable in its cpgf::GMetaField wrapper
	 * 		(ReflectedData's getGlobalVariable)
	 * 	Use the GMetaField's set method (the first parameter should be NULL if
	 * 		this is a global variable,
	 * 		 the second is the new value)
	 * 		i.e. x->set(object, 15); Sets the GMetaField's value to 15
	 *
	 * You can also use this approach to get the value of the variable:
	 * 		After getting the GMetaField:
	 * 		GVariant xValue = x->get(NULL); //The parameter is the instance
	 * 				//to get the value from.  Since this is global, use NULL.
	 * 		//convert the GVariant to an int
	 * 		int xAsInt = cpgf::fromVariant<int>(xValue)
	 *
	 *
	 * @param val value to store the variable in. This is likely a COPY of
	 * 		the global variable.
	 * @param signature Signature for the variable (i.e. int x, string n, etc.)
	 * @param success True if the variable could be accessed. If false, the val
	 * 	 	of val is unknown.
	 */
	template <typename ValueType>
	static bool getGlobalVariable(ValueType& val, std::string signature);


	/**
	 * Sets a global variable with the specified signature.
	 *
	 * @param val value to store in the global variable.
	 * @param signature Signature for the variable (i.e. int x, string n, etc.)
	 * @param success True if the variable could be set.
	 */
	template <typename ValueType>
	static bool setGlobalVariable(const ValueType& val, std::string signature);

//	/**
//		 * Sets a global variable with the specified signature.
//		 *
//		 * @param val value to store in the global variable.
//		 * @param signature Signature for the variable (i.e. int x, string n, etc.)
//		 * @param success True if the variable could be set.
//		 */
//		template <typename ValueType>
//		static bool setGlobalVariable(ValueType val, std::string signature);
//	template <typename ReturnType>
//	static INVOKE_RESULT invokeInternalTest(
//			const ReflectedObject * object, std::string signature,
//			bool useReturn, ReturnType & returnVal, const cpgf::GVariant & p);
};


//template <typename ReturnType>
//ReturnType LookingGlass::invoke(ReturnType &returnVal, std::string name, cpgf::GVariant params...)
//{
//}


#define REF_INVOKE(N, unused) \
		template <typename ReturnType>  \
		bool LookingGlass::invokeReturn(ReflectedObjectPtr object, \
				std::string functionSignature, \
				ReturnType & returnVal GPP_COMMA_IF(N) \
				GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p)) \
				{ \
	return invokeInternal(object, functionSignature, true, returnVal GPP_COMMA_IF(N) \
			GPP_REPEAT(N, GPP_COMMA_PARAM, p)) == INVOKE_SUCCESS; \
				}

GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE


#define REF_INVOKE(N, unused) \
		template <typename ReturnType>  \
		bool LookingGlass::invokeReturn(std::string functionSignature, \
				ReturnType & returnVal GPP_COMMA_IF(N) \
				GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p)) \
				{ \
	return invokeInternal(NULL, functionSignature, true, returnVal GPP_COMMA_IF(N) \
			GPP_REPEAT(N, GPP_COMMA_PARAM, p)) == INVOKE_SUCCESS; \
				}

GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE


//ReflectedObjectPtr object;


/*
#define REF_INVOKE(N, unused) \
template <typename ReturnType>  \
bool LookingGlass::invokeWithReturn(std::string functionSignature, \
	ReturnType & returnVal GPP_COMMA_IF(N) \
	GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p)) \
{ \
	bool success = false; \
	 \
	ReflectedData* data = ReflectedData::getDataInstance(); \
	if (data->doesFunctionExist(functionSignature)) \
	{ \
		const cpgf::GMetaMethod* func = data->getFunction(functionSignature); \
		cpgf::GVariant result = func->invoke(NULL GPP_COMMA_IF(N) \
				GPP_REPEAT(N, GPP_COMMA_PARAM, p));  \
				 \
		if (cpgf::canFromVariant<ReturnType>(result)) \
		{ \
			returnVal = cpgf::fromVariant<ReturnType>(result); \
			success = true; \
		} \
	} \
	 \
	return success; \
}


	GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE

 */



template <typename ValueType>
bool LookingGlass::getGlobalVariable(ValueType& val, std::string signature)
{
	ReflectedData* data = ReflectedData::getDataInstance();
	bool success = false;

	if (data->doesVariableExist(signature))
	{
		const ReflectedField * field = data->getGlobalField(signature);
		if (field->isAccessible()) {
			val = (cpgf::fromVariant<ValueType>(field->getField()->get(NULL)));
			success = true;
		}
	}

	return success;
}

template <typename ValueType>
bool LookingGlass::setGlobalVariable(const ValueType& val, std::string signature)
{
	ReflectedData* data = ReflectedData::getDataInstance();
	bool success = false;

	if (data->doesVariableExist(signature))
	{
		const ReflectedField * field = data->getGlobalField(signature);
		try
		{
			if (field->isAccessible()) {
				field->getField()->set(NULL, val);
				success = true;
			}
		}
		catch (...)
		{
			//failed, probably wrong data type
		}
	}

	return success;
}

//
//template <typename ValueType>
//bool LookingGlass::setGlobalVariable(ValueType val, std::string signature)
//{
//
//	ReflectedData* data = ReflectedData::getDataInstance();
//	bool success = false;
//
//	if (data->doesVariableExist(signature))
//	{
//		const cpgf::GMetaField* field = data->getGlobalVariable(signature);
//		try
//		{
//			field->set(NULL, val);
//			success = true;
//		}
//		catch (...)
//		{
//			//failed, probably wrong data type
//		}
//	}
//
//	return success;
//}


#define REF_INVOKE(N, unused) \
		template <typename ReturnType>  \
		LookingGlass::INVOKE_RESULT LookingGlass::invokeInternal( \
				const ReflectedObject * object, std::string signature, \
				bool useReturn, ReturnType & returnVal GPP_COMMA_IF(N) \
				GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p)) \
				{ \
	LookingGlass::INVOKE_RESULT success = INVOKE_SUCCESS; \
	const ReflectedMethod * func = NULL; \
	bool exists = false; \
	void * objectForCall = NULL; \
	\
	ReflectedData* data = ReflectedData::getDataInstance(); \
	\
	if (object == NULL) /* this is a function, then */ \
		{ \
		if (data->doesFunctionExist(signature)) \
		{ \
			func = data->getFunction(signature); \
			exists = true; \
		} \
		else \
		{ \
			success = INVOKE_BAD_METHOD; \
		} \
		} \
		else \
		{ \
			if (object->getClass() != NULL)  \
			{ \
				const ReflectedClass* c = object->getClass(); \
				if (c->doesMethodExist(signature, All_Access, true)) \
				{ \
					func = c->getMethod(signature); \
					objectForCall = object->getObject(); \
					exists = true; \
				} \
				else \
				{ \
					success = INVOKE_BAD_METHOD; \
				} \
			} \
			else \
			{ \
				success = INVOKE_BAD_CLASS; \
			} \
		} \
		\
		if (exists && func->isAccessible()) \
		{ \
			cpgf::GVariant result = func->getMethod()->invoke(objectForCall GPP_COMMA_IF(N) \
					GPP_REPEAT(N, GPP_COMMA_PARAM, p));  \
					\
					if (useReturn) \
					{ \
						if (cpgf::canFromVariant<ReturnType>(result)) \
						{ \
							returnVal = cpgf::fromVariant<ReturnType>(result); \
						} \
						else \
						{ \
							success = INVOKE_BAD_RETURN; \
						} \
					} \
		} \
		else \
		{ \
			if (func && !func->isAccessible()) \
			{ \
				success = INVOKE_NOT_ACCESSIBLE; \
			} \
		} \
		\
		return success; \
				}


GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE




//template <typename ReturnType>
//LookingGlass::INVOKE_RESULT LookingGlass::invokeInternalTest(
//		const ReflectedObject * object, std::string signature,
//		bool useReturn, ReturnType & returnVal)
//{
//	LookingGlass::INVOKE_RESULT success = INVOKE_SUCCESS;
//	const ReflectedMethod * func = NULL;
//	bool exists = false;
//	void * objectForCall = NULL;
//
//	ReflectedData* data = ReflectedData::getDataInstance();
//
//	if (object == NULL) /* this is a function, then */
//	{
//		if (data->doesFunctionExist(signature))
//		{
//			func = data->getFunction(signature);
//			exists = true;
//		}
//		else
//		{
//			success = INVOKE_BAD_METHOD;
//		}
//	}
//	else
//	{
//		if (object->getClass() != NULL)
//		{
//			const ReflectedClass* c = object->getClass();
//			if (c->doesMethodExist(signature, true))
//			{
//				func = c->getMethod(signature);
//				objectForCall = object->getObject();
//				exists = true;
//			}
//			else
//			{
//				success = INVOKE_BAD_METHOD;
//			}
//		}
//		else
//		{
//			success = INVOKE_BAD_CLASS;
//		}
//	}
//
//	if (exists && func->isAccessible())
//	{
//		cpgf::GVariant result = func->getMethod()->invoke(objectForCall);
//
//		if (useReturn)
//		{
//			if (cpgf::canFromVariant<ReturnType>(result))
//			{
//				returnVal = cpgf::fromVariant<ReturnType>(result);
//			}
//			else
//			{
//				success = INVOKE_BAD_RETURN;
//			}
//		}
//	}
//	else
//	{
//		if (func && !func->isAccessible())
//		{
//			success = INVOKE_NOT_ACCESSIBLE;
//		}
//	}
//
//	return success;
//}
//

//template <typename ReturnType>
//LookingGlass::INVOKE_RESULT LookingGlass::invokeInternalTest(
//		const ReflectedObject * object, std::string signature,
//		bool useReturn, ReturnType & returnVal, const cpgf::GVariant & p)
//{
//	LookingGlass::INVOKE_RESULT success = INVOKE_SUCCESS;
//	const cpgf::GMetaMethod* func = NULL;
//	bool exists = false;
//	void * objectForCall = NULL;
//
//	ReflectedData* data = ReflectedData::getDataInstance();
//
//	if (object == NULL) /* this is a function, then */
//	{
//		if (data->doesFunctionExist(signature))
//		{
//			func = data->getFunction(signature);
//			exists = true;
//		}
//		else
//		{
//			success = INVOKE_BAD_METHOD;
//		}
//	}
//	else
//	{
//		if (object->getClass() != NULL)
//		{
//			const ReflectedClass* c = object->getClass();
//			if (c->doesMethodExist(signature))
//			{
//				func = c->getMethod(signature);
//				objectForCall = object->getObject();
//				exists = true;
//			}
//			else
//			{
//				success = INVOKE_BAD_METHOD;
//			}
//		}
//		else
//		{
//			success = INVOKE_BAD_CLASS;
//		}
//	}
//
//	if (exists)
//	{
//		cpgf::GVariant result = func->invoke(objectForCall, p);
//
//		if (useReturn)
//		{
//			if (cpgf::canFromVariant<ReturnType>(result))
//			{
//				returnVal = cpgf::fromVariant<ReturnType>(result);
//			}
//			else
//			{
//				success = INVOKE_BAD_RETURN;
//			}
//		}
//	}
//
//	return success;
//}



/*
 * #define REF_INVOKE(N, unused) \
template <typename ReturnType>  \
LookingGlass::INVOKE_RESULT LookingGlass::invoke( \
				std::string className, std::string signature, \
				bool useReturn, ReturnType & returnVal GPP_COMMA_IF(N) \
				GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p)) \
{ \
	LookingGlass::INVOKE_RESULT success = INVOKE_SUCCESS; \
	const cpgf::GMetaMethod* func; \
	bool exists = false; \
	\
	ReflectedData* data = ReflectedData::getDataInstance(); \
	\
	if (className == "") \
	{ \
		if (data->doesFunctionExist(signature)) \
		{ \
			func = data->getFunction(signature); \
			exists = true; \
		} \
		else \
		{ \
			success = INVOKE_BAD_METHOD; \
		} \
	} \
	else \
	{ \
		if (data->doesClassExist(className)) \
		{ \
			const ReflectedClass* c = data->getClass(className); \
			if (c->doesMethodExist(signature)) \
			{ \
				func = c->getMethod(signature); \
				exists = true; \
			} \
			else \
			{ \
				success = INVOKE_BAD_METHOD; \
			} \
		} \
		else \
		{ \
			success = INVOKE_BAD_CLASS; \
		} \
	} \
	\
	if (exists) \
	{ \
		cpgf::GVariant result = func->invoke(NULL GPP_COMMA_IF(N) \
				GPP_REPEAT(N, GPP_COMMA_PARAM, p));  \
				\
		if (useReturn) \
		{ \
			if (cpgf::canFromVariant<ReturnType>(result)) \
			{ \
				returnVal = cpgf::fromVariant<ReturnType>(result); \
			} \
			else \
			{ \
				success = INVOKE_BAD_RETURN; \
			} \
		} \
	} \
	\
	return success; \
}


	GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE
 *
 */

}

#endif /*CPPREFLECTIONSUPPORT_H_*/
