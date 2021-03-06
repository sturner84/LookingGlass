/*
 * ReflectedData.h
 *
 *  Created on: Oct 19, 2013
 *      Author: saturner
 */

#ifndef REFLECTEDDATA_H_
#define REFLECTEDDATA_H_

#include <cpgf/gmetaclass.h>
#include <cpgf/gmetacommon.h>
#include <string>

#include "lgReflectedClass.h"
#include "lgReflectionUtil.h"
#include "lgReflectedDataBase.h"




namespace cpptesting {

class ReflectedData : public ReflectedDataBase {

private:
	static ReflectedData instance; //singleton

	//private constructor
	ReflectedData();
	bool loaded;


protected:

	std::map<std::string, ReflectedClass *> classes;

	virtual void loadData();
	virtual void loadData(const cpgf::GMetaClass* gclass, bool isGlobal);
	virtual void loadBaseClasses();

	/**
	 * Loads a single non-reflected item like private methods, non-reflected
	 * base classes, etc.
	 *
	 * @param nrItem item to load
	 */
	virtual	void loadNonReflectedData(const cpgf::GMetaNonReflectedItem * nrItem);


	/**
	 * Searches through all classes and inner classes to find a class.
	 * Assumes the signature is in the form <outer>::<inner>::<inner inner>...
	 *
	 * @param signature Signature of the class
	 * @return The reflected class or NULL if it is not found
	 */
	virtual ReflectedClass * findClass(ClassSignature signature) const;

public:

	virtual ~ReflectedData();

	/**
	 * Gets access to a singleton with the data being reflected.
	 *
	 * @return a pointer to the data being reflected
	 */
	static ReflectedData * getDataInstance();


	/**
	 * Checks to see if a function exists using the function's signature.
	 *
	 * Usage:
	 * 	int foo(int, double)
	 * 		Check to see if a function foo exists that returns an int and
	 * 		takes an int and double as parameters
	 *
	 *  @param functionSignature Signature of the function
	 *  @param filter Filter to apply to the results. By default this
	 *  allows Public_Access, non-inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 *  @return true is that function exists
	 */
	virtual	bool doesFunctionExist(MethodSignature functionSignature,
			ItemFilter filter = ItemFilter()) const;


	/**
	 * Gets the number of function signatures that are being reflected
	 *
	 *  @param filter Filter to apply to the results. By default this
	 *  allows Public_Access, non-inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 * @return the number of function signatures
	 */
	virtual	size_t getFunctionCount(ItemFilter filter = ItemFilter()) const;


	/**
	 * Gets a list of all function signatures that are being reflected
	 *
	 *  @param filter Filter to apply to the results. By default this
	 *  allows Public_Access, non-inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 * @return a list of function signatures
	 */
	virtual	const std::vector<std::string> getFunctionNames(
			ItemFilter filter = ItemFilter()) const;

	/**
	 * Gets a function that corresponds to the signature or NULL
	 *
	 *  @param filter Filter to apply to the results. By default this
	 *  allows All_Access, inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 * @return a metadata object or NULL
	 */
	virtual	const ReflectedMethod * getFunction(MethodSignature signature,
			ItemFilter filter = ItemFilter(All_Access, true)) const;



	/**
	 * Gets a list of all functions
	 *
	 *  @param filter Filter to apply to the results. By default this
	 *  allows Public_Access, non-inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 * @return List of functions
	 */
	virtual std::vector<const ReflectedMethod *> getFunctions(
			ItemFilter filter = ItemFilter());

	/**
	 * Gets a list of the closest functions to the name provided
	 *
	 * @param name Name to find
	 *  @param filter Filter to apply to the results. By default this
	 *  allows Public_Access, non-inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return List of functions that are close in name to the name given
	 */
	virtual std::vector<const ReflectedMethod *> getClosestFunctions(
			MethodSignature name, ItemFilter filter = ItemFilter(),
			int count = MAX_SIMILAR);


	/**
	 * Gets a string comparing the name to the closest names found
	 *
	 * @param name Name to find
	 *  @param filter Filter to apply to the results. By default this
	 *  allows Public_Access, non-inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return String with a list of functions that are close in name
	 * to the name given
	 */
	virtual std::string getClosestFunctionsString(
			MethodSignature name, ItemFilter filter = ItemFilter(),
			int count = MAX_SIMILAR);

