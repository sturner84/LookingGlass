/*
 * ReflectedDataBase.h
 *
 *  Created on: Oct 25, 2013
 *      Author: Administrator
 */

#ifndef REFLECTEDDATABASE_H_
#define REFLECTEDDATABASE_H_

#include <cpgf/gmetaclass.h>
#include <cpgf/gmetacommon.h>
#include <cpgf/gpp.h>
#include <string>

#include "lgReflectionUtil.h"
#include "lgReflectedItems.h"
#include "lgTestingSupport.h"

namespace cpptesting {



class ReflectedDataBase {
protected:
	/**
	 * All methods/functions in the class/at global scope
	 */
	std::map<std::string, const ReflectedMethod *> methods;

	/**
	 * All fields/variables in the class/at gloabal scope
	 */
	std::map<std::string, const ReflectedField *> fields;

	/**
	 * All operators in the class/at gloabal scope
	 * //TODO not supported yet?
	 */
	std::map<std::string, const ReflectedOperator *> operators;

	/**
	 * All enums in the class/at gloabal scope
	 *
	 */
	std::map<std::string, const ReflectedEnum *> enums;

	/**
	 * Loads data from the class (methods, fields, operators and enums).
	 * Works for both the global class and other classes.
	 *
	 * @param gclass Class to load
	 */
	void loadData(const cpgf::GMetaClass* gclass);

	/**
	 * Destroys the data
	 */
	virtual ~ReflectedDataBase();

	/**
	 * Gets an item from one of the maps by name
	 *
	 * @param dataMap Map with the data to access
	 * @param name Name (signature) to find
	 *
	 * @return The item (method, variable, etc.) or NULL if not found
	 */
	template <class T>
	T getItem(const std::map<std::string, T> &dataMap, const std::string &name) const;

	/**
	 * Gets a list of all of the names from one of the maps
	 *
	 * @param vMap Map with the data to access
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 * @param inherited True if inherited items should be included.
	 *
	 * @return List of names in the map
	 */
	template <class T>
	const std::vector<std::string> getNames(
			const std::map<std::string, T> &vMap, VisibilityAccessType vis,
			bool inherited) const;

	/**
	 * Counts the number of values in the map. The value is different depending
	 * on whether you include inherited values or not.
	 *
	 * @param vMap Map with the data to access
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 * @param inherited True if inherited items should be included.
	 *
	 * @return Number of item in the map
	 */
	template <class T>
	size_t countItems(
			const std::map<std::string, T> &vMap, VisibilityAccessType vis,
			bool inherited) const;

	/**
	 * Gets a list of all of the items from one of the maps
	 *
	 * @param vMap Map with the data to access
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 * @param inherited True if inherited items should be included.
	 *
	 * @return List of items in the map
	 *
	 */
	template <class T>
	std::vector<T> getItems(
		const std::map<std::string, T> &vMap, VisibilityAccessType vis,
		bool inherited) const;

	/**
	 * Gets a list of the items matching the names in names
	 *
	 * @param vMap Map with the data to access
	 * @param names List of names of the items to retreive
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 * @param inherited True if inherited items should be included.
	 *
	 * @return List of items in the map
	 *
	 */
	template <class T>
	std::vector<T> getItems(
			const std::map<std::string, T> &vMap,
			const std::vector<std::string> &names, VisibilityAccessType vis,
			bool inherited) const;

	/**
	 * Gets a list of all of the items closest in name to the supplied name
	 *
	 * @param vMap Map with the data to access
	 * @param name Name to find
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 * @param inherited True if inherited items should be included.
	 *
	 * @return List of items closest in name
	 *
	 */
	template <class T>
	std::vector<T> getClosest(
			const std::map<std::string, T> &vMap, std::string name,
			VisibilityAccessType vis, bool inherited, int count) const;

	/**
	 * Gets a string comparing the closest items to the name to the name
	 *
	 * @param vMap Map with the data to access
	 * @param name Name to find
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 * @param inherited True if inherited items should be included.
	 *
	 * @return String comparing the closest items to the name
	 *
	 */
	template <class T>
	std::string getClosestString(
			const std::map<std::string, T> &vMap, std::string name,
			VisibilityAccessType vis, bool inherited, int count) const;


