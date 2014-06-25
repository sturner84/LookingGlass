/*
 * lgReflectedClass.cpp
 *
 *  Created on: Nov 6, 2013
 *      Author: Administrator
 */


#include "lgReflectedClass.h"
#include "lgReflectionSupport.h"
#include "lgReflectedData.h"

namespace cpptesting {

//ReflectedObjectReference ReflectedObjectReference::instance; //singleton
//
//ReflectedObjectReference::ReflectedObjectReference() {
//	//do nothing
//}
//
//
//ReflectedObjectReference ReflectedObjectReference::getInstance() {
//	return instance;
//}
//
//
//void ReflectedObjectReference::addObject(const ReflectedClass * rClass,
//		ReflectedObjectPtr rObject) {
//
//	std::map<void *, boost::weak_ptr<ReflectedObject> > & objList
//		= createdObjects[rClass];
//
//	boost::weak_ptr<ReflectedObject> cPointer(rObject);
//	objList[rObject.get()->getObject()] = cPointer;
//}
//
//
//ReflectedObjectPtr ReflectedObjectReference::getObject(
//		const ReflectedClass * rClass,
//		void * ptr) {
//
//	std::map<void *, boost::weak_ptr<ReflectedObject> > & objList
//			= createdObjects[rClass];
//
//	std::map<void *, boost::weak_ptr<ReflectedObject> >::iterator found
//			= objList.find(ptr);
//
//		if (found != objList.end()) {
//			ReflectedObjectPtr newPtr(found->second.lock());
//
//			if (newPtr) {
//				return newPtr;
//			}
//			else {
//				//invalid so, delete it and return null
//				objList.erase(found);
//			}
//		}
//		ReflectedObjectPtr nullPtr;
//		return nullPtr;
//}
//
//
//bool ReflectedObjectReference::hasObject(const ReflectedClass * rClass,
//		void * ptr) {
//
//	std::map<void *, boost::weak_ptr<ReflectedObject> > & objList
//				= createdObjects[rClass];
//
//	return objList.count(ptr) != 0;
//}




ReflectedBaseClass::ReflectedBaseClass(const ReflectedClass * c, int modifiers) :
				ReflectedItem(c->getName(), c->getName(), true, false, Public,
						Class, modifiers)
{
	mClass = c;
	ClassSignature sig(signature);
	name = sig.getName();
}

ReflectedBaseClass::ReflectedBaseClass(ClassSignature sig, VisibilityType vis,
		int modifiers) :
				ReflectedItem(sig, sig, true, false, vis, Class, modifiers)
{
	mClass = NULL;
	//	name = nName;
	name = sig.getName();
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


/**
 * @brief Constructor for the object
 *
 * @param c Class from which this was created
 * @param obj Object returned from creating the reflected class
 */
//ReflectedObject::ReflectedObject(const ReflectedClass * c, const cpgf::GVariant & obj) {
//	mClass = c;
//	gObject = obj;
//}


/**
 * Gets the reflected class that is referred to in the signature (if
 * possible).  Returns NULL if it is not a reflected type.
 *
 * @param signature Signature of the function/method
 * @return Reflected class or NULL
 */
const ReflectedClass * ReflectedObject::getReturnTypeFromSignature(
		MethodSignature signature) {
	TypeSignature returnType = signature.getReturnType();

	ReflectedData* data = ReflectedData::getDataInstance();
//TODO modify this so that it gets inner classes as appropriate????
	return data->getClassFromType(returnType);
}

/**
 * Determines if the return value should be copied (i.e. it isn't a
 * reference or a pointer.
 *
 * @param signature Signature of the function/method
 * @return true if it should be copied
 */
bool ReflectedObject::shouldCopyReturnValue(MethodSignature signature) {
	TypeSignature returnType = signature.getReturnType();

	return !returnType.isPointerType()
	&& !returnType.isReferenceType();
}

/**
 * Creates a copy of the Return Value
 *
 * @param returnValue value to copy
 * @return a copy of the parameter or, if it shouldn't be copied,
 * 	the original value
 */
cpgf::GVariant ReflectedObject::copyReturnValue(const ReflectedClass * rClass,
		cpgf::GVariant returnValue) {

	ClassSignature cSig = ClassSignature(rClass->getName());
	MethodSignature copyName = cSig.createCopyFunctionSignature();
	if (rClass->doesNonReflectedMethodExist(copyName)) {
		const ReflectedMethod * cMethod = rClass->getNonReflectedMethod(
				copyName);

		cpgf::GVariant copyVal = cMethod->getMethod()->invoke(
				cpgf::fromVariant<void *>(returnValue), returnValue);

		return copyVal;
	}

	return returnValue;
}


ReflectedObject::~ReflectedObject()
{
	mClass->getClass()->destroyInstance(mObject);
	//	mClass->getClass()->destroyInstance(cpgf::fromVariant<void *>(gObject));
}


const ReflectedClass * ReflectedObject::getClass() const
{
	return mClass;
}

void * ReflectedObject::getObject() const
{
	return mObject;
	//	return cpgf::fromVariant<void *>(gObject);
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
		bool ReflectedObject::invoke(MethodSignature methodSignature \
				GPP_COMMA_IF(N) \
				GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p)) \
				{ \
	int r; \
	return LookingGlass::invokeInternal(this, methodSignature, false, r, false \
			GPP_COMMA_IF(N) \
			GPP_REPEAT(N, GPP_COMMA_PARAM, p)) == LookingGlass::INVOKE_SUCCESS; \
				}

GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE



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
//const ReflectedClass * rClass,
#define REF_INVOKE(N, unused) \
		bool ReflectedObject::invokeReturn(MethodSignature methodSignature, \
				ReflectedObjectPtr & returnVal \
				GPP_COMMA_IF(N) \
				GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p)) \
				{ \
	const ReflectedClass * rClass \
	= getReturnTypeFromSignature(methodSignature); \
	if (rClass == NULL) { \
		return LookingGlass::INVOKE_BAD_CLASS; \
	} \
	cpgf::GVariant resultVal; \
	bool result = LookingGlass::invokeInternal(this, \
			methodSignature, true, resultVal, false GPP_COMMA_IF(N) \
			GPP_REPEAT(N, GPP_COMMA_PARAM, p)) == LookingGlass::INVOKE_SUCCESS; \
			if (result) { \
				if (shouldCopyReturnValue(methodSignature)) { \
					resultVal = copyReturnValue(rClass, resultVal); \
				} \
				returnVal = rClass->castTo(resultVal);   \
			} \
			return result; \
				}

GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE


ReflectedObject::operator void *() {
	return getObject();
}


ReflectedObject::operator cpgf::GVariant(){
	return cpgf::GVariant(getObject());
}


//ReflectedObjectPtr object(this);


void ReflectedClass::loadData(const cpgf::GMetaClass* gclass, bool isGlobal)
{
	ReflectedDataBase::loadData(gclass, false);
//	std::string signature;

	for (size_t i = 0; i < gclass->getConstructorCount(); i++)
	{
		//		CPGF_TRACE("Adding Constructors")
		MethodSignature mSignature(gclass, gclass->getConstructorAt(i));
		constructors[mSignature]
		             = new ReflectedConstructor(gclass->getConstructorAt(i), false);
		ReflectedNamespace::addItem(gclass->getConstructorAt(i)->getNamespace(),
				constructors[mSignature]);
	}


	if (gclass->getClassCount() > 0)
	{
		for (size_t i = 0; i < gclass->getClassCount(); i++)
		{
			//CPGF_TRACE("Loading " << i << " " << gclass->getClassAt(i)->getName() << "\r\n")
			//create new ReflectedClass objects with these
			//but don't add the global class
			//if (cpgf::getGlobalMetaClass() != gclass->getClassAt(i)) {
			ReflectedClass * innerClass = new ReflectedClass(
					gclass->getClassAt(i), false);
			ClassSignature cSig(gclass->getClassAt(i));
			innerClasses[cSig.getSignature()] = innerClass;
			//			CPGF_TRACE("Namespace " << i << " " << gclass->getClassAt(i)->getName() << "\r\n")

			ReflectedNamespace::addItem(gclass->getClassAt(i)->getNamespace(),
					innerClasses[cSig.getSignature()]);
			//}
			//add subclasses
			//			CPGF_TRACE("Loading " << i << " " << gclass->getClassAt(i)->getName() << " subclasses" << "\r\n")

			innerClass->loadData(gclass->getClassAt(i), false);
			//			CPGF_TRACE("Loading " << i << " " << gclass->getClassAt(i)->getName() << " done" << "\r\n")

		}
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
	//	CPGF_TRACE("Done Class")
}


void ReflectedClass::loadBaseMethods(const std::map<std::string,
		const ReflectedMethod *> & baseMethods,
		VisibilityType publicVis,
		VisibilityType protectedVis)
{
	//add all methods from this class
	//	CPGF_TRACE("Loading Base Methods " << "\r\n")
	for (std::map<std::string, const ReflectedMethod *>::const_iterator i
			= baseMethods.begin(); i != baseMethods.end(); i++) {
		//		CPGF_TRACE("Loading Base Methods " << i->first << "\r\n")
		//new method that isn't a destructor
		MethodSignature mSignature(i->first);
		if (methods.count(i->first) == 0
				&& !mSignature.isDestructor()) {
			//			CPGF_TRACE("Creating new Base Methods " << i->first << "\r\n")
			//it has to be originally public and be being mapped to a public
			//visibility.  With protected/private inheritance it may not be
			//visibile anymore.
			if (i->second->isAccessible() && publicVis == Public) { //is reflected
				methods[i->first] = new ReflectedMethod(
						i->second->getMethod(), true, false);
			}
			else {
				VisibilityType vis = i->second->getVisibility();
				if (vis == Public) {
					vis = publicVis;
				} else {
					if (vis == Protected) {
						vis = protectedVis;
					}
				}
				methods[i->first] = new ReflectedMethod(i->first, true, false,
						vis, i->second->getModifiers());
			}
			//			CPGF_TRACE("Created new Base Methods " << i->first << "\r\n")
		}
	}
}


void ReflectedClass::loadBaseEnums(const std::map<std::string,
		const ReflectedEnum *> & baseEnums,
		VisibilityType publicVis,
		VisibilityType protectedVis)
{
	//add all enums from this class
	for (std::map<std::string, const ReflectedEnum *>::const_iterator i
			= baseEnums.begin(); i != baseEnums.end(); i++) {
		if (enums.count(i->first) == 0) { //new enum
			//it has to be originally public and be being mapped to a public
			//visibility.  With protected/private inheritance it may not be
			//visible anymore.
			if (i->second->isAccessible() && publicVis == Public)  { //is reflected
				enums[i->first] = new ReflectedEnum(
						i->second->getEnum(), true, false);
			}
			else {
				VisibilityType vis = i->second->getVisibility();
				if (vis == Public) {
					vis = publicVis;
				} else {
					if (vis == Protected) {
						vis = protectedVis;
					}
				}
				enums[i->first] = new ReflectedEnum(i->first, true, false,
						vis, i->second->getModifiers());
			}
		}
	}
}

//void ReflectedClass::loadBaseOperators(std::map<std::string,
//		const ReflectedOperator *> & baseOperators)
//{
//	//add all operators from this class
//	for (std::map<std::string, const ReflectedOperator *>::const_iterator i
//			= baseOperators.begin(); i != baseOperators.end(); i++) {
//		if (operators.count(i->first) == 0) { //new operator
//			operators[i->first] = new ReflectedOperator(
//					i->second->getOperator(), true);
//		}
//	}
//}

void ReflectedClass::loadBaseFields(const std::map<std::string,
		const ReflectedField *> & baseFields,
		VisibilityType publicVis,
		VisibilityType protectedVis)
{
	//add all fields from this class
	for (std::map<std::string, const ReflectedField *>::const_iterator i
			= baseFields.begin(); i != baseFields.end(); i++) {
		if (fields.count(i->first) == 0) { //new field
			//it has to be originally public and be being mapped to a public
			//visibility.  With protected/private inheritance it may not be
			//visibile anymore.
			if (i->second->isAccessible() && publicVis == Public)  { //is reflected
				fields[i->first] = new ReflectedField(
						i->second->getField(), true, false);
			}
			else {
				VisibilityType vis = i->second->getVisibility();
				if (vis == Public) {
					vis = publicVis;
				} else {
					if (vis == Protected) {
						vis = protectedVis;
					}
				}
				fields[i->first] = new ReflectedField(i->first, true, false,
						vis, i->second->getModifiers());
			}
		}
	}
}


void ReflectedClass::loadBaseClasses(std::map<std::string,
		ReflectedClass *> & classes) {
//	std::string signature;

	for (size_t i = 0; i < mClass->getBaseCount(); i++)
	{
		const cpgf::GMetaClass* baseClass = mClass->getBaseClass(i);
		int modifiers = mClass->getBaseClassModifiers(i);
		//std::string name = getName();
		//		CPGF_TRACE("Loading Base Classes " << i << "\r\n")
		if (baseClass) {
			ClassSignature cSignature(baseClass->getName());
			//			CPGF_TRACE("Loading Base Classes " << signature << "\r\n")
			if (classes.count(cSignature) == 1) {
				baseClasses[cSignature] = new ReflectedBaseClass(
						classes[cSignature], modifiers);
				//				CPGF_TRACE("Loading Base Classes created " << signature << "\r\n")
				//check if its classes have been loaded
				if (classes[cSignature]->baseClasses.size() == 0) {
					//may not have been, so load it
					classes[cSignature]->loadBaseClasses(classes);
				}
				//				CPGF_TRACE("Loading Base Classes loaded " << signature << "\r\n")
				// add all methods, fields, operators, enums
				loadBaseMethods(classes[cSignature]->methods);
				//				CPGF_TRACE("Loading Base Classes loaded methods " << signature << "\r\n")
				loadBaseFields(classes[cSignature]->fields);
				//				CPGF_TRACE("Loading Base Classes loaded field " << signature << "\r\n")
				//				loadBaseOperators(classes[signature]->operators);
				loadBaseEnums(classes[cSignature]->enums);
				//				CPGF_TRACE("Loading Base Classes loaded methods " << signature << "\r\n")
			}
			else { //nothing reflected
				//baseClasses[signature] = NULL;
				//				CPGF_TRACE("Loading Base Classes non-reflected " << signature << "\r\n")
				baseClasses[cSignature] = new ReflectedBaseClass(cSignature,
						Public, modifiers);
			}
		}
		//		CPGF_TRACE("Loading Base Classes " << signature << " done\r\n")
	}

}


/**
 * Loads a single non-reflected item like private methods, non-reflected
 * base classes, etc.
 *
 * @param nrItem item to load
 */
void ReflectedClass::loadNonReflectedData(
		const cpgf::GMetaNonReflectedItem * nrItem) {
	std::string sig = nrItem->getSignature();
	VisibilityType vis = ReflectedItem::cpgfToLGVisibility(
			nrItem->getVisibility());

	//			switch (nrItem->getVisibility()) {
	//
	//			}
	//CPGF_TRACE("Constructor \r\n")
	switch (nrItem->getItemCategory()) {
	case cpgf::mcatConstructor:
	{
		MethodSignature cSignature(sig);
		constructors[cSignature]
		             = new ReflectedConstructor(cSignature, false, vis,
		            		 nrItem->getModifiers());
		ReflectedNamespace::addItem(nrItem->getNamespace(),
				constructors[cSignature]);
		break;
	}
	case cpgf::mcatBaseClass:
	{
		ClassSignature bcSignature(sig);
		baseClasses[bcSignature] = new ReflectedBaseClass(bcSignature, vis,
				nrItem->getModifiers());
		break;
	}
	case cpgf::mcatClass: //inner class
	{
		ClassSignature icSignature(sig);
		innerClasses[icSignature] = new ReflectedClass(icSignature, false, vis,
				nrItem->getModifiers());
		break;
	}
	default:
		ReflectedDataBase::loadNonReflectedData(nrItem);
	}
}


void ReflectedClass::loadNonReflectedBaseClassMembers(
		const std::map<std::string, ReflectedClass *> & allClasses) {
	for (std::map<std::string, const ReflectedBaseClass *>::const_iterator i
			= baseClasses.begin(); i != baseClasses.end(); i++) {
		//		CPGF_TRACE("Loading Base Methods " << i->first << "\r\n")
		loadNonReflectedBaseClassMembers(allClasses, i->first,
				i->second->getVisibility());
	}
}


/**
 * Loads info about baseclasses that are not-reflected.
 *
 * In cases where you have protected or private inheritance, this method
 * loads the members of that (reflected) class into the inheriting class with
 * the appropriate deduction in visibility.
 *
 * @param signature Name of base class
 * @param baseVis visibility of of the base class
 */
void ReflectedClass::loadNonReflectedBaseClassMembers(
		const std::map<std::string, ReflectedClass *> & allClasses,
		std::string signature,
		VisibilityType baseVis) {

	std::map<std::string, ReflectedClass *>::const_iterator c
	= allClasses.find(signature);
	if (c != allClasses.end() && (baseVis == Protected || baseVis == Private)) {
		ReflectedClass * rClass = c->second;;
		//if public inheritance, item's visibility stays the same
		//but since this should have been loaded elsewhere or the class is
		//not reflected, ignore it.
		//if protected inheritance, public items -> protected
		VisibilityType publicVis = Protected;
		VisibilityType protectedVis = Protected;
		if (baseVis == Private) {
			//if private inheritance, public, protected items -> private
			publicVis = Private;
			protectedVis = Private;
		}

		loadBaseMethods(rClass->methods, publicVis, protectedVis);
		loadBaseFields(rClass->fields, publicVis, protectedVis);
		loadBaseEnums(rClass->enums, publicVis, protectedVis);
	}
}



/**
 * Keeps a copy of the objects being created so that values cast to
 * this class that reference the same object will not delete the object
 * at the wrong time.
 *
 * @param ptr Pointer to store as a weak reference
 */
void ReflectedClass::storeObjectPtr(ReflectedObjectPtr ptr) const {
	//	ReflectedObjectReference ror = ReflectedObjectReference::getInstance();
	//	ror.addObject(this, ptr);

	boost::weak_ptr<ReflectedObject> cPointer(ptr);
	createdObjects[ptr.get()->getObject()] = cPointer;
}

/**
 * Finds if this object already exists. If so, returns a strong pointer to
 * it.  Otherwise, returns NULL.
 *
 * @param ptr Pointer to find
 * @return Reference to the pointer or NULL
 */
ReflectedObjectPtr ReflectedClass::getStoredObject(void * ptr) const {

	//	ReflectedObjectReference ror = ReflectedObjectReference::getInstance();
	//	return ror.getObject(this, ptr);
	//

	std::map<void *, boost::weak_ptr<ReflectedObject> >::iterator found
	= createdObjects.find(ptr);

	if (found != createdObjects.end()) {
		ReflectedObjectPtr newPtr(found->second.lock());

		if (newPtr) {
			return newPtr;
		}
		else {
			//invalid so, delete it and return null
			createdObjects.erase(found);
		}
	}
	ReflectedObjectPtr nullPtr;
	return nullPtr;
}

/**
 * Determines if this pointer exists already.
 *
 * @param ptr Pointer to find.
 * @return true if it exists.
 */
bool ReflectedClass::hasStoredObject(void * ptr) const {

	//	ReflectedObjectReference ror = ReflectedObjectReference::getInstance();
	//	return ror.hasObject(this, ptr);
	bool found = false;
	if (createdObjects.count(ptr) != 0 && getStoredObject(ptr) != NULL) {
		found = true;
	}
	return found;
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
		const ReflectedObjectPtr ReflectedClass::create(MethodSignature signature \
				GPP_COMMA_IF(N) \
				GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p)) const \
				{ \
	if (doesConstructorExist(signature, ItemFilter(false))) \
	{ \
		const cpgf::GMetaConstructor * construct = getConstructor(signature, ItemFilter(false)) \
		->getConstructor(); \
		void * object = construct->invoke( \
				GPP_REPEAT(N, GPP_COMMA_PARAM, p)); \
				ReflectedObjectPtr ptr (new ReflectedObject(this, object)); \
				storeObjectPtr(ptr); \
				return ptr; \
	} \
	ReflectedObjectPtr ptr; \
	return ptr; \
				}

GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE


//
//	bool ReflectedClass::instanceOf(ReflectedObjectPtr rObject) const {
//
//}



ReflectedObjectPtr ReflectedClass::castTo(const cpgf::GVariant & object) const {
	//	if (hasStoredObject(cpgf::fromVariant<void *>(object))) {
	//			return getStoredObject(cpgf::fromVariant<void *>(object));
	//		}
	//
	//	ReflectedObjectPtr ptr(new ReflectedObject(this, object));
	//	return ptr;
	return castTo(cpgf::fromVariant<void *>(object));
}

ReflectedObjectPtr ReflectedClass::castTo(void * object) const {

	if (hasStoredObject(object)) {
		return getStoredObject(object);
	}

	ReflectedObjectPtr ptr(new ReflectedObject(this, object));
	return ptr;
}


///**
// * Gets the class name
// *
// * @return Name of the class
// */
//const std::string ReflectedClass::getName() const
//{
//	return mClass->getName();
//}



ReflectedClass::ReflectedClass(const cpgf::GMetaClass* c, bool isGlobal) :
				ReflectedItem(c->getName(), c->getName(), false, isGlobal, Public, Class,
						c->getModifiers())
{
	mClass = c;
	loadData(mClass, false);
	//get rid of the static modifier
	mods &= ~cpgf::metaModifierStatic;

	ClassSignature name(c);
	signature = name.getSignature();
}


/**
 * Creates non-reflected class
 *
 * @param c Reflected class
 */
ReflectedClass::ReflectedClass(ClassSignature name, bool isGlobal,
		VisibilityType vis, int modifiers) :
			ReflectedItem(name.getName(), name, false, isGlobal, vis,
					Class, modifiers) {
	mClass = NULL;
	//get rid of the static modifier
	mods &= ~cpgf::metaModifierStatic;
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

	for (std::map<std::string, ReflectedClass*>::iterator it
			= innerClasses.begin(); it != innerClasses.end(); it++)
	{
		delete it->second;
	}
}

const cpgf::GMetaClass* ReflectedClass::getClass() const
{
	return mClass;
}


bool ReflectedClass::doesMethodExist(MethodSignature functionSignature,
		ItemFilter filter) const
{
	return ReflectedDataBase::doesMethodExist(functionSignature, filter);
}

size_t ReflectedClass::getMethodCount(ItemFilter filter) const
{
	return ReflectedDataBase::getMethodCount(filter);
}


const std::vector<std::string> ReflectedClass::getMethodNames(
		ItemFilter filter) const
{
	return ReflectedDataBase::getMethodNames(filter);
}

const ReflectedMethod * ReflectedClass::getMethod(MethodSignature signature,
		ItemFilter filter) const
{
	return ReflectedDataBase::getMethod(signature, filter);
}


std::vector<const ReflectedMethod *> ReflectedClass::getMethods(
		ItemFilter filter)
{
	return getItems(methods, filter);
}

std::vector<const ReflectedMethod *> ReflectedClass::getClosestMethods(
		MethodSignature name, ItemFilter filter, int count)
{
	return getClosest(methods, name, filter, count);
}

std::string ReflectedClass::getClosestMethodsString(
		MethodSignature name, ItemFilter filter, int count)
{
	return getClosestString(methods, name, filter, count);
}

bool ReflectedClass::doesFieldExist(FieldSignature signature,
		ItemFilter filter) const
{
	return ReflectedDataBase::doesFieldExist(signature, filter);
}

size_t ReflectedClass::getVariableCount(ItemFilter filter) const
{
	return ReflectedDataBase::getVariableCount(filter);
}

const std::vector<std::string> ReflectedClass::getVariableNames(
		ItemFilter filter) const
{
	return ReflectedDataBase::getVariableNames(filter);
}

size_t ReflectedClass::getConstantCount(ItemFilter filter) const
{
	return ReflectedDataBase::getConstantCount(filter);
}

const std::vector<std::string> ReflectedClass::getConstantNames(
		ItemFilter filter) const
{
	return ReflectedDataBase::getConstantNames(filter);
}


size_t ReflectedClass::getFieldCount(ItemFilter filter) const
{
	return ReflectedDataBase::getFieldCount(filter);
}

const std::vector<std::string> ReflectedClass::getFieldNames(
		ItemFilter filter) const
{
	return ReflectedDataBase::getFieldNames(filter);
}


const ReflectedField * ReflectedClass::getField(FieldSignature signature,
		ItemFilter filter) const
{
	return ReflectedDataBase::getField(signature, filter);
}


std::vector<const ReflectedField *> ReflectedClass::getFields(
		ItemFilter filter)
{
	return getItems(fields, filter);
}

std::vector<const ReflectedField *> ReflectedClass::getClosestFields(
		FieldSignature name, ItemFilter filter, int count)
{
	return getClosest(fields, name, filter, count);
}

std::string ReflectedClass::getClosestFieldsString(
		FieldSignature name, ItemFilter filter, int count)
{
	return getClosestString(fields, name, filter, count);
}

bool ReflectedClass::doesConstructorExist(MethodSignature signature,
		ItemFilter filter) const
{
	return exists(constructors, signature, filter);
	//return (constructors.count(ReflectionUtil::correctSignature(signature)) == 1);
}

size_t ReflectedClass::getConstructorCount(ItemFilter filter) const
{
	return countItems(constructors, filter);
}

const std::vector<std::string> ReflectedClass::getConstructorNames(
		ItemFilter filter) const
{
	return getNames(constructors, filter);
}

const ReflectedConstructor * ReflectedClass::getConstructor(
		MethodSignature signature, ItemFilter filter) const
{
	return getItem(constructors, signature, filter);
}


std::vector<const ReflectedConstructor *> ReflectedClass::getConstructors(
		ItemFilter filter)
{
	return getItems(constructors, filter);
}

std::vector<const ReflectedConstructor *> ReflectedClass::getClosestConstructors(
		MethodSignature name, ItemFilter filter, int count)
{
	return getClosest(constructors, name, filter, count);
}

std::string ReflectedClass::getClosestConstructorsString(
		MethodSignature name, ItemFilter filter,  int count)
{
	return getClosestString(constructors, name, filter, count);
}



bool ReflectedClass::hasBaseClass(ClassSignature name, ItemFilter filter) const {
	return exists(baseClasses, name, filter);
	//return (baseClasses.count(ReflectionUtil::correctType(name)) == 1);
}

size_t ReflectedClass::getBaseClassCount(ItemFilter filter) const
{
	return countItems(baseClasses, filter);
}


const std::vector<std::string> ReflectedClass::getBaseClassNames(
		ItemFilter filter) const
{
	return getNames(baseClasses, filter);
}

const ReflectedBaseClass * ReflectedClass::getBaseClass(ClassSignature name,
		ItemFilter filter) const
{
	return getItem(baseClasses, name, filter);
}



std::vector<const ReflectedBaseClass *> ReflectedClass::getClosestBaseClasses(
		ClassSignature name, ItemFilter filter, int count) {
	return getClosest(baseClasses, name, filter, count);
}



std::string ReflectedClass::getClosestBaseClassesString(
		ClassSignature name, ItemFilter filter, int count) {
	return getClosestString(baseClasses, name, filter, count);
}


bool ReflectedClass::hasInnerClass(ClassSignature name,
		ItemFilter filter) const {
	return exists(innerClasses, name, filter);
}


size_t ReflectedClass::getInnerClassCount(ItemFilter filter) const {
	return countItems(innerClasses, filter);
}



const std::vector<std::string> ReflectedClass::getInnerClassNames(
		ItemFilter filter) const {
	return getNames(innerClasses, filter);
}


ReflectedClass * ReflectedClass::getInnerClass(ClassSignature name,
		ItemFilter filter) const {
	return getItem(innerClasses, name, filter);
}



std::vector<ReflectedClass *> ReflectedClass::getClosestInnerClasses(
		ClassSignature name, ItemFilter filter, int count) {
	return getClosest(innerClasses, name, filter, count);
}



std::string ReflectedClass::getClosestInnerClassesString(
		ClassSignature name, ItemFilter filter, int count){
	return getClosestString(innerClasses, name, filter, count);
}




bool operator==(ReflectedObjectPtr const & p1, ReflectedObjectPtr const & p2) {
	if (p1 == NULL) {
		if (p2 == NULL) {
			return true;
		}
		if (p2.get()->getObject() == NULL) {
			return true;
		}

		return false;
	}

	if (p2 == NULL) {
		if (p1.get()->getObject() == NULL) {
			return true;
		}
		return false;
	}

	return p1.get()->getObject() == p2.get()->getObject();
}


}