	/**
	 * Determines if a value (variable or constant) exists with this signature
	 *
	 * @param signature Signature of the value (i.e. int x or const double y)
	 *  @param filter Filter to apply to the results. By default this
	 *  allows Public_Access, non-inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 * @return true if the field exists
	 */
	virtual bool doesVariableExist(FieldSignature signature,
			ItemFilter filter = ItemFilter()) const;


	/**
	 * Gets the number of variables declared with a global scope
	 *
	 * @return number of variables with a global scope.
	 */
	virtual	size_t getGlobalVariableCount(ItemFilter filter = ItemFilter()) const;


	/**
	 * Gets the signatures of variables declared with a
	 * global scope
	 *
	 *  @param filter Filter to apply to the results. By default this
	 *  allows Public_Access, non-inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 * @return a list of the signatures of variables
	 * with a global scope.
	 */
	virtual	const std::vector<std::string> getGlobalVariableNames(
			ItemFilter filter = ItemFilter()) const;

	/**
	 * Gets the number of constants declared with a global scope
	 *
	 *  @param filter Filter to apply to the results. By default this
	 *  allows Public_Access, non-inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 * @return number of constants with a global scope.
	 */
	virtual	size_t getGlobalConstantCount(ItemFilter filter = ItemFilter()) const;

	/**
	 * Gets the signatures of constants declared with a
	 * global scope
	 *
	 *  @param filter Filter to apply to the results. By default this
	 *  allows Public_Access, non-inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 * @return a list of the signatures of constants
	 * with a global scope.
	 */
	virtual	const std::vector<std::string> getGlobalConstantNames(
			ItemFilter filter = ItemFilter()) const;
	//all variables and fields


	/**
	 * Gets the number of constants and variables declared with a global scope
	 *
	 *  @param filter Filter to apply to the results. By default this
	 *  allows Public_Access, non-inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 * @return number of constants and variables with a global scope.
	 */
	virtual	size_t getGlobalFieldCount(ItemFilter filter = ItemFilter()) const;

	/**
	 * Gets the signatures of constants and variables declared with a
	 * global scope
	 *
	 *  @param filter Filter to apply to the results. By default this
	 *  allows Public_Access, non-inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 * @return a list of the signatures of constants and variables
	 * with a global scope.
	 */
	virtual	const std::vector<std::string> getGlobalFieldNames(
			ItemFilter filter = ItemFilter()) const;


	/**
	 * Gets a variable or constant that corresponds to the signature or NULL
	 *  @param filter Filter to apply to the results. By default this
	 *  allows All_Access, inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 * @return a metadata object or NULL
	 */
	virtual	const ReflectedField * getGlobalField(FieldSignature signature,
			ItemFilter filter = ItemFilter(All_Access, true)) const;


	/**
	 * Gets a list of all global variables
	 *
	 *  @param filter Filter to apply to the results. By default this
	 *  allows Public_Access, non-inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 * @return List of variables in the class
	 */
	virtual std::vector<const ReflectedField *> getGlobalFields(
			ItemFilter filter = ItemFilter());

	/**
	 * Gets a list of the closest variables to the name provided
	 *
	 * @param name Name to find
	 *  @param filter Filter to apply to the results. By default this
	 *  allows Public_Access, non-inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return List of variables that are close in name to the name given
	 */
	virtual std::vector<const ReflectedField *> getClosestGlobalFields(
			FieldSignature name, ItemFilter filter = ItemFilter(),
			int count = MAX_SIMILAR);


	/**
	 * Gets a string comparing the name to the closest names found
	 *
	 * @param name Name to find
	 *  @param filter Filter to apply to the results. By default this
	 *  allows Public_Access, non-inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return String with a list of operators that are close in name
	 * to the name given
	 */
	virtual std::string getClosestGlobalFieldsString(
			FieldSignature name, ItemFilter filter = ItemFilter(),
			int count = MAX_SIMILAR);

	/**
	 * Gets the number of classes
	 *
	 *	This does not include inner classes.
	 *
	 *  @param filter Filter to apply to the results. By default this
	 *  allows Public_Access, non-inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 * @return number of classes
	 */
	virtual	size_t getClassCount(ItemFilter filter = ItemFilter()) const;

	/**
	 * Checks to see if a class exists using the class's name.
	 *
	 *
	 *
	 * @param name Name of the class
	 *	If trying to find an inner class, the name should be in the form:
	 *		<outer>::<inner>
	 *
	 *  @param filter Filter to apply to the results. By default this
	 *  allows Public_Access, non-inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 *  @return true is that class exists
	 */
	virtual	bool doesClassExist(ClassSignature name,
			ItemFilter filter = ItemFilter()) const;


