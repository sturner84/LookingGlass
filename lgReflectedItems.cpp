/*
 * lgReflectedEnum.cpp
 *
 *  Created on: Feb 12, 2014
 *      Author: Administrator
 */

#include "lgReflectedItems.h"
#include <climits>
#include <sstream>
#include "lgTestingSupport.h"
#include "lgReflectionUtil.h"

namespace cpptesting {



ReflectedItem::ReflectedItem(std::string nName, std::string sig, bool inherit,
		bool isGlobal, VisibilityType vis, ReflectedItemType iType,
		int modifiers)
{
	name = nName;
	signature = sig;
	inherited = inherit;
	global = isGlobal;
	visibility = vis;
	type = iType;
	mods = modifiers;
}

ReflectedItem::~ReflectedItem() {
	//do nothing
}

std::string ReflectedItem::getName() const
{
	return name;
}


/**
 * Gets the signature of the item
 *
 * @return signature of the item
 */
std::string ReflectedItem::getSignature() const {
	return signature;
}

bool ReflectedItem::isInherited() const
{
	return inherited;
}

VisibilityType ReflectedItem::getVisibility() const
{
	return visibility;
}

ReflectedItemType ReflectedItem::getType() const
{
	return type;
}


/**
 * Gets the modifiers for this item
 * @return modifiers for this item
 */
int ReflectedItem::getModifiers() const {
	return mods;
}


/**
 * Converts cpgf visibility enumeration to LookingGlass's VisibilityType
 * enumeration
 *
 * @param vis cpgf visibility enumeration
 * @return Corresponding VisibilityType
 */
VisibilityType ReflectedItem::cpgfToLGVisibility(
		cpgf::GMetaVisibility vis) {
	VisibilityType newVis;

	switch (vis) {
	case cpgf::mPrivate:
		newVis = Private;
		break;
	case cpgf::mProtected:
		newVis = Protected;
		break;
	case cpgf::mPublic:
		newVis = Public;
		break;
	default:
		newVis = Public;
	}

	return newVis;
}


void ReflectedItem::addModifier(int & mods, std::string & currentString,
			int modToCheck, std::string modStr) {
	if ((mods & modToCheck) != 0) {
		if (currentString != "") {
			currentString += ", ";
		}
		currentString += modStr;
		mods &= ~modToCheck;
	}
}

std::string  ReflectedItem::modifiersToString(int modifiers) {
	std::string modSet = "";

	//if it happens to be set, get rid of it.
	modifiers &= ~cpgf::metaModifierNoFree;

	addModifier(modifiers, modSet, cpgf::metaModifierStatic, "Static");
	addModifier(modifiers, modSet, cpgf::metaModifierVirtual, "Virtual");
	addModifier(modifiers, modSet, cpgf::metaModifierPureVirtual, "Pure Virtual");
	addModifier(modifiers, modSet, cpgf::metaModifierTemplate, "Template");
	addModifier(modifiers, modSet, cpgf::metaModifierConst, "Const");
	addModifier(modifiers, modSet, cpgf::metaModifierVolatile, "Volatile");
	addModifier(modifiers, modSet, cpgf::metaModifierInline, "Inline");
	addModifier(modifiers, modSet, cpgf::metaModifierExplicit, "Explicit");
	addModifier(modifiers, modSet, cpgf::metaModifierExtern, "Extern");
	addModifier(modifiers, modSet, cpgf::metaModifierMutable, "Mutable");

	if (modifiers != 0) {
		if (modSet != "") {
			modSet += ", ";
		}
		modSet += "Other";
	}

	modSet = "{" + modSet + "}";

	return modSet;
}



/**
 * Determines if this item is at the global level. This includes classes,
 * functions, global enums, and global variables
 *
 * @return true if the item is global
 */
bool ReflectedItem::isGlobal() {
	return global;
}


ReflectedMethod::ReflectedMethod(const cpgf::GMetaMethod * m,
		bool inherit, bool isGlobal) :
		ReflectedItem(m->getName(), "", inherit, isGlobal,
				Public, Method, m->getModifiers())
{
	method = m;
	if (isGlobal) {
			//functions are marked static, which technically they are,
			//but they aren't declared static so this clear it
			mods &= ~cpgf::metaModifierStatic;
		}
	signature = ReflectionUtil::createFunctionSignature(method);
}


ReflectedMethod::ReflectedMethod(std::string sig, bool inherit, bool isGlobal,
		VisibilityType vis, int modifiers)
	: ReflectedItem(sig, sig, inherit, isGlobal, vis, Method, modifiers)
{
	FunctionInfo info = ReflectionUtil::divideFunctionSignature(sig);
	name = info.name;
	method = NULL;
}


const cpgf::GMetaMethod * ReflectedMethod::getMethod() const
{
	return method;
}

bool ReflectedMethod::isAccessible() const
{
	return method;
}




ReflectedConstructor::ReflectedConstructor(const cpgf::GMetaConstructor * c,
		bool inherit) : ReflectedItem(c->getName(), "", inherit,
				false, Public, Constructor, c->getModifiers())
{
	constructor = c;
	signature = ReflectionUtil::createConstructorSignature(
			static_cast<const cpgf::GMetaClass *>(c->getOwnerItem()), c);
}


ReflectedConstructor::ReflectedConstructor(std::string sig, bool inherit,
		VisibilityType vis, int modifiers)
	: ReflectedItem(sig, sig, inherit, false, vis, Constructor, modifiers)
{
	FunctionInfo info = ReflectionUtil::divideFunctionSignature(sig);
	name = info.name;
	constructor = NULL;
}


const cpgf::GMetaConstructor * ReflectedConstructor::getConstructor() const
{
	return constructor;
}

bool ReflectedConstructor::isAccessible() const
{
	return constructor;
}



ReflectedField::ReflectedField(const cpgf::GMetaField * f, bool inherit,
		bool isGlobal) :
		ReflectedItem(f->getName(), "", inherit, isGlobal,
				Public, Field, f->getModifiers())

{
	field = f;
	if (isGlobal) {
		//global variable are marked static, which technically they are,
		//but they aren't declared static so this clear it

		mods &= ~cpgf::metaModifierStatic;
	}
	signature = ReflectionUtil::createFieldSignature(field);
}


ReflectedField::ReflectedField(std::string sig, bool inherit,
		bool isGlobal, VisibilityType vis, int modifiers)
	: ReflectedItem(sig, "", inherit, isGlobal, vis, Field, modifiers)
{
	field = NULL;
	FieldInfo info = ReflectionUtil::divideFieldSignature(sig);
	name = info.name;
}


const cpgf::GMetaField * ReflectedField::getField() const
{
	return field;
}

bool ReflectedField::isAccessible() const
{
	return field;
}

//ReflectedOperator::ReflectedOperator(const cpgf::GMetaOperator * o,
//		bool inherit, bool isGlobal) : ReflectedItem(o->getName(), inherit,
//				isGlobal, Public, Operator, o->getModifiers())
//{
//	op = o;
//	signature = ReflectionUtil::createFunctionSignature(op);
//}
//
//
//ReflectedOperator::ReflectedOperator(std::string sig, bool inherit,
//		bool isGlobal, VisibilityType vis, int modifiers)
//	: ReflectedItem(sig, sig, inherit, isGlobal, vis, Operator, modifiers)
//{
//	FunctionInfo info = ReflectionUtil::divideFunctionSignature(sig);
//	name = info.name;
//	op = NULL;
//}
//
//const cpgf::GMetaOperator * ReflectedOperator::getOperator() const
//{
//	return op;
//}
//
//bool ReflectedOperator::isAccessible() const
//{
//	return op;
//}




void ReflectedEnum::loadData(const cpgf::GMetaEnum* genum)
{
	for (size_t i = 0; i < genum->getCount(); i++) {
		enumByNames[genum->getKey(i)]
		            = cpgf::fromVariant<int>(genum->getValue(i));
		enumByValues[cpgf::fromVariant<int>(genum->getValue(i))]
		             = genum->getKey(i);
	}

}





ReflectedEnum::ReflectedEnum(const cpgf::GMetaEnum* e, bool inherit,
		bool isGlobal) :
		//enums are marked static, which technically they are, but they aren't declared static
		//so this clear it
		ReflectedItem(e->getName(), "", inherit, isGlobal, Public, Enum,
				e->getModifiers() & ~cpgf::metaModifierStatic)
{
	mEnum = e;
	loadData(e);
	//TODO get signature
}

ReflectedEnum::ReflectedEnum(std::string sig, bool inherit,
		bool isGlobal, VisibilityType vis, int modifiers)
	: ReflectedItem(sig, sig, inherit, isGlobal, vis, Enum, modifiers)
{
	mEnum = NULL;
	EnumInfo info = ReflectionUtil::divideEnumSignature(sig);

	name = info.name;
	enumByNames = info.names;
	enumByValues = info.values;
//	name = ReflectionUtil::getEnumName(nName);
//	enumByNames = ReflectionUtil::getEnumValues(nName);
//
//	for (std::map<std::string, int>::iterator i = enumByNames.begin();
//			i != enumByNames.end(); i++) {
//		enumByValues[i->second] = i->first;
//	}

}


ReflectedEnum::~ReflectedEnum()
{
	//do nothing
}


const cpgf::GMetaEnum * ReflectedEnum::getEnum() const
{
	return mEnum;
}


size_t ReflectedEnum::getNumValues() const
{
//	if (mEnum)
//	{
//		return mEnum->getCount();
//	}

	return enumByNames.size();
}


std::string ReflectedEnum::getName(size_t index) const
{
	if (!mEnum || index >= mEnum->getCount()) {
		return "";
	}

	return mEnum->getKey(index);
}


int ReflectedEnum::getValue(size_t index) const
{
	if (!mEnum || index >= mEnum->getCount()) {
		return INT_MIN;
	}

	return cpgf::fromVariant<int>(mEnum->getValue(index));
}



std::vector<std::string> ReflectedEnum::getNames() const
{
	std::vector<std::string> names;

	for (typename std::map<std::string, int>::const_iterator it = enumByNames.begin();
			it != enumByNames.end(); it++)
	{
		names.push_back(it->first);
	}

	return names;
}


std::vector<int> ReflectedEnum::getValues() const
{
	std::vector<int> values;

	for (typename std::map<int, std::string>::const_iterator it = enumByValues.begin();
			it != enumByValues.end(); it++)
	{
		values.push_back(it->first);
	}

	return values;
}


int ReflectedEnum::getValueForName(std::string name) const
{
	if (enumByNames.count(name) == 1) {
		std::map<std::string, int>::const_iterator it = enumByNames.find(name);
		return it->second;
	}

	return INT_MIN;
}


std::string ReflectedEnum::getNameForValue(int value) const
{
	if (enumByValues.count(value) == 1) {
		std::map<int, std::string>::const_iterator it = enumByValues.find(value);
		return it->second;
	}

	return "";
}

bool ReflectedEnum::isAccessible() const
{
	return mEnum;
}




/**
 * Global namespace
 */
ReflectedNamespacePtr ReflectedNamespace::global = ReflectedNamespacePtr(
		new ReflectedNamespace(""));


/**
 * @brief Divides a namespace string into a list of namespaces from most
 * general to most specific.
 *
 * So cpptesting::inner::test would become:
 * cpptesting
 * cpptesting::inner
 * cpptesting::inner::test
 *
 * @param ns namespace to divide
 * @return vector with the namespaces
 */
std::vector<std::string> ReflectedNamespace::splitNamespace(std::string ns) {
	std::vector<std::string> spaces;
	size_t pos = 0;

	pos = ns.find("::", pos);
	while (pos != std::string::npos) {
		spaces.push_back(ns.substr(0, pos));
		pos = ns.find("::", pos + 2);
	}

	spaces.push_back(ns);

	return spaces;
}


ReflectedNamespace::ReflectedNamespace(std::string fullName) :
	ReflectedItem("", fullName, false, true, Public, Namespace, 0)
{
	//fullNamespace = fullName;
	if (signature.find("::") != std::string::npos)
	{
		//get just the last part of the full namespace
		name = signature.substr(signature.rfind("::") + 2);
	}
	else
	{
		name = signature;
	}

	if (name == "")
	{
		isAnonymous = true;
	}
	else {
		isAnonymous = false;
	}
}






ReflectedNamespace::~ReflectedNamespace()
{
	//do nothing
}

std::string ReflectedNamespace::getFullName() const
{
	return signature;
}

bool ReflectedNamespace::isAccessible() const
{
	return false;
}

std::vector<const ReflectedItem *> ReflectedNamespace::getEnclosedItems() const
{
	return items;
}

const ReflectedItem * ReflectedNamespace::getEnclosedItem(std::string sig) const
{
	for(size_t i = 0; i < items.size(); i++)
	{
		if (items[i]->getSignature() == sig)
		{
			return items[i];
		}
	}

	return NULL;
}

std::vector<ReflectedNamespacePtr> ReflectedNamespace::getEnclosedNamespaces()
	const
{
	return enclosedNamespaces;
}

ReflectedNamespacePtr ReflectedNamespace::getEnclosedNamespace(
		std::string name) const
{
	for(size_t i = 0; i < enclosedNamespaces.size(); i++)
	{
		if (enclosedNamespaces[i]->getName() == name
				|| enclosedNamespaces[i]->getFullName() == name)
		{
			return enclosedNamespaces[i];
		}
	}

	return ReflectedNamespacePtr();
}

bool ReflectedNamespace::isAnonymousNamespace() const
{
	return isAnonymous;
}

void ReflectedNamespace::addItem(const ReflectedItem * item)
{
	if (item)
	{
		bool found = false;
		for(std::vector<const ReflectedItem *>::iterator i = items.begin();
				!found && i != items.end(); i++)
		{
			if ((*i)->getSignature() == item->getSignature())
			{
				found = true;
			}
		}

		if (!found)
		{
			items.push_back(item);
		}
	}
}

void ReflectedNamespace::addNamespace(ReflectedNamespacePtr nNamespace)
{
	if (nNamespace)
	{
		bool found = false;
		for(std::vector<ReflectedNamespacePtr>::iterator i =
				enclosedNamespaces.begin();
				!found && i != enclosedNamespaces.end(); i++)
		{
			if ((*i)->getName() == nNamespace->getName())
			{
				found = true;
			}
		}

		if (!found)
		{
			enclosedNamespaces.push_back(
					ReflectedNamespacePtr(nNamespace));
		}
	}
}


/**
 * Creates a namespace (and all parent namespaces) within the global
 *  namespace.
 *
 *  @param nNamespace namespace to add
 *  @return Pointer to the added namespace
 */
ReflectedNamespacePtr ReflectedNamespace::addNamespace(
		std::string nNamespace) {
	std::vector<std::string> spaces = splitNamespace(nNamespace);
	ReflectedNamespacePtr current = global;
	ReflectedNamespacePtr next = ReflectedNamespacePtr();

	if (nNamespace == "") { //global namespace
		return global;
	}

	for (size_t i = 0; i < spaces.size(); i++) {
		next = current->getEnclosedNamespace(spaces[i]);

		if (!next) {
			next = ReflectedNamespacePtr(new ReflectedNamespace(spaces[i]));
			current->addNamespace(next);
		}
		current = next;
	}

	return current;
}

/**
 * Adds the reflected item to the namespace.  The namespaces are created as
 * needed.
 *
 * @param nNamespace namespace for the item
 * @param item Item to add
 */
void ReflectedNamespace::addItem(std::string nNamespace,
		const ReflectedItem * item) {
//	CPGF_TRACE("Adding ns " << nNamespace << " " << item->getName() << "\r\n")
	ReflectedNamespacePtr ns = addNamespace(nNamespace);
//	CPGF_TRACE("Adding item " << nNamespace << " " << item->getName() << "\r\n")
	ns->addItem(item);
//	CPGF_TRACE("Added item " << nNamespace << " " << item->getName() << "\r\n")
}

/**
 * Gets the namespace object associated with the name.
 *
 * @param nNamespace namespace to get
 * @return pointer to the namespace or NULL if it does not exist
 */
ReflectedNamespacePtr ReflectedNamespace::getNamespace(
		std::string nNamespace) {
	std::vector<std::string> spaces = splitNamespace(nNamespace);
	ReflectedNamespacePtr current = global;
	ReflectedNamespacePtr next = ReflectedNamespacePtr();

	if (nNamespace == "") { //global namespace
		return global;
	}

	for (size_t i = 0; i < spaces.size(); i++) {
		next = current->getEnclosedNamespace(spaces[i]);
		if (!next) {
			return ReflectedNamespacePtr();
		}
		current = next;
	}

	return current;
}


/**
 * @brief Gets a list of all items (methods, variables, etc.) in the
 * namespace
 *
 * @param nNamespace The name of the namespace
 * @return List of items in the namespace. If the namespace does not
 * exist, an empty list is returned
 */
std::vector<const ReflectedItem *> ReflectedNamespace::getEnclosedItems(
		std::string nNamespace) {
	ReflectedNamespacePtr ns = getNamespace(nNamespace);
	if (ns) {
		return ns->getEnclosedItems();
	}

	std::vector<const ReflectedItem *> empty;
	return empty;
}


/**
 * @brief Gets an item (method, variable, etc.) from the
 * namespace
 *
 * @param nNamespace The name of the namespace
 * @param name Name of the item to get (signature)
 *
 * @return The item or NULL if that name does not exist or if the namespace
 * does not exist
 */
const ReflectedItem * ReflectedNamespace::getEnclosedItem(
		std::string nNamespace,	std::string name) {
	ReflectedNamespacePtr ns = getNamespace(nNamespace);
	if (ns) {
		return ns->getEnclosedItem(name);
	}

	return NULL;
}


/**
 * Gets all namespaces for this code
 *
 * @return list of namespaces
 */
std::vector<ReflectedNamespacePtr> ReflectedNamespace::getAllNamespaces() {
	std::vector<ReflectedNamespacePtr> spaces;

	spaces.push_back(global);
	ReflectedNamespacePtr current;
//	CPGF_TRACE("Adding Namespaces")
	for (size_t i = 0; i < spaces.size(); i++) {
		current = spaces[i];
		spaces.insert(spaces.end(), current->enclosedNamespaces.begin(),
				current->enclosedNamespaces.end());
	}

	return spaces;
}


/**
 * Gets all number of namespaces in the code (including the global one).
 *
 * This always returns at least 1
 *
 * @return number of namespaces
 */
size_t ReflectedNamespace::getNamespaceCount() {
	return getAllNamespaces().size();
}

}


