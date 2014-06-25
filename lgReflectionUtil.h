/*
 * RecflectionUtil.h
 *
 *  Created on: Oct 19, 2013
 *      Author: Administrator
 */

#ifndef RECFLECTIONUTIL_H_
#define RECFLECTIONUTIL_H_

#include <vector>
#include <cpgf/gmetaclass.h>
#include <cpgf/gmetacommon.h>
#include <cpgf/gmetaoperatorop.h>

namespace cpptesting {


/**
 * @class Signature
 * Base class for all item signatures.
 * This stores the name, signature, and modifiers for a signature and provides
 * some common methods.
 *
 * @author scturner
 * @version 6/23/2014
 */
class Signature {
protected:
	/**
	 * Name of the item (variable name, method name, etc.)
	 */
	std::string name;
	/**
	 * Modifiers applied to the item (static, virtual, etc.)
	 * List of modifiers from cpgf/gmetacommon.h that apply here:
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
	 */
	int modifiers;

	/**
	 * signature of the item (int x, double foo(int), etc.)
	 * Kept mostly as a convenience since it is used a lot.
	 */
	std::string signature;

	/**
	 * Creates a string based on the contents of the vector separated by the
	 * delimiter. Simple version of a join function.
	 *
	 * @param v List of strings to join
	 * @param delim String to put between the items in the list
	 * @return The list in string form separated by delim.
	 */
	virtual std::string buildStr(std::vector<std::string> v, std::string delim) const;


	/**
	 * Tries to find the modifier in the signature and removes it if found.
	 *
	 * This won't work for const since const can be part of a parameter
	 * declaration.
	 *
	 * This won't work for pure virtual since = 0 can be part of a (default)
	 * parameter declaration.
	 *
	 * @param modifierToFind Modifier being searched for (static, virtual, etc.)
	 * @param sig Signature to check
	 *
	 * @return true if the modifier was found and removed.
	 */
	virtual bool findModifier(std::string modifierToFind, std::string & sig) const;

	/**
	 * Default constructor
	 */
	Signature() : name(""), modifiers(0), signature("") {}

	//	/**
	//	 * Utility method that corrects the formatting of types so they are in
	//	 * a standard format and can be compared.
	//	 *
	//	 * @param type type signature
	//	 * @return Corrected signature
	//	 *
	//	 * TODO move to a more robust comparison scheme using clang
	//	 */
	//	virtual std::string correctType(std::string type);
	//TODO document
	virtual std::vector<std::string> & correctModifiers(
			std::vector<std::string> &v);
	virtual void swapModifiers(std::vector<std::string> & values,
			int & first, int & second);

	//reorder volatile const mutable
	virtual void reorderModifiers(std::vector<std::string> & values,
			int & volatilePos, int & constPos, int & mutablePos);

public:
	/**
	 * Creates a signature from a know good signature
	 */
	Signature(const std::string & sig) : name(sig), modifiers(0), signature(sig) {}

	/**
	 * Creates a signature from a know good signature
	 */
	Signature(const char * sig) : name(sig), modifiers(0), signature(sig) {}

	/**
	 * Destroys this signature
	 */
	virtual ~Signature() { }

	/**
	 * Gets the item's name (x, foo, etc.)
	 * @return Item's name
	 */
	virtual std::string getName() const { return name; }

	/**
	 * Gets the modifiers for this item (static, virtual, etc.)
	 * @return Modifiers for this item.
	 */
	virtual int getModifiers() const { return modifiers; }

	//TODO allow modifiers to be added/changed
	// use instead of modifiers parameter
	//TODO include some notion of exact match?

	/**
	 * Gets the full signature of the item
	 */
	virtual std::string getSignature() const { return signature; }

	/**
	 * Casts the Signature to a string as needed
	 */
	virtual operator std::string() const { return signature; }
};


/**
 * @class TypeSignature
 * Signature for a single type.
 *
 * The name for this is the same as the signature
 *
 * @author scturner
 * @version 6/23/2014
 */
class TypeSignature : public Signature {
protected:
	/**
		 * Creates a type and formats it for comparison
		 *
		 * @param type Type as a string
		 */
	virtual void createType(const std::string &  type);
public:

	/**
	 * Creates a type and formats it for comparison
	 *
	 * @param type Type as a string
	 */
	TypeSignature(const std::string &  type) { createType(type); }

	/**
	 * Creates a type and formats it for comparison
	 *
	 * @param type Type as a string
	 */
	TypeSignature(const char * type) { createType(type); }

