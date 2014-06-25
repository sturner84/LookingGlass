/*
 * ReflectedClass.h
 *
 *  Created on: Oct 19, 2013
 *      Author: saturner
 */

#ifndef REFLECTEDCLASS_H_
#define REFLECTEDCLASS_H_

#include <cpgf/gmetaclass.h>
#include <map>
#include <string>
#include <cstddef>
#include "lgReflectedDataBase.h"
#include "lgReflectedItems.h"
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace cpptesting {

class ReflectedClass;
class ReflectedObjectPtr;
class ReflectedData;



///**
// * @class ReflectedObjectReference
// * @brief Records the references to all objects created by each of the
// * reflected classes.
// *
// * Records the references to all objects created by each of the
// * reflected classes. This allows references to those objects to be returned
// * from method and not prematurely deleted.
// */
//class ReflectedObjectReference {
//	friend class ReflectedClass;
//
//private:
//	ReflectedObjectReference();
//
//	static ReflectedObjectReference instance; //singleton
//
//	std::map<const ReflectedClass *,
//		std::map<void *, boost::weak_ptr<ReflectedObject> > > createdObjects;
//protected:
//	/**
//	 * Gets the instance of this class.
//	 * @return The single instance of this class
//	 */
//	static ReflectedObjectReference getInstance();
//
//	/**
//	 * Keeps a copy of the objects being created so that values cast to
//	 * this class that reference the same object will not delete the object
//	 * at the wrong time.
//	 *
//	 * @param rClass Class that is the type of the object
//	 * @param ptr Pointer to store as a weak reference
//	 */
//	void addObject(const ReflectedClass * rClass, ReflectedObjectPtr rObject);
//
//	/**
//	 * Finds if this object already exists. If so, returns a strong pointer to
//	 * it.  Otherwise, returns NULL.
//	 *
//	 * @param rClass Class that is the type of the object
//	 * @param ptr Pointer to find
//	 * @return Reference to the pointer or NULL
//	 */
//	ReflectedObjectPtr getObject(const ReflectedClass * rClass,
//			void * ptr);
//
//	/**
//	 * Determines if this pointer exists already.
//	 *
//	 * @param rClass Class that is the type of the object
//	 * @param ptr Pointer to find.
//	 * @return true if it exists.
//	 */
//	bool hasObject(const ReflectedClass * rClass,
//			void * ptr);
//public:
//
//};



/**
 * @class ReflectedObject
 * @brief Represents a base class of another class.  This class may or may not
 * be reflected.
 *
 * isAccessible determines if there is a reflected base class here or if it is
 * just a name.  If isAccessible is true, getClass returns a pointer to the
 * class.  If false, getClass returns null.
 */
class ReflectedBaseClass : public ReflectedItem
{
protected:
	/**
	 * Reference to the class
	 */
	const ReflectedClass * mClass;
	//	/**
	//	 * name of the class
	//	 */
	//	std::string name;

public:

	/**
	 * Creates an object that refers to another reflected class.
	 * Assumes that c is not NULL.
	 *
	 * @param c Class to use as a base class
	 */
	ReflectedBaseClass(const ReflectedClass * c, int modifiers);

	/**
	 * Creates an object that refers to a class that was not reflected
	 *
	 * @param sig Signature of the class
	 * @param vis Visibility of the class
	 */
	ReflectedBaseClass(ClassSignature sig, VisibilityType vis, int modifiers);

	/**
	 * Gets a reference to the base class if it is being reflected.  Otherwise
	 * this is NULL.
	 *
	 * @return reflected class or NULL
	 */
	virtual	const ReflectedClass * getClass() const;

	//	/**
	//	 * Gets the name of the class
	//	 *
	//	 * @return base class name
	//	 */
	//	std::string getName();

	/**
	 * Returns true if the reflected class is being reflected
	 *
	 * @return true if reflected
	 */
	virtual bool isAccessible() const;

	/**
	 * Determines if this base class is virtual or not. T
	 * @return true if virtual
	 */
	virtual bool isVirtual() const {
		return (mods & cpgf::metaModifierVirtual) != 0;
	}


	/**
	 * TODO not currently supported
	 * Determines if this base class is a template.
	 * @return true if a template
	 */
	virtual bool isTemplate() const {
		return (mods & cpgf::metaModifierTemplate) != 0;
	}
};



/**
 * @class ReflectedObject
 * @brief Represents an instance of a ReflectedClass. Methods can be invoked
 * directly on this Object.
 */
class ReflectedObject
{
	friend class ReflectedClass;
protected:
	/**
	 * Reference to the class
	 */
	const ReflectedClass * mClass;
	/**
	 * Reference to the object returned from instantiating the reflected class.
	 */
	void * mObject;
//	cpgf::GVariant gObject;

	/**
	 * @brief Constructor for the object
	 *
	 * @param c Class from which this was created
	 * @param obj Object returned from creating the reflected class
	 */
	ReflectedObject(const ReflectedClass * c, void * obj);

	/**
	 * @brief Constructor for the object
	 *
	 * @param c Class from which this was created
	 * @param obj Object returned from creating the reflected class
	 */
//	ReflectedObject(const ReflectedClass * c, const cpgf::GVariant & obj);



	/**
	 * Gets the reflected class that is referred to in the signature (if
	 * possible).  Returns NULL if it is not a reflected type.
	 *
	 * @param signature Signature of the function/method
	 * @return Reflected class or NULL
	 */
	virtual	const ReflectedClass * getReturnTypeFromSignature(MethodSignature signature);

	/**
	 * Determines if the return value should be copied (i.e. it isn't a
	 * reference or a pointer.
	 *
	 * @param signature Signature of the function/method
	 * @return true if it should be copied
	 */
	virtual	bool shouldCopyReturnValue(MethodSignature signature);

	/**
	 * Creates a copy of the Return Value
	 *
	 * @param className the name of the class whose value is being copied
	 * @param returnValue value to copy
	 * @return a copy of the parameter or, if it shouldn't be copied,
	 * 	the original value
	 */
	virtual	cpgf::GVariant copyReturnValue(const ReflectedClass * rClass,
			cpgf::GVariant returnValue);

public:

	/**
	 * Destroys this instance
	 */
	virtual	~ReflectedObject();

	/**
	 * Gets a reference to the reflected class that this is an instance of
	 *
	 * @return reflected class
	 */
	virtual	const ReflectedClass * getClass() const;

	/**
	 * Gets a reference to the object returned from instantiating the class.
	 * This should only be used when the cpgf library is being called directly
	 *
	 * @return A pointer to the raw object
	 */
	virtual	void * getObject() const;

	/**
	 * Gets the name of the class
	 *
	 * @return Name of the class
	 */
	virtual	const std::string getClassName();


	/**
	 * Invokes a method with the specified parameters. No value is returned
	 * 	from the method invoked.The signature for this method is:
	 *  	bool invoke(string methodSignature,
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
	virtual	bool invoke(MethodSignature methodSignature \
				GPP_COMMA_IF(N) \
				GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p));

	GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE

	/**
	 * Invokes a method of this object with the specified parameters
	 * and returns the result
	 *  (through the returnVal parameter).
	 *  The signature for this method is:
	 *  	bool invokeReturn(string methodSignature, <ReturnType> returnVal,
	 *  		GVariant p0, ...)
	 *  		Most types can be used for GVariant
	 *
	 * @param methodSignature Method to call (i.e. int foo(double x, char y))
	 * @param returnVal Value returned from the method Undefined if success = false
	 * @param p<1-28> Parameter for the method.  Currently up to 28 parameters are
	 * 		permitted.
	 *
	 * @return True is the method was successfully called.
	 * 		If the method was called but the return value could not be converted
	 * 		to the ReturnType, then this is false.
	 */
#define REF_INVOKE(N, unused) \
		template <typename ReturnType> \
	bool invokeReturn(MethodSignature methodSignature, \
				ReturnType & returnVal GPP_COMMA_IF(N) \
				GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p));

	GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE



	//TODO check with * to object and ** to object
	/**
	 * Invokes a method of this object with the specified parameters
	 * and returns the result
	 *  (through the returnVal parameter).
	 *  The signature for this method is:
	 *  	bool invokeReturn(string methodSignature, <ReturnType> returnVal,
	 *  		GVariant p0, ...)
	 *  		Most types can be used for GVariant
	 *
	 * @param methodSignature Method to call (i.e. int foo(double x, char y))
	 * @param returnVal Value returned from the method Undefined if success = false
	 * @param p<1-28> Parameter for the method.  Currently up to 28 parameters are
	 * 		permitted.
	 *
	 * @return True is the method was successfully called.
	 * 		If the method was called but the return value could not be converted
	 * 		to the ReturnType, then this is false.
	 */
#define REF_INVOKE(N, unused) \
	virtual	bool invokeReturn(MethodSignature methodSignature, \
				ReflectedObjectPtr & returnVal \
				GPP_COMMA_IF(N) \
				GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p));

	GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE

	//	bool invokeReturnTest(std::string methodSignature,
	//			int & returnVal);

	/**
	 * Gets a variable in this object with the specified signature.
	 *
	 * NOTE: This returns a copy of the value and changes to the value will not
	 * change the variable.  To change the variable use setField or:
	 * 	Get a ReflectedData object (ReflectedData::getDataInstance())
	 * 	Get the variable in its cpgf::GMetaField wrapper
	 * 		(ReflectedData's getGlobalVariable)
	 * 	Use the GMetaField's set method (the first parameter should be the
	 * 		class instance (i.e. this class's getObject()),
	 * 		 the second is the new value)
	 * 		i.e. x->set(object, 15); Sets the GMetaField's value to 15
	 *
	 * You can also use this approach to get the value of the variable:
	 * 		After getting the GMetaField:
	 * 		GVariant xValue = x->get(object); //The parameter is the instance
	 * 				//to get the value from (i.e. this class's getObject())
	 * 		//convert the GVariant to an int
	 * 		int xAsInt = cpgf::fromVariant<int>(xValue)
	 *
	 *
	 * @param val value to store the variable in. This is likely a COPY of
	 * 		the global variable.
	 * @param signature Signature for the variable (i.e. int x, string n, etc.)
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
	 * @return True if the variable could be accessed. If false, the val
	 * 	 	of val is unknown.
	 */
	template <typename ValueType>
	bool getField(ValueType& val, FieldSignature signature,
			int modifiers = ReflectedDataBase::ALLOW_ALL_MODIFIERS, bool allowMoreMods = true);


	/**
	 * Sets a variable in this instance with the specified signature.
	 *
	 * @param val value to store in the variable.
	 * @param signature Signature for the variable (i.e. int x, string n, etc.)
	 * * @param modifiers set of modifiers (ORed together) that the item should
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
	 * @return True if the variable could be set.
	 */
	template <typename ValueType>
	bool setField(ValueType& val, FieldSignature signature,
			int modifiers = ReflectedDataBase::ALLOW_ALL_MODIFIERS, bool allowMoreMods = true);

	/**
	 * Casts the ReflectedObject to a void * for method calls.
	 */
	virtual	operator void *();

	/**
	 * Casts the ReflectedObject to a GVariant for method calls.
	 */
	virtual	operator cpgf::GVariant();
};


