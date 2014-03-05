/*
 * lgReflectedEnum.h
 *
 *  Created on: Feb 12, 2014
 *      Author: Administrator
 */

#ifndef LGREFLECTEDENUM_H_
#define LGREFLECTEDENUM_H_

#include <cpgf/gmetaclass.h>
#include <map>
#include <string>

namespace cpptesting {

/**
 * Possible visibilities for the c++ fields/methods/classes
 */
enum VisibilityType
{
	Public = 1,
	Protected = 2,
	Private = 4,
};

enum VisibilityAccessType
{
	Public_Access = 1,
	Protected_Access = 2,
	Private_Access = 4,
	All_Access = 7,
};


/**
 * Possible types of reflected items (methods, variables, namespaces, etc.
 */
enum ReflectedItemType
{
	Namespace,
	Class,
	Method, //includes functions and methods
	Field, //includes global variables and fields
	Enum,
	Operator,
	Constructor,
};


/**
 * @class ReflectedItem
 * @brief Represents an item being reflected.  This may or may not be actually
 * reflected and may just be a name.
 *
 *
 */
class ReflectedItem
{
protected:
	/**
	 * name/signature of the item being reflected
	 */
	std::string name;
	/**
	 * determines if the item was inherited from another class
	 */
	bool inherited;
//	/**
//	 * Determines if the item can be accessed (could be private or may not
//	 * exist.
//	 */
//	bool isAccessible;

	/**
	 * Visibility of the item
	 */
	VisibilityType visibility;

	/**
	 * Type of item being reflected (method, variable, etc.)
	 */
	ReflectedItemType type;
public:
	/**
	 * @param nName Name/signature of the item
	 * @param inherit true if the item was inherited
	 * @param accessible true if the item was reflected and can be accessed
	 * @param vis Visibility of the item
	 * @param iType The type of item being reflected (method, variable, etc.)
	 */
	ReflectedItem(std::string nName, bool inherit, VisibilityType vis,
			ReflectedItemType iType);

	/**
	 * Destructor
	 */
	virtual ~ReflectedItem();

	/**
	 * Gets the name/signature of the item
	 *
	 * @return name/signature of the item
	 */
	virtual std::string getName() const;

	/**
	 * Returns true if the item is being reflected and is accessible
	 *
	 * @return true if reflected
	 */
	virtual bool isAccessible() const = 0;

	/**
	 * Returns true if the item was inherited
	 *
	 * @return true if inherited
	 */
	virtual bool isInherited() const;

	/**
	 * Returns the visibility of the item
	 *
	 * @return Visibility of the item
	 */
	virtual VisibilityType getVisibility() const;

	/**
	 * Returns the type of the item.
	 *
	 * @return Type of the item (method, variable, etc.)
	 */
	virtual ReflectedItemType getType() const;
};


class ReflectedMethod : public ReflectedItem
{
protected:
	/**
	 * Method being reflected
	 */
	const cpgf::GMetaMethod * method;
public:
	/**
	 * Creates an accessible method.  Assumes that m is not NULL and that
	 * it is public.
	 *
	 * @param m Method being reflected
	 * @param inherit Determines if the method was inherited or not
	 */
	ReflectedMethod(const cpgf::GMetaMethod * m, bool inherit);

	/**
	 * Creates an unaccessible method.
	 *
	 * @param nName Name of method
	 * @param inherit Determines if the method was inherited or not
	 * @param vis Visibility of the method
	 */
	ReflectedMethod(std::string nName, bool inherit, VisibilityType vis);

	/**
	 * Gets the method being reflected
	 *
	 * @return The reflected object or NULL if not reflecting anything
	 */
	virtual const cpgf::GMetaMethod * getMethod() const;

	virtual bool isAccessible() const;
};


class ReflectedConstructor : public ReflectedItem
{
protected:
	/**
	 * Constructor being reflected
	 */
	const cpgf::GMetaConstructor * constructor;
public:
	/**
	 * Creates an accessible constructor.  Assumes that c is not NULL and that
	 * it is public.
	 *
	 * @param c Constructor being reflected
	 * @param inherit Determines if the constructor was inherited or not
	 */
	ReflectedConstructor(const cpgf::GMetaConstructor * c, bool inherit);

