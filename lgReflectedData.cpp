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

//	for (std::map<std::string, ReflectedNamespace *>::iterator it
//			= namespaces.begin(); it != namespaces.end(); it++)
//	{
//		delete it->second;
//	}
}


void ReflectedData::loadData()
{
//	loadOperatorNames();
	cpgf::GMetaClass *global = cpgf::getGlobalMetaClass();
	if (global)
	{
		//all functions should be in the first set of "classes"
		//there shouldn't be anything is this class, but check anyway
		ReflectedDataBase::loadData(global, true);
		if (global->getClassCount() > 0)
		{
			for (size_t i = 0; i < global->getClassCount(); i++)
			{
				//load global level classes
//				CPGF_TRACE("Adding Class Data")
				ReflectedDataBase::loadData(global->getClassAt(i), true);
				loadData(global->getClassAt(i), true);
			}
		}
//		CPGF_TRACE("Adding Bases Data")
		//setup base classes for each class
		loadBaseClasses();
//		CPGF_TRACE("Done Data")
	}

	for(std::map<string, ReflectedClass *>::iterator i = classes.begin();
				i != classes.end(); i++) {
	//		CPGF_TRACE("Loading Base Data" << i->first << " " << i->second << "\r\n")
			i->second->loadNonReflectedBaseClassMembers(classes);
		}
}

void ReflectedData::loadData(const cpgf::GMetaClass* gclass, bool isGlobal)
{
	if (gclass->getClassCount() > 0)
	{
		for (size_t i = 0; i < gclass->getClassCount(); i++)
		{
//			CPGF_TRACE("Loading " << i << " " << gclass->getClassAt(i)->getName() << "\r\n")
			//create new ReflectedClass objects with these
			//but don't add the global class
			//if (cpgf::getGlobalMetaClass() != gclass->getClassAt(i)) {
			classes[gclass->getClassAt(i)->getName()]
			        = new ReflectedClass(gclass->getClassAt(i), isGlobal);
//			CPGF_TRACE("Namespace " << i << " " << gclass->getClassAt(i)->getName() << "\r\n")

			ReflectedNamespace::addItem(gclass->getClassAt(i)->getNamespace(),
					classes[gclass->getClassAt(i)->getName()]);
			//}
			//add subclasses
//			CPGF_TRACE("Loading " << i << " " << gclass->getClassAt(i)->getName() << " subclasses" << "\r\n")

		//	loadData(gclass->getClassAt(i), false);
//			CPGF_TRACE("Loading " << i << " " << gclass->getClassAt(i)->getName() << " done" << "\r\n")

		}
	}

}


void ReflectedData::loadBaseClasses() {
//	CPGF_TRACE("Loading Base Data" << "\r\n")
	for(std::map<string, ReflectedClass *>::iterator i = classes.begin();
			i != classes.end(); i++) {
//		CPGF_TRACE("Loading Base Data" << i->first << " " << i->second << "\r\n")
		i->second->loadBaseClasses(classes);
	}
//	CPGF_TRACE("Loading Base Data done" << "\r\n")
}




/**
 * Loads a single non-reflected item like private methods, non-reflected
 * base classes, etc.
 *
 * @param nrItem item to load
 */
void ReflectedData::loadNonReflectedData(
		const cpgf::GMetaNonReflectedItem * nrItem) {

	if (nrItem->getItemCategory() == cpgf::mcatClass) {
		std::string signature = nrItem->getSignature();
		VisibilityType vis = ReflectedItem::cpgfToLGVisibility(
					nrItem->getVisibility());

		classes[signature]
		        = new ReflectedClass(signature, false, vis,
		        		nrItem->getModifiers());
		ReflectedNamespace::addItem(nrItem->getNamespace(),
				classes[signature]);
	}
	else {
		ReflectedDataBase::loadNonReflectedData(nrItem);
	}
}