/**
 * @class ReflectedClass
 * @brief A wrapper around the reflected class from cpfg
 */
class ReflectedClass : public ReflectedDataBase, public ReflectedItem {
	friend class ReflectedData;
	friend class ReflectedObject;
	friend class LookingGlass;

protected:

	/**
	 * Pointer to raw, reflected class
	 */
	const cpgf::GMetaClass* mClass;
	/**
	 * Maps of constructors stored by their signatures
	 *  i.e. Foo(int, double) //for class Foo
	 */
	std::map<std::string, const ReflectedConstructor *> constructors;

	/**
	 * Map of base classes stored by their names
	 */
	std::map<std::string, const ReflectedBaseClass *> baseClasses;

	/**
	 * Map of inner classes store by their names
	 */
	std::map<std::string, ReflectedClass *> innerClasses;

	mutable std::map<void *, boost::weak_ptr<ReflectedObject> > createdObjects;

	/**
	 * Loads data from the reflected class into the maps (like constructors)
	 *
	 * @param gclass Reflected class
	 */
	virtual void loadData(const cpgf::GMetaClass* gclass, bool isGlobal);

	/**
	 * Matches the base classes for this class with a ReflectedClass
	 */
	virtual void loadBaseClasses(std::map<std::string, ReflectedClass *>
	& classes);

