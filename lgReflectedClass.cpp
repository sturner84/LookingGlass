/*
 * lgReflectedClass.cpp
 *
 *  Created on: Nov 6, 2013
 *      Author: Administrator
 */


#include "lgReflectedClass.h"
#include "lgReflectionSupport.h"

namespace cpptesting {


ReflectedBaseClass::ReflectedBaseClass(const ReflectedClass * c) :
		ReflectedItem(c->getName(), true, Public, Class)
{
	mClass = c;
//	if (c != NULL)
//	{
//		name = mClass->getName();
//	}
}

ReflectedBaseClass::ReflectedBaseClass(std::string nName, VisibilityType vis) :
		ReflectedItem(nName, true, vis, Class)
{
	mClass = NULL;
//	name = nName;
}


const ReflectedClass * ReflectedBaseClass::getClass() const
{
	return mClass;
}


//std::string ReflectedBaseClass::getName()
//{
//	return name;
//}


bool ReflectedBaseClass::isAccessible() const
{
	return mClass;
}








ReflectedObject::ReflectedObject(const ReflectedClass * c, void * obj)
{
	mClass = c;
	mObject = obj;
}

ReflectedObject::~ReflectedObject()
{
	mClass->getClass()->destroyInstance(mObject);
}


const ReflectedClass * ReflectedObject::getClass() const
{
	return mClass;
}

void * ReflectedObject::getObject() const
{
	return mObject;
}

const std::string ReflectedObject::getClassName()
{
	if (mClass != NULL)
	{
		return mClass->getName();
	}
	return "";
}


/**
 * Invokes a method with the specified parameters. No value is returned
 * 	from the method invoked.The signature for this function is:
 *  	bool invokeFunction(string methodSignature,
 *  		GVariant p0, ...)
 *  		Most types can be used for GVariant
 *
 * @param methodSignature Method to call (i.e. int foo(double x, char y))
 * @param p<1-28> Parameter for the method.  Currently up to 28 parameters are
 * 		permitted.
 *
 * @return True is the method was successfully called.
 * 		If the method was called but the return value could not be converted
 * 		to the ReturnType, then this is false.
 */
#define REF_INVOKE(N, unused) \
	bool ReflectedObject::invoke(std::string methodSignature \
		GPP_COMMA_IF(N) \
		GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p)) \
{ \
	int r; \
	return LookingGlass::invokeInternal(this, methodSignature, false, r GPP_COMMA_IF(N) \
		GPP_REPEAT(N, GPP_COMMA_PARAM, p)) == LookingGlass::INVOKE_SUCCESS; \
}

GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE
//ReflectedObjectPtr object(this);


void ReflectedClass::loadData(const cpgf::GMetaClass* gclass)
{
	ReflectedDataBase::loadData(gclass);
	std::string signature;

	for (size_t i = 0; i < gclass->getConstructorCount(); i++)
	{
		signature = ReflectionUtil::createConstructorSignature(
						gclass, gclass->getConstructorAt(i));
		constructors[ReflectionUtil::correctSignature(signature)]
		    = new ReflectedConstructor(gclass->getConstructorAt(i), false);
	}

//	for (size_t i = 0; i < gclass->getBaseCount(); i++)
//	{
//		const cpgf::GMetaClass* baseClass = gclass->getBaseClass(i);
//		signature = ReflectionUtil::correctType(baseClass->getName());
//		baseClasses[signature] = baseClass;
//	}

	//add all methods from this class
//	for (std::map<std::string, const cpgf::GMetaMethod*>::const_iterator i = methods.begin();
//			i != methods.end(); i++) {
//		allMethods[i->first] = i->second;
//	}
}


void ReflectedClass::loadBaseMethods(std::map<std::string, const ReflectedMethod *> & baseMethods)
{
	//add all methods from this class
	for (std::map<std::string, const ReflectedMethod *>::const_iterator i
			= baseMethods.begin(); i != baseMethods.end(); i++) {
		if (methods.count(i->first) == 0) { //new method
			methods[i->first] = new ReflectedMethod(
					i->second->getMethod(), true);
		}
	}
}


void ReflectedClass::loadBaseEnums(std::map<std::string,
		const ReflectedEnum *> & baseEnums)
{
	//add all enums from this class
	for (std::map<std::string, const ReflectedEnum *>::const_iterator i
			= baseEnums.begin(); i != baseEnums.end(); i++) {
		if (enums.count(i->first) == 0) { //new enum
			enums[i->first] = new ReflectedEnum(
					i->second->getEnum(), true);
		}
	}
}

void ReflectedClass::loadBaseOperators(std::map<std::string,
		const ReflectedOperator *> & baseOperators)
{
	//add all operators from this class
	for (std::map<std::string, const ReflectedOperator *>::const_iterator i
			= baseOperators.begin(); i != baseOperators.end(); i++) {
		if (operators.count(i->first) == 0) { //new operator
			operators[i->first] = new ReflectedOperator(
					i->second->getOperator(), true);
		}
	}
}

