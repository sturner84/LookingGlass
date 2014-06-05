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


class ReflectionUtil
{
private:
	static ReflectionUtil instance;

	ReflectionUtil();

//	std::map<std::string, std::string> operatorSymToName;
//	std::map<std::string, std::string> operatorNameToSym;
//	std::map<int, std::string> operatorNumberToSym;
//	std::map<std::string, int> operatorSymToNumber;

//	void loadOperatorNames();
	static std::string getType(const cpgf::GMetaType &type);

	static std::string buildStr(std::vector<std::string> v, std::string delim);
	static std::vector<std::string> & correctModifiers(
			std::vector<std::string> &v);
	static void findParams(std::string signature, int &openParen, int &closeParen);
	static std::vector<std::string> getParams(std::string signature, int openParen,
			int closeParen);

//	static int getNumberFromName(std::string name);

public:

//	/**
//	 * Looks up the c++ operator name based on an operators reflected function
//	 * (i.e. _opAdd, _opLeftShift, etc.)
//	 *
//	 * prefix ++ and -- will return ++operator and --operator to
//	 * 	differential them from the postfix operators
//	 *
//	 * The operator[] is divided into two operations: get and set. The function
//	 * _opArrayGet will return operator[]Get and _opArraySet will return
//	 * operator[]Set.
//	 *
//	 * @param opSymbol the name of the reflected function
//	 *
//	 * @return Name of the operator  (i.e. operator+, operator<<, etc.)
//	 */
//	static std::string getOperatorSymbol(std::string opName);
//
//	/**
//	 * Looks up an operator's reflected function based on the c++ operator name
//	 * (i.e. operator+, operator<<, etc.)
//	 *
//	 * prefix ++ and -- are designated as ++operator and --operator to
//	 * 	differential them from the postfix operators
//	 *
//	 * The operator[] is divided into two operations: get and set. To access
//	 * them you need operator[]Get and operator[]Set respectively.
//	 *
//	 * @param opSymbol Symbol of the operator  (i.e. operator+, operator<<, etc.)
//	 *
//	 * @return the name of the reflected function
//	 */
//	static std::string getOperatorName(std::string opSymbol);
//
//	/**
//	 * Looks up the c++ operator name based on an operators reflected number
//	 * (defined in gmetaoperatorop.h)
//	 *
//	 * prefix ++ and -- will return ++operator and --operator to
//	 * 	differential them from the postfix operators
//	 *
//
//	 * @param operatorNum the number of the reflected operator
//	 *
//	 * @return Name of the operator  (i.e. operator+, operator<<, etc.)
//	 */
//	static std::string getOperatorSymbol(int operatorNum);
//
//	/**
//	 * Looks up an operator's reflected number based on the c++ operator name
//	 * (i.e. operator+, operator<<, etc.)
//	 *
//	 * prefix ++ and -- are designated as ++operator and --operator to
//	 * 	differential them from the postfix operators
//	 *
//	 * @param opSymbol Symbol of the operator  (i.e. operator+, operator<<, etc.)
//	 *
//	 * @return the number of the reflected operator
//	 */
//	static int getOperatorNumber(std::string opSymbol);

	//fields
	/**
	 * Creates a field signature from reflected data
	 *
	 * @param field Field to get signature of
	 * @return Signature of the field (i.e. int x)
	 */
	static std::string createFieldSignature(const cpgf::GMetaField *field);


	/**
	 * Creates a field signature from a type and name. The type is normalized
	 * (somewhat) so that it is consistent with other signatures.
	 *
	 * @param fieldType Type of the field (i.e. const int)
	 * @param name Name of the field
	 * @return Signature of the field (i.e. int x)
	 */
	static std::string createFieldSignature(const std::string & fieldType,
			const std::string & name);

	//methods
	/**
	 * Creates a function/method signature from reflected data
	 *
	 * @param method Function/method to get signature of
	 * @return Signature of the function/method (i.e. int foo(int, double)
	 */
	static std::string createFunctionSignature(const cpgf::GMetaMethod *method);
	//	static std::string createFunctionSignature(std::string returnType, std::string name,
	//			int paramCount, std::string params...);
	//methods
	/**
	 * Creates a function/method signature from a return type and name
	 * and list of parameter types. The type is normalized
	 * (somewhat) so that it is consistent with other signatures.
	 *
	 * @param returnType Return type of the function/method (i.e. const int)
	 * @param name Name of the function/method
	 * @param params List of parameter types
	 * @return Signature of the function/method (i.e. int foo(int, double)
	 */
	static std::string createFunctionSignature(std::string returnType, std::string name,
			const std::vector<std::string> & params, bool isConst = false);

