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
#include <boost/shared_ptr.hpp>

namespace cpptesting {



//const int metaModifierNone = 0;
//const int metaModifierStatic = 1 << 0;
//const int metaModifierVirtual = 1 << 2;
//const int metaModifierPureVirtual = 1 << 3;
//const int metaModifierTemplate = 1 << 4;
//const int metaModifierConst = 1 << 5;
//const int metaModifierVolatile = 1 << 6;
//const int metaModifierInline = 1 << 7;
//const int metaModifierExplicit = 1 << 8;
//const int metaModifierExtern = 1 << 9;
//const int metaModifierMutable = 1 << 10;

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
private:
	static void addModifier(int & mods, std::string & currentString,
			int modToCheck, std::string modStr);
protected:
	/**
	 * name of the item being reflected (i.e. x, foo, etc.)
	 */
	std::string name;

	/**
	 * Signature of the item being reflected (i.e. int x, void foo(), etc.)
	 */
	std::string signature;
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

	/**
	 * modifiers for this item
	 */
	int mods;

	/**
	 * Determines if this item is global in nature (i.e. a class, a function,
	 * a global enum or variable).
	 */
	bool global;
public:
	/**
	 * @param nName Name/signature of the item
	 * @param inherit true if the item was inherited
	 * @param accessible true if the item was reflected and can be accessed
	 * @param vis Visibility of the item
	 * @param iType The type of item being reflected (method, variable, etc.)
	 * @param modifiers Modifiers being applied to this item (const, virtual,
	 * 	static, etc.)
	 */
	ReflectedItem(std::string nName, std::string sig, bool inherit,
			bool isGlobal, VisibilityType vis,	ReflectedItemType iType,
			int modifiers);

	/**
	 * Destructor
	 */
	virtual ~ReflectedItem();

	/**
	 * Gets the name of the item
	 *
	 * @return name of the item
	 */
	virtual std::string getName() const;


	/**
	 * Gets the signature of the item
	 *
	 * @return signature of the item
	 */
	virtual std::string getSignature() const;

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


	/**
	 * Gets the modifiers for this item
	 * @return modifiers for this item
	 */
	virtual int getModifiers() const;

	/**
	 * Converts cpgf visibility enumeration to LookingGlass's VisibilityType
	 * enumeration
	 *
	 * @param vis cpgf visibility enumeration
	 * @return Corresponding VisibilityType
	 */
	static VisibilityType cpgfToLGVisibility(cpgf::GMetaVisibility vis);

	/**
	 * Returns a string representation of the modifiers represented by the
	 * parameter.
	 *
	 * The string appears as a set with a list of modifier names.  The names
	 * include:
	 * 	Static
	 * 	Virtual
	 * 	Pure Virtual
	 * 	Template
	 * 	Const
	 * 	Volatile
	 * 	Inline
	 * 	Explicit
	 * 	Extern
	 * 	Mutable
	 * 	Other  //used for invalid modifiers
	 *
	 * @param modifiers Modifiers to represent in string form
	 * @return String representation of the modifiers
	 */
	static std::string modifiersToString(int modifiers);

	/**
	 * Returns a string representation of the modifiers associated with this
	 * item.
	 *
	 * The string appears as a set with a list of modifier names.  The names
	 * include:
	 * 	Static
	 * 	Virtual
	 * 	Pure Virtual
	 * 	Template
	 * 	Const
	 * 	Volatile
	 * 	Inline
	 * 	Explicit
	 * 	Extern
	 * 	Mutable
	 * 	Other  //used for invalid modifiers
	 *
	 * @return String representation of the modifiers
	 */
	virtual std::string getModifiersAsString() {
		return modifiersToString(mods);
	}

	/**
	 * Determines if this item is at the global level. This includes classes,
	 * functions, global enums, and global variables
	 *
	 * @return true if the item is global
	 */
	virtual bool isGlobal();
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
	ReflectedMethod(const cpgf::GMetaMethod * m,
			bool inherit, bool isGlobal);