	/**
	 * Determines if a name exists in the map.
	 *
	 * @param vMap Map with data to acces
	 * @param name Name to find
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 * @param inherited True if inherited items should be included.
	 *
	 * @return True if this map contains the name
	 *
	 */
	template <class T>
	bool exists(const std::map<std::string, T> &vMap, std::string name,
			VisibilityAccessType vis, bool inherited) const;

	/**
	 * Checks to see if a function exists using the function's signature.
	 *
	 * Usage:
	 * 	int foo(int, double)
	 * 		Check to see if a function foo exists that returns an int and
	 * 		takes an int and double as parameters
	 *
	 *  @param functionSignature Signature of the function
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited Determines if inherited values are included (by default
	 *  	this is false)
	 *  @return true is that function exists
	 */
	virtual bool doesMethodExist(std::string functionSignature,
			VisibilityAccessType vis = Public_Access, bool inherited = false) const;


	/**
	 * Gets the number of function signatures that are being reflected
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 * @param inherited Determines if inherited values are included (by default
	 *  	this is false)
	 *
	 * @return the number of function signatures
	 */
	virtual size_t getMethodCount(VisibilityAccessType vis = Public_Access,
			bool inherited = false) const;


	/**
	 * Gets a list of all function signatures that are being reflected
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited Determines if inherited values are included (by default
	 *  	this is false)
	 * @return a list of function signatures
	 */
	virtual const std::vector<std::string> getMethodNames(
			VisibilityAccessType vis = Public_Access, bool inherited = false) const;

	/**
	 * Gets a function that corresponds to the signature or NULL
	 *
	 * @return a metadata object or NULL
	 */
	virtual const ReflectedMethod * getMethod(std::string signature) const;


	/**
	 * Determines if a field exists with this signature
	 *
	 * @param signature Signature of the value (i.e. int x or const double y)
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited Determines if inherited values are included (by default
	 *  	this is false)
	 * @return true if the field exists
	 */
	virtual bool doesFieldExist(std::string signature, VisibilityAccessType vis = Public_Access,
			bool inherited = false) const;

	/**
	 * Gets the number of variables declared with a global scope
	* @param vis Visibility of items to retrieve. Generally this is Public_Access.
	  *  @param inherited Determines if inherited values are included (by default
	 *  	this is false)
	 * @return number of variables with a global scope.
	 */
	virtual size_t getVariableCount(VisibilityAccessType vis = Public_Access,
			bool inherited = false) const;


	/**
	 * Gets the signatures of variables declared with a
	 * global scope
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited Determines if inherited values are included (by default
	 *  	this is false)
	 * @return a list of the signatures of variables
	 * with a global scope.
	 */
	virtual const std::vector<std::string> getVariableNames(
			VisibilityAccessType vis = Public_Access, bool inherited = false) const;

	/**
	 * Gets the number of constants declared with a global scope
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited Determines if inherited values are included (by default
	 *  	this is false)
	 * @return number of constants with a global scope.
	 */
	virtual size_t getConstantCount(VisibilityAccessType vis = Public_Access,
			bool inherited = false) const;

	/**
	 * Gets the signatures of constants declared with a
	 * global scope
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited Determines if inherited values are included (by default
	 *  	this is false)
	 * @return a list of the signatures of constants
	 * with a global scope.
	 */
	virtual const std::vector<std::string> getConstantNames(
			VisibilityAccessType vis = Public_Access, bool inherited = false) const;
	//all variables and fields


	/**
	 * Gets the number of constants and variables declared with a global scope
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited Determines if inherited values are included (by default
	 *  	this is false)
	 * @return number of constants and variables with a global scope.
	 */
	virtual size_t getFieldCount(VisibilityAccessType vis = Public_Access,
			bool inherited = false) const;

	/**
	 * Gets the signatures of constants and variables declared with a
	 * global scope
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited Determines if inherited values are included (by default
	 *  	this is false)
	 * @return a list of the signatures of constants and variables
	 * with a global scope.
	 */
	virtual const std::vector<std::string> getFieldNames(VisibilityAccessType vis = Public_Access,
			bool inherited = false) const;


	/**
	 * Gets a variable that corresponds to the signature or NULL
	 *
	 * @return a metadata object or NULL
	 */
	virtual const ReflectedField * getField(std::string signature) const;


public:

