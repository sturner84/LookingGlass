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


//TODO include a getter for all methods/fields/etc.?

namespace cpptesting {

class ReflectedData : public ReflectedDataBase {

private:
	static ReflectedData instance; //singleton

	std::map<std::string, ReflectedClass *> classes;
	std::map<std::string, ReflectedNamespace *> namespaces; //TODO not used yet

	//private constructor
	ReflectedData();
	bool loaded;

	virtual void loadData();
	virtual void loadData(const cpgf::GMetaClass* gclass);
	virtual void loadBaseClasses();

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
	 *  @return true is that function exists
	 */
	bool doesFunctionExist(std::string functionSignature) const;


	/**
	 * Gets the number of function signatures that are being reflected
	 *
	 * @return the number of function signatures
	 */
	size_t getFunctionCount() const;


	/**
	 * Gets a list of all function signatures that are being reflected
	 *
	 * @return a list of function signatures
	 */
	const std::vector<std::string> getFunctionNames() const;

	/**
	 * Gets a function that corresponds to the signature or NULL
	 *
	 * @return a metadata object or NULL
	 */
	const ReflectedMethod * getFunction(std::string signature) const;



	/**
	 * Gets a list of all functions
	 *
	 * @param inherited True if inherited functions should be included.
	 *
	 * @return List of functions
	 */
	virtual std::vector<const ReflectedMethod *> getFunctions();

	/**
	 * Gets a list of the closest functions to the name provided
	 *
	 * @param name Name to find
	 * @param inherited True if inherited functions should be included.
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return List of functions that are close in name to the name given
	 */
	virtual std::vector<const ReflectedMethod *> getClosestFunctions(
			std::string name, int count = MAX_SIMILAR);


	/**
	 * Gets a string comparing the name to the closest names found
	 *
	 * @param name Name to find
	 * @param inherited True if inherited values should be included.
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return String with a list of functions that are close in name
	 * to the name given
	 */
	virtual std::string getClosestFunctionsString(
			std::string name, int count = MAX_SIMILAR);

	/**
	 * Determines if a value (variable or constant) exists with this signature
	 *
	 * @param signature Signature of the value (i.e. int x or const double y)
	 *
	 * @return true if the field exists
	 */
	virtual bool doesVariableExist(std::string signature) const;


	/**
	 * Gets the number of variables declared with a global scope
	 *
	 * @return number of variables with a global scope.
	 */
	size_t getGlobalVariableCount() const;


	/**
	 * Gets the signatures of variables declared with a
	 * global scope
	 *
	 * @return a list of the signatures of variables
	 * with a global scope.
	 */
	const std::vector<std::string> getGlobalVariableNames() const;

	/**
	 * Gets the number of constants declared with a global scope
	 *
	 * @return number of constants with a global scope.
	 */
	size_t getGlobalConstantCount() const;

	/**
	 * Gets the signatures of constants declared with a
	 * global scope
	 *
	 * @return a list of the signatures of constants
	 * with a global scope.
	 */
	const std::vector<std::string> getGlobalConstantNames() const;
	//all variables and fields


	/**
	 * Gets the number of constants and variables declared with a global scope
	 *
	 * @return number of constants and variables with a global scope.
	 */
	size_t getGlobalFieldCount() const;

	/**
	 * Gets the signatures of constants and variables declared with a
	 * global scope
	 *
	 * @return a list of the signatures of constants and variables
	 * with a global scope.
	 */
	const std::vector<std::string> getGlobalFieldNames() const;


	/**
	 * Gets a variable or constant that corresponds to the signature or NULL
	 *
	 * @return a metadata object or NULL
	 */
	const ReflectedField * getGlobalField(std::string signature) const;


	/**
	 * Gets a list of all global variables
	 *
	 * @param inherited True if inherited variables should be included.
	 *
	 * @return List of variables in the class
	 */
	virtual std::vector<const ReflectedField *> getGlobalFields();