	/**
	 * Creates an unaccessible constructor.
	 *
	 * @param nName Name of constructor
	 * @param inherit Determines if the constructor was inherited or not
	 * @param vis Visibility of the constructor
	 */
	ReflectedConstructor(std::string nName, bool inherit, VisibilityType vis);

	/**
	 * Gets the constructor being reflected
	 *
	 * @return The reflected object or NULL if not reflecting anything
	 */
	const cpgf::GMetaConstructor * getConstructor() const;

	virtual bool isAccessible() const;
};


class ReflectedField : public ReflectedItem
{
protected:
	/**
	 * Method being reflected
	 */
	const cpgf::GMetaField * field;
public:
	/**
	 * Creates an accessible field.  Assumes that f is not NULL and that
	 * it is public.
	 *
	 * @param f Field being reflected
	 * @param inherit Determines if the field was inherited or not
	 */
	ReflectedField(const cpgf::GMetaField * f, bool inherit);

	/**
	 * Creates an unaccessible field.
	 *
	 * @param nName Name of field
	 * @param inherit Determines if the field was inherited or not
	 * @param vis Visibility of the field
	 */
	ReflectedField(std::string nName, bool inherit, VisibilityType vis);

	/**
	 * Gets the method being reflected
	 *
	 * @return The reflected object or NULL if not reflecting anything
	 */
	virtual const cpgf::GMetaField * getField() const;

	virtual bool isAccessible() const;
};


class ReflectedOperator : public ReflectedItem
{
protected:
	/**
	 * Operator being reflected
	 */
	const cpgf::GMetaOperator * op;
public:
	/**
	 * Creates an accessible operator.  Assumes that of is not NULL and that
	 * it is public.
	 *
	 * @param o Operator being reflected
	 * @param inherit Determines if the operator was inherited or not
	 */
	ReflectedOperator(const cpgf::GMetaOperator * o, bool inherit);

	/**
	 * Creates an unaccessible operator.
	 *
	 * @param nName Name of operator
	 * @param inherit Determines if the operator was inherited or not
	 * @param vis Visibility of the operator
	 */
	ReflectedOperator(std::string nName, bool inherit, VisibilityType vis);

	/**
	 * Gets the operator being reflected
	 *
	 * @return The reflected object or NULL if not reflecting anything
	 */
	virtual const cpgf::GMetaOperator * getOperator() const;

	virtual bool isAccessible() const;
};




/**
 * @class ReflectedEnum
 * @brief Represents a reflected enum and allows access to the enumeration's
 * names and (int) values.
 */
class ReflectedEnum : public ReflectedItem {
protected:
	/**
		 * Pointer to raw, reflected enum
		 */
		const cpgf::GMetaEnum* mEnum;
		/**
		 * Maps of constructors stored by their signatures
		 *  i.e. Foo(int, double) //for class Foo
		 */
		std::map<std::string, int> enumByNames;
		std::map<int, std::string> enumByValues;

		/**
		 * Loads data from the reflected class into the maps (like constructors)
		 *
		 * @param gclass Reflected class
		 */
		virtual void loadData(const cpgf::GMetaEnum* genum);

	public:

		/**
		 * Creates a wrapped for the reflected class
		 *
		 * @param e Reflected enum
		 * @param inherit Weather this is inherited or not
		 */
		ReflectedEnum(const cpgf::GMetaEnum* e, bool inherit);

		/**
		 * Creates a wrapped for the reflected class
		 *
		 * @param nName Name of the enum
		 * @param inherit Weather this is inherited or not
		 * @param vis Visibility of the enum
		 */
		ReflectedEnum(std::string nName, bool inherit, VisibilityType vis);

		/**
		 * Cleans up the wrapper class
		 */
		virtual ~ReflectedEnum();

		/**
		 * Gets a reference to the underlying reflected object
		 *
		 * @return Reflected enum
		 */
		const cpgf::GMetaEnum * getEnum() const;

		/**
		 * Gets the number of values in this enum
		 *
		 * @return The number of values in this enum
		 */
		size_t getNumValues() const;