	/**
	 * Gets the string version of the cpgf type
	 *
	 * @param type cpgf reflected type info
	 */
	TypeSignature(const cpgf::GMetaType &type);

	/**
	 * Given a string representation of a type, returns if it is a pointer.
	 *
	 * @return true if it is a pointer
	 */
	virtual bool isPointerType() const;

	/**
	 * Given a string representation of a type, returns if it is a reference.
	 *
	 * @return true if it is a reference
	 */
	virtual bool isReferenceType() const;
};


/**
 * @class FieldSignature
 * Represents a field which its type and name.
 *
 * @author scturner
 * @version 6/23/2014
 */
class FieldSignature : public Signature {
protected:
	/**
	 * Type of this field
	 */
	TypeSignature type;

	/**
	 * Determines, based on the signatures, the modifiers for this function/
	 * method.
	 *
	 * sig is returned without those modifiers
	 *
	 * Attempts to find the following modifiers
	 * 	const
	 * 	static
	 * 	extern
	 * 	volatile
	 * 	mutable
	 * 	[template] - not supported
	 *
	 * @param sig Signature of the function/method
	 * @return modifiers for this function
	 */
	int removeModifiers(std::string & sig);

	/**
	 * Divides a field/variable into type, name
	 *
	 * @param sig Signature to divide
	 *
	 */
	virtual void divideSignature(const std::string &  sig);


	/**
	 * Creates a field signature from field declaration. The type is normalized
	 * (somewhat) so that it is consistent with other signatures.
	 *
	 * @param field Field declaration (i.e. const int x)
	 */
	virtual void createField(const std::string & field);

	/**
		 * Creates a field signature from a type and name. The type is normalized
		 * (somewhat) so that it is consistent with other signatures.
		 *
		 * @param fieldType Type of the field (i.e. const int)
		 * @param nName Name of the field
		 * @param mods Modifiers for this field
		 */
	virtual void createField(const std::string &  fieldType,
			const std::string &  nName, int mods);
public:
	/**
	 * Gets the type of this field
	 */
	virtual TypeSignature getType() const { return type; }

	/**
	 * Creates a field signature from reflected data
	 *
	 * @param field Field to get signature of
	 */
	FieldSignature(const cpgf::GMetaField *field);

	/**
	 * Creates a field signature from a type and name. The type is normalized
	 * (somewhat) so that it is consistent with other signatures.
	 *
	 * @param fieldType Type of the field (i.e. const int)
	 * @param nName Name of the field
	 * @param mods Modifiers for this field
	 */
	FieldSignature(const std::string &  fieldType, const std::string &  nName,
			int mods) : type("") {
		createField(fieldType, nName, mods);
	}

	/**
	 * Creates a field signature from a type and name. The type is normalized
	 * (somewhat) so that it is consistent with other signatures.
	 *
	 * @param fieldType Type of the field (i.e. const int)
	 * @param nName Name of the field
	 * @param mods Modifiers for this field
	 */
	FieldSignature(const char * fieldType, const char * nName, int mods)
		: type("") {
		createField(fieldType, nName, mods);
	}

	/**
	 * Creates a field signature from field declaration. The type is normalized
	 * (somewhat) so that it is consistent with other signatures.
	 *
	 * @param field Field declaration (i.e. const int x)
	 */
	FieldSignature(const std::string & field) : type("") {
		createField(field);
	};

	/**
	 * Creates a field signature from field declaration. The type is normalized
	 * (somewhat) so that it is consistent with other signatures.
	 *
	 * @param field Field declaration (i.e. const int x)
	 * @param name Name of the field
	 */
	FieldSignature(const char * field) : type("") { createField(field); };

	/**
	 * Destroys this FieldSignature
	 */
	virtual ~FieldSignature() { }
};


/**
 * @class MethodSignature
 * Represents a method/function with its name, return type, and parameters
 *
 * @author scturner
 * @version 6/23/2014
 */
class MethodSignature : public Signature {
protected:
	/**
	 * Return type of the method as a String
	 */
	TypeSignature returnType;

	/**
	 * Parameters (type) of the method as strings
	 */
	std::vector<TypeSignature> params;

	//TODO document

	virtual void findParams(const std::string &  signature, int &openParen,
			int &closeParen);
	virtual std::vector<TypeSignature> getParams(const std::string &  signature,
			int openParen, int closeParen);

	/**
	 * Creates a string based on the contents of the vector separated by the
	 * delimiter. Simple version of a join function.
	 *
	 * @param v List of strings to join
	 * @param delim String to put between the items in the list
	 * @return The list in string form separated by delim.
	 */
	virtual std::string buildStr(std::vector<TypeSignature> v, const std::string &  delim);