	/**
	 * Gets a list of all classes that are being reflected
	 *
	 * This does not include inner classes
	 *
	 *  @param filter Filter to apply to the results. By default this
	 *  allows Public_Access, non-inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 * @return a list of class names
	 */
	virtual	const std::vector<std::string> getClassNames(
			ItemFilter filter = ItemFilter()) const;

	/**
	 * Gets a class that corresponds to the name or NULL
	 *
	 *	If trying to find an inner class, the name should be in the form:
	 *		<outer>::<inner>
	 *
	 *  @param filter Filter to apply to the results. By default this
	 *  allows All_Access, inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 * @return a ReflectedClass object or NULL
	 */
	virtual	const ReflectedClass * getClass(ClassSignature signature,
			ItemFilter filter = ItemFilter(All_Access, true)) const;


	/**
	 * Gets a class that corresponds to the type or NULL.
	 *
	 * This ignores modifiers like const or volitile and * and & and returns
	 * the class if it is being reflected
	 *
	 *	If trying to find an inner class, the name should be in the form:
	 *		<outer>::<inner>
	 *
	 *	TODO modify this so that it finds inner classes reliably
	 *
	 * @return a ReflectedClass object or NULL
	 */
	virtual	const ReflectedClass * getClassFromType(TypeSignature signature) const;



	/**
	 * Gets a list of all classes
	 *
	 * This does not include inner classes.
	 *
	 *  @param filter Filter to apply to the results. By default this
	 *  allows Public_Access, non-inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 * @return List of classes
	 */
	virtual std::vector<ReflectedClass *> getClasses(
			ItemFilter filter = ItemFilter());

	/**
	 * Gets a list of the closest classes to the name provided
	 *
	 * This does not include inner classes.
	 *
	 * @param name Name to find
	 *  @param filter Filter to apply to the results. By default this
	 *  allows Public_Access, non-inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return List of classes that are close in name to the name given
	 */
	virtual std::vector<ReflectedClass *> getClosestClasses(
			ClassSignature name, ItemFilter filter = ItemFilter(),
			int count = MAX_SIMILAR);


	/**
	 * Gets a string comparing the name to the closest names found
	 *
	 * This does not include inner classes.
	 *
	 * @param name Name to find
	 *  @param filter Filter to apply to the results. By default this
	 *  allows Public_Access, non-inherited values with any modifiers.
	 *  See ItemFilter for more details
	 *
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return String with a list of classes that are close in name
	 * to the name given
	 */
	virtual std::string getClosestClassesString(
			ClassSignature name, ItemFilter filter = ItemFilter(),
			int count = MAX_SIMILAR);


	/**
	 * Gets the number of namespaces
	 *
	 * @return number of namespaces
	 */
	virtual	size_t getNamespaceCount() const;

	/**
	 * Gets a list of all namespaces that are being reflected
	 *
	 * @return a list of namespaces
	 */
	virtual	const std::vector<std::string> getNamespaceNames() const;

	/**
	 * Checks to see if a namespace exists using the namespace's full name.
	 *
	 *The name is the full namespace including all enclosing namespaces
	 * (i.e. cpptesting::internal::test )
	 *
	 *  @param name Full name of the namespace
	 *  @return true is that namespace exists
	 */
	virtual	bool doesNamespaceExist(std::string name) const;

	/**
	 * Gets a namespace that corresponds to the name or NULL
	 *
	 * The name is the full namespace including all enclosing namespaces
	 * (i.e. cpptesting::internal::test )
	 *
	 * @return a ReflectedNamespace object or NULL
	 */
	virtual	ReflectedNamespacePtr getNamespace(std::string name) const;


	/**
	 * Gets a list of all namespaces
	 *
	 * @param inherited True if inherited namespaces should be included.
	 *
	 * @return List of namespaces
	 */
	virtual std::vector<ReflectedNamespacePtr> getNamespaces();

	/**
	 * Gets a list of the closest namespaces to the name provided
	 *
	 * @param name Name to find
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return List of namespaces that are close in name to the name given
	 */
	virtual std::vector<ReflectedNamespacePtr> getClosestNamespaces(
			std::string name, int count = MAX_SIMILAR);


	/**
	 * Gets a string comparing the name to the closest names found
	 *
	 * @param name Name to find
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return String with a list of namespaces that are close in name
	 * to the name given
	 */
	virtual std::string getClosestNamespacesString(
			std::string name, int count = MAX_SIMILAR);
};







}
#endif /* REFLECTEDDATA_H_ */
