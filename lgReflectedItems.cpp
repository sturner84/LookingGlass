/*
 * lgReflectedEnum.cpp
 *
 *  Created on: Feb 12, 2014
 *      Author: Administrator
 */

#include "lgReflectedItems.h"
#include <climits>

namespace cpptesting {



ReflectedItem::ReflectedItem(std::string nName, bool inherit,
		VisibilityType vis, ReflectedItemType iType)
{
	name = nName;
	inherited = inherit;
	visibility = vis;
	type = iType;
}

ReflectedItem::~ReflectedItem() {
	//do nothing
}

std::string ReflectedItem::getName() const
{
	return name;
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

ReflectedMethod::ReflectedMethod(const cpgf::GMetaMethod * m, bool inherit) :
		ReflectedItem(m->getName(), inherit, Public, Method)
{
	method = m;
}


ReflectedMethod::ReflectedMethod(std::string nName, bool inherit,
		VisibilityType vis) : ReflectedItem(nName, inherit, vis, Method)
{
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




ReflectedConstructor::ReflectedConstructor(const cpgf::GMetaConstructor * c, bool inherit) :
		ReflectedItem(c->getName(), inherit, Public, Constructor)
{
	constructor = c;
}


ReflectedConstructor::ReflectedConstructor(std::string nName, bool inherit,
		VisibilityType vis) : ReflectedItem(nName, inherit, vis, Constructor)
{
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



ReflectedField::ReflectedField(const cpgf::GMetaField * f, bool inherit) :
		ReflectedItem(f->getName(), inherit, Public, Field)

{
	field = f;
}


ReflectedField::ReflectedField(std::string nName, bool inherit,
		VisibilityType vis) : ReflectedItem(nName, inherit, vis, Field)
{
	field = NULL;
}


const cpgf::GMetaField * ReflectedField::getField() const
{
	return field;
}

bool ReflectedField::isAccessible() const
{
	return field;
}

ReflectedOperator::ReflectedOperator(const cpgf::GMetaOperator * o,
		bool inherit) : ReflectedItem(o->getName(), inherit, Public, Operator)
{
	op = o;
}


ReflectedOperator::ReflectedOperator(std::string nName, bool inherit,
		VisibilityType vis) : ReflectedItem(nName, inherit, vis, Operator)
{
	op = NULL;
}

const cpgf::GMetaOperator * ReflectedOperator::getOperator() const
{
	return op;
}

bool ReflectedOperator::isAccessible() const
{
	return op;
}




void ReflectedEnum::loadData(const cpgf::GMetaEnum* genum)
{
	for (size_t i = 0; i < genum->getCount(); i++) {
		enumByNames[genum->getKey(i)]
		            = cpgf::fromVariant<int>(genum->getValue(i));
		enumByValues[cpgf::fromVariant<int>(genum->getValue(i))]
		             = genum->getKey(i);
	}

}





ReflectedEnum::ReflectedEnum(const cpgf::GMetaEnum* e, bool inherit) :
		ReflectedItem(e->getName(), inherit, Public, Enum)
{
	mEnum = e;
	loadData(e);
}

ReflectedEnum::ReflectedEnum(std::string nName, bool inherit,
		VisibilityType vis) : ReflectedItem(nName, inherit, vis, Enum)
{
	mEnum = NULL;
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
	if (mEnum)
	{
		return mEnum->getCount();
	}

	return 0;
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





ReflectedNamespace::ReflectedNamespace(std::string fullName) :
	ReflectedItem("", false, Public, Namespace)
{
	fullNamespace = fullName;
	if (fullNamespace.find("::") != std::string::npos)
	{
		//get just the last part of the full namespace
		name = fullNamespace.substr(fullNamespace.rfind("::") + 2);
	}
	else
	{
		name = fullNamespace;
	}

	if (name == "")
	{
		isAnonymous = true;
	}
}


ReflectedNamespace::~ReflectedNamespace()
{
	//do nothing
}

std::string ReflectedNamespace::getFullName() const
{
	return fullNamespace;
}

bool ReflectedNamespace::isAccessible() const
{
	return false;
}

std::vector<const ReflectedItem *> ReflectedNamespace::getEnclosedItems() const
{
	return items;
}

const ReflectedItem * ReflectedNamespace::getEnclosedItem(std::string name) const
{
	for(size_t i = 0; i < items.size(); i++)
	{
		if (items[i]->getName() == name)
		{
			return items[i];
		}
	}

	return NULL;
}

std::vector<ReflectedNamespace *> ReflectedNamespace::getEnclosedNamespaces() const
{
	return enclosedNamespaces;
}

ReflectedNamespace * ReflectedNamespace::getEnclosedNamespace(std::string name)	const
{
	for(size_t i = 0; i < enclosedNamespaces.size(); i++)
	{
		if (enclosedNamespaces[i]->getName() == name)
		{
			return enclosedNamespaces[i];
		}
	}

	return NULL;
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
			if ((*i)->getName() == item->getName())
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

void ReflectedNamespace::addNamespace(ReflectedNamespace * nNamespace)
{
	if (nNamespace)
	{
		bool found = false;
		for(std::vector<ReflectedNamespace *>::iterator i =
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
			enclosedNamespaces.push_back(nNamespace);
		}
	}
}

}