	/**
	 * Determines, based on the signatures, the modifiers for this function/
	 * method.
	 *
	 */
	virtual int removeModifiers(std::string & sig);

	/**
	 * Divides a function/method into return type, name, parameters.
	 *
	 * @param sig Signature to divide
	 *
	 */
	virtual void divideSignature(const std::string &  sig);

	/**
	 * Creates a constructor signature from reflected data
	 *
	 * @param nName Name of class that contains the constructor
	 * @param construct Constructor to get signature of
	 */
	virtual void createConstructor(const std::string &  nName,
			const cpgf::GMetaConstructor * construct);

	/**
	 * Creates a constructor signature from a name
	 * and list of parameter types. The type is normalized
	 * (somewhat) so that it is consistent with other signatures.
	 *
	 * @param nName Name of the function/method
	 * @param paramStrs List of parameter types
	 * @param mods Modifiers for this method
	 */
	virtual void createConstructor(const std::string &  nName,
			const std::vector<std::string> & paramStrs, int mods);


	/**
	 * Creates functions/methods/operators/constructors so they are in
	 * a standard format and can be compared.
	 *
	 *	Unlike with the fields, the const modifier is included in the signature
	 *	since
	 *		int foo()
	 *		int foo() const
	 *	are both valid, separate declarations and can coexist.
	 *
	 * @param sig functions/methods/operators/constructors signature
	 *
	 * TODO move to a more robust comparison scheme using clang
	 */
	virtual void createMethod(const std::string &  sig);

	/**
	 * Creates a function/method signature from a return type and name
	 * and list of parameter types. The type is normalized
	 * (somewhat) so that it is consistent with other signatures.
	 *
	 * @param rType Return type of the function/method (i.e. const int)
	 * @param nName Name of the function/method
	 * @param paramStrs List of parameter types
	 * @param mods Modifiers for this method
	 */
	virtual void createMethod(const std::string &  rType, const std::string &  nName,
			const std::vector<std::string> & paramStrs, int mods);

public:
	/**
	 * Gets the return type
	 * @return Return type as String
	 */
	virtual TypeSignature getReturnType() const { return returnType; }

	/**
	 * Gets all of the parameters as strings
	 * @return list of parameters
	 */
	virtual std::vector<TypeSignature> getParameters() const { return params; }

	//TODO add methods to add/remove/get/count the parameters?


	//methods
	/**
	 * Creates a function/method signature from reflected data
	 *
	 * @param method Function/method to get signature of
	 */
	MethodSignature(const cpgf::GMetaMethod *method);


	/**
	 * Creates a function/method signature from a return type and name
	 * and list of parameter types. The type is normalized
	 * (somewhat) so that it is consistent with other signatures.
	 *
	 * @param rType Return type of the function/method (i.e. const int)
	 * @param nName Name of the function/method
	 * @param paramStrs List of parameter types
	 * @param mods Modifiers for this method
	 */
	MethodSignature(const std::string &  rType, const std::string &  nName,
			const std::vector<std::string> & paramStrs, int mods) : returnType("") {
		createMethod(rType, nName, paramStrs, mods);
	}

	/**
	 * Creates a function/method signature from a return type and name
	 * and list of parameter types. The type is normalized
	 * (somewhat) so that it is consistent with other signatures.
	 *
	 * @param rType Return type of the function/method (i.e. const int)
	 * @param nName Name of the function/method
	 * @param paramStrs List of parameter types
	 * @param mods Modifiers for this method
	 */
	MethodSignature(const char * rType, const char * nName,
			const std::vector<std::string> & paramStrs, int mods) : returnType("") {
		createMethod(rType, nName, paramStrs, mods);
	}

	//constructors
	/**
	 * Creates a constructor signature from reflected data
	 *
	 * @param mClass Class that contains the constructor
	 * @param construct Constructor to get signature of
	 */
	MethodSignature(const cpgf::GMetaClass *mClass,
			const cpgf::GMetaConstructor *construct);

	/**
	 * Creates a constructor signature from reflected data
	 *
	 * @param nName Name of class that contains the constructor
	 * @param construct Constructor to get signature of
	 */
	MethodSignature(const std::string &  nName,
			const cpgf::GMetaConstructor * construct);

	/**
	 * Creates a constructor signature from reflected data
	 *
	 * @param nName Name of class that contains the constructor
	 * @param construct Constructor to get signature of
	 */
	MethodSignature(const char * nName,
			const cpgf::GMetaConstructor * construct) : returnType("") {
		createConstructor(nName, construct);
	}

