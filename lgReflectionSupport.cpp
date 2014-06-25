/*
 * CppReflectionSupport.cpp
 *
 *  Created on: Jul 10, 2013
 *      Author: Administrator
 */

#include "lgReflectionSupport.h"
#include "lgReflectedData.h"
#include "lgTestingSupport.h"

#include <iostream>
#include <typeinfo>

#include <cpgf/gmetacommon.h>

namespace cpptesting {


//std::map<std::string, const cpgf::GMetaMethod*> GlobalChecks::allFunctions;
//std::map<std::string, const cpgf::GMetaField*> GlobalChecks::allFields;
//std::map<std::string, const cpgf::GMetaOperator*> GlobalChecks::allOperators;
//std::map<std::string, const cpgf::GMetaConstructor*> GlobalChecks::allConstructors;
//std::map<std::string, std::string> GlobalChecks::operatorSymToName;
//std::map<std::string, std::string> GlobalChecks::operatorNameToSym;
//bool GlobalChecks::loaded = false;

LookingGlass::LookingGlass()
{
	//do nothing
}


const std::string MAIN_RETURN_STR = "int ";
const std::string MAIN_NO_PARAM_STR = "()";
const std::string MAIN_ARGS_STR = "(int, char * *)";
const std::string MAIN_ARGS_ENV_STR = "(int, char * *, char * *)";
const std::string MAIN_ARGS_ENV_OTHER_STR = "(int, char * *, char * *, char * *)";


//void GlobalChecks::invoke(std::string name, cpgf::GVariant params...)
//{
//}

const ReflectedMethod * LookingGlass::getMain(std::string newMainName)
{
	ReflectedData* data = ReflectedData::getDataInstance();

	if (data->doesFunctionExist(MAIN_RETURN_STR + newMainName + MAIN_NO_PARAM_STR))
	{
		return data->getFunction(MAIN_RETURN_STR + newMainName + MAIN_NO_PARAM_STR);
	}

	if (data->doesFunctionExist(MAIN_RETURN_STR + newMainName + MAIN_ARGS_STR))
	{
		return data->getFunction(MAIN_RETURN_STR + newMainName + MAIN_ARGS_STR);
	}

	if (data->doesFunctionExist(MAIN_RETURN_STR + newMainName + MAIN_ARGS_ENV_STR))
	{
		return data->getFunction(MAIN_RETURN_STR + newMainName + MAIN_ARGS_ENV_STR);
	}

	if (data->doesFunctionExist(MAIN_RETURN_STR + newMainName + MAIN_ARGS_ENV_OTHER_STR))
	{
		return data->getFunction(MAIN_RETURN_STR + newMainName + MAIN_ARGS_ENV_OTHER_STR);
	}

	return NULL;
}


const ReflectedMethod * LookingGlass::getMain(LookingGlass::MAIN_TYPE mainType, std::string newMainName)
{
	ReflectedData* data = ReflectedData::getDataInstance();

	if (mainType == MAIN_NO_PARAM)
	{
		return data->getFunction(MAIN_RETURN_STR + newMainName + MAIN_NO_PARAM_STR);
	}

	if (mainType == MAIN_ARGS)
	{
		return data->getFunction(MAIN_RETURN_STR + newMainName + MAIN_ARGS_STR);
	}

	if (mainType == MAIN_ARGS_ENV)
	{
		return data->getFunction(MAIN_RETURN_STR + newMainName + MAIN_ARGS_ENV_STR);
	}

	if (mainType == MAIN_ARGS_ENV_OTHER)
	{
		return data->getFunction(MAIN_RETURN_STR + newMainName + MAIN_ARGS_ENV_OTHER_STR);
	}

	return NULL;
}

LookingGlass::MAIN_TYPE LookingGlass::doesMainExist(std::string newMainName)
{
	ReflectedData* data = ReflectedData::getDataInstance();

	if (data->doesFunctionExist(MAIN_RETURN_STR + newMainName + MAIN_NO_PARAM_STR))
	{
		return MAIN_NO_PARAM;
	}

	if (data->doesFunctionExist(MAIN_RETURN_STR + newMainName + MAIN_ARGS_STR))
	{
		return MAIN_ARGS;
	}

	if (data->doesFunctionExist(MAIN_RETURN_STR + newMainName + MAIN_ARGS_ENV_STR))
	{
		return MAIN_ARGS_ENV;
	}

	if (data->doesFunctionExist(MAIN_RETURN_STR + newMainName + MAIN_ARGS_ENV_OTHER_STR))
	{
		return MAIN_ARGS_ENV_OTHER;
	}

	return NO_MAIN;
}

int LookingGlass::invokeMain(std::string newMainName)
{
	std::string output;
	return invokeMain(newMainName, output, "", 0, NULL, NULL, NULL);
}

int LookingGlass::invokeMain(std::string newMainName, int argc, char** argv)
{
	std::string output;
	return invokeMain(newMainName, output, "", argc, argv, NULL, NULL);
}

int LookingGlass::invokeMain(std::string newMainName, int argc, char** argv, char** env)
{
	std::string output;
	return invokeMain(newMainName, output, "", argc, argv, env, NULL);
}

int LookingGlass::invokeMain(std::string newMainName, int argc, char** argv, char** env, char** other)
{
	std::string output;
	return invokeMain(newMainName, output, "", argc, argv, env, other);
}

int LookingGlass::invokeMain(std::string newMainName, std::string &output)
{
	return invokeMain(newMainName, output, "", 0, NULL, NULL, NULL);
}

int LookingGlass::invokeMain(std::string newMainName, std::string &output, int argc, char** argv)
{
	return invokeMain(newMainName, output, "", argc, argv, NULL, NULL);
}

int LookingGlass::invokeMain(std::string newMainName, std::string &output, int argc, char** argv,
		char** env)
{
	return invokeMain(newMainName, output, "", argc, argv, env, NULL);
}


int LookingGlass::invokeMain(std::string newMainName, std::string &output, int argc, char** argv,
		char** env, char** other)
{
	return invokeMain(newMainName, output, "", argc, argv, env, other);
}

int LookingGlass::invokeMain(std::string newMainName, std::string &output, std::string input)
{
	return invokeMain(newMainName, output, input, 0, NULL, NULL, NULL);
}

int LookingGlass::invokeMain(std::string newMainName, std::string &output, std::string input, int argc, char** argv)
{
	return invokeMain(newMainName, output, input, argc, argv, NULL, NULL);
}

int LookingGlass::invokeMain(std::string newMainName, std::string &output,  std::string input, int argc, char** argv,
		char** env)
{
	return invokeMain(newMainName, output, input, argc, argv, NULL, NULL);
}


int LookingGlass::invokeMain(std::string newMainName, std::string &output,
		std::string input, int argc, char** argv,
		char** env, char** other)
{
	StdIORedirector testHelp;

	MAIN_TYPE type = doesMainExist(newMainName);
	int returnVal = -1;
	cpgf::GVariant result;

	testHelp.redirectCout();
	testHelp.redirectCin();

	testHelp.resetOutput();
	testHelp.setInput(input);

	const cpgf::GMetaMethod* main = NULL;
	const ReflectedMethod * rMain = getMain(type, newMainName);

	if (rMain) {
		main = rMain->getMethod();
	}

	if (type == MAIN_NO_PARAM)
	{
		result = main->invoke(NULL);
	}

	if (type == MAIN_ARGS)
	{
		result = main->invoke(NULL, argc, argv);
	}

	if (type == MAIN_ARGS_ENV)
	{
		result = main->invoke(NULL, argc, argv, env);
	}

	if (type == MAIN_ARGS_ENV_OTHER)
	{
		result = main->invoke(NULL, argc, argv, env, other);
	}


	output = testHelp.getOutput();

	testHelp.restoreCout();
	testHelp.restoreCin();

	if (type == NO_MAIN)
	{
		throw ReflectionException("When trying to call main, no main "
				"method was found. Using " + newMainName + " as the name"
				" for main after reflection.");
	}

	returnVal = cpgf::fromVariant<int>(result);

	return returnVal;
}




#define REF_INVOKE(N, unused) \
bool LookingGlass::invoke(MethodSignature functionSignature \
	GPP_COMMA_IF(N) \
	GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p)) \
{ \
	int r; \
	return invokeInternal(NULL, functionSignature, false, r, false \
			GPP_COMMA_IF(N) \
			GPP_REPEAT(N, GPP_COMMA_PARAM, p)) == INVOKE_SUCCESS; \
}

	GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE

//ReflectedObjectPtr object;

#define REF_INVOKE(N, unused) \
bool LookingGlass::invoke(ReflectedObjectPtr object, \
		MethodSignature functionSignature \
	GPP_COMMA_IF(N) \
	GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p)) \
{ \
	int r; \
	return invokeInternal(object.get(), functionSignature, false, r, false \
			GPP_COMMA_IF(N) \
			GPP_REPEAT(N, GPP_COMMA_PARAM, p)) == INVOKE_SUCCESS; \
}

	GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE



/*
#define REF_INVOKE(N, unused) \
bool LookingGlass::invokeFunction(std::string functionSignature \
	GPP_COMMA_IF(N) \
	GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p)) \
{ \
	bool success = false; \
	 \
	ReflectedData* data = ReflectedData::getDataInstance(); \
	if (data->doesFunctionExist(functionSignature)) \
	{ \
		const cpgf::GMetaMethod* func = data->getFunction(functionSignature); \
		func->invoke(NULL GPP_COMMA_IF(N) \
				GPP_REPEAT(N, GPP_COMMA_PARAM, p));  \
				 \
		success = true; \
	} \
	 \
	return success; \
}

	GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE
*/


	const ReflectedMethod * LookingGlass::getFunctionObject(
			const ReflectedObject * object,
			MethodSignature nSignature, bool allowNonReflected) {

		ReflectedData* data = ReflectedData::getDataInstance();
		//std::string signature = nSignature;
		const ReflectedMethod * func;

		if (object == NULL) {
			if (data->doesFunctionExist(nSignature)) {
				func = data->getFunction(nSignature);
			}
			else {
				throw INVOKE_BAD_METHOD;
			}
		}
		else
		{
			if (object->getClass() != NULL)
			{
				const ReflectedClass* c = object->getClass();
				ItemFilter filter(All_Access, true); //all visibilities and inherited methods
				if (c->doesMethodExist(nSignature, filter))
				{
					func = c->getMethod(nSignature, filter);
				}
				else
				{
					if (allowNonReflected && c->doesNonReflectedMethodExist(
							nSignature, filter)) {
						func = c->getNonReflectedMethod(nSignature);
					}
					else {
						throw INVOKE_BAD_METHOD;
					}
				}
			}
			else
			{
				throw INVOKE_BAD_CLASS;
			}
		}

		return func;

	}


	void * LookingGlass::getObjectForCall(const ReflectedObject * object) {
		if (object != NULL) {
			return object->getObject();
		}

		return NULL;
	}


}