	/**
	 * Creates an unaccessible method.
	 *
	 * @param sig Signature of method
	 * @param inherit Determines if the method was inherited or not
	 * @param vis Visibility of the method
	 */
	ReflectedMethod(std::string sig, bool inherit, bool isGlobal,
			VisibilityType vis, int modifiers);

	/**
	 * Gets the method being reflected
	 *
	 * @return The reflected object or NULL if not reflecting anything
	 */
	virtual const cpgf::GMetaMethod * getMethod() const;

	virtual bool isAccessible() const;

	/**
	 * Determines if this method is static or not.
	 * @return true if static
	 */
	virtual bool isStatic() const {
		return (mods & cpgf::metaModifierStatic) != 0;
	}

	/**
	 * Determines if this method is virtual or not. This will return true
	 * if the method is pure virtual too.
	 * @return true if virtual
	 */
	virtual bool isVirtual() const {
		return (mods & cpgf::metaModifierVirtual) != 0;
	}

	/**
	 * Determines if this method is pure virtual or not.
	 * @return true if pure virtual
	 */
	virtual bool isPureVirtual() const {
		return (mods & cpgf::metaModifierPureVirtual) != 0;
	}

	/**
	 * TODO not currently supported
	 * Determines if this method is a template.
	 * @return true if a template
	 */
	virtual bool isTemplate() const {
		return (mods & cpgf::metaModifierTemplate) != 0;
	}

	/**
	 * Determines if this method is const or not.
	 * @return true if const
	 */
	virtual bool isConst() const {
		return (mods & cpgf::metaModifierConst) != 0;
	}

	/**
	 * Determines if this method is inline or not.
	 * @return true if inline
	 */
	virtual bool isInline() const {
		return (mods & cpgf::metaModifierInline) != 0;
	}

	/**
	 * Determines if this method (function) is declared extern or not.
	 * @return true if extern
	 */
	virtual bool isExtern() const {
		return (mods & cpgf::metaModifierExtern) != 0;
	}
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
	 * @param sig Signature of the constructor
	 * @param inherit Determines if the constructor was inherited or not
	 */
	ReflectedConstructor(const cpgf::GMetaConstructor * c,
			bool inherit);

	/**
	 * Creates an unaccessible constructor.
	 *
	 * @param sig Name of constructor
	 * @param inherit Determines if the constructor was inherited or not
	 * @param vis Visibility of the constructor
	 */
	ReflectedConstructor(std::string sig, bool inherit,
			VisibilityType vis, int modifiers);

	/**
	 * Gets the constructor being reflected
	 *
	 * @return The reflected object or NULL if not reflecting anything
	 */
	virtual	const cpgf::GMetaConstructor * getConstructor() const;

	virtual bool isAccessible() const;


	/**
	 * TODO not currently supported
	 * Determines if this constructor is a template.
	 * @return true if a template
	 */
	virtual bool isTemplate() const {
		return (mods & cpgf::metaModifierTemplate) != 0;
	}

	/**
	 * TODO not currently supported
	 * Determines if this constructor is inline or not.
	 * @return true if inline
	 */
	virtual bool isInline() const {
		return (mods & cpgf::metaModifierInline) != 0;
	}

	/**
	 * TODO not currently supported
	 * Determines if this constructor is explicit or not.
	 * @return true if explicit
	 */
	virtual bool isExplicit() const {
		return (mods & cpgf::metaModifierExplicit) != 0;
	}

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
	ReflectedField(const cpgf::GMetaField * f, bool inherit, bool isGlobal);

	/**
	 * Creates an unaccessible field.
	 *
	 * @param sig Signature of field
	 * @param inherit Determines if the field was inherited or not
	 * @param vis Visibility of the field
	 */
	ReflectedField(std::string sig, bool inherit, bool isGlobal,
			VisibilityType vis, int modifiers);

	/**
	 * Gets the method being reflected
	 *
	 * @return The reflected object or NULL if not reflecting anything
	 */
	virtual const cpgf::GMetaField * getField() const;

	virtual bool isAccessible() const;


	/**
	 * Determines if this field is static or not.
	 * @return true if static
	 */
	virtual bool isStatic() const {
		return (mods & cpgf::metaModifierStatic) != 0;
	}