/**
 * Searches through all classes and inner classes to find a class.
 * Assumes the signature is in the form <outer>::<inner>::<inner inner>...
 *
 * @param signature Signature of the class
 * @return The reflected class or NULL if it is not found
 */
ReflectedClass * ReflectedData::findClass(ClassSignature signature) const {
	std::string outerName = "";
	std::string restName = signature;
	std::string innerName = "";
	std::string fullName;
	ReflectedClass * item = NULL;

	size_t pos = restName.find("::");
	if (pos != std::string::npos) {
		//has an inner class
		outerName = restName.substr(0, pos);
		restName = restName.substr(pos + 2, std::string::npos);
	}
	else {
		outerName = restName;
		restName = "";
	}

	fullName = outerName;
	item = classes.find(outerName)->second;

	while (item != NULL && restName != "") {
		pos = restName.find("::");
		if (pos != std::string::npos) {
			innerName = restName.substr(0, pos);
			restName = restName.substr(pos + 2, std::string::npos);
		}
		else {
			innerName = restName;
			restName = "";
		}
		fullName += "::" + innerName;
		item = item->getInnerClass(fullName);
	}

	return item;
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




bool ReflectedData::doesFunctionExist(MethodSignature functionSignature,
		ItemFilter filter) const
{
	return doesMethodExist(functionSignature, filter);
}


size_t ReflectedData::getFunctionCount(ItemFilter filter) const
{
	return getMethodCount(filter);
}


const std::vector<std::string> ReflectedData::getFunctionNames(
		ItemFilter filter) const
{

	return getMethodNames(filter);
}


const ReflectedMethod * ReflectedData::getFunction(MethodSignature signature,
		ItemFilter filter) const
{
	return getMethod(signature, filter);
}


std::vector<const ReflectedMethod *> ReflectedData::getFunctions(ItemFilter filter)
{
	return getItems(methods, filter);
}

std::vector<const ReflectedMethod *> ReflectedData::getClosestFunctions(
		MethodSignature name, ItemFilter filter, int count)
{
	return getClosest(methods, name,filter, count);
}

std::string ReflectedData::getClosestFunctionsString(
		MethodSignature name, ItemFilter filter, int count)
{
	return getClosestString(methods, name, filter, count);
}



size_t ReflectedData::getClassCount(ItemFilter filter) const
{
	return classes.size();
}



bool ReflectedData::doesClassExist(ClassSignature name, ItemFilter filter) const
{
	return getClass(name, filter) != NULL;
	//return (classes.count(StringUtil::trim(name)) == 1);
}

const std::vector<std::string> ReflectedData::getClassNames(
		ItemFilter filter) const
{
	return getNames(classes, filter);
}

const ReflectedClass* ReflectedData::getClass(ClassSignature signature,
		ItemFilter filter) const
{
	std::string name = StringUtil::trim(signature);
	const ReflectedClass * item = findClass(name);

//	if (classes.count(name) == 1)
	if (item != NULL)
	{
//		if (modifiers == ALLOW_ALL_MODIFIERS || modifiers == item->getModifiers()
//				|| (allowMoreMods && (modifiers & item->getModifiers()) != 0)) {
		if (filter.isAllowed(item, signature.getModifiers())) {
			return item;
		}
		//return classes[name]; //cannot do this because the method is const
	}
	return NULL;
}


/**
 * Gets a class that corresponds to the type or NULL.
 *
 * This ignores modifiers like const or volatile and * and & and returns
 * the class if it is being reflected
 *
 * @return a ReflectedClass object or NULL
 */
const ReflectedClass * ReflectedData::getClassFromType(TypeSignature signature)
	const {
	std::string newSig = signature;

	boost::regex matcher(REMOVE_MODS);
	std::string newType;
//TODO handle inner classes??
	newSig = boost::regex_replace(newSig, matcher, REMOVE_MODS_REPLACE,
			boost::match_default | boost::format_all);

	return getClass(StringUtil::trim(newSig), ItemFilter());
}



std::vector<ReflectedClass *> ReflectedData::getClasses(ItemFilter filter)
{
	return getItems(classes, filter);
}

std::vector<ReflectedClass *> ReflectedData::getClosestClasses(
		ClassSignature name, ItemFilter filter, int count)
{
	return getClosest(classes, name, filter, count);
}

std::string ReflectedData::getClosestClassesString(
		ClassSignature name, ItemFilter filter, int count)
{
	return getClosestString(classes, name, filter, count);
}



bool ReflectedData::doesVariableExist(FieldSignature signature,
		ItemFilter filter) const
{
	return ReflectedDataBase::doesFieldExist(signature, filter);
}


size_t ReflectedData::getGlobalVariableCount(ItemFilter filter) const
{
	return getVariableCount(filter);
}

const std::vector<std::string> ReflectedData::getGlobalVariableNames(
		ItemFilter filter) const
{
	return getVariableNames(filter);
}

size_t ReflectedData::getGlobalConstantCount(ItemFilter filter) const
{
//	return getGlobalConstantNames().size();
	return getConstantCount(filter);
}

const std::vector<std::string> ReflectedData::getGlobalConstantNames(
		ItemFilter filter) const
{
	return getConstantNames(filter);
}



size_t ReflectedData::getGlobalFieldCount(ItemFilter filter) const
{
	//return globalVars.size();
	return getFieldCount(filter);
}


const std::vector<std::string> ReflectedData::getGlobalFieldNames(
		ItemFilter filter) const
{
	//return getNames(globalVars);
	return getFieldNames(filter);
}



const ReflectedField * ReflectedData::getGlobalField(FieldSignature signature,
		ItemFilter filter) const
{
	return getField(signature, filter);
}


std::vector<const ReflectedField *> ReflectedData::getGlobalFields(
		ItemFilter filter)
{
	return getItems(fields, filter);
}

std::vector<const ReflectedField *> ReflectedData::getClosestGlobalFields(
		FieldSignature name, ItemFilter filter, int count)
{
	return getClosest(fields, name, filter, count);
}

std::string ReflectedData::getClosestGlobalFieldsString(
		FieldSignature name, ItemFilter filter, int count)
{
	return getClosestString(fields, name, filter, count);
}




size_t ReflectedData::getNamespaceCount() const
{
	return ReflectedNamespace::getNamespaceCount();
}



bool ReflectedData::doesNamespaceExist(std::string name) const
{
	return (ReflectedNamespace::getNamespace(StringUtil::trim(name)) != NULL);
}

const std::vector<std::string> ReflectedData::getNamespaceNames() const
{
	std::vector<std::string> names;
	std::vector<ReflectedNamespacePtr> ns
		= ReflectedNamespace::getAllNamespaces();

	for (size_t i = 0; i < ns.size(); i++) {
		names.push_back(ns[i]->getFullName());
	}

	return names;
}

ReflectedNamespacePtr ReflectedData::getNamespace(std::string name) const
{
	return ReflectedNamespace::getNamespace(StringUtil::trim(name));
}


std::vector<ReflectedNamespacePtr> ReflectedData::getNamespaces()
{
	return ReflectedNamespace::getAllNamespaces();
}

std::vector<ReflectedNamespacePtr> ReflectedData::getClosestNamespaces(
		std::string name, int count)
{
	std::vector<std::string> names = StringDistance::getNClosest(name,
			getNamespaceNames(), count);

	std::vector<ReflectedNamespacePtr> ns;

	for (size_t i = 0; i < names.size(); i++) {
		ns.push_back(getNamespace(names[i]));
	}

	return ns;
}

std::string ReflectedData::getClosestNamespacesString(
		std::string name, int count)
{
	return StringDistance::getNClosestAsString(name,
				getNamespaceNames(), count);
}





}