	/**
	 * Creates a constructor signature from a name
	 * and list of parameter types. The type is normalized
	 * (somewhat) so that it is consistent with other signatures.
	 *
	 * @param nName Name of the function/method
	 * @param paramStrs List of parameter types
	 * @param mods Modifiers for this method
	 */
	MethodSignature(const std::string &  nName,
			const std::vector<std::string> & paramStrs, int mods) : returnType("") {
		createConstructor(nName, paramStrs, mods);
	}

	/**
	 * Creates a constructor signature from a name
	 * and list of parameter types. The type is normalized
	 * (somewhat) so that it is consistent with other signatures.
	 *
	 * @param nName Name of the function/method
	 * @param paramStrs List of parameter types
	 * @param mods Modifiers for this method
	 */
	MethodSignature(const char * nName,
			const std::vector<std::string> & paramStrs, int mods) : returnType("") {
		createConstructor(nName, paramStrs, mods);
	}

	/**
	 * Creates functions/methods/operators/constructors so they are in
	 * a standard format and can be compared.
	 *
	 *	Unlike with the fields, the const modifier is included in the signature
	 *	since
	 *		int foo()
	 *		int foo() const
	 *	are both valid, separate declarations and can coexist.
	 *
	 * @param sig functions/methods/operators/constructors signature
	 *
	 * TODO move to a more robust comparison scheme using clang
	 */
	MethodSignature(const std::string &  sig) : returnType("") {
		createMethod(sig);
	}

	/**
	 * Creates functions/methods/operators/constructors so they are in
	 * a standard format and can be compared.
	 *
	 *	Unlike with the fields, the const modifier is included in the signature
	 *	since
	 *		int foo()
	 *		int foo() const
	 *	are both valid, separate declarations and can coexist.
	 *
	 * @param sig functions/methods/operators/constructors signature
	 *
	 * TODO move to a more robust comparison scheme using clang
	 */
	MethodSignature(const char * sig) : returnType("") {
		createMethod(sig);
	}


	/**
	 * Destroys this MethodSignature
	 */
	virtual ~MethodSignature() { }

	/**
	 * Determines if the operator is one of the postfix operators
	 * (x++ or x--)
	 *
	 * @return true if it is the signature of an postfix operator
	 */
	virtual bool isPostFixOperator() const;

	/**
	 * Determines if this signature matches the pattern of names for auto-
	 * generated array setters (see createArraySetterSignature). Signatures
	 * should have the form:
	 * 	 opErAToRWrapper_<class name>_<return type>_array_<params>
	 * 	 where spaces are replaced with _, & is replaced with ref, and
	 * 	 is replaced with ptr.
	 * This method checks the signature against the regex
	 * 	 .*\bopErAToRWrapper_.*_array_.*
	 * to see if it is valid.
	 *
	 * @return true if it is the signature of an array setter method
	 */
	virtual bool isArraySetterSignature() const;

	/**
	 * Determines if this signature matches the operator[] method and that it
	 * does not return a const result.
	 *
	 * @return true if it is the signature of an array setter method
	 */
	virtual bool isSettableArrayOperator() const;


	/**
	 * Determines if this signature matches the pattern of names for auto-
	 * generated copy function (see createCopyFunctionSignature). Signatures
	 * should have the form:
	 * 	 cOpYwRaPpEr_<class name>
	 *
	 * This method checks the signature against the regex
	 * 	 .*\bopErAToRWrapper_.*
	 * to see if it is valid.
	 *
	 * @return true if it is the signature of an array setter method
	 */
	virtual bool isCopyFunctionSignature() const;


	/**
	 * Determines if the name of this method refers to a destructor
	 *
	 * @return true if it is a destructor
	 */
	virtual bool isDestructor() const;



	/**
	 * Utility method that creates a method signature for the auxiliary function
	 * used to simulate setting a value with the [] operator
	 * (i.e. x[2] = 4)
	 *
	 * The result returned has the form:
	 * 	opErAToRWrapper_<class name>_<return type>_array_<params>
	 * 	where spaces are replaced with _, & is replaced with ref, and
	 * 	* is replaced with ptr
	 *
	 * @param className Name of the class
	 * @return signature for the array setter function or "" if this method
	 * is an [] operator signature
	 */
	MethodSignature createArraySetterSignature(const std::string &  className) const;
};



/**
 * @class MethodSignature
 * Represents a class
 *
 * @author scturner
 * @version 6/23/2014
 */
class ClassSignature : public Signature {
protected:
	/**
	 * Name of the enclosing class for this class
	 */
	std::string enclosingClass;