	/**
	 * Default number of similar items to return in the getClosestXXX methods
	 */
	static int MAX_SIMILAR;

	// operators
	//TODO can use doesFunctionExist(string) for operators too??
	/**
	 * Checks to see if an operator exists using the signature.
	 *
	 * Usage:
	 * 	int foo(int, double) //TODO update documentation
	 * 		Check to see if a function foo exists that returns an int and
	 * 		takes an int and double as parameters
	 *
	 *  @param functionSignature Signature of the function
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited Determines if inherited values are included (by default
	 *  	this is false)
	 *  @return true is that function exists
	 */
	virtual bool doesOperatorExist(std::string functionSignature,
			VisibilityAccessType vis = Public_Access, bool inherited = false) const;

	/**
	 * Gets the number of operators that are being reflected
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited Determines if inherited values are included (by default
	 *  	this is false)
	 * @return the number of operators
	 */
	virtual size_t getOperatorCount(VisibilityAccessType vis = Public_Access,
			bool inherited = false) const;

	/**
	 * Gets a list of all operators that are being reflected
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited Determines if inherited values are included (by default
	 *  	this is false)
	 * @return a list of operators
	 */
	virtual const std::vector<std::string> getOperatorNames(VisibilityAccessType vis = Public_Access,
			bool inherited = false) const;

	/**
	 * Gets an operator that corresponds to the signature or NULL
	 *
	 * @return a metadata object or NULL
	 */
	virtual const ReflectedOperator * getOperator(std::string signature) const;


	/**
	 * Gets a list of all operators in the class
	 *
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 * @param inherited True if inherited operators should be included.
	 *
	 * @return List of operators in the class
	 */
	virtual std::vector<const ReflectedOperator *> getOperators(
			VisibilityAccessType vis = Public_Access, bool inherited = false);

	/**
	 * Gets a list of the closest operators to the name provided
	 *
	 * @param name Name to find
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 * @param inherited True if inherited operators should be included.
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return List of operators that are close in name to the name given
	 */
	virtual std::vector<const ReflectedOperator *> getClosestOperators(
			std::string name, VisibilityAccessType vis = Public_Access,
			bool inherited = false, int count = MAX_SIMILAR);


	/**
	 * Gets a string comparing the name to the closest names found
	 *
	 * @param name Name to find
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 * @param inherited True if inherited values should be included.
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return String with a list of operators that are close in name
	 * to the name given
	 */
	virtual std::string getClosestOperatorsString(
			std::string name, VisibilityAccessType vis = Public_Access,
			bool inherited = false, int count = MAX_SIMILAR);

	//ignore values, just look for name
	/**
	 * Checks to see if an enum exists using the name.
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited Determines if inherited values are included (by default
	 *  	this is false)
	 *  @param name Name of the enum
	 *  @return true is that enum exists
	 */
	virtual bool doesEnumExist(std::string name, VisibilityAccessType vis = Public_Access,
			bool inherited = false) const;

	/**
	 * Gets the number of enums that are being reflected
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited Determines if inherited values are included (by default
	 *  	this is false)
	 * @return the number of enums
	 */
	virtual size_t getEnumCount(VisibilityAccessType vis = Public_Access,
			bool inherited = false) const;

	/**
	 * Gets a list of all enums that are being reflected
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 *  @param inherited Determines if inherited values are included (by default
	 *  	this is false)
	 * @return a list of enums
	 */
	virtual const std::vector<std::string> getEnumNames(
			VisibilityAccessType vis = Public_Access, bool inherited = false) const;

	/**
	 * Gets an enum that corresponds to the name or NULL
	 *
	 * @return an enum object or NULL
	 */
	virtual const ReflectedEnum * getEnum(std::string signature) const;


	/**
	 * Gets a list of all enums in the class
	 *
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 * @param inherited True if inherited enums should be included.
	 *
	 * @return List of enums in the class
	 */
	virtual std::vector<const ReflectedEnum *> getEnums(
			VisibilityAccessType vis = Public_Access, bool inherited = false);

	/**
	 * Gets a list of the closest enums to the name provided
	 *
	 * @param name Name to find
	* @param vis Visibility of items to retrieve. Generally this is Public_Access.
	  * @param inherited True if inherited enums should be included.
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return List of enums that are close in name to the name given
	 */
	virtual std::vector<const ReflectedEnum *> getClosestEnums(
			std::string name, VisibilityAccessType vis = Public_Access,
			bool inherited = false, int count = MAX_SIMILAR);