	/**
	 * Loads the methods from the base class
	 */
	virtual void loadBaseMethods(const std::map<std::string, const ReflectedMethod *>
	& baseMethods,  VisibilityType publicVis = Public,
	VisibilityType protectedVis = Protected);

	/**
	 * Loads the enums from the base class
	 */
	virtual void loadBaseEnums(const std::map<std::string, const ReflectedEnum *>
	& baseEnums,  VisibilityType publicVis = Public,
	VisibilityType protectedVis = Protected);

//	/**
//	 * Loads the operators from the base class
//	 */
//	virtual void loadBaseOperators(std::map<std::string,
//			const ReflectedOperator *> & baseOperators);

	/**
	 * Loads the fields from the base class
	 */
	virtual void loadBaseFields(const std::map<std::string, const ReflectedField *>
	& baseFields,  VisibilityType publicVis = Public,
	VisibilityType protectedVis = Protected);

	/**
	 * Loads a single non-reflected item like private methods, non-reflected
	 * base classes, etc.
	 *
	 * @param nrItem item to load
	 */
	virtual	void loadNonReflectedData(const cpgf::GMetaNonReflectedItem * nrItem);


	/**
	 * Loads info about baseclasses that are not-reflected.
	 *
	 * In cases where you have protected or private inheritance, this method
	 * loads the members of that (reflected) class into the inheriting class with
	 * the appropriate deduction in visibility.
	 *
	 * @param allClasses map of all classes being reflected
	 */
	virtual void loadNonReflectedBaseClassMembers(
			const std::map<std::string, ReflectedClass *> & allClasses);

