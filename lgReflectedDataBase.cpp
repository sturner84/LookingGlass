/*
 * ReflectedDataBase.cpp
 *
 *  Created on: Oct 25, 2013
 *      Author: Administrator
 */

#include "lgReflectedDataBase.h"
#include "lgTestingSupport.h"

namespace cpptesting {

const int ReflectedDataBase::MAX_SIMILAR = 5;
const int ReflectedDataBase::ALLOW_ALL_MODIFIERS = -1;


/**
 * Determines if the name of the method meets the pattern of one of the
 * auto-generated support methods. If so, it should be excluded from the
 * normal lists of methods
 *
 * @param methodName Name of the method
 * @return true if it is one of the auto-generated support methods. See
 * isArraySetterSignature and isCopyFunctionSignature in ReflectionUtil
 */
bool ReflectedDataBase::isNonReflectedMethod(MethodSignature methodName) {
	return methodName.isArraySetterSignature()
		|| methodName.isCopyFunctionSignature();
}


void ReflectedDataBase::loadData(const cpgf::GMetaClass* gclass, bool isGlobal)
{

//CPGF_TRACE("Adding Methods Base\r\n")
	for (size_t k = 0; k < gclass->getMethodCount(); k++)
	{
		MethodSignature mSignature(gclass->getMethodAt(k));
//		methods[signature] = gclass->getMethodAt(k);
//		CPGF_TRACE("Adding Methods Base " << signature << "\r\n")
		if (isNonReflectedMethod(mSignature)) {
			nonReflectedMethods[mSignature] = new ReflectedMethod(
					gclass->getMethodAt(k), false, isGlobal);
//			CPGF_TRACE("Adding Methods Base ns " << signature << "\r\n")
			ReflectedNamespace::addItem(gclass->getMethodAt(k)->getNamespace(),
					nonReflectedMethods[mSignature]);
		}
		else {
			methods[mSignature] = new ReflectedMethod(gclass->getMethodAt(k),
					false, isGlobal);
//			CPGF_TRACE("Adding Methods Base real ns " << signature << "\r\n")
			ReflectedNamespace::addItem(gclass->getMethodAt(k)->getNamespace(),
					methods[mSignature]);
		}
//
//		std::cout << signature << " "<< ReflectionUtil::correctSignature(signature) << "\n";
//		std::cout.flush();
	}
//CPGF_TRACE("Adding Fields\r\n")
	for (size_t i = 0; i < gclass->getFieldCount(); i++)
	{
		FieldSignature fSignature(gclass->getFieldAt(i));
//		CPGF_TRACE("Adding " << gclass->getName() << " : " << signature << "\r\n")
		fields[fSignature] = new ReflectedField(gclass->getFieldAt(i), false,
				isGlobal);
		ReflectedNamespace::addItem(gclass->getFieldAt(i)->getNamespace(),
				fields[fSignature]);
//		CPGF_TRACE("Added " << gclass->getName() << " : " << signature << "\r\n")
	}

//CPGF_TRACE("Adding Enums\r\n")
	for (size_t i = 0; i < gclass->getEnumCount(); i++)
	{
		EnumSignature eSignature(gclass->getEnumAt(i));

		enums[eSignature.getName()] = new ReflectedEnum(gclass->getEnumAt(i), false,
				isGlobal);
		ReflectedNamespace::addItem(gclass->getEnumAt(i)->getNamespace(),
				enums[eSignature.getName()]);
//
//		std::cout << signature << " "<< ReflectionUtil::correctEnum(signature) << "\n";
//		std::cout.flush();
	}


//CPGF_TRACE("Adding NR\r\n")
	loadNonReflectedClassData(gclass);
//CPGF_TRACE("Done loadData Base\r\n")
}



/**
 * Loads data that is not reflected like private methods, non-reflected
 * base classes, etc.
 *
 * @param gclass Class to load
 */
void ReflectedDataBase::loadNonReflectedClassData(const cpgf::GMetaClass* gclass) {
	for (size_t i = 0; i < gclass->getNonReflectedCount(); i++) {
		loadNonReflectedData(gclass->getNonReflectedAt(i));
	}
}


/**
 * Loads a single non-reflected item like private methods, non-reflected
 * base classes, etc.
 *
 * @param nrItem item to load
 */
	void ReflectedDataBase::loadNonReflectedData(
		const cpgf::GMetaNonReflectedItem * nrItem) {
	std::string sig = nrItem->getSignature();
	VisibilityType vis = ReflectedItem::cpgfToLGVisibility(
				nrItem->getVisibility());

	//			switch (nrItem->getVisibility()) {
	//
	//			}

	switch (nrItem->getItemCategory()) {
	case cpgf::mcatField:
	{
		FieldSignature fSignature(sig);
		fields[fSignature] =
				new ReflectedField(fSignature, false, false, vis,
	            		 nrItem->getModifiers());;
		ReflectedNamespace::addItem(nrItem->getNamespace(),
				fields[fSignature]);
		break;
	}
	case cpgf::mcatMethod:
	{
		MethodSignature mSignature(sig);
		methods[mSignature] = new ReflectedMethod(mSignature, false, false, vis,
       		 nrItem->getModifiers());
		ReflectedNamespace::addItem(nrItem->getNamespace(),
				methods[mSignature]);
		break;
	}
	case cpgf::mcatEnum: {
		//signature = ReflectionUtil::correctEnum(signature);
		EnumSignature eSignature(sig);
		enums[eSignature.getName()]
		      = new ReflectedEnum(eSignature, false, false, vis,
	            		 nrItem->getModifiers());
		ReflectedNamespace::addItem(nrItem->getNamespace(),
				enums[eSignature.getName()]);
		break;
	}
	default:
		CPGF_TRACE("Item category for non-reflected item is invalid: "
				<< nrItem->getItemCategory())
	}
}


ReflectedDataBase::~ReflectedDataBase()
{
	for (std::map<std::string, const ReflectedEnum *>::iterator it
			= enums.begin(); it != enums.end(); it++)
	{
		delete it->second;
	}

	for (std::map<std::string, const ReflectedMethod *>::iterator it
			= methods.begin(); it != methods.end(); it++)
	{
		delete it->second;
	}

	for (std::map<std::string, const ReflectedField *>::iterator it
			= fields.begin(); it != fields.end(); it++)
	{
		delete it->second;
	}

	for (std::map<std::string, const ReflectedMethod *>::iterator it
			= nonReflectedMethods.begin(); it != nonReflectedMethods.end(); it++)
	{
		delete it->second;
	}


	//	for (std::map<std::string, const ReflectedOperator*>::iterator it
//			= operators.begin(); it != operators.end(); it++)
//	{
//		delete it->second;
//	}
}

bool ReflectedDataBase::doesMethodExist(MethodSignature functionSignature,
		ItemFilter filter) const
{
	return exists(methods, functionSignature, filter);
}




size_t ReflectedDataBase::getMethodCount(ItemFilter filter) const
{
	return countItems(methods, filter);
}

const std::vector<std::string> ReflectedDataBase::getMethodNames(
		ItemFilter filter) const
{
	return getNames(methods, filter);
}


const ReflectedMethod * ReflectedDataBase::getMethod(MethodSignature signature,
		ItemFilter filter) const
{
	return getItem(methods, signature, filter);
}



bool ReflectedDataBase::doesNonReflectedMethodExist(
		MethodSignature functionSignature,
		ItemFilter filter) const
{
	return exists(nonReflectedMethods, functionSignature,
			filter);

}




size_t ReflectedDataBase::getNonReflectedMethodCount(ItemFilter filter) const
{
	return countItems(nonReflectedMethods, filter);
}

const std::vector<std::string> ReflectedDataBase::getNonReflectedMethodNames(
		ItemFilter filter) const
{
	return getNames(nonReflectedMethods, filter);
}


const ReflectedMethod * ReflectedDataBase::getNonReflectedMethod(
		MethodSignature signature, ItemFilter filter) const
{
	return getItem(nonReflectedMethods, signature, filter);
}






//ignore values, just look for name
bool ReflectedDataBase::doesEnumExist(EnumSignature name,
		ItemFilter filter) const
{
	return exists(enums, name, filter);
}


size_t ReflectedDataBase::getEnumCount(ItemFilter filter) const
{
	return countItems(enums, filter);
	//return enums.size();
}

const std::vector<std::string> ReflectedDataBase::getEnumNames(
		ItemFilter filter) const
{
	return getNames(enums, filter);
}

const ReflectedEnum * ReflectedDataBase::getEnum(EnumSignature signature,
		ItemFilter filter) const
{
//	std::string name = ReflectionUtil::correctEnum(signature);
//	if (globalEnums.count(name) == 1)
//	{
//		return globalEnums[name];
//	}
//	return NULL;
	return getItem(enums, signature, filter);
}



std::vector<const ReflectedEnum *> ReflectedDataBase::getEnums(
		ItemFilter filter)
{
	return getItems(enums, filter);
}

std::vector<const ReflectedEnum *> ReflectedDataBase::getClosestEnums(
		EnumSignature name, ItemFilter filter, int count)
{
	return getClosest(enums, name, filter, count);
}

std::string ReflectedDataBase::getClosestEnumsString(
		EnumSignature name, ItemFilter filter, int count)
{
	return getClosestString(enums, name, filter, count);
}


bool ReflectedDataBase::doesFieldExist(FieldSignature name,
		ItemFilter filter) const
{
	return exists(fields, name, filter);

//	const ReflectedField * m = getItem(fields,
//				ReflectionUtil::correctField(name));
//		return (m && (inherited || !m->isInherited()));
	//return (fields.count(ReflectionUtil::correctField(name)) == 1);
}

size_t ReflectedDataBase::getVariableCount(ItemFilter filter) const
{
	return getVariableNames(filter).size();
}

const std::vector<std::string> ReflectedDataBase::getVariableNames(
		ItemFilter filter) const
{
	std::vector<std::string> names;

	for (std::map<std::string, const ReflectedField *>::const_iterator it
			= fields.begin(); it != fields.end(); it++)
	{
		const ReflectedField * field = it->second;
//		const cpgf::GMetaType &type = field->getField()->getItemType();

		// check these too?
		//		bool isPointerToConst() const;
		//		bool isPointerToConstVolatile() const;

//		if (!type.isConst() && !type.isConstVolatile()
//				&& !type.isReferenceToConst()
//				&& !type.isReferenceToConstVolatile()
//				&& (inherit || !field->isInherited())
//				&& (vis & field->getVisibility()))
//		if (!field->isConst()
//				&& (inherit || !field->isInherited())
//				&& (vis & field->getVisibility()))
		ItemFilter varFilter(filter.getVisibility(), filter.getInherited(),
				filter.getRequiredModifiers(),
				filter.getExcludedModifiers() | cpgf::metaModifierConst);
		if (varFilter.isAllowed(field))
		{
			names.push_back(it->first);
		}
	}

	return names;
}

size_t ReflectedDataBase::getConstantCount(ItemFilter filter) const
{
	return getConstantNames(filter).size();
}

const std::vector<std::string> ReflectedDataBase::getConstantNames(
		ItemFilter filter) const
{
	std::vector<std::string> names;

	for (std::map<std::string, const ReflectedField *>::const_iterator it
			= fields.begin(); it != fields.end(); it++)
	{
		const ReflectedField * field = it->second;
//		const cpgf::GMetaType &type = field->getField()->getItemType();

		//check these too?
//		bool isPointerToConst() const;
//		bool isPointerToConstVolatile() const;

//		if ((type.isConst() || type.isConstVolatile()
//				|| type.isReferenceToConst()
//				|| type.isReferenceToConstVolatile())
//				&& (inherit || !field->isInherited())
//				&& (vis & field->getVisibility()))
//		if (field->isConst()
//				&& (inherit || !field->isInherited())
//				&& (vis & field->getVisibility()))
		if (filter.isAllowed(field, cpgf::metaModifierConst))
		{
			names.push_back(it->first);
		}
	}

	return names;
}



size_t ReflectedDataBase::getFieldCount(ItemFilter filter) const
{
	return countItems(fields, filter);
	//return fields.size();
}


const std::vector<std::string> ReflectedDataBase::getFieldNames(
		ItemFilter filter) const
{
	return getNames(fields, filter);
}



const ReflectedField * ReflectedDataBase::getField(FieldSignature signature,
		ItemFilter filter) const
{
	return getItem(fields, signature, filter);
}


}