void ReflectedClass::loadBaseFields(std::map<std::string,
		const ReflectedField *> & baseFields)
{
	//add all fields from this class
	for (std::map<std::string, const ReflectedField *>::const_iterator i
			= baseFields.begin(); i != baseFields.end(); i++) {
		if (fields.count(i->first) == 0) { //new field
			fields[i->first] = new ReflectedField(
					i->second->getField(), true);
		}
	}
}


void ReflectedClass::loadBaseClasses(std::map<std::string,
		ReflectedClass *> & classes) {
	std::string signature;

	for (size_t i = 0; i < mClass->getBaseCount(); i++)
	{
		//TODO this is returning null when the base class is not reflected
		//so there isn't a way currently to get all the base classes
		const cpgf::GMetaClass* baseClass = mClass->getBaseClass(i);
		//std::string name = getName();

		if (baseClass) {
			signature = ReflectionUtil::correctType(baseClass->getName());

			if (classes.count(signature) == 1) {
				baseClasses[signature] = new ReflectedBaseClass(classes[signature]);

				//check if its classes have been loaded
				if (classes[signature]->baseClasses.size() == 0) {
					//may not have been, so load it
					classes[signature]->loadBaseClasses(classes);
				}
				// add all methods, fields, operators, enums
				loadBaseMethods(classes[signature]->methods);
				loadBaseFields(classes[signature]->fields);
				loadBaseOperators(classes[signature]->operators);
				loadBaseEnums(classes[signature]->enums);

			}
			else { //nothing reflected
				//baseClasses[signature] = NULL;
				//TODO assuming public for now
				baseClasses[signature] = new ReflectedBaseClass(signature,
						Public);
			}
		}

	}

}



/**
 * Creates an object of this class with the specified parameters
 * and returns the object
 *  The signature for this method is:
 *  	const ReflectedObject * creater(GVariant p0, ...)
 *  		Most types can be used for GVariant
 *
 * @param p<1-28> Parameter for the method.  Currently up to 28 parameters are
 * 		permitted.
 *
 * @return A pointer to the new object is successful. NULL if not.
 */
#define REF_INVOKE(N, unused) \
		const ReflectedObjectPtr ReflectedClass::create(std::string signature \
				GPP_COMMA_IF(N) \
				GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p)) const \
{ \
	if (doesConstructorExist(signature, Public_Access)) \
	{ \
		const cpgf::GMetaConstructor * construct = getConstructor(signature) \
			->getConstructor(); \
		void * object = construct->invoke( \
				GPP_REPEAT(N, GPP_COMMA_PARAM, p)); \
		ReflectedObjectPtr ptr (new ReflectedObject(this, object)); \
		return ptr; \
	} \
	ReflectedObjectPtr ptr; \
	return ptr; \
}

GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE

///**
// * Gets the class name
// *
// * @return Name of the class
// */
//const std::string ReflectedClass::getName() const
//{
//	return mClass->getName();
//}



ReflectedClass::ReflectedClass(const cpgf::GMetaClass* c) :
		ReflectedItem(c->getName(), false, Public, Class)
{
	mClass = c;
	loadData(mClass);
}

ReflectedClass::~ReflectedClass()
{
	for (std::map<std::string, const ReflectedBaseClass *>::const_iterator i
			= baseClasses.begin(); i != baseClasses.end(); i++)
	{
		delete i->second;
	}

	for (std::map<std::string, const ReflectedConstructor *>::const_iterator i
			= constructors.begin(); i != constructors.end(); i++)
	{
		delete i->second;
	}
}

const cpgf::GMetaClass* ReflectedClass::getClass() const
{
	return mClass;
}


bool ReflectedClass::doesMethodExist(std::string functionSignature,
		VisibilityAccessType vis, bool inherit) const
{
	return ReflectedDataBase::doesMethodExist(functionSignature, vis, inherit);
}

size_t ReflectedClass::getMethodCount(VisibilityAccessType vis,
		bool inherit) const
{
	return ReflectedDataBase::getMethodCount(vis, inherit);
}


const std::vector<std::string> ReflectedClass::getMethodNames(
		VisibilityAccessType vis, bool inherit) const
{
	return ReflectedDataBase::getMethodNames(vis, inherit);
}

const ReflectedMethod * ReflectedClass::getMethod(std::string signature) const
{
	return ReflectedDataBase::getMethod(signature);
}


std::vector<const ReflectedMethod *> ReflectedClass::getMethods(
		VisibilityAccessType vis, bool inherited)
{
	return getItems(methods, vis, inherited);
}

std::vector<const ReflectedMethod *> ReflectedClass::getClosestMethods(
		std::string name, VisibilityAccessType vis, bool inherited, int count)
{
	return getClosest(methods, name, vis, inherited, count);
}

std::string ReflectedClass::getClosestMethodsString(
		std::string name, VisibilityAccessType vis, bool inherited, int count)
{
	return getClosestString(methods, name, vis, inherited, count);
}