	/**
	 * Loads info about baseclasses that are not-reflected.
	 *
	 * In cases where you have protected or private inheritance, this method
	 * loads the members of that (reflected) class into the inheriting class with
	 * the appropriate deduction in visibility.
	 *
	 * @param allClasses map of all classes being reflected
	 * @param signature Name of base class
	 * @param baseVis visibility of of the base class
	 */
	virtual void loadNonReflectedBaseClassMembers(
			const std::map<std::string, ReflectedClass *> & allClasses,
			std::string signature, VisibilityType baseVis);

	/**
	 * Tries to cast the object to this class.
	 *
	 * @param object Object to cast
	 * @return a pointer to an object from this class or NULL if the cast
	 *   fails
	 */
	virtual	ReflectedObjectPtr castTo(const cpgf::GVariant & object) const;

	/**
	 * Tries to cast the object to this class.
	 *
	 * @param object Object to cast
	 * @return a pointer to an object from this class or NULL if the cast
	 *   fails
	 */
	virtual	ReflectedObjectPtr castTo(void * object) const;

	/**
	 * Keeps a copy of the objects being created so that values cast to
	 * this class that reference the same object will not delete the object
	 * at the wrong time.
	 *
	 * @param ptr Pointer to store as a weak reference
	 */
	virtual	void storeObjectPtr(ReflectedObjectPtr ptr) const;

	/**
	 * Finds if this object already exists. If so, returns a strong pointer to
	 * it.  Otherwise, returns NULL.
	 *
	 * @param ptr Pointer to find
	 * @return Reference to the pointer or NULL
	 */
	virtual	ReflectedObjectPtr getStoredObject(void * ptr) const;

	/**
	 * Determines if this pointer exists already.
	 *
	 * @param ptr Pointer to find.
	 * @return true if it exists.
	 */
	virtual	bool hasStoredObject(void * ptr) const;

public:

	/**
	 * Creates a wrapped for the reflected class
	 *
	 * @param c Reflected class
	 */
	ReflectedClass(const cpgf::GMetaClass* c, bool isGlobal);

	/**
	 * Creates non-reflected class
	 *
	 * @param c Reflected class
	 */
	ReflectedClass(ClassSignature name, bool isGlobal, VisibilityType vis,
			int modifiers);

	/**
	 * Cleans up the wrapper class
	 */
	virtual ~ReflectedClass();


	virtual bool isAccessible() const {return mClass != NULL;};

	/**
	 * TODO not currently supported
	 * Determines if this class is a template.
	 * @return true if a template
	 */
	virtual bool isTemplate() const {
		return (mods & cpgf::metaModifierTemplate) != 0;
	}

	/**
	 * Creates an object of this class with the specified parameters
	 *
	 * This does not need to be deleted.  The ReflectedObjectPtr is a boost
	 * shared_ptr and will delete itself.
	 *
	 * and returns the object
	 *  The signature for this method is:
	 *  	const ReflectedObjectPtr create(std::string signature, GVariant p0, ...)
	 *  		Most types can be used for GVariant
	 *
	 * @param signature The signature of the constructor to call (i.e. (int, int, double))
	 * @param p<1-28> Parameter for the method.  Currently up to 28 parameters are
	 * 		permitted.
	 *
	 * @return A pointer to the new object is successful. NULL if not.
	 */
#define REF_INVOKE(N, unused) \
		virtual	const ReflectedObjectPtr create(MethodSignature signature GPP_COMMA_IF(N) \
				GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p)) const;

	GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE

//	/**
//	 * Determines if rObject is an instance of this class
//	 *
//	 * @param rObject object to check
//	 *
//	 * @return true if rObject is an instance of this class
//	 */
//	bool instanceOf(ReflectedObjectPtr rObject) const;




	//	/**
	//	 * Gets the class name
	//	 *
	//	 * @return Name of the class
	//	 */
	//	const std::string getName() const;

	/**
	 * Gets the GMetaClass representing the class
	 *
	 * @return the Metadata class
	 */
	virtual	const cpgf::GMetaClass* getClass() const;

	/**
	 * Checks to see if a method exists using the method's signature.
	 *
	 * Usage:
	 * 	int foo(int, double)
	 * 		Check to see if a method foo exists that returns an int and
	 * 		takes an int and double as parameters
	 *
	 *  @param methodSignature Signature of the method
	 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited True if inherited items should be included.
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
	 *  @return true is that method exists
	 */
	virtual bool doesMethodExist(MethodSignature methodSignature,
			VisibilityAccessType vis = Public_Access,
			bool inherit = false, int modifiers = ALLOW_ALL_MODIFIERS,
			bool allowMoreMods = true) const;


	/**
	 * Gets the number of method signatures that are being reflected
	 *
	 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited True if inherited items should be included.
	 * @return the number of method signatures
	 */
	virtual size_t getMethodCount(VisibilityAccessType vis = Public_Access,
			bool inherit = false) const;


	/**
	 * Gets a list of all method signatures that are being reflected
	 *
	 *	This only includes methods defined in the class.  It does not include
	 *	inherited methods.
	 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited True if inherited items should be included.
	 *
	 * @return a list of method signatures
	 */
	virtual const std::vector<std::string> getMethodNames(
			VisibilityAccessType vis = Public_Access,
			bool inherit = false) const;

	/**
	 * Gets a method that corresponds to the signature or NULL
	 *
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
	 * @return a metadata object or NULL
	 */
	virtual const ReflectedMethod * getMethod(MethodSignature signature,
			int modifiers = ALLOW_ALL_MODIFIERS, bool allowMoreMods = true) const;


	/**
	 * Gets a list of all methods in the class
	 *
	 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited True if inherited items should be included.
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
	 * @return List of methods in the class
	 */
	virtual std::vector<const ReflectedMethod *> getMethods(
			VisibilityAccessType vis = Public_Access, bool inherited = false,
			int modifiers = ALLOW_ALL_MODIFIERS, bool allowMoreMods = true);

	/**
	 * Gets a list of the closest methods to the name provided
	 *
	 * @param name Name to find
	 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited True if inherited items should be included.
	 *  @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return List of methods that are close in name to the name given
	 */
	virtual std::vector<const ReflectedMethod *> getClosestMethods(
			MethodSignature name, VisibilityAccessType vis = Public_Access,
			bool inherited = false, int count = MAX_SIMILAR);


	/**
	 * Gets a string comparing the name to the closest names found
	 *
	 * @param name Name to find
	 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited True if inherited items should be included.
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return String with a list of methods that are close in name
	 * to the name given
	 */
	virtual std::string getClosestMethodsString(
			MethodSignature name, VisibilityAccessType vis = Public_Access,
			bool inherited = false, int count = MAX_SIMILAR);

	/**
	 * Determines if a field exists with this signature
	 *
	 * @param signature Signature of the value (i.e. int x or const double y)
	 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited True if inherited items should be included.
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
	 *
	 * @return true if the field exists
	 */
	virtual bool doesFieldExist(FieldSignature signature, VisibilityAccessType vis = Public_Access,
			bool inherit = false, int modifiers = ALLOW_ALL_MODIFIERS,
			bool allowMoreMods = true) const;

	/**
	 * Gets the number of variables declared
	 *
	 * @return number of variables
	 */
	virtual size_t getVariableCount(VisibilityAccessType vis = Public_Access,
			bool inherit = false) const;


	/**
	 * Gets the signatures of variables
	 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited True if inherited items should be included.
	 *
	 * @return a list of the signatures of variables
	 * with a global scope.
	 */
	virtual const std::vector<std::string> getVariableNames(
			VisibilityAccessType vis = Public_Access, bool inherit = false) const;

	/**
	 * Gets the number of constants declared
	 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited True if inherited items should be included.
	 *
	 * @return number of constants with a global scope.
	 */
	virtual size_t getConstantCount(VisibilityAccessType vis = Public_Access,
			bool inherit = false) const;

	/**
	 * Gets the signatures of constants declared
	 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited True if inherited items should be included.
	 *
	 * @return a list of the signatures of constants
	 * with a global scope.
	 */
	virtual const std::vector<std::string> getConstantNames(
			VisibilityAccessType vis = Public_Access,
			bool inherit = false) const;
	//all variables and fields


	/**
	 * Gets the number of constants and variables declared with a global scope
	 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited True if inherited items should be included.
	 *
	 * @return number of constants and variables with a global scope.
	 */
	virtual size_t getFieldCount(VisibilityAccessType vis = Public_Access,
			bool inherit = false) const;

	/**
	 * Gets the signatures of constants and variables declared with a
	 * global scope
	 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited True if inherited items should be included.
	 *
	 * @return a list of the signatures of constants and variables
	 * with a global scope.
	 */
	virtual const std::vector<std::string> getFieldNames(
			VisibilityAccessType vis = Public_Access, bool inherit = false) const;

	/**
	 * Gets a variable that corresponds to the signature or NULL. The value
	 * returned is to a generic field.  When using get or set, an object
	 * instance will need to be provided.
	 *
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
	 *
	 * @return a metadata object or NULL
	 */
	virtual const ReflectedField * getField(FieldSignature signature,
			int modifiers = ALLOW_ALL_MODIFIERS, bool allowMoreMods = true) const;



	/**
	 * Gets a list of all fields in the class
	 *
	 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited True if inherited items should be included.
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
	 *
	 * @return List of fields in the class
	 */
	virtual std::vector<const ReflectedField *> getFields(
			VisibilityAccessType vis = Public_Access, bool inherited = false,
			int modifiers = ALLOW_ALL_MODIFIERS, bool allowMoreMods = true);

	/**
	 * Gets a list of the closest fields to the name provided
	 *
	 * @param name Name to find
	 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited True if inherited items should be included.
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return List of fields that are close in name to the name given
	 */
	virtual std::vector<const ReflectedField *> getClosestFields(
			FieldSignature name, VisibilityAccessType vis = Public_Access,
			bool inherited = false, int count = MAX_SIMILAR);


	/**
	 * Gets a string comparing the name to the closest names found
	 *
	 * @param name Name to find
	 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited True if inherited items should be included.
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return String with a list of fields that are close in name
	 * to the name given
	 */
	virtual std::string getClosestFieldsString(
			FieldSignature name, VisibilityAccessType vis = Public_Access,
			bool inherited = false, int count = MAX_SIMILAR);

	/**
	 * Checks to see if a constructor exists using the constructor's signature.
	 *
	 * Usage:
	 * 	Foo(int, double)
	 * 		Check to see if a constructor for class Foo exists
	 * 		that takes an int and double as parameters
	 *
	 *  @param signature Signature of the constructor
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
	 *  @return true is that constructor exists
	 */
	virtual bool doesConstructorExist(MethodSignature signature,
			VisibilityAccessType vis = Public_Access,
			int modifiers = ALLOW_ALL_MODIFIERS, bool allowMoreMods = true) const;

	/**
	 * Gets the number of constructor signatures that are being reflected for
	 * this class.
	 * If there is an implicit default constructor, it will be counted here.
	 *
	 * @return the number of constructor signatures
	 */
	virtual size_t getConstructorCount(VisibilityAccessType vis = Public_Access) const;


	/**
	 * Gets a list of all constructor signatures that are being reflected for
	 * this class.
	 *
	 * If there is an implicit default constructor, it will show up here.
	 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *
	 * @return a list of constructor signatures
	 */
	virtual const std::vector<std::string> getConstructorNames(
			VisibilityAccessType vis = Public_Access) const;

	/**
	 * Gets a constructor that corresponds to the signature or NULL.
	 * Constructor signatures have the form: <class name>(<param>, <param>,...)
	 *
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
	 * @return a metadata object or NULL
	 */
	virtual const ReflectedConstructor * getConstructor(MethodSignature signature,
			int modifiers = ALLOW_ALL_MODIFIERS, bool allowMoreMods = true) const;


	/**
	 * Gets a list of all constructor in the class
	 *
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
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
	 * @return List of constructor in the class
	 */
	virtual std::vector<const ReflectedConstructor *> getConstructors(
			VisibilityAccessType vis = Public_Access,
			int modifiers = ALLOW_ALL_MODIFIERS, bool allowMoreMods = true);

	/**
	 * Gets a list of the closest constructor to the name provided
	 *
	 * @param name Name to find
	 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return List of constructor that are close in name to the name given
	 */
	virtual std::vector<const ReflectedConstructor *> getClosestConstructors(
			MethodSignature name, VisibilityAccessType vis = Public_Access,
			int count = MAX_SIMILAR);


	/**
	 * Gets a string comparing the name to the closest names found
	 *
	 * @param name Name to find
	 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return String with a list of constructor that are close in name
	 * to the name given
	 */
	virtual std::string getClosestConstructorsString(
			MethodSignature name, VisibilityAccessType vis = Public_Access,
			int count = MAX_SIMILAR);

	//	/**
	//	 * Checks to see if a method exists and is callable from this class.
	//	 *
	//	 * This is similar to doesMethodExist except it includes inherited,
	//	 * reflected methods.  So, if the base class is reflected, its methods
	//	 * will appear here.
	//	 *
	//	 * Usage:
	//	 * 	Foo(int, double)
	//	 * 		Check to see if a method for class Foo exists
	//	 * 		that takes an int and double as parameters
	//	 *
	//	 *  @param signature Signature of the method
	//	 *  @return true is that method exists
	//	 */
	//	virtual bool doesCallableMethodExist(std::string signature) const;
	//
	//	/**
	//	 * Gets the number of method signatures that are being reflected for
	//	 * this class.
	//	 *
	//	 * This is similar to getMethodCount except it includes inherited,
	//	 * reflected methods.  So, if the base class is reflected, its methods
	//	 * will appear here.
	//	 *
	//	 * @return the number of method signatures
	//	 */
	//	virtual size_t getCallableMethodCount() const;
	//
	//
	//	/**
	//	 * Gets a list of all method signatures that are being reflected for
	//	 * this class.
	//	 *
	//	 * This is similar to getMethodNames except it includes inherited,
	//	 * reflected methods.  So, if the base class is reflected, its methods
	//	 * will appear here.
	//	 *
	//	 * @return a list of method signatures
	//	 */
	//	virtual const std::vector<std::string> getCallableMethodNames() const;
	//
	//	/**
	//	 * Gets a method that corresponds to the signature or NULL.
	//	 *
	//	 * This is similar to getMethod except it includes inherited,
	//	 * reflected methods.  So, if the base class is reflected, its methods
	//	 * will appear here.
	//	 *
	//	 * @return a metadata object or NULL
	//	 */
	//	virtual const cpgf::GMetaMethod* getCallableMethod(std::string signature) const;
	//

	/**
	 * Checks to see if the class has a (public) base class by the name of name.
	 *
	 *
	 *  @param name Name of the base class
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
	 *  @return true is name is the name of one of the base classes
	 */
	virtual bool hasBaseClass(ClassSignature name,
			VisibilityAccessType vis = Public_Access,
			int modifiers = ALLOW_ALL_MODIFIERS, bool allowMoreMods = true) const;

	/**
	 * Gets the number of base classes for this class.
	 *
	 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *
	 * @return the number of base classes for this class
	 */
	virtual size_t getBaseClassCount(VisibilityAccessType vis = Public_Access) const;


	/**
	 * Gets a list of all base classes for this class.
	 *
	 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *
	 * @return a list of base class names
	 */
	virtual const std::vector<std::string> getBaseClassNames(
			VisibilityAccessType vis = Public_Access) const;

	/**
	 * Gets a base class for this class.
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
	virtual const ReflectedBaseClass * getBaseClass(ClassSignature name,
			int modifiers = ALLOW_ALL_MODIFIERS, bool allowMoreMods = true) const;


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
	virtual std::vector<const ReflectedBaseClass *> getClosestBaseClasses(
			ClassSignature name, VisibilityAccessType vis = Public_Access,
			int count = MAX_SIMILAR);


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
	virtual std::string getClosestBaseClassesString(
			ClassSignature name, VisibilityAccessType vis = Public_Access,
			int count = MAX_SIMILAR);

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
		virtual bool hasInnerClass(ClassSignature name,
				VisibilityAccessType vis = Public_Access,
				int modifiers = ALLOW_ALL_MODIFIERS, bool allowMoreMods = true) const;

		/**
		 * Gets the number of inner classes for this class.
		 *
		 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
		 *
		 * @return the number of inner classes for this class
		 */
		virtual size_t getInnerClassCount(VisibilityAccessType vis = Public_Access) const;


		/**
		 * Gets a list of all inner classes for this class.
		 *
		 *  @param vis Visibility of items to retrieve. Generally this is Public_Access.
		 *
		 * @return a list of inner class names
		 */
		virtual const std::vector<std::string> getInnerClassNames(
				VisibilityAccessType vis = Public_Access) const;

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
		virtual ReflectedClass * getInnerClass(ClassSignature name,
				int modifiers = ALLOW_ALL_MODIFIERS, bool allowMoreMods = true) const;


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
		virtual std::vector<ReflectedClass *> getClosestInnerClasses(
				ClassSignature name, VisibilityAccessType vis = Public_Access,
				int count = MAX_SIMILAR);


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
		virtual std::string getClosestInnerClassesString(
				ClassSignature name, VisibilityAccessType vis = Public_Access,
				int count = MAX_SIMILAR);

};