	/**
	 * TODO not currently supported
	 * Determines if this field is a template.
	 * @return true if a template
	 */
	virtual bool isTemplate() const {
		return (mods & cpgf::metaModifierTemplate) != 0;
	}

	/**
	 * Determines if this field is const or not.
	 * @return true if const
	 */
	virtual bool isConst() const {
		return (mods & cpgf::metaModifierConst) != 0;
	}

	/**
	 * Determines if this field is volatile or not.
	 * @return true if volatile
	 */
	virtual bool isVolatile() const {
		return (mods & cpgf::metaModifierVolatile) != 0;
	}

	/**
	 * TODO not currently supported
	 * Determines if this field is volatile or not.
	 * @return true if volatile
	 */
	virtual bool isMutable() const {
		return (mods & cpgf::metaModifierMutable) != 0;
	}

	/**
	 * Determines if this field (variable) is declared extern or not.
	 * @return true if extern
	 */
	virtual bool isExtern() const {
		return (mods & cpgf::metaModifierExtern) != 0;
	}
};


//class ReflectedOperator : public ReflectedItem
//{
//protected:
//	/**
//	 * Operator being reflected
//	 */
//	const cpgf::GMetaOperator * op;
//public:
//	/**
//	 * Creates an accessible operator.  Assumes that of is not NULL and that
//	 * it is public.
//	 *
//	 * @param o Operator being reflected
//	 * @param inherit Determines if the operator was inherited or not
//	 */
//	ReflectedOperator(const cpgf::GMetaOperator * o, bool inherit,
//			bool isGlobal);
//
//	/**
//	 * Creates an unaccessible operator.
//	 *
//	 * @param sig Signature of operator
//	 * @param inherit Determines if the operator was inherited or not
//	 * @param vis Visibility of the operator
//	 */
//	ReflectedOperator(std::string sig, bool inherit, bool isGlobal,
//			VisibilityType vis, int modifiers);
//
//	/**
//	 * Gets the operator being reflected
//	 *
//	 * @return The reflected object or NULL if not reflecting anything
//	 */
//	virtual const cpgf::GMetaOperator * getOperator() const;
//
//	virtual bool isAccessible() const;
//};




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
		ReflectedEnum(const cpgf::GMetaEnum* e, bool inherit, bool isGlobal);

		/**
		 * Creates a wrapped for the reflected class
		 *
		 * @param nName Name of the enum
		 * @param inherit Weather this is inherited or not
		 * @param vis Visibility of the enum
		 */
		ReflectedEnum(std::string nName, bool inherit, bool isGlobal,
				VisibilityType vis, int modifiers);

		/**
		 * Cleans up the wrapper class
		 */
		virtual ~ReflectedEnum();

		/**
		 * Gets a reference to the underlying reflected object
		 *
		 * @return Reflected enum
		 */
		virtual	const cpgf::GMetaEnum * getEnum() const;

		/**
		 * Gets the number of values in this enum
		 *
		 * @return The number of values in this enum
		 */
		virtual	size_t getNumValues() const;
//TODO fix these for non-reflected enums
		/**
		 * Gets the name of the value at a position.
		 *
		 * @return The name of a value (by position). If index is invalid,
		 * 	"" is returned.
		 */
		virtual	std::string getName(size_t index) const;

		/**
		 * Gets the value as an int at a position.
		 *
		 * @return The value (by position). If index is invalid,
		 * 	INT_MIN is returned.
		 */
		virtual	int getValue(size_t index) const;


		/**
		 * Gets a list of all of the names in this enum
		 *
		 * @return All names in this enum
		 */
		virtual	std::vector<std::string> getNames() const;

		/**
		 * Gets a list of all of the values in this enum
		 *
		 * @return All value in this enum
		 */
		virtual	std::vector<int> getValues() const;

		/**
		 * Returns the matching value for this name.
		 *
		 * @return The value for the name.  If the name is not found,
		 * INT_MIN is returned.
		 */
		virtual	int getValueForName(std::string name) const;

		/**
		 * Returns the matching anem for this value.
		 *
		 * @return The name for the value.  If the value is not found,
		 * "" is returned.
		 */
		virtual	std::string getNameForValue(int value) const;

		/**
		 * Returns true if the item is being reflected and is accessible
		 *
		 * @return true if reflected
		 */
		virtual bool isAccessible() const;
};




