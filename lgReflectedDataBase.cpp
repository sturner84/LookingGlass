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
bool ReflectedDataBase::isNonReflectedMethod(std::string methodName) {
	return ReflectionUtil::isArraySetterSignature(methodName)
		|| ReflectionUtil::isCopyFunctionSignature(methodName);
}


void ReflectedDataBase::loadData(const cpgf::GMetaClass* gclass, bool isGlobal)
{
	std::string signature;
//CPGF_TRACE("Adding Methods Base\r\n")
	for (size_t k = 0; k < gclass->getMethodCount(); k++)
	{
		signature = ReflectionUtil::correctSignature(
				ReflectionUtil::createFunctionSignature(gclass->getMethodAt(k)));
//		methods[signature] = gclass->getMethodAt(k);
//		CPGF_TRACE("Adding Methods Base " << signature << "\r\n")
		if (isNonReflectedMethod(signature)) {
			nonReflectedMethods[signature] = new ReflectedMethod(
					gclass->getMethodAt(k), false, isGlobal);
//			CPGF_TRACE("Adding Methods Base ns " << signature << "\r\n")
			ReflectedNamespace::addItem(gclass->getMethodAt(k)->getNamespace(),
					nonReflectedMethods[signature]);
		}
		else {
			methods[signature] = new ReflectedMethod(gclass->getMethodAt(k),
					false, isGlobal);
//			CPGF_TRACE("Adding Methods Base real ns " << signature << "\r\n")
			ReflectedNamespace::addItem(gclass->getMethodAt(k)->getNamespace(),
					methods[signature]);
		}
//
//		std::cout << signature << " "<< ReflectionUtil::correctSignature(signature) << "\n";
//		std::cout.flush();
	}
//CPGF_TRACE("Adding Fields\r\n")
	for (size_t i = 0; i < gclass->getFieldCount(); i++)
	{
		signature = ReflectionUtil::correctField(
				ReflectionUtil::createFieldSignature(gclass->getFieldAt(i)));
//		CPGF_TRACE("Adding " << gclass->getName() << " : " << signature << "\r\n")
		//fields[signature] = gclass->getFieldAt(i);
		fields[signature] =
				new ReflectedField(gclass->getFieldAt(i), false, isGlobal);
		ReflectedNamespace::addItem(gclass->getFieldAt(i)->getNamespace(),
				fields[signature]);
//		CPGF_TRACE("Added " << gclass->getName() << " : " << signature << "\r\n")
//		std::cout << signature << " "<< ReflectionUtil::correctField(signature) << "\n";
//		std::cout.flush();
	}

//	for (size_t i = 0; i < gclass->getOperatorCount(); i++)
//	{
//		signature = ReflectionUtil::createOperatorSignature(
//				gclass->getOperatorAt(i));
////		operators[signature] = gclass->getOperatorAt(i);
//		operators[ReflectionUtil::correctSignature(signature)]
//		            = new ReflectedOperator(gclass->getOperatorAt(i), false);
////
////		std::cout << signature << " "<< ReflectionUtil::correctSignature(signature) << "\n";
////		std::cout.flush();
//	}

//	for (size_t i = 0; i < gclass->getConstructorCount(); i++)
//	{
//		allConstructors[createConstructorSignature(gclass, gclass->getConstructorAt(i))] = gclass->getConstructorAt(i);
//		std::cout << createConstructorSignature(gclass, gclass->getConstructorAt(i)) << "\n";
//		std::cout.flush();
//	}
//CPGF_TRACE("Adding Enums\r\n")
	for (size_t i = 0; i < gclass->getEnumCount(); i++)
	{
		signature = gclass->getEnumAt(i)->getName();
//		enums[signature] = gclass->getEnumAt(i);
		enums[ReflectionUtil::correctEnum(signature)]
		            = new ReflectedEnum(gclass->getEnumAt(i), false, isGlobal);
		ReflectedNamespace::addItem(gclass->getEnumAt(i)->getNamespace(),
				enums[signature]);
//
//		std::cout << signature << " "<< ReflectionUtil::correctEnum(signature) << "\n";
//		std::cout.flush();
	}

//	if (gclass->getClassCount() > 0)
//	{
//		for (size_t i = 0; i < gclass->getClassCount(); i++)
//		{
//			// create new ReflectedClass objects with these
//			//loadData(gclass->getClassAt(i));
//		}
//	}
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
	std::string signature = nrItem->getSignature();
	VisibilityType vis = ReflectedItem::cpgfToLGVisibility(
				nrItem->getVisibility());

	//			switch (nrItem->getVisibility()) {
	//
	//			}

	switch (nrItem->getItemCategory()) {
	case cpgf::mcatField:
		signature = ReflectionUtil::correctField(signature);
		fields[signature] =
				new ReflectedField(signature, false, false, vis,
	            		 nrItem->getModifiers());;
		ReflectedNamespace::addItem(nrItem->getNamespace(),
				fields[signature]);
		break;
	case cpgf::mcatMethod:
		signature = ReflectionUtil::correctSignature(signature);
		methods[signature] = new ReflectedMethod(signature, false, false, vis,
       		 nrItem->getModifiers());
		ReflectedNamespace::addItem(nrItem->getNamespace(),
				methods[signature]);
		break;
	case cpgf::mcatEnum: {
		//signature = ReflectionUtil::correctEnum(signature);
		EnumInfo info = ReflectionUtil::divideEnumSignature(signature);
		//std::string name = ReflectionUtil::getEnumName(signature);
		enums[info.name]
		      = new ReflectedEnum(signature, false, false, vis,
	            		 nrItem->getModifiers());
		ReflectedNamespace::addItem(nrItem->getNamespace(),
				enums[info.name]);
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

bool ReflectedDataBase::doesMethodExist(std::string functionSignature,
		VisibilityAccessType vis, bool inherited, int modifiers,
		bool allowMoreMods) const
{
	return exists(methods, ReflectionUtil::correctSignature(functionSignature),
			vis, inherited, modifiers, allowMoreMods);

//	const ReflectedMethod * m = getItem(methods,
//			ReflectionUtil::correctSignature(functionSignature));
//	return (m && (inherited || !m->isInherited()));

//	return (methods.count(ReflectionUtil::correctSignature(functionSignature)) == 1);
//	return getItem(functions, ReflectionUtil::correctSignature(functionSignature)) != NULL;
}




size_t ReflectedDataBase::getMethodCount(VisibilityAccessType vis,
		bool inherit) const
{
	return countItems(methods, vis, inherit);
}

const std::vector<std::string> ReflectedDataBase::getMethodNames(
		VisibilityAccessType vis, bool inherit) const
{
	return getNames(methods, vis, inherit);
}


const ReflectedMethod * ReflectedDataBase::getMethod(std::string signature,
		int modifiers, bool allowMoreMods) const
{
	return getItem(methods, ReflectionUtil::correctSignature(signature),
			modifiers, allowMoreMods);
}



bool ReflectedDataBase::doesNonReflectedMethodExist(
		std::string functionSignature,
		VisibilityAccessType vis, bool inherited) const
{
	return exists(nonReflectedMethods,
			ReflectionUtil::correctSignature(functionSignature),
			vis, inherited, ALLOW_ALL_MODIFIERS, false);

}




size_t ReflectedDataBase::getNonReflectedMethodCount(VisibilityAccessType vis,
		bool inherit) const
{
	return countItems(nonReflectedMethods, vis, inherit);
}

const std::vector<std::string> ReflectedDataBase::getNonReflectedMethodNames(
		VisibilityAccessType vis, bool inherit) const
{
	return getNames(nonReflectedMethods, vis, inherit);
}


const ReflectedMethod * ReflectedDataBase::getNonReflectedMethod(
		std::string signature) const
{
	return getItem(nonReflectedMethods,
			ReflectionUtil::correctSignature(signature), ALLOW_ALL_MODIFIERS, false);
}



//bool ReflectedDataBase::doesOperatorExist(std::string functionSignature,
//		VisibilityAccessType vis, bool inherited) const
//{
//	return exists(operators, ReflectionUtil::correctSignature(functionSignature),
//			vis, inherited);
//
////	const ReflectedOperator * m = getItem(operators,
////			ReflectionUtil::correctSignature(functionSignature));
////	return (m && (inherited || !m->isInherited()));
//	//return (operators.count(ReflectionUtil::correctSignature(functionSignature)) == 1);
//}
//
//size_t ReflectedDataBase::getOperatorCount(VisibilityAccessType vis,
//		bool inherit) const
//{
//	return countItems(operators, vis, inherit);
//}
//
//const std::vector<std::string> ReflectedDataBase::getOperatorNames(
//		VisibilityAccessType vis, bool inherit) const
//{
//	return getNames(operators, vis, inherit);
//}
//
//const ReflectedOperator * ReflectedDataBase::getOperator(std::string signature) const
//{
//	return getItem(operators, ReflectionUtil::correctSignature(signature));
//}
//
//
//std::vector<const ReflectedOperator *> ReflectedDataBase::getOperators(
//		VisibilityAccessType vis, bool inherited)
//{
//	return getItems(operators, vis, inherited);
//}
//
//std::vector<const ReflectedOperator *> ReflectedDataBase::getClosestOperators(
//		std::string name, VisibilityAccessType vis, bool inherited, int count)
//{
//	return getClosest(operators, name, vis, inherited, count);
//}
//
//std::string ReflectedDataBase::getClosestOperatorsString(
//		std::string name, VisibilityAccessType vis, bool inherited, int count)
//{
//	return getClosestString(operators, name, vis, inherited, count);
//}


//ignore values, just look for name
bool ReflectedDataBase::doesEnumExist(std::string name,
		VisibilityAccessType vis, bool inherited, int modifiers,
		bool allowMoreMods) const
{
	return exists(enums, ReflectionUtil::correctEnum(name), vis, inherited,
			modifiers, allowMoreMods);

//	const ReflectedEnum * m = getItem(enums,
//			ReflectionUtil::correctEnum(name));
//	return (m && (vis & m->getVisibility())
//			&& (inherited || !m->isInherited()));
	//return (enums.count(ReflectionUtil::correctEnum(name)) == 1);
//	return getItem(globalEnums, ReflectionUtil::correctEnum(name)) != NULL;
}


size_t ReflectedDataBase::getEnumCount(VisibilityAccessType vis,
		bool inherit) const
{
	return countItems(enums, vis, inherit);
	//return enums.size();
}

const std::vector<std::string> ReflectedDataBase::getEnumNames(
		VisibilityAccessType vis, bool inherit) const
{
	return getNames(enums, vis, inherit);
}

const ReflectedEnum * ReflectedDataBase::getEnum(std::string signature,
		int modifiers, bool allowMoreMods) const
{
//	std::string name = ReflectionUtil::correctEnum(signature);
//	if (globalEnums.count(name) == 1)
//	{
//		return globalEnums[name];
//	}
//	return NULL;
	return getItem(enums, ReflectionUtil::correctEnum(signature),
			modifiers, allowMoreMods);
}



std::vector<const ReflectedEnum *> ReflectedDataBase::getEnums(
		VisibilityAccessType vis, bool inherited, int modifiers,
		bool allowMoreMods)
{
	return getItems(enums, vis, inherited, modifiers, allowMoreMods);
}

std::vector<const ReflectedEnum *> ReflectedDataBase::getClosestEnums(
		std::string name, VisibilityAccessType vis, bool inherited, int count)
{
	return getClosest(enums, name, vis, inherited, count);
}

std::string ReflectedDataBase::getClosestEnumsString(
		std::string name, VisibilityAccessType vis, bool inherited, int count)
{
	return getClosestString(enums, name, vis, inherited, count);
}


bool ReflectedDataBase::doesFieldExist(std::string name,
		VisibilityAccessType vis, bool inherited, int modifiers,
		bool allowMoreMods) const
{
	return exists(fields, ReflectionUtil::correctField(name), vis, inherited,
			modifiers, allowMoreMods);

//	const ReflectedField * m = getItem(fields,
//				ReflectionUtil::correctField(name));
//		return (m && (inherited || !m->isInherited()));
	//return (fields.count(ReflectionUtil::correctField(name)) == 1);
}

size_t ReflectedDataBase::getVariableCount(VisibilityAccessType vis,
		bool inherit) const
{
	return getVariableNames(vis, inherit).size();
}

const std::vector<std::string> ReflectedDataBase::getVariableNames(
		VisibilityAccessType vis, bool inherit) const
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
		if (!field->isConst()
				&& (inherit || !field->isInherited())
				&& (vis & field->getVisibility()))
		{
			names.push_back(it->first);
		}
	}

	return names;
}

size_t ReflectedDataBase::getConstantCount(VisibilityAccessType vis,
		bool inherit) const
{
	return getConstantNames(vis, inherit).size();
}

const std::vector<std::string> ReflectedDataBase::getConstantNames(
		VisibilityAccessType vis, bool inherit) const
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
		if (field->isConst()
				&& (inherit || !field->isInherited())
				&& (vis & field->getVisibility()))
		{
			names.push_back(it->first);
		}
	}

	return names;
}



size_t ReflectedDataBase::getFieldCount(VisibilityAccessType vis,
		bool inherit) const
{
	return countItems(fields, vis, inherit);
	//return fields.size();
}


const std::vector<std::string> ReflectedDataBase::getFieldNames(
		VisibilityAccessType vis, bool inherit) const
{
	return getNames(fields, vis, inherit);
}



const ReflectedField * ReflectedDataBase::getField(std::string signature,
		int modifiers, bool allowMoreMods) const
{
//	std::string name = ReflectionUtil::correctField(signature);
//	if (fields.count(name) == 1)
//	{
//		return globalVars[name];
//	}
//	return NULL;

	return getItem(fields, ReflectionUtil::correctField(signature), modifiers,
			allowMoreMods);
}


}