template <typename ValueType>
bool ReflectedObject::getField(ValueType& val, FieldSignature signature,
		int modifiers, bool allowMoreMods)
{
	bool success = false;

	if (mClass->doesFieldExist(signature, All_Access, true, modifiers,
			allowMoreMods))
	{
		const ReflectedField* field = mClass->getField(signature, modifiers,
				allowMoreMods);
		if (field->isAccessible()) {
			val = (cpgf::fromVariant<ValueType>(field->getField()->get(
					getObject())));
			success = true;
		}
	}

	return success;
}

template <typename ValueType>
bool ReflectedObject::setField(ValueType& val, FieldSignature signature,
		int modifiers, bool allowMoreMods)
{
	bool success = false;

	if (mClass->doesFieldExist(signature, All_Access, true, modifiers,
			allowMoreMods))
	{
		const ReflectedField * field = mClass->getField(signature, modifiers,
				allowMoreMods);
		try
		{
			if (field->isAccessible())
			{
				field->getField()->set(getObject(), val);
				success = true;
			}
		}
		catch (...)
		{
			//failed, probably wrong data type
		}
	}
	return success;
}




/**
 * @class ReflectedObjectPtr
 * @brief Shared pointer to a ReflectedObject variable
 */
//typedef boost::shared_ptr<ReflectedObject> ReflectedObjectPtr;