	/**
	 * Determines, based on the signatures, the modifiers for this function/
	 * method.
	 *
	 * sig is returned without those modifiers
	 *
	 * Attempts to find the following modifiers
	 * 	virutal (for base classes)
	 * 	[template] - not supported
	 *
	 * @param sig Signature of the function/method
	 * @return modifiers for this function
	 */
	int removeModifiers(std::string sig);

	/**
	 * Divides a class into name, enclosing class, and modifiers.
	 *
	 * @param sig Signature of the class
	 */
	void divideSignature(const std::string &  sig);

	/**
	 * Corrects the formatting of
	 * (inner) classes so they are in
	 * a standard format and can be compared.
	 *
	 * @param sig class signature
	 *
	 * TODO move to a more robust comparison scheme using clang
	 */
	virtual void createClass (const std::string & sig);

public:
	/**
	 * Gets the name of the enclosingClass
	 */
	std::string getEnclosingClass() const { return enclosingClass; }

	/**
	 * Corrects the formatting of
	 * (inner) classes so they are in
	 * a standard format and can be compared.
	 *
	 * @param sig class signature
	 *
	 * TODO move to a more robust comparison scheme using clang
	 */
	ClassSignature(const std::string & sig) { createClass(sig); }

	/**
	 * Corrects the formatting of
	 * (inner) classes so they are in
	 * a standard format and can be compared.
	 *
	 * @param sig class signature
	 *
	 * TODO move to a more robust comparison scheme using clang
	 */
	ClassSignature(const char * sig) { createClass(sig); }

	/**
	 * Corrects the formatting of
	 * (inner) classes so they are in
	 * a standard format and can be compared.
	 *
	 * @param parent Parent of the reflectedClass
	 * @param cClass reflected class
	 *
	 */
	ClassSignature(const cpgf::GMetaClass * cClass);

	/**
	 * Utility method that creates a method signature for the auxiliary function
	 * used to copy objects of reflected classes when they are returned from
	 * methods/functions. This avoids the problem where they go out of scope
	 * before they are used.
	 *
	 * The result returned has the form:
	 * 	cOpYwRaPpEr_<class name>
	 *
	 * @return signature for the copy function
	 */
	MethodSignature createCopyFunctionSignature() const;
};


/**
 * @class EnumSignature
 * Represents an enum with its name and values
 *
 * @author scturner
 * @version 6/23/2014
 */
class EnumSignature : public Signature {
protected:
	/**
	 * Mapping of each enum name to its value
	 */
	std::map<std::string, int> names;

	/**
	 * Mapping of each value to its name
	 */
	std::map<int, std::string> values;


	/**
	 * Divides an enum into a name and a list of enums and their values
	 *
	 * @param sig Signature of the class
	 */
	void divideSignature(const std::string &  sig);

	/**
	 * Corrects the formatting of enums so they are in
	 * a standard format and can be compared.
	 *
	 * @param enumStr enum signature
	 *
	 * TODO move to a more robust comparison scheme using clang
	 */
	virtual void createEnum(const std::string &  enumStr);

public:
	/**
	 * Gets the mapping from enum names to their values
	 * @return Enum names and their values
	 */
	virtual std::map<std::string, int> getEnumNamesToValues() const {
		return names;
	}

	/**
	 * Gets the mapping from enum values to their names
	 * @return Enum values and their names
	 */
	virtual std::map<int, std::string> getEnumValuesToNames() const {
		return values;
	}

	/**
	 * Corrects the formatting of enums so they are in
	 * a standard format and can be compared.
	 *
	 * @param enumStr enum signature
	 *
	 * TODO move to a more robust comparison scheme using clang
	 */
	EnumSignature(const std::string &  enumStr) { createEnum(enumStr); }

	/**
	 * Corrects the formatting of enums so they are in
	 * a standard format and can be compared.
	 *
	 * @param enumStr enum signature
	 *
	 * TODO move to a more robust comparison scheme using clang
	 */
	EnumSignature(const char * enumStr) { createEnum(enumStr); }


	/**
	 * Corrects the formatting of enums so they are in
	 * a standard format and can be compared.
	 *
	 * @param eEnum reflected enum
	 *
	 */
	EnumSignature(const cpgf::GMetaEnum * eEnum);

	/**
	 * Casts the Signature to a string as needed
	 */
	virtual operator std::string() const { return name; }
};




}
#endif /* RECFLECTIONUTIL_H_ */
