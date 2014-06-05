/*
 * ReflectedData.cpp
 *
 *  Created on: Oct 19, 2013
 *      Author: Administrator
 */


#include "lgReflectedData.h"
#include "lgReflectionUtil.h"
#include "lgTestingSupport.h"


namespace cpptesting {

ReflectedData ReflectedData::instance; //singleton


const std::string REMOVE_MODS = "(&)|(\\*)|(const)|(volatile)";
const std::string REMOVE_MODS_REPLACE = "(?1)(?2)(?3)(?4)";
//TODO


//private constructor
ReflectedData::ReflectedData()
{
	loaded = false;
	//loadData();
}

ReflectedData::~ReflectedData()
{
	for (std::map<std::string, ReflectedClass*>::iterator it
				= classes.begin(); it != classes.end(); it++)
	{
		delete it->second;
	}

	for (std::map<std::string, ReflectedNamespace *>::iterator it
			= namespaces.begin(); it != namespaces.end(); it++)
	{
		delete it->second;
	}
}


void ReflectedData::loadData()
{
//	loadOperatorNames();
	cpgf::GMetaClass *global = cpgf::getGlobalMetaClass();
	if (global)
	{
		//all functions should be in the first set of "classes"
		//there shouldn't be anything is this class, but check anyway
		ReflectedDataBase::loadData(global);
		if (global->getClassCount() > 0)
		{
			for (size_t i = 0; i < global->getClassCount(); i++)
			{
				//load global level classes
				ReflectedDataBase::loadData(global->getClassAt(i));
				loadData(global->getClassAt(i));
			}
		}

		//setup base classes for each class
		loadBaseClasses();
	}

}

void ReflectedData::loadData(const cpgf::GMetaClass* gclass)
{

	if (gclass->getClassCount() > 0)
	{
		for (size_t i = 0; i < gclass->getClassCount(); i++)
		{
			//create new ReflectedClass objects with these
			//but don't add the global class
			//if (cpgf::getGlobalMetaClass() != gclass->getClassAt(i)) {
				classes[gclass->getClassAt(i)->getName()]
				        = new ReflectedClass(gclass->getClassAt(i));
			//}
			//add subclasses
			loadData(gclass->getClassAt(i));
		}
	}
}


void ReflectedData::loadBaseClasses() {
	for(std::map<string, ReflectedClass *>::iterator i = classes.begin();
			i != classes.end(); i++) {
		i->second->loadBaseClasses(classes);
	}
}

ReflectedData* ReflectedData::getDataInstance()
{
	if (!instance.loaded)
	{
		instance.loadData();
		instance.loaded = true;
	}
	return &instance;
}




bool ReflectedData::doesFunctionExist(std::string functionSignature) const
{
	return doesMethodExist(functionSignature, All_Access, true);
}


size_t ReflectedData::getFunctionCount() const
{
	return getMethodCount(All_Access, true);
}


const std::vector<std::string> ReflectedData::getFunctionNames() const
{

	return getMethodNames(All_Access, true);
}


const ReflectedMethod * ReflectedData::getFunction(std::string signature) const
{
	return getMethod(signature);
}


std::vector<const ReflectedMethod *> ReflectedData::getFunctions()
{
	return getItems(methods, All_Access, true);
}

std::vector<const ReflectedMethod *> ReflectedData::getClosestFunctions(
		std::string name, int count)
{
	return getClosest(methods, name, All_Access, true, count);
}

std::string ReflectedData::getClosestFunctionsString(
		std::string name, int count)
{
	return getClosestString(methods, name, All_Access, true, count);
}



size_t ReflectedData::getClassCount() const
{
	return classes.size();
}



bool ReflectedData::doesClassExist(std::string name) const
{
	return (classes.count(StringUtil::trim(name)) == 1);
}

const std::vector<std::string> ReflectedData::getClassNames() const
{
	return getNames(classes, All_Access, true);
}

const ReflectedClass* ReflectedData::getClass(std::string signature) const
{
	std::string name = StringUtil::trim(signature);
	if (classes.count(name) == 1)
	{
		return classes.find(name)->second;
		//return classes[name]; //cannot do this because the method is const
	}
	return NULL;
}


/**
 * Gets a class that corresponds to the type or NULL.
 *
 * This ignores modifiers like const or volitile and * and & and returns
 * the class if it is being reflected
 *
 * @return a ReflectedClass object or NULL
 */
const ReflectedClass * ReflectedData::getClassFromType(std::string signature)
	const {
	std::string newSig = signature;

	boost::regex matcher(REMOVE_MODS);
	std::string newType;

	newSig = boost::regex_replace(newSig, matcher, REMOVE_MODS_REPLACE,
			boost::match_default | boost::format_all);

	return getClass(StringUtil::trim(newSig));
}



std::vector<ReflectedClass *> ReflectedData::getClasses()
{
	return getItems(classes, All_Access, true);
}

std::vector<ReflectedClass *> ReflectedData::getClosestClasses(
		std::string name, int count)
{
	return getClosest(classes, name, All_Access, true, count);
}

std::string ReflectedData::getClosestClassesString(
		std::string name, int count)
{
	return getClosestString(classes, name, All_Access, true, count);
}



bool ReflectedData::doesVariableExist(std::string signature) const
{
	return ReflectedDataBase::doesFieldExist(signature, All_Access, true);
}


size_t ReflectedData::getGlobalVariableCount() const
{
	return getVariableCount(All_Access, true);
}

const std::vector<std::string> ReflectedData::getGlobalVariableNames() const
{
	return getVariableNames(All_Access, true);
}

size_t ReflectedData::getGlobalConstantCount() const
{
//	return getGlobalConstantNames().size();
	return getConstantCount(All_Access, true);
}

const std::vector<std::string> ReflectedData::getGlobalConstantNames() const
{
	return getConstantNames(All_Access, true);
}



size_t ReflectedData::getGlobalFieldCount() const
{
	//return globalVars.size();
	return getFieldCount(All_Access, true);
}


const std::vector<std::string> ReflectedData::getGlobalFieldNames() const
{
	//return getNames(globalVars);
	return getFieldNames(All_Access, true);
}



const ReflectedField * ReflectedData::getGlobalField(std::string signature) const
{
	return getField(signature);
}


std::vector<const ReflectedField *> ReflectedData::getGlobalFields()
{
	return getItems(fields, All_Access, true);
}

std::vector<const ReflectedField *> ReflectedData::getClosestGlobalFields(
		std::string name, int count)
{
	return getClosest(fields, name, All_Access, true, count);
}

std::string ReflectedData::getClosestGlobalFieldsString(
		std::string name, int count)
{
	return getClosestString(fields, name, All_Access, true, count);
}




size_t ReflectedData::getNamespaceCount() const
{
	return namespaces.size();
}



bool ReflectedData::doesNamespaceExist(std::string name) const
{
	return (namespaces.count(StringUtil::trim(name)) == 1);
}

const std::vector<std::string> ReflectedData::getNamespaceNames() const
{
	return getNames(namespaces, All_Access, true);
}

ReflectedNamespace * ReflectedData::getNamespace(std::string name) const
{
	std::string tname = StringUtil::trim(name);
	if (namespaces.count(tname) == 1)
	{
		return namespaces.find(tname)->second;
	}
	return NULL;
}


std::vector<ReflectedNamespace *> ReflectedData::getNamespaces()
{
	return getItems(namespaces, All_Access, true);
}

std::vector<ReflectedNamespace *> ReflectedData::getClosestNamespaces(
		std::string name, int count)
{
	return getClosest(namespaces, name, All_Access, true, count);
}

std::string ReflectedData::getClosestNamespacesString(
		std::string name, int count)
{
	return getClosestString(namespaces, name, All_Access, true, count);
}

}