	/**
	 * Gets a string comparing the name to the closest names found
	 *
	 * @param name Name to find
	 * @param vis Visibility of items to retrieve. Generally this is Public_Access.
	 * @param inherited True if inherited values should be included.
	 * @param count Maximum number to return (may be less or more if there
	 *  are values that are equally close to the name)
	 *
	 * @return String with a list of enums that are close in name
	 *  to the name given
	 */
	virtual std::string getClosestEnumsString(
			std::string name, VisibilityAccessType vis = Public_Access,
			bool inherited = false, int count = MAX_SIMILAR);

};


template <class T>
T ReflectedDataBase::getItem(const std::map<std::string, T> & dataMap,
		const std::string & name) const
{
	if (dataMap.count(name) == 1)
	{
		return dataMap.find(name)->second;
		//return dataMap[name];
	}

	return NULL;
}


template <class T>
const std::vector<std::string> ReflectedDataBase::getNames(
		const std::map<std::string, T> &vMap, VisibilityAccessType vis,
		bool inherited) const
{
	std::vector<std::string> names;

	for (typename std::map<std::string, T>::const_iterator it = vMap.begin();
			it != vMap.end(); it++)
	{
		//get either the objects declared in the class or if
		// inherited is true, everything
		if ((vis & it->second->getVisibility()) &&
				(inherited || !it->second->isInherited()))
		{
			names.push_back(it->first);
		}
	}

	return names;
}

template <class T>
size_t ReflectedDataBase::countItems(
		const std::map<std::string, T> &vMap, VisibilityAccessType vis,
		bool inherited) const
{
	size_t count = 0;

	for (typename std::map<std::string, T>::const_iterator it = vMap.begin();
			it != vMap.end(); it++)
	{
		//get the objects declared in the class
		if ((vis & it->second->getVisibility()) &&
						(inherited || !it->second->isInherited()))
		{
			count++;
		}
	}

	return count;
}




template <class T>
std::vector<T> ReflectedDataBase::getItems(
	const std::map<std::string, T> &vMap, VisibilityAccessType vis,
	bool inherited) const
{
	std::vector<T> items;

	for (typename std::map<std::string, T>::const_iterator it = vMap.begin();
			it != vMap.end(); it++)
	{
		//get either the objects declared in the class or if
		// inherited is true, everything
		if ((vis & it->second->getVisibility()) &&
						(inherited || it->second->isInherited() == false))
		{
			items.push_back(it->second);
		}
	}

	return items;
}


template <class T>
std::vector<T> ReflectedDataBase::getItems(
		const std::map<std::string, T> &vMap,
		const std::vector<std::string> &names, VisibilityAccessType vis,
		bool inherited) const
{
	std::vector<T> items;

	for (std::vector<std::string>::const_iterator it = names.begin();
			it != names.end(); it++)
	{
		T item = getItem(vMap, (*it));
		//get either the objects declared in the class or if
		// inherited is true, everything
		if (item && (vis & item->getVisibility()) &&
				(inherited || item->isInherited() == false))
		{
			items.push_back(item);
		}
	}

	return items;
}


template <class T>
std::vector<T> ReflectedDataBase::getClosest(
		const std::map<std::string, T> &vMap, std::string name,
		VisibilityAccessType vis, bool inherited, int count) const
{
	std::vector<std::string> names = StringDistance::getNClosest(name,
			getNames(vMap, vis, inherited), count);

	return getItems(vMap, names, vis, inherited);
}

template <class T>
std::string ReflectedDataBase::getClosestString(
		const std::map<std::string, T> &vMap, std::string name,
		VisibilityAccessType vis, bool inherited, int count) const
{
	return StringDistance::getNClosestAsString(name,
			getNames(vMap, vis, inherited), count);
}


template <class T>
bool ReflectedDataBase::exists(const std::map<std::string, T> &vMap,
		std::string name, VisibilityAccessType vis, bool inherited) const
{
	T m = getItem(vMap, name);
	return (m && (vis & m->getVisibility())
			&& (inherited || !m->isInherited()));
}


}

#endif /* REFLECTEDDATABASE_H_ */