	//constructors
	/**
	 * Creates a constructor signature from reflected data
	 *
	 * @param mClass Class that contains the constructor
	 * @param construct Constructor to get signature of
	 * @return Signature of the constructor (i.e. Foo(int, double)
	 */
	static std::string createConstructorSignature(const cpgf::GMetaClass *mClass,
			const cpgf::GMetaConstructor *construct);
	//	static std::string createConstructorSignature(std::string name,
	//			int paramCount, std::string params...);
	/**
	 * Creates a constructor signature from a name
	 * and list of parameter types. The type is normalized
	 * (somewhat) so that it is consistent with other signatures.
	 *
	 * @param name Name of the function/method
	 * @param params List of parameter types
	 * @return Signature of the constructor (i.e. Foo(int, double)
	 */
	static std::string createConstructorSignature(std::string name,
			const std::vector<std::string> & params);

//	//operators
//	/**
//	 * Creates a operator signature from reflected data
//	 *
//	 * @param op operator to get signature of
//	 * @return Signature of the operator (i.e. int operator+(int)
//	 */
//	static std::string createOperatorSignature(const cpgf::GMetaOperator *op);
//	//	static std::string createOperatorSignature(std::string returnType, std::string name,
//	//			int paramCount, std::string params...);
//	/**
//	 * Creates a operator signature from a return type and name
//	 * and list of parameter types. The type is normalized
//	 * (somewhat) so that it is consistent with other signatures.
//	 *
//	 * @param returnType Return type of the operator (i.e. const int)
//	 * @param name Name of the operator
//	 * @param params List of parameter types
//	 * @return Signature of the function/method (i.e. int foo(int, double)
//	 */
//	static std::string createOperatorSignature(std::string returnType, std::string name,
//			const std::vector<std::string> & params);


//TODO change [] to *?
	/**
	 * Utility method that corrects the formatting of fields so they are in
	 * a standard format and can be compared.
	 *
	 * @param field field signature
	 * @return Corrected signature
	 *
	 * TODO move to a more robust comparison scheme using clang
	 */
	static std::string correctField(std::string field);

	/**
	 * Utility method that corrects the formatting of types so they are in
	 * a standard format and can be compared.
	 *
	 * @param type type signature
	 * @return Corrected signature
	 *
	 * TODO move to a more robust comparison scheme using clang
	 */
	static std::string correctType(std::string type);

	/**
	 * Utility method that corrects the formatting of enums so they are in
	 * a standard format and can be compared.
	 *
	 * @param enumStr enum signature
	 * @return Corrected signature
	 *
	 * TODO move to a more robust comparison scheme using clang
	 */
	static std::string correctEnum(std::string enumStr);

	/**
	 * Utility method that corrects the formatting of
	 * functions/methods/operators/constructors so they are in
	 * a standard format and can be compared.
	 *
	 * @param signature functions/methods/operators/constructors signature
	 * @return Corrected signature
	 *
	 * TODO move to a more robust comparison scheme using clang
	 */
	static std::string correctSignature(std::string signature);



	/**
	 * Determines if the operator is one of the postfix operators
	 * (x++ or x--)
	 *
	 * @param sig signature to check
	 * @return true if it is the signature of an postfix operator
	 */
	static bool isPostFixOperator(std::string sig);

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
	 * @param sig signature to check
	 * @return true if it is the signature of an array setter method
	 */
	static bool isArraySetterSignature(std::string sig);

	/**
	 * Determines if this signature matches the operator[] method and that it
	 * does not return a const result.
	 *
	 * @param sig signature to check
	 * @return true if it is the signature of an array setter method
	 */
	static bool isSettableArrayOperator(std::string sig);

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
	 * @param arrayOpSignature signature of the operator being called
	 * 	(i.e. string & operator[](int) )
	 * @return signature for the array setter function
	 */
	static std::string createArraySetterSignature(std::string className,
			std::string arrayOpSignature);


	/**
	 * Takes a function/method signature and returns the return type
	 *
	 * @param signature signature of the function/method
	 * @return the return type of the function/method as a string
	 */
	static std::string getReturnType(std::string signature);

	/**
	 * Given a string representation of a type, returns if it is a pointer.
	 *
	 * @param type Type signature
	 * @return true if it is a pointer
	 */
	static bool isPointerType(std::string type);

	/**
	 * Given a string representation of a type, returns if it is a reference.
	 *
	 * @param type Type signature
	 * @return true if it is a reference
	 */
	static bool isReferenceType(std::string type);


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
	 * @param sig signature to check
	 * @return true if it is the signature of an array setter method
	 */
	static bool isCopyFunctionSignature(std::string sig);


	/**
	 * Utility method that creates a method signature for the auxiliary function
	 * used to copy objects of reflected classes when they are returned from
	 * methods/functions. This avoids the problem where they go out of scope
	 * before they are used.
	 *
	 * The result returned has the form:
	 * 	cOpYwRaPpEr_<class name>
	 *
	 * @param className Name of the class
	 * @return signature for the copy function
	 */
	static std::string createCopyFunctionSignature(std::string className);


};

}
#endif /* RECFLECTIONUTIL_H_ */