class ReflectedNamespace;
typedef boost::shared_ptr<ReflectedNamespace> ReflectedNamespacePtr;

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
//	std::string fullNamespace;
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
	std::vector<ReflectedNamespacePtr> enclosedNamespaces;

	/**
	 * @brief Divides a namespace string into a list of namespaces from most
	 * general to most specific.
	 *
	 * So cpptesting::inner::test would become:
	 * cpptesting
	 * cpptesting::inner
	 * cpptesting::inner::test
	 *
	 * @param ns namespace to divide
	 * @return vector with the namespaces
	 */
	static std::vector<std::string> splitNamespace(std::string ns);

	/**
	 * Global namespace
	 */
	static ReflectedNamespacePtr global;
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
	 * @param sig Signature of the item to get (signature)
	 *
	 * @return The item or NULL if that name does not exist
	 */
	virtual const ReflectedItem * getEnclosedItem(std::string sig) const;

	/**
	 * @brief Gets a list of all namespaces enclosed in the
	 * namespace
	 *
	 * @return List of the namespaces
	 */
	virtual std::vector<ReflectedNamespacePtr> getEnclosedNamespaces() const;

	/**
	 * @brief Gets a namespace enclosed in the
	 * namespace
	 *
	 * @param name Name of the namespace to get
	 *
	 * @return The namespace or NULL if that name does not exist
	 */
	virtual ReflectedNamespacePtr getEnclosedNamespace(std::string name) const;

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
	 * @brief Adds a namespace to the current namespace
	 *
	 * @param nNamespace Namespace to add
	 */
	virtual void addNamespace(ReflectedNamespacePtr nNamespace);


	/**
	 * Returns a pointer to the global namespace.
	 *
	 * @return the global namespace
	 */
	static ReflectedNamespacePtr getGlobalNamespace() { return global; }


	/**
	 * Creates a namespace (and all parent namespaces) within the global
	 *  namespace.
	 *
	 *  @param nNamespace namespace to add
	 *  @return Pointer to the added namespace
	 */
	static ReflectedNamespacePtr addNamespace(std::string nNamespace);

	/**
	 * Adds the reflected item to the namespace.  The namespaces are created as
	 * needed.
	 *
	 * @param nNamespace namespace for the item
	 * @param item Item to add
	 */
	static void addItem(std::string nNamespace, const ReflectedItem * item);

	/**
	 * Gets the namespace object associated with the name.
	 *
	 * @param nNamespace namespace to get
	 * @return pointer to the namespace or NULL if it does not exist
	 */
	static ReflectedNamespacePtr getNamespace(std::string nNamespace);


	/**
	 * @brief Gets a list of all items (methods, variables, etc.) in the
	 * namespace
	 *
	 * @param nNamespace The name of the namespace
	 * @return List of items in the namespace. If the namespace does not
	 * exist, an empty list is returned
	 */
	static std::vector<const ReflectedItem *> getEnclosedItems(
			std::string nNamespace);


	/**
	 * @brief Gets an item (method, variable, etc.) from the
	 * namespace
	 *
	 * @param nNamespace The name of the namespace
	 * @param name Name of the item to get (signature)
	 *
	 * @return The item or NULL if that name does not exist or if the namespace
	 * does not exist
	 */
	static const ReflectedItem * getEnclosedItem(std::string nNamespace,
			std::string name);

	/**
	 * Gets all namespaces for this code
	 *
	 * @return list of namespaces
	 */
	static std::vector<ReflectedNamespacePtr> getAllNamespaces();


	/**
	 * Gets all number of namespaces in the code (including the global one).
	 *
	 * This always returns at least 1
	 *
	 * @return number of namespaces
	 */
	static size_t getNamespaceCount();
};


}



#endif /* LGREFLECTEDENUM_H_ */