		/**
		 * Gets the name of the value at a position.
		 *
		 * @return The name of a value (by position). If index is invalid,
		 * 	"" is returned.
		 */
		std::string getName(size_t index) const;

		/**
		 * Gets the value as an int at a position.
		 *
		 * @return The value (by position). If index is invalid,
		 * 	INT_MIN is returned.
		 */
		int getValue(size_t index) const;


		/**
		 * Gets a list of all of the names in this enum
		 *
		 * @return All names in this enum
		 */
		std::vector<std::string> getNames() const;

		/**
		 * Gets a list of all of the values in this enum
		 *
		 * @return All value in this enum
		 */
		std::vector<int> getValues() const;

		/**
		 * Returns the matching value for this name.
		 *
		 * @return The value for the name.  If the name is not found,
		 * INT_MIN is returned.
		 */
		int getValueForName(std::string name) const;

		/**
		 * Returns the matching anem for this value.
		 *
		 * @return The name for the value.  If the value is not found,
		 * "" is returned.
		 */
		std::string getNameForValue(int value) const;

		/**
		 * Returns true if the item is being reflected and is accessible
		 *
		 * @return true if reflected
		 */
		virtual bool isAccessible() const;
};



/**
 * @class ReflectedNamespace
 * @brief Represents a namespace.  This is an enumeration but allows access
 * to enclosed data.
 *
 *
 */
class ReflectedNamespace : public ReflectedItem
{
protected:
	/**
	 * @brief Full name of the namespace (including any enclosing namespaces). The
	 * namespaces are separated by :: (i.e. cpptesting::inner::test, etc.)
	 */
	std::string fullNamespace;
	/**
	 * @brief Determines if this is an anonymous namespace.
	 */
	bool isAnonymous;
	/**
	 * @brief List of items in this namespace
	 */
	std::vector<const ReflectedItem *> items;

	/**
	 * @brief List of namespaces this one encloses.
	 */
	std::vector<ReflectedNamespace *> enclosedNamespaces;
public:

	/**
	 * @brief Creates a namespace.
	 *
	 * @param fullName Name of operator
	 */
	ReflectedNamespace(std::string fullName);

	/**
	 * @brief Destructor
	 */
	virtual ~ReflectedNamespace();

	/**
	 * @brief Gets full namespace
	 *
	 * @return The full namespace
	 */
	virtual std::string getFullName() const;

	/**
	 * @brief Determines if this can be accessed.
	 * @return Always returns false
	 */
	virtual bool isAccessible() const;

	/**
	 * @brief Gets a list of all items (methods, variables, etc.) in the
	 * namespace
	 *
	 * @return List of items in the namespace
	 */
	virtual std::vector<const ReflectedItem *> getEnclosedItems() const;

	/**
	 * @brief Gets an item (method, variable, etc.) from the
	 * namespace
	 *
	 * @param name Name of the item to get (signature)
	 *
	 * @return The item or NULL if that name does not exist
	 */
	virtual const ReflectedItem * getEnclosedItem(std::string name) const;

	/**
	 * @brief Gets a list of all namespaces enclosed in the
	 * namespace
	 *
	 * @return List of the namespaces
	 */
	virtual std::vector<ReflectedNamespace *> getEnclosedNamespaces() const;

	/**
	 * @brief Gets a namespace enclosed in the
	 * namespace
	 *
	 * @param name Name of the namespace to get
	 *
	 * @return The namespace or NULL if that name does not exist
	 */
	virtual ReflectedNamespace * getEnclosedNamespace(std::string name) const;

	/**
	 * @brief Determines if the namespace is anonymous
	 *
	 * @return True if anonymous
	 */
	virtual bool isAnonymousNamespace() const;

	/**
	 * @brief Adds an item (methods, variables, etc.)
	 *
	 * @param item Item to add
	 */
	virtual void addItem(const ReflectedItem * item);

	/**
	 * @brief Adds a namespace
	 *
	 * @param nNamespace Namespace to add
	 */
	virtual void addNamespace(ReflectedNamespace * nNamespace);
};


}



#endif /* LGREFLECTEDENUM_H_ */