class ReflectedObjectPtr : public boost::shared_ptr<ReflectedObject> {

public:
	ReflectedObjectPtr() : boost::shared_ptr<ReflectedObject>() { };

	explicit ReflectedObjectPtr(ReflectedObject * p) :
		boost::shared_ptr<ReflectedObject>(p) { }

	~ReflectedObjectPtr() {};

	ReflectedObjectPtr(boost::shared_ptr<ReflectedObject> const & r) : boost::shared_ptr<ReflectedObject>(r) { }
//	ReflectedObjectPtr(ReflectedObjectPtr const & r) :
//		boost::shared_ptr<ReflectedObject>(r) { }

	ReflectedObjectPtr(boost::shared_ptr<ReflectedObject> const & r, element_type * p) :
		boost::shared_ptr<ReflectedObject>(r, p) { }

	explicit ReflectedObjectPtr(boost::weak_ptr<ReflectedObject> const & r) :
		boost::shared_ptr<ReflectedObject>(r) { }

	ReflectedObjectPtr & operator=(boost::shared_ptr<ReflectedObject> const & r) {
		boost::shared_ptr<ReflectedObject>::operator=(r);
		return *this;
	}

	ReflectedObjectPtr & operator=(ReflectedObjectPtr const & r) {
		boost::shared_ptr<ReflectedObject>::operator=(r);
		return *this;
	}

	/**
	 * Cast operator to GVariant - Gets the method pointer and not not
	 * smart pointer
	 */
	operator cpgf::GVariant() { return cpgf::GVariant(get()->getObject()); }
};


///**
// * Determines if rObject is an instance of rClass
// *
// * @param rClass Potential class of the object
// * @param rObject object to check
// *
// * @return true if rObject is an instance of rClass
// */
//bool instanceOf(const ReflectedClass * rClass, ReflectedObjectPtr rObject);


bool operator==(ReflectedObjectPtr const & p1, ReflectedObjectPtr const & p2);




}








#endif /* REFLECTEDCLASS_H_ */