bool ReflectedClass::doesFieldExist(std::string signature,
		VisibilityAccessType vis, bool inherit) const
{
	return ReflectedDataBase::doesFieldExist(signature, vis, inherit);
}

size_t ReflectedClass::getVariableCount(VisibilityAccessType vis,
		bool inherit) const
{
	return ReflectedDataBase::getVariableCount(vis, inherit);
}

const std::vector<std::string> ReflectedClass::getVariableNames(
		VisibilityAccessType vis, bool inherit) const
{
	return ReflectedDataBase::getVariableNames(vis, inherit);
}

size_t ReflectedClass::getConstantCount(VisibilityAccessType vis,
		bool inherit) const
{
	return ReflectedDataBase::getConstantCount(vis, inherit);
}

const std::vector<std::string> ReflectedClass::getConstantNames(
		VisibilityAccessType vis, bool inherit) const
{
	return ReflectedDataBase::getConstantNames(vis, inherit);
}


size_t ReflectedClass::getFieldCount(VisibilityAccessType vis,
		bool inherit) const
{
	return ReflectedDataBase::getFieldCount(vis, inherit);
}

const std::vector<std::string> ReflectedClass::getFieldNames(
		VisibilityAccessType vis, bool inherit) const
{
	return ReflectedDataBase::getFieldNames(vis, inherit);
}


const ReflectedField * ReflectedClass::getField(std::string signature) const
{
	return ReflectedDataBase::getField(signature);
}


std::vector<const ReflectedField *> ReflectedClass::getFields(
		VisibilityAccessType vis, bool inherited)
{
	return getItems(fields, vis, inherited);
}

std::vector<const ReflectedField *> ReflectedClass::getClosestFields(
		std::string name, VisibilityAccessType vis, bool inherited, int count)
{
	return getClosest(fields, name, vis, inherited, count);
}

std::string ReflectedClass::getClosestFieldsString(
		std::string name, VisibilityAccessType vis, bool inherited, int count)
{
	return getClosestString(fields, name, vis, inherited, count);
}

bool ReflectedClass::doesConstructorExist(std::string signature,
		VisibilityAccessType vis) const
{
	return exists(constructors, ReflectionUtil::correctSignature(signature),
			vis, true);
	//return (constructors.count(ReflectionUtil::correctSignature(signature)) == 1);
}

size_t ReflectedClass::getConstructorCount(VisibilityAccessType vis) const
{
	return countItems(constructors, vis, true);
}

const std::vector<std::string> ReflectedClass::getConstructorNames(
		VisibilityAccessType vis) const
{
	return getNames(constructors, vis, true);
}

const ReflectedConstructor * ReflectedClass::getConstructor(
		std::string signature) const
{
	return getItem(constructors, ReflectionUtil::correctSignature(signature));
}


std::vector<const ReflectedConstructor *> ReflectedClass::getConstructors(
		VisibilityAccessType vis)
{
	return getItems(constructors, vis, true);
}

std::vector<const ReflectedConstructor *> ReflectedClass::getClosestConstructors(
		std::string name, VisibilityAccessType vis, int count)
{
	return getClosest(constructors, name, vis, true, count);
}

std::string ReflectedClass::getClosestConstructorsString(
		std::string name, VisibilityAccessType vis,  int count)
{
	return getClosestString(constructors, name, vis, true, count);
}


//bool ReflectedClass::doesCallableMethodExist(std::string signature) const {
//	return (allMethods.count(ReflectionUtil::correctSignature(signature)) == 1);
//}
//
//
//size_t ReflectedClass::getCallableMethodCount() const
//{
//	return allMethods.size();
//}
//
//
//const std::vector<std::string> ReflectedClass::getCallableMethodNames() const
//{
//	return getNames(allMethods);
//}
//
//const cpgf::GMetaMethod* ReflectedClass::getCallableMethod(std::string signature) const
//{
//	return getItem(allMethods, ReflectionUtil::correctSignature(signature));
//}


bool ReflectedClass::hasBaseClass(std::string name, VisibilityAccessType vis) const {
	return exists(baseClasses, ReflectionUtil::correctType(name),
				vis, true);
	//return (baseClasses.count(ReflectionUtil::correctType(name)) == 1);
}

size_t ReflectedClass::getBaseClassCount(VisibilityAccessType vis) const
{
	return countItems(baseClasses, vis, true);
}


const std::vector<std::string> ReflectedClass::getBaseClassNames(
		VisibilityAccessType vis) const
{
	return getNames(baseClasses, vis, true);
}

const ReflectedBaseClass * ReflectedClass::getBaseClass(std::string name) const
{
	return getItem(baseClasses, ReflectionUtil::correctType(name));
}



//bool ReflectedObject::invokeReturnTest(std::string methodSignature,
//	int & returnVal)
//{
//
//return LookingGlass::invokeInternalTest(this, methodSignature, true, returnVal) == LookingGlass::INVOKE_SUCCESS;
//}





}
