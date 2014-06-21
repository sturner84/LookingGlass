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

	ClassInfo info = ReflectionUtil::divideClassSignature(signature);
	name = info.name;
}

ReflectedBaseClass::ReflectedBaseClass(std::string sig, VisibilityType vis,
		int modifiers) :
				ReflectedItem(sig, sig, true, false, vis, Class, modifiers)
{
	mClass = NULL;
	//	name = nName;
	ClassInfo info = ReflectionUtil::divideClassSignature(signature);
	name = info.name;
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
		std::string signature) {
	std::string returnType = ReflectionUtil::getReturnType(signature);

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
bool ReflectedObject::shouldCopyReturnValue(std::string signature) {
	std::string returnType = ReflectionUtil::getReturnType(signature);

	return !ReflectionUtil::isPointerType(returnType)
	&& !ReflectionUtil::isReferenceType(returnType);
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

	std::string copyName = ReflectionUtil::createCopyFunctionSignature(
			rClass->getName());
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
		bool ReflectedObject::invoke(std::string methodSignature \
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
		bool ReflectedObject::invokeReturn(std::string methodSignature, \
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
	std::string signature;

	for (size_t i = 0; i < gclass->getConstructorCount(); i++)
	{
		//		CPGF_TRACE("Adding Constructors")
		signature = ReflectionUtil::correctSignature(
				ReflectionUtil::createConstructorSignature(
						gclass, gclass->getConstructorAt(i)));
		constructors[signature]
		             = new ReflectedConstructor(gclass->getConstructorAt(i), false);
		ReflectedNamespace::addItem(gclass->getConstructorAt(i)->getNamespace(),
				constructors[signature]);
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
			innerClasses[gclass->getClassAt(i)->getName()] = innerClass;
			//			CPGF_TRACE("Namespace " << i << " " << gclass->getClassAt(i)->getName() << "\r\n")

			ReflectedNamespace::addItem(gclass->getClassAt(i)->getNamespace(),
					innerClasses[gclass->getClassAt(i)->getName()]);
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
		if (methods.count(i->first) == 0
				&& !ReflectionUtil::isDestructor(i->first)) {
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
			//visibile anymore.
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
	std::string signature;

	for (size_t i = 0; i < mClass->getBaseCount(); i++)
	{
		const cpgf::GMetaClass* baseClass = mClass->getBaseClass(i);
		int modifiers = mClass->getBaseClassModifiers(i);
		//std::string name = getName();
		//		CPGF_TRACE("Loading Base Classes " << i << "\r\n")
		if (baseClass) {
			signature = ReflectionUtil::correctType(baseClass->getName());
			//			CPGF_TRACE("Loading Base Classes " << signature << "\r\n")
			if (classes.count(signature) == 1) {
				baseClasses[signature] = new ReflectedBaseClass(
						classes[signature], modifiers);
				//				CPGF_TRACE("Loading Base Classes created " << signature << "\r\n")
				//check if its classes have been loaded
				if (classes[signature]->baseClasses.size() == 0) {
					//may not have been, so load it
					classes[signature]->loadBaseClasses(classes);
				}
				//				CPGF_TRACE("Loading Base Classes loaded " << signature << "\r\n")
				// add all methods, fields, operators, enums
				loadBaseMethods(classes[signature]->methods);
				//				CPGF_TRACE("Loading Base Classes loaded methods " << signature << "\r\n")
				loadBaseFields(classes[signature]->fields);
				//				CPGF_TRACE("Loading Base Classes loaded field " << signature << "\r\n")
				//				loadBaseOperators(classes[signature]->operators);
				loadBaseEnums(classes[signature]->enums);
				//				CPGF_TRACE("Loading Base Classes loaded methods " << signature << "\r\n")
			}
			else { //nothing reflected
				//baseClasses[signature] = NULL;
				//				CPGF_TRACE("Loading Base Classes non-reflected " << signature << "\r\n")
				baseClasses[signature] = new ReflectedBaseClass(signature,
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
	std::string signature = nrItem->getSignature();
	VisibilityType vis = ReflectedItem::cpgfToLGVisibility(
			nrItem->getVisibility());

	//			switch (nrItem->getVisibility()) {
	//
	//			}
	//CPGF_TRACE("Constructor \r\n")
	switch (nrItem->getItemCategory()) {
	case cpgf::mcatConstructor: //move
		signature = ReflectionUtil::correctSignature(signature);
		constructors[signature]
		             = new ReflectedConstructor(signature, false, vis,
		            		 nrItem->getModifiers());
		ReflectedNamespace::addItem(nrItem->getNamespace(),
				constructors[signature]);
		break;
	case cpgf::mcatBaseClass:
		signature = ReflectionUtil::correctClassSignature(signature);
		baseClasses[signature] = new ReflectedBaseClass(signature, vis,
				nrItem->getModifiers());
		break;
	case cpgf::mcatClass: //inner class
		signature = ReflectionUtil::correctClassSignature(signature);
		innerClasses[signature] = new ReflectedClass(signature, false, vis,
				nrItem->getModifiers());
		break;
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

//TODO call this from ReflectedData after everything else has been loaded
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
		const ReflectedObjectPtr ReflectedClass::create(std::string signature \
				GPP_COMMA_IF(N) \
				GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p)) const \
				{ \
	if (doesConstructorExist(signature, Public_Access, ALLOW_ALL_MODIFIERS, false)) \
	{ \
		const cpgf::GMetaConstructor * construct = getConstructor(signature, ALLOW_ALL_MODIFIERS, false) \
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
	//TODO create full signature?
}


/**
 * Creates non-reflected class
 *
 * @param c Reflected class
 */
ReflectedClass::ReflectedClass(std::string name, bool isGlobal,
		VisibilityType vis, int modifiers) :
			ReflectedItem(name, name, false, isGlobal, vis, Class, modifiers) {
	mClass = NULL;
	//get rid of the static modifier
	mods &= ~cpgf::metaModifierStatic;
	//TODO create full signature?
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


bool ReflectedClass::doesMethodExist(std::string functionSignature,
		VisibilityAccessType vis, bool inherit, int modifiers,
		bool allowMoreMods) const
{
	return ReflectedDataBase::doesMethodExist(functionSignature, vis, inherit,
			modifiers, allowMoreMods);
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

const ReflectedMethod * ReflectedClass::getMethod(std::string signature,
		int modifiers, bool allowMoreMods) const
{
	return ReflectedDataBase::getMethod(signature, modifiers, allowMoreMods);
}


std::vector<const ReflectedMethod *> ReflectedClass::getMethods(
		VisibilityAccessType vis, bool inherited, int modifiers,
		bool allowMoreMods)
{
	return getItems(methods, vis, inherited, modifiers, allowMoreMods);
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
		VisibilityAccessType vis, bool inherit, int modifiers,
		bool allowMoreMods) const
{
	return ReflectedDataBase::doesFieldExist(signature, vis, inherit, modifiers,
			allowMoreMods);
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


const ReflectedField * ReflectedClass::getField(std::string signature,
		int modifiers, bool allowMoreMods) const
{
	return ReflectedDataBase::getField(signature, modifiers, allowMoreMods);
}


std::vector<const ReflectedField *> ReflectedClass::getFields(
		VisibilityAccessType vis, bool inherited, int modifiers,
		bool allowMoreMods)
{
	return getItems(fields, vis, inherited, modifiers, allowMoreMods);
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
		VisibilityAccessType vis, int modifiers,
		bool allowMoreMods) const
{
	return exists(constructors, ReflectionUtil::correctSignature(signature),
			vis, true, modifiers, allowMoreMods);
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
		std::string signature, int modifiers,
		bool allowMoreMods) const
{
	return getItem(constructors, ReflectionUtil::correctSignature(signature),
			modifiers, allowMoreMods);
}


std::vector<const ReflectedConstructor *> ReflectedClass::getConstructors(
		VisibilityAccessType vis, int modifiers, bool allowMoreMods)
{
	return getItems(constructors, vis, true, modifiers, allowMoreMods);
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


bool ReflectedClass::hasBaseClass(std::string name, VisibilityAccessType vis,
		int modifiers, bool allowMoreMods) const {
	return exists(baseClasses, ReflectionUtil::correctType(name),
			vis, true, modifiers, allowMoreMods);
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

const ReflectedBaseClass * ReflectedClass::getBaseClass(std::string name,
		int modifiers, bool allowMoreMods) const
{
	return getItem(baseClasses, ReflectionUtil::correctType(name), modifiers,
			allowMoreMods);
}


/**
 * Gets a list of the closest base class to the name provided
 *
 * @param name Name to find
 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
 * @param count Maximum number to return (may be less or more if there
 *  are values that are equally close to the name)
 *
 * @return List of base classes that are close in name to the name given
 */
std::vector<const ReflectedBaseClass *> ReflectedClass::getClosestBaseClasses(
		std::string name, VisibilityAccessType vis, int count) {
	return getClosest(baseClasses, name, vis, true, count);
}


/**
 * Gets a string comparing the name to the closest names found
 *
 * @param name Name to find
 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
 * @param count Maximum number to return (may be less or more if there
 *  are values that are equally close to the name)
 *
 * @return String with a list of base classes that are close in name
 * to the name given
 */
std::string ReflectedClass::getClosestBaseClassesString(
		std::string name, VisibilityAccessType vis, int count) {
	return getClosestString(baseClasses, name, vis, true, count);
}

/**
 * Checks to see if the class has a (public) base class by the name of name.
 *
 *
 *  @param name Name of the inner class
 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
 * @param modifiers set of modifiers (ORed together) that the item should
 * 	have.  Use ReflectedData::ALLOW_ALL_MODIFIERS to ignore modifiers
 * 	(default behavior).  List of modifiers from cpgf/gmetacommon.h that
 * 	apply here:
 * 		metaModifierNone
 * 		metaModifierStatic
 * 		metaModifierVirtual
 * 		metaModifierPureVirtual
 * 		metaModifierTemplate
 * 		metaModifierConst
 * 		metaModifierVolatile
 * 		metaModifierInline
 * 		metaModifierExplicit
 * 		metaModifierExtern
 * 		metaModifierMutable
 *
 * @param allowMoreMods true if the field can have other modifiers aside
 * 	from those listed.  (defaults to true)
 *  @return true is name is the name of one of the inner classes
 */
bool ReflectedClass::hasInnerClass(std::string name,
		VisibilityAccessType vis, int modifiers, bool allowMoreMods) const {
	return exists(innerClasses, ReflectionUtil::correctType(name),
				vis, true, modifiers, allowMoreMods);
}

/**
 * Gets the number of inner classes for this class.
 *
 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
 *
 * @return the number of inner classes for this class
 */
size_t ReflectedClass::getInnerClassCount(VisibilityAccessType vis) const {
	return countItems(innerClasses, vis, true);
}


/**
 * Gets a list of all inner classes for this class.
 *
 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
 *
 * @return a list of inner class names
 */
const std::vector<std::string> ReflectedClass::getInnerClassNames(
		VisibilityAccessType vis) const {
	return getNames(innerClasses, vis, true);
}

/**
 * Gets a inner class for this class.
 *
 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
 * @param modifiers set of modifiers (ORed together) that the item should
 * 	have.  Use ReflectedData::ALLOW_ALL_MODIFIERS to ignore modifiers
 * 	(default behavior).  List of modifiers from cpgf/gmetacommon.h that
 * 	apply here:
 * 		metaModifierNone
 * 		metaModifierStatic
 * 		metaModifierVirtual
 * 		metaModifierPureVirtual
 * 		metaModifierTemplate
 * 		metaModifierConst
 * 		metaModifierVolatile
 * 		metaModifierInline
 * 		metaModifierExplicit
 * 		metaModifierExtern
 * 		metaModifierMutable
 *
 * @param allowMoreMods true if the field can have other modifiers aside
 * 	from those listed.  (defaults to true)
 * @return a ReflectedClass object or NULL
 */
ReflectedClass * ReflectedClass::getInnerClass(std::string name,
		int modifiers, bool allowMoreMods) const {
	return getItem(innerClasses, ReflectionUtil::correctType(name), modifiers,
				allowMoreMods);
}


/**
 * Gets a list of the closest inner class to the name provided
 *
 * @param name Name to find
 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
 * @param count Maximum number to return (may be less or more if there
 *  are values that are equally close to the name)
 *
 * @return List of inner classes that are close in name to the name given
 */
std::vector<ReflectedClass *> ReflectedClass::getClosestInnerClasses(
		std::string name, VisibilityAccessType vis,
		int count) {
	return getClosest(innerClasses, name, vis, true, count);
}


/**
 * Gets a string comparing the name to the closest names found
 *
 * @param name Name to find
 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
 * @param count Maximum number to return (may be less or more if there
 *  are values that are equally close to the name)
 *
 * @return String with a list of inner classes that are close in name
 * to the name given
 */
std::string ReflectedClass::getClosestInnerClassesString(
		std::string name, VisibilityAccessType vis, int count){
	return getClosestString(innerClasses, name, vis, true, count);
}



//bool ReflectedObject::invokeReturnTest(std::string methodSignature,
//	int & returnVal)
//{
//
//return LookingGlass::invokeInternalTest(this, methodSignature, true, returnVal) == LookingGlass::INVOKE_SUCCESS;
//}



//bool instanceOf(const ReflectedClass * rClass, ReflectedObjectPtr rObject) {
//	if (rClass != NULL) {
//		return rClass->instanceOf(rObject);
//	}
//
//	return false;
//}



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