	/**
	 * Gets a list of the closest variables to the name provided
	 *
	 * @param name Name to find
	 * @param inherited True if inherited variables should be included.
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return List of variables that are close in name to the name given
	 */
	virtual std::vector<const ReflectedField *> getClosestGlobalFields(
			std::string name, int count = MAX_SIMILAR);


	/**
	 * Gets a string comparing the name to the closest names found
	 *
	 * @param name Name to find
	 * @param inherited True if inherited values should be included.
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return String with a list of operators that are close in name
	 * to the name given
	 */
	virtual std::string getClosestGlobalFieldsString(
			std::string name, int count = MAX_SIMILAR);

	/**
	 * Gets the number of classes
	 *
	 * @return number of classes
	 */
	size_t getClassCount() const;

	/**
	 * Checks to see if a class exists using the class's name.
	 *
	 *
	 *
	 *  @param name Name of the class
	 *  @return true is that class exists
	 */
	bool doesClassExist(std::string name) const;


	/**
	 * Gets a list of all classes that are being reflected
	 *
	 * @return a list of class names
	 */
	const std::vector<std::string> getClassNames() const;

	/**
	 * Gets a class that corresponds to the name or NULL
	 *
	 * @return a ReflectedClass object or NULL
	 */
	const ReflectedClass * getClass(std::string signature) const;


	/**
	 * Gets a class that corresponds to the type or NULL.
	 *
	 * This ignores modifiers like const or volitile and * and & and returns
	 * the class if it is being reflected
	 *
	 * @return a ReflectedClass object or NULL
	 */
	const ReflectedClass * getClassFromType(std::string signature) const;



	/**
	 * Gets a list of all classes
	 *
	 * @param inherited True if inherited classes should be included.
	 *
	 * @return List of classes
	 */
	virtual std::vector<ReflectedClass *> getClasses();

	/**
	 * Gets a list of the closest classes to the name provided
	 *
	 * @param name Name to find
	 * @param inherited True if inherited classes should be included.
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return List of classes that are close in name to the name given
	 */
	virtual std::vector<ReflectedClass *> getClosestClasses(
			std::string name, int count = MAX_SIMILAR);


	/**
	 * Gets a string comparing the name to the closest names found
	 *
	 * @param name Name to find
	 * @param inherited True if inherited values should be included.
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return String with a list of classes that are close in name
	 * to the name given
	 */
	virtual std::string getClosestClassesString(
			std::string name, int count = MAX_SIMILAR);

	//TODO namespaces - NOT implemented yet
	/**
	 * Gets the number of namespaces
	 *
	 * @return number of namespaces
	 */
	size_t getNamespaceCount() const;

	/**
	 * Gets a list of all namespaces that are being reflected
	 *
	 * @return a list of namespaces
	 */
	const std::vector<std::string> getNamespaceNames() const;

	/**
	 * Checks to see if a namespace exists using the namespace's full name.
	 *
	 *The name is the full namespace including all enclosing namespaces
	 * (i.e. cpptesting::internal::test )
	 *
	 *  @param name Full name of the namespace
	 *  @return true is that namespace exists
	 */
	bool doesNamespaceExist(std::string name) const;

	/**
	 * Gets a namespace that corresponds to the name or NULL
	 *
	 * The name is the full namespace including all enclosing namespaces
	 * (i.e. cpptesting::internal::test )
	 *
	 * @return a ReflectedNamespace object or NULL
	 */
	ReflectedNamespace * getNamespace(std::string name) const;


	/**
	 * Gets a list of all namespaces
	 *
	 * @param inherited True if inherited namespaces should be included.
	 *
	 * @return List of namespaces
	 */
	virtual std::vector<ReflectedNamespace *> getNamespaces();

	/**
	 * Gets a list of the closest namespaces to the name provided
	 *
	 * @param name Name to find
	 * @param inherited True if inherited namespaces should be included.
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return List of namespaces that are close in name to the name given
	 */
	virtual std::vector<ReflectedNamespace *> getClosestNamespaces(
			std::string name, int count = MAX_SIMILAR);


	/**
	 * Gets a string comparing the name to the closest names found
	 *
	 * @param name Name to find
	 * @param inherited True if inherited values should be included.
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
