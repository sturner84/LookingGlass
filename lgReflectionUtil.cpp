/*
 * ReflecteionUtil.cpp
 *
 *  Created on: Oct 19, 2013
 *      Author: Administrator
 */


#include "lgReflectionUtil.h"
#include "lgTestingSupport.h"
#include <boost/regex.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

namespace cpptesting {


const std::string CLEAN_SPACES = "(\\s*\\[\\s*)|(\\s*\\])|(\\s*<\\s*)|(\\s*>\\s*)|(\\s+)|(^\\s+)|(\\s+$)";
const std::string CLEAN_SPACES_REPLACE = "(?1\\[)(?2\\])(?3 < )(?4 > )(?5 )(?6)(?7)";

const std::string SIGNATURE_CLEAN_REGEX = "(\\s+)|(,\\s*)|(\\s+\\()"
		"|(\\s*\\);?)|(^\\s+)|(\\s+$)|(\\s*\\*\\s*)|(\\s*&\\s*)|([^\\s,\\(]*::)";

const std::string SIGNATURE_CLEAN_REPLACE = "(?1 )(?2, )(?3\\()(?4\\))(?5)"
		"(?6)(?7 \\* )(?8 & )(?9)";
const std::string SIGNATURE_CLEAN_ENUM_REGEX = SIGNATURE_CLEAN_REGEX
		+ "|(\\s*\\{\\s*)|(\\s*\\}\\s*)";
const std::string SIGNATURE_CLEAN_ENUM_REPLACE = SIGNATURE_CLEAN_REPLACE
		+ "(?10\\{)(?11\\})";

//use these instead
const std::string TYPE_CLEAN_REGEX_NO_ARRAY = "(\\s*\\[\\s*)|(\\s*\\])"
		"|(\\s*<\\s*)|(\\s*>\\s*)|(\\s+)|(\\()|(\\))|(^\\s+)|(\\s+$)"
		"|([^\\s,\\(]*::)|(\\s*\\*\\s*)|(\\s*&\\s*)";
const std::string TYPE_CLEAN_REPLACE_NO_ARRAY = "(?1\\[)(?2\\])(?3 < )(?4 > )(?5 )(?6 )"
		"(?7 )(?8)(?9)(?10)(?11 \\* )(?12 & )";

const std::string TYPE_CLEAN_REGEX = "(\\s*<\\s*)|(\\s*>\\s*)|(\\s+)|(\\()|(\\))|(^\\s+)|(\\s+$)"
		"|([^\\s,\\(]*::)|(\\s*\\*\\s*)|(\\s*&\\s*)|(\\[\\s*\\])";
const std::string TYPE_CLEAN_REPLACE = "(?1 < )(?2 > )(?3 )(?4 )(?5 )(?6)(?7)(?8)(?9 \\* )"
		"(?10 & )(?11 * )";

const std::string ARRAY_MATCH_REGEX = "\\[.*\\]";

const std::string FAKE_OPERATOR_NAME = "FAKE@OPERATOR@NAME";
//3 options operator(), operator<symbols>, operator type
const std::string OPERATOR_MATCH_REGEX = "(?:$|\\*|\\s)(operator"
		"(?:(?:\\s*\\(\\s*\\)\\s*)|(?:[^a-zA-Z_1-9]+)|(?:\\s+[^)]+)))(?:\\()";
const std::string OPERATOR_MATCH_REPLACE = "(?1 " + FAKE_OPERATOR_NAME + "\\()";

const std::string CLEAN_OPNAME = "(&)|(\\*)|(\\()|(,)|(operator)|(\\[)|(\\])|(\\))";
const std::string CLEAN_OPNAME_REPLACE = "(?1_ref_)(?2_ptr_)(?3)(?4)(?5_array_)(?6)(?7)(?8)";


const std::string POSTFIX_OPERATOR_PATTERN = ".*\\boperator\\s*((\\+\\+)|(--))\\s*\\(\\s*\\S+.*\\).*";
const std::string ARRAY_WRAPPER_PREFIX = "opErAToRWrapper_";
const std::string ARRAY_WRAPPER_PATTERN = ".*\\bopErAToRWrapper_.*_array_.*";
const std::string ARRAY_OPERATOR_PATTERN = ".*\\boperator\\s*\\[\\s*\\].*";
const std::string ARRAY_OPERATOR_CONST_PATTERN =
		".*[*&\\s]const[\\s&]*\\boperator\\s*\\[\\s*\\].*";

const std::string ARRAY_OPERATOR_REGEX = "(operator\\s*\\[\\s*\\])";

const std::string COPY_WRAPPER_PREFIX = "cOpYwRaPpEr_";
const std::string COPY_WRAPPER_PATTERN = ".*\\bcOpYwRaPpEr_.*";


const std::string CLEAN_FIELD_TYPE = "(volatile)|(mutable)";
const std::string CLEAN_FIELD_TYPE_REPLACE = "(?1)(?2)";
//const std::string CLEAN_FIELD_TYPE = "((volatile)|(mutable)|(const(?:\\s[^*]*))";
//const std::string CLEAN_FIELD_TYPE_REPLACE = "(?1)(?2)(?3)";


ReflectionUtil ReflectionUtil::instance;

ReflectionUtil::ReflectionUtil()
{
//	loadOperatorNames();
}


//void ReflectionUtil::loadOperatorNames()
//{
//	operatorNumberToSym[cpgf::mopAdd] = "operator+";
//	operatorNumberToSym[cpgf::mopSub] = "operator-";
//	operatorNumberToSym[cpgf::mopMul] = "operator*";
//	operatorNumberToSym[cpgf::mopDiv] = "operator/";
//	operatorNumberToSym[cpgf::mopMod] = "operator%";
//	operatorNumberToSym[cpgf::mopPlus] = "operator+";
//	operatorNumberToSym[cpgf::mopNeg] = "operator-";
//	operatorNumberToSym[cpgf::mopAddAssign] = "operator+=";
//	operatorNumberToSym[cpgf::mopSubAssign] = "operator-=";
//	operatorNumberToSym[cpgf::mopMulAssign] = "operator*=";
//	operatorNumberToSym[cpgf::mopDivAssign] = "operator/=";
//	operatorNumberToSym[cpgf::mopModAssign] = "operator%=";
//	operatorNumberToSym[cpgf::mopAssign] = "operator=";
//	operatorNumberToSym[cpgf::mopEqual] = "operator==";
//	operatorNumberToSym[cpgf::mopNotEqual] = "operator!=";
//	operatorNumberToSym[cpgf::mopGreater] = "operator>";
//	operatorNumberToSym[cpgf::mopLessor] = "operator<";
//	operatorNumberToSym[cpgf::mopGreaterEqual] = "operator>=";
//	operatorNumberToSym[cpgf::mopLessorEqual] = "operator<=";
//	operatorNumberToSym[cpgf::mopLogicAnd] = "operator&&";
//	operatorNumberToSym[cpgf::mopLogicOr] = "operator||";
//	operatorNumberToSym[cpgf::mopLogicNot] = "operator!";
//	operatorNumberToSym[cpgf::mopBitAnd] = "operator&";
//	operatorNumberToSym[cpgf::mopBitOr] = "operator|";
//	operatorNumberToSym[cpgf::mopBitXor] = "operator^";
//	operatorNumberToSym[cpgf::mopBitLeftShift] = "operator<<";
//	operatorNumberToSym[cpgf::mopBitRightShift] = "operator>>";
//	operatorNumberToSym[cpgf::mopBitNot] = "operator~";
//	operatorNumberToSym[cpgf::mopBitAndAssign] = "operator&=";
//	operatorNumberToSym[cpgf::mopBitOrAssign] = "operator|=";
//	operatorNumberToSym[cpgf::mopBitXorAssign] = "operator^=";
//	operatorNumberToSym[cpgf::mopBitLeftShiftAssign] = "operator<<=";
//	operatorNumberToSym[cpgf::mopBitRightShiftAssign] = "operator>>=";
//	operatorNumberToSym[cpgf::mopPreInc] = "++operator";
//	operatorNumberToSym[cpgf::mopPreDec] = "--operator";
//	operatorNumberToSym[cpgf::mopPostInc] = "operator++";
//	operatorNumberToSym[cpgf::mopPostDec] = "operator--";
//	operatorNumberToSym[cpgf::mopComma] = "operator,";
//	operatorNumberToSym[cpgf::mopSubscript] = "operator[]";
//	operatorNumberToSym[cpgf::mopFunctor] = "operator()";
//	operatorNumberToSym[cpgf::mopAddress] = "operator&";
//	operatorNumberToSym[cpgf::mopDerefer] = "operator*";
//	operatorNumberToSym[cpgf::mopPointerMember] = "operator->*";
//	operatorNumberToSym[cpgf::mopCast] = "operator T()";
//	operatorNumberToSym[cpgf::mopMember] = "operator->";
//
//
//	operatorSymToNumber["operator+"] = cpgf::mopAdd;
//	operatorSymToNumber["operator-"] = cpgf::mopSub;
//	operatorSymToNumber["operator*"] = cpgf::mopMul;
//	operatorSymToNumber["operator/"] = cpgf::mopDiv;
//	operatorSymToNumber["operator%"] = cpgf::mopMod;
//	operatorSymToNumber["operator+"] = cpgf::mopPlus;
//	operatorSymToNumber["operator-"] = cpgf::mopNeg;
//	operatorSymToNumber["operator+="] = cpgf::mopAddAssign;
//	operatorSymToNumber["operator-="] = cpgf::mopSubAssign;
//	operatorSymToNumber["operator*="] = cpgf::mopMulAssign;
//	operatorSymToNumber["operator/="] = cpgf::mopDivAssign;
//	operatorSymToNumber["operator%="] = cpgf::mopModAssign;
//	operatorSymToNumber["operator="] = cpgf::mopAssign;
//	operatorSymToNumber["operator=="] = cpgf::mopEqual;
//	operatorSymToNumber["operator!="] = cpgf::mopNotEqual;
//	operatorSymToNumber["operator>"] = cpgf::mopGreater;
//	operatorSymToNumber["operator<"] = cpgf::mopLessor;
//	operatorSymToNumber["operator>="] = cpgf::mopGreaterEqual;
//	operatorSymToNumber["operator<="] = cpgf::mopLessorEqual;
//	operatorSymToNumber["operator&&"] = cpgf::mopLogicAnd;
//	operatorSymToNumber["operator||"] = cpgf::mopLogicOr;
//	operatorSymToNumber["operator!"] = cpgf::mopLogicNot;
//	operatorSymToNumber["operator&"] = cpgf::mopBitAnd;
//	operatorSymToNumber["operator|"] = cpgf::mopBitOr;
//	operatorSymToNumber["operator^"] = cpgf::mopBitXor;
//	operatorSymToNumber["operator<<"] = cpgf::mopBitLeftShift;
//	operatorSymToNumber["operator>>"] = cpgf::mopBitRightShift;
//	operatorSymToNumber["operator~"] = cpgf::mopBitNot;
//	operatorSymToNumber["operator&="] = cpgf::mopBitAndAssign;
//	operatorSymToNumber["operator|="] = cpgf::mopBitOrAssign;
//	operatorSymToNumber["operator^="] = cpgf::mopBitXorAssign;
//	operatorSymToNumber["operator<<="] = cpgf::mopBitLeftShiftAssign;
//	operatorSymToNumber["operator>>="] = cpgf::mopBitRightShiftAssign;
//	operatorSymToNumber["++operator"] = cpgf::mopPreInc;
//	operatorSymToNumber["--operator"] = cpgf::mopPreDec;
//	operatorSymToNumber["operator++"] = cpgf::mopPostInc;
//	operatorSymToNumber["operator--"] = cpgf::mopPostDec;
//	operatorSymToNumber["operator,"] = cpgf::mopComma;
//	operatorSymToNumber["operator[]"] = cpgf::mopSubscript;
//	operatorSymToNumber["operator()"] = cpgf::mopFunctor;
//	operatorSymToNumber["operator&"] = cpgf::mopAddress;
//	operatorSymToNumber["operator*"] = cpgf::mopDerefer;
//	operatorSymToNumber["operator->*"] = cpgf::mopPointerMember;
//	operatorSymToNumber["operator T()"] = cpgf::mopCast;
//	operatorSymToNumber["operator->"] = cpgf::mopMember;
//
//	operatorNameToSym["_opAdd"] = "operator+";
//	operatorNameToSym["_opSub"] = "operator-";
//	operatorNameToSym["_opMul"] = "operator*";
//	operatorNameToSym["_opDiv"] = "operator/";
//	operatorNameToSym["_opMod"] = "operator%";
//	operatorNameToSym["_opPlus"] = "operator+";
//	operatorNameToSym["_opMinus"] = "operator-";
//	operatorNameToSym["_opAddAssign"] = "operator+=";
//	operatorNameToSym["_opSubAssign"] = "operator-=";
//	operatorNameToSym["_opMulAssign"] = "operator*=";
//	operatorNameToSym["_opDivAssign"] = "operator/=";
//	operatorNameToSym["_opModAssign"] = "operator%=";
//	operatorNameToSym["_opAssign"] = "operator=";
//	operatorNameToSym["_opEqual"] = "operator==";
//	operatorNameToSym["_opNotEqual"] = "operator!=";
//	operatorNameToSym["_opGreater"] = "operator>";
//	operatorNameToSym["_opLess"] = "operator<";
//	operatorNameToSym["_opGreaterEqual"] = "operator>=";
//	operatorNameToSym["_opLessEqual"] = "operator<=";
//	operatorNameToSym["_opAnd"] = "operator&&";
//	operatorNameToSym["_opOr"] = "operator||";
//	operatorNameToSym["_opNot"] = "operator!";
//	operatorNameToSym["_opBitAnd"] = "operator&";
//	operatorNameToSym["_opBitOr"] = "operator|";
//	operatorNameToSym["_opBitXor"] = "operator^";
//	operatorNameToSym["_opLeftShift"] = "operator<<";
//	operatorNameToSym["_opRightShift"] = "operator>>";
//	operatorNameToSym["_opBitNot"] = "operator~";
//	operatorNameToSym["_opBitAndAssign"] = "operator&=";
//	operatorNameToSym["_opBitOrAssign"] = "operator|=";
//	operatorNameToSym["_opBitXorAssign"] = "operator^=";
//	operatorNameToSym["_opLeftShiftAssign"] = "operator<<=";
//	operatorNameToSym["_opRightShiftAssign"] = "operator>>=";
//	operatorNameToSym["_opInc"] = "++operator";
//	operatorNameToSym["_opDec"] = "--operator";
//	operatorNameToSym["_opIncSuffix"] = "operator++";
//	operatorNameToSym["_opDecSuffix"] = "operator--";
//	operatorNameToSym["_opComma"] = "operator,";
//	operatorNameToSym["_opArrayGet"] = "operator[]Get";
//	operatorNameToSym["_opArraySet"] = "operator[]Set";
//	operatorNameToSym["_opFunction"] = "operator()";
//	operatorNameToSym["_opAddress"] = "operator&";
//	operatorNameToSym["_opDerefer"] = "operator*";
//	operatorNameToSym["_opPointerMember"] = "operator->*";
//	operatorNameToSym["_opCast"] = "operator T()";
//	operatorNameToSym["_opMember"] = "operator->";
//
//	operatorSymToName["operator+"] = "_opAdd";
//	operatorSymToName["operator-"] = "_opSub";
//	operatorSymToName["operator*"] = "_opMul";
//	operatorSymToName["operator/"] = "_opDiv";
//	operatorSymToName["operator%"] = "_opMod";
//	operatorSymToName["operator+"] = "_opPlus";
//	operatorSymToName["operator-"] = "_opMinus";
//	operatorSymToName["operator+="] = "_opAddAssign";
//	operatorSymToName["operator-="] = "_opSubAssign";
//	operatorSymToName["operator*="] = "_opMulAssign";
//	operatorSymToName["operator/="] = "_opDivAssign";
//	operatorSymToName["operator%="] = "_opModAssign";
//	operatorSymToName["operator="] = "_opAssign";
//	operatorSymToName["operator=="] = "_opEqual";
//	operatorSymToName["operator!="] = "_opNotEqual";
//	operatorSymToName["operator>"] = "_opGreater";
//	operatorSymToName["operator<"] = "_opLess";
//	operatorSymToName["operator>="] = "_opGreaterEqual";
//	operatorSymToName["operator<="] = "_opLessEqual";
//	operatorSymToName["operator&&"] = "_opAnd";
//	operatorSymToName["operator||"] = "_opOr";
//	operatorSymToName["operator!"] = "_opNot";
//	operatorSymToName["operator&"] = "_opBitAnd";
//	operatorSymToName["operator|"] = "_opBitOr";
//	operatorSymToName["operator^"] = "_opBitXor";
//	operatorSymToName["operator<<"] = "_opLeftShift";
//	operatorSymToName["operator>>"] = "_opRightShift";
//	operatorSymToName["operator~"] = "_opBitNot";
//	operatorSymToName["operator&="] = "_opBitAndAssign";
//	operatorSymToName["operator|="] = "_opBitOrAssign";
//	operatorSymToName["operator^="] = "_opBitXorAssign";
//	operatorSymToName["operator<<="] = "_opLeftShiftAssign";
//	operatorSymToName["operator>>="] = "_opRightShiftAssign";
//	operatorSymToName["++operator"] = "_opInc";
//	operatorSymToName["--operator"] = "_opDec";
//	operatorSymToName["operator++"] = "_opIncSuffix";
//	operatorSymToName["operator--"] = "_opDecSuffix";
//	operatorSymToName["operator,"] = "_opComma";
//	operatorSymToName["operator[]Get"] = "_opArrayGet";
//	operatorSymToName["operator[]Set"] = "_opArraySet";
//	operatorSymToName["operator()"] = "_opFunction";
//	operatorSymToName["operator&"] = "_opAddress";
//	operatorSymToName["operator*"] = "_opDerefer";
//	operatorSymToName["operator->*"] = "_opPointerMember";
//	operatorSymToName["operator T()"] = "_opCast";
//	operatorSymToName["operator->"] = "_opMember";
//
//
//}
//
//
//
//std::string ReflectionUtil::getOperatorSymbol(std::string opName)
//{
//	return instance.operatorNameToSym[opName];
//}
//
//std::string ReflectionUtil::getOperatorName(std::string opSymbol)
//{
//	return instance.operatorSymToName[opSymbol];
//}
//
//std::string ReflectionUtil::getOperatorSymbol(int operatorNum)
//{
//	return instance.operatorNumberToSym[operatorNum];
//}
//
//int ReflectionUtil::getOperatorNumber(std::string opSymbol)
//{
//	return instance.operatorSymToNumber[opSymbol];
//}



/**
 * Determines, based on the signatures, the modifiers for this function/
 * method.
 *
 * Attempts to find the following modifiers
 * 	virtual
 * 	pure virtual
 * 	const
 * 	static
 * 	inline
 * 	extern
 * 	[template] - not supported
 *
 *
 * @param sig Signature of the function/method
 * @return modifiers for this function
 */
//TODO test me
int ReflectionUtil::getFieldModifiers(std::string sig) {
	return getAndRemoveFieldModifiers(sig);
}

/**
 * Determines, based on the signatures, the modifiers for this function/
 * method.
 *
 * sig is returned without those modifiers
 *
 * Attempts to find the following modifiers
 * 	virtual
 * 	pure virtual
 * 	const
 * 	static
 * 	inline
 * 	extern
 * 	[template] - not supported
 *
 * @param sig Signature of the function/method
 * @return modifiers for this function
 */
int ReflectionUtil::getAndRemoveFieldModifiers(std::string & sig) {

	boost::regex matcher;
	matcher.assign(TYPE_CLEAN_REGEX_NO_ARRAY);
	sig = StringUtil::trim(boost::regex_replace(sig, matcher,
				TYPE_CLEAN_REPLACE_NO_ARRAY,
				boost::match_default | boost::format_all));

	int modifiers = cpgf::metaModifierNone;
	//static, extern, volatile, mutable
	if (findModifier("mutable", sig)) {
		modifiers |= cpgf::metaModifierMutable;
	}
	if (findModifier("static", sig)) {
		modifiers |= cpgf::metaModifierStatic;
	}
	if (findModifier("volatile", sig)) {
		modifiers |= cpgf::metaModifierVolatile;
	}
	if (findModifier("extern", sig)) {
		modifiers |= cpgf::metaModifierExtern;
	}

	//const
	std::string name = "";
	size_t arrayPos = sig.find("[");
	size_t namePos = sig.rfind(" ", arrayPos);
	if (namePos != string::npos) {
		name = StringUtil::trim(sig.substr(namePos, string::npos));
		sig = StringUtil::trim(sig.substr(0, namePos));

	}

	matcher.assign("(?:\\s|^|\\*)(const)(?:\\s|&|$)(?:[^*]*$)");
	if (boost::regex_search(sig, matcher)) {
		modifiers |= cpgf::metaModifierConst;
		sig = StringUtil::trim(boost::regex_replace(sig, matcher, "(?1)",
						boost::match_default | boost::format_all));
	}

	//TODO template - not handled yet


	if (name != "") {
		sig = sig + " " + name;
	}

	return modifiers;
}


/**
 * Divides a function/method into return type, name, parameters.
 *
 * The vector returned from the method has the return type in position 0,
 * the name in position 1, anything after the parameters (like const) is in
 * position 2 and any parameters follow.
 *
 * @param sig Signature of the function/method
 * @return A vector with the return type, name, and parameters.
 */
FieldInfo ReflectionUtil::divideFieldSignature(std::string sig) {
	FieldInfo info;

	boost::regex matcher;
	std::string newField;

	newField = sig;

	matcher.assign(TYPE_CLEAN_REGEX_NO_ARRAY);
	newField = StringUtil::trim(boost::regex_replace(newField, matcher,
			TYPE_CLEAN_REPLACE_NO_ARRAY,
			boost::match_default | boost::format_all));

	//assumes it has a name
	size_t pos = newField.rfind(" ");

	if (pos != string::npos)
	{
		info.type = newField.substr((string::size_type) 0,
				(string::size_type) pos);
		info.name = newField.substr((string::size_type) pos,
				(string::size_type) string::npos);

		matcher.assign(ARRAY_MATCH_REGEX);
		boost::match_results<std::string::iterator> match;
//		std::string::const_iterator start = ;
//		std::string::const_iterator end = ;

		while (boost::regex_search(info.name.begin(), info.name.end(), match, matcher))
		{
			info.type += std::string(match[0].first, match[0].second);
			info.name = std::string(info.name.begin(), match[0].first) +
					std::string(match[0].second, info.name.end());
		}

		info.type = correctType(info.type);
	}

	//this expects a name, so add a fake one and remove it afterwards
	info.type += " x";
	info.modifiers = getAndRemoveFieldModifiers(info.type);
	info.type = info.type.substr(0, info.type.length() - 2);

	//clean out any extra spaces in there
	matcher.assign(CLEAN_SPACES);
	info.name = StringUtil::trim(boost::regex_replace(info.name, matcher,
			CLEAN_SPACES_REPLACE, boost::match_default | boost::format_all));
	info.type = StringUtil::trim(boost::regex_replace(info.type, matcher,
			CLEAN_SPACES_REPLACE, boost::match_default | boost::format_all));

	return info;
}


std::string ReflectionUtil::createFieldSignature(const cpgf::GMetaField *field)
{
	return getType(field->getItemType()) +
			" " /*+ mClass->getName() + "::"*/ + field->getName();
}


std::string ReflectionUtil::createFunctionSignature(const cpgf::GMetaMethod *method)
{
	std::string sig;

	if (method->hasResult())
	{
		sig += getType(method->getResultType()) + " ";
	}
	else
	{
//		CPGF_TRACE(method->getName() << " " << !isDestructor(method->getName()) << "\r\n")
		if (!isDestructor(method->getName())) {
			sig = "void ";
		}
	}


//	if (mClass->getName() != "")
//	{
//		sig += mClass->getQualifiedName() + "::";
//	}

//	std::string opName = ReflectionUtil::getOperatorSymbol(method->getName());

//	if (opName == "")
//	{
		sig += method->getName() + "(";
//	}
//	else
//	{
//		sig += opName + "(";
//	}

	std::string pType;

	for (size_t i = 0; i < method->getParamCount(); i++)
	{
		pType = getType(method->getParamType(i));
		if (pType != "") {
			if (i != 0) {
				sig += ", ";
			}
			sig += pType;

			if (i == method->getParamCount() - 1 && method->isVariadic()) {
				sig += "...";
			}
		}
	}

	sig += ")";

//	if (method->getItemType().isConstFunction() ||
//			method->getItemType().isConstVolatileFunction())
	if ((method->getModifiers() & cpgf::metaModifierConst) != 0)
	{
		sig += " const";
	}

	return sig;
}

std::string ReflectionUtil::createConstructorSignature(const cpgf::GMetaClass *mClass, const cpgf::GMetaConstructor *construct)
{
	std::string sig;

	sig += mClass->getName() + "(";

	for (size_t i = 0; i < construct->getParamCount(); i++)
	{
		sig += getType(construct->getParamType(i));
		if (i != construct->getParamCount() - 1)
		{
			sig += ", ";
		}
		else
		{
			if (construct->isVariadic())
			{
				sig += "...";
			}
		}
	}

	sig += ")";

	return sig;
}


//int ReflectionUtil::getNumberFromName(std::string name) {
//	stringstream sString;
//	int value = -1;
//
//	sString << name;
//	sString >> value;
//
//	return value;
//}


//std::string ReflectionUtil::createOperatorSignature(const cpgf::GMetaOperator *op)
//{
//	std::string sig;
//
//	if (op->hasResult())
//	{
//		sig += getType(op->getResultType());
//		sig += " ";
//	}
//	else
//	{
//		sig = "void ";
//	}
////	if (mClass->getName() != "")
////	{
////		sig += mClass->getName() + "::";
////	}
//	int opNumber = getNumberFromName(op->getName());
//	sig += getOperatorSymbol(opNumber) + "(";
//
//	for (size_t i = 0; i < op->getParamCount(); i++)
//	{
//		sig += getType(op->getParamType(i));
//		if (i != op->getParamCount() - 1)
//		{
//			sig += ", ";
//		}
//	}
//
//	sig += ")";
//
////	if (op->getItemType().isConst())
////	{
////		sig += " const";
////	}
//	if (op->getItemType().isConst() ||
//			op->getItemType().isConstFunction() ||
//			op->getItemType().isConstVolatileFunction())
//	{
//		sig += " const";
//	}
//
//	return sig;
//}

std::string ReflectionUtil::getType(const cpgf::GMetaType &type)
{
	if (type.getBaseName()) {
		return type.getBaseName();
	}

//	if(! type.getBaseType().isEmpty()) {
//			const cpgf::GMetaTypedItem * item = cpgf::getGlobalMetaClass()->getModule()->findItemByType(type.getBaseType());
//			if(item != NULL) {
//				return item->getQualifiedName().c_str();
//			}
//		}

	return type.getBaseType().getStdTypeInfo().name();
}




std::string ReflectionUtil::buildStr(std::vector<std::string> v,
			std::string delim)
{
	std::string resultStr;
	for (size_t i = 0; i < v.size(); i++)
	{
		if (i != 0)
		{
			resultStr += delim;
		}
		resultStr += v[i];
	}

	return resultStr;
}

std::vector<std::string> & ReflectionUtil::correctModifiers(
		std::vector<std::string> &v)
{

	//fix up modifiers
	int cPos = -1;
	int vPos = -1;
//	int mPos = -1;
	for (size_t i = 0; i < v.size(); )
	{
		if (v[i] == "*" || v[i] == "&")
		{
			i++;
			cPos = vPos /*= mPos */= -1;
		}
		else
		{
			if (v[i] == "const")
			{

				if (cPos == -1)
				{
					cPos = i;
					i++;
				}
				else { //if it already exists, delete it
					v.erase(v.begin() + i);
				}
			}
			else
			{
				if (v[i] == "volatile")
				{
					if (vPos == -1)
					{
						vPos = i;
						i++;
					}
					else { //if it already exists, delete it
						v.erase(v.begin() + i);
					}
				}
//				else {
//					if (v[i] == "mutable")
//					{
//						if (mPos == -1)
//						{
//							mPos = i;
//							i++;
//						}
//						else { //if it already exists, delete it
//							v.erase(v.begin() + i);
//						}
//					}
//				}
			}
		}
		//if out of order, swap strings and positions
		if (cPos != -1 && vPos != -1 && cPos > vPos) {
			std::string swap = v[cPos];
			v[cPos] = v[vPos];
			v[vPos] = swap;

			int swapPos = cPos;
			cPos = vPos;
			vPos = swapPos;
		}
//		reorderModifiers(v, vPos, cPos, mPos);
	}

	return v;
}


void ReflectionUtil::swapModifiers(std::vector<std::string> & values,
		int & first, int & second) {
	if (first != -1 && second != -1 && first < second) {
		std::string swap = values[first];
		values[first] = values[second];
		values[second] = swap;

		int swapPos = first;
		first = second;
		second = swapPos;
	}
}

//reorder volatile const mutable
void ReflectionUtil::reorderModifiers(std::vector<std::string> & values,
		int & volatilePos, int & constPos, int & mutablePos) {

	//out of order
	swapModifiers(values, volatilePos, constPos);
	swapModifiers(values, volatilePos, mutablePos);
	swapModifiers(values, constPos, mutablePos);
}


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
bool ReflectionUtil::findModifier(std::string modifierToFind, std::string & sig) {
	std::string newSig = sig;
	std::string regex = "(?:^|\\s+)" + modifierToFind + "(?:\\s+|$)";
	boost::regex matcher;


	matcher.assign(regex);
	newSig = StringUtil::trim(boost::regex_replace(newSig, matcher,
			" ", boost::match_default | boost::format_all));

	if (sig != newSig) {
		sig = newSig;
		return true;
	}

	return false;
}


std::string ReflectionUtil::correctField(std::string field)
{

//	std::string resultStr;
//
//	boost::regex matcher;
//	std::string newField;
//
//	matcher.assign(TYPE_CLEAN_REGEX_NO_ARRAY);
//	newField = StringUtil::trim(boost::regex_replace(field, matcher,
//			TYPE_CLEAN_REPLACE_NO_ARRAY,
//			boost::match_default | boost::format_all));
//
//	//assumes it has a name
//	size_t pos = newField.rfind(" ");
//
//	if (pos != string::npos)
//	{
//		std::string type = newField.substr((string::size_type) 0,
//				(string::size_type) pos);
//		std::string name = newField.substr((string::size_type) pos,
//				(string::size_type) string::npos);
//
//		matcher.assign(ARRAY_MATCH_REGEX);
//		boost::match_results<std::string::iterator> match;
////		std::string::const_iterator start = ;
////		std::string::const_iterator end = ;
//
//		while (boost::regex_search(name.begin(), name.end(), match, matcher))
//		{
//			type += std::string(match[0].first, match[0].second);
//			name = std::string(name.begin(), match[0].first) +
//					std::string(match[0].second, name.end());
//		}
//
//		std::string fixType = correctType(type);
//		//remove volatile, mutable, and const from type
//		matcher.assign(CLEAN_FIELD_TYPE);
////		CPGF_TRACE("before " << fixType << "\r\n")
//		fixType = boost::regex_replace(fixType, matcher, CLEAN_FIELD_TYPE_REPLACE,
//						boost::match_default | boost::format_all);
////		CPGF_TRACE("after  " << fixType << "\r\n")
//		newField = fixType + " " + name;
//	}
//	//clean out any extra spaces in there
//	//put in other correct* methods - done?
//	matcher.assign(CLEAN_SPACES);
//	newField = boost::regex_replace(newField, matcher, CLEAN_SPACES_REPLACE,
//				boost::match_default | boost::format_all);
	FieldInfo info = divideFieldSignature(field);
	return info.type + " " + info.name;
//	return newField;
}


std::string ReflectionUtil::correctType(std::string type)
{
//	const std::string TYPE_CLEAN_REGEX = "(\\s+)|(\\()|(\\))|(^\\s+)|(\\s+$)([^\\s,]*::)";
//	const std::string TYPE_CLEAN_REPLACE = "(?1 )(?2 )(?3 )(?4)(?5)(?6)";
	std::string resultStr;
	std::set<std::string> typeModifiers;
	typeModifiers.insert("*");
	typeModifiers.insert("&");
	typeModifiers.insert("const");
	typeModifiers.insert("volatile");
	typeModifiers.insert("mutable");

	boost::regex matcher;
	std::string newType;

	matcher.assign(TYPE_CLEAN_REGEX);
	newType = boost::regex_replace(type, matcher, TYPE_CLEAN_REPLACE,
			boost::match_default | boost::format_all);

	std::vector<std::string> tokens;
	std::vector<std::string> modifiers;
	std::vector<std::string> typeTokens;
	boost::split(tokens, newType, boost::is_any_of(" "));

	//divide into modifiers and tokens
	for (std::vector<std::string>::iterator it = tokens.begin();
			it != tokens.end(); it++)
	{
		if (typeModifiers.count(*it) > 0)
		{
			modifiers.push_back(*it);
		}
		else
		{
			typeTokens.push_back(*it);
		}
	}

	// fix up modifiers
	correctModifiers(modifiers);

	//create string from type and then modifiers
	resultStr = buildStr(typeTokens, " ") + " " + buildStr(modifiers, " ");

	return StringUtil::trim(resultStr);
}

void ReflectionUtil::findParams(std::string signature, int &openParen, int &closeParen)
{
	closeParen = -1;
	openParen = -1 ;
	int parenCount = 0;


	for (int i = signature.length() - 1; i >=0 &&
		(parenCount != 0 || closeParen == -1); i--)
	{
		if (signature[i] == ')')
		{
			parenCount++;
			if (closeParen == -1)
			{
				closeParen = i;
			}
		}
		if (signature[i] == '(')
		{
			parenCount--;
			if (parenCount == 0)
			{
				openParen = i;
			}
		}
	}
}


std::vector<std::string> ReflectionUtil::getParams(std::string signature,
		int start, int end)
{
	std::string sig = signature;
	std::vector<std::string> params;

	if (start != -1 && end != -1)
	{
		sig = signature.substr((string::size_type) start,
				(string::size_type) end - start + 1);
	}

	boost::split(params, sig, boost::is_any_of(","));

	for (size_t i = 0; i < params.size(); i++)
	{
		params[i] = correctType((std::string) params[i]);
	}

	return params;
}


/**
 * Divides an enum into a name and a list of enums and their values
 *
 * @param sig Signature of the enum
 * @return name and a list of enums and their values
 */
EnumInfo ReflectionUtil::divideEnumSignature(std::string sig) {
	EnumInfo info;

	boost::regex matcher;
	std::string newSig;
	matcher.assign(SIGNATURE_CLEAN_ENUM_REGEX);
	newSig = boost::regex_replace(sig, matcher, SIGNATURE_CLEAN_ENUM_REPLACE,
			boost::match_default | boost::format_all);

	size_t openPos = newSig.find("{", 0);

	if (openPos != std::string::npos) {
		info.name = StringUtil::trim(newSig.substr(0, openPos - 1));

		size_t closePos = newSig.find("}", 0);
		std::string values;
		if (openPos != std::string::npos) {
			values = StringUtil::trim(newSig.substr(openPos + 1, closePos - 1 - openPos));

			std::vector<std::string> tokens;
			std::vector<std::string> nameValue;
			std::string name;
			std::string token;
			int value;

			Tokenizer::tokenize(values, tokens, "([^,]+)");
			int lastValue = -1;
			for (size_t i = 0; i < tokens.size(); i++) {
				token = StringUtil::trim(tokens[i]);

				nameValue.clear();
				//divide into name and value
				Tokenizer::tokenize(token, nameValue, "([^\\s=]+)");
				if (nameValue.size() >= (size_t) 1) {
					name = nameValue[0];
					if (nameValue.size() >= (size_t) 2) {
						std::stringstream s;
						s << nameValue[1];
						s >> value;
						lastValue = value;
					}
					else {
						lastValue++;
						value = lastValue;
					}
					//add to map
					info.names[name] = value;
					info.values[value] = name;
				}

			}
		}
	}
	else {
		info.name = StringUtil::trim(newSig);
	}

	return info;
}




std::string ReflectionUtil::correctEnum(std::string enumStr)
{
//	boost::regex matcher;
//	std::string newEnum;
//	matcher.assign(SIGNATURE_CLEAN_ENUM_REGEX);
//	newEnum = boost::regex_replace(enumStr, matcher, SIGNATURE_CLEAN_ENUM_REPLACE,
//			boost::match_default | boost::format_all);
	EnumInfo info = divideEnumSignature(enumStr);

	return info.name;
}



/**
 * Determines, based on the signatures, the modifiers for this function/
 * method.
 *
 * Attempts to find the following modifiers
 * 	virtual
 * 	pure virtual
 * 	const
 * 	static
 * 	inline
 * 	extern
 * 	[template] - not supported
 *
 *
 * @param sig Signature of the function/method
 * @return modifiers for this function
 */
//TODO test me
int ReflectionUtil::getFunctionModifiers(std::string sig) {
	return getAndRemoveFunctionModifiers(sig);
}

/**
 * Determines, based on the signatures, the modifiers for this function/
 * method.
 *
 * sig is returned without those modifiers
 *
 * Attempts to find the following modifiers
 * 	virtual
 * 	pure virtual
 * 	const
 * 	static
 * 	inline
 * 	extern
 * 	[template] - not supported
 *
 * @param sig Signature of the function/method
 * @return modifiers for this function
 */
int ReflectionUtil::getAndRemoveFunctionModifiers(std::string & sig) {
	int modifiers = cpgf::metaModifierNone;
	//virtual, static, inline, extern
	if (findModifier("virtual", sig)) {
		modifiers |= cpgf::metaModifierVirtual;
	}
	if (findModifier("static", sig)) {
		modifiers |= cpgf::metaModifierStatic;
	}
	if (findModifier("inline", sig)) {
		modifiers |= cpgf::metaModifierInline;
	}
	if (findModifier("extern", sig)) {
		modifiers |= cpgf::metaModifierExtern;
	}

	//const, pure virtual
	size_t pos = sig.rfind(")");
	if (pos != string::npos) {
//		sig = sig.substr(0, pos);
		std::string afterParamStr = StringUtil::trim(sig.substr(pos + 1,
							string::npos));
		if (findModifier("const", afterParamStr)) {
			modifiers |= cpgf::metaModifierConst;
		}
		if (findModifier("=\\s*0", afterParamStr)) {
			modifiers |= cpgf::metaModifierPureVirtual;
		}
	}
	//TODO template - not handled yet
	return modifiers;
}


/**
 * Divides a function/method into return type, name, parameters.
 *
 *
 * @param sig Signature of the function/method
 * @return A vector with the return type, name, and parameters.
 */
FunctionInfo ReflectionUtil::divideFunctionSignature(std::string sig) {
	int openParen;
	int closeParen;
	boost::regex matcher;
	std::string newSig;
	std::string afterParamStr;
	FunctionInfo info;

	newSig = sig;

	matcher.assign(SIGNATURE_CLEAN_REGEX);
	newSig = boost::regex_replace(newSig, matcher, SIGNATURE_CLEAN_REPLACE,
			boost::match_default | boost::format_all);

	findParams(newSig, openParen, closeParen);

	info.modifiers = getAndRemoveFunctionModifiers(newSig);

	//get the name and return value
	info.returnType = "";
	info.name = StringUtil::trim(newSig.substr((string::size_type) 0,
			(string::size_type) openParen));
	size_t pos;
	//find the name
	pos = info.name.rfind(" ");
	//		for (pos = nameStr.length() - 1; pos != string::npos && nameStr[pos] != ' '; pos--)
	//		{
	//			//do nothing
	//		}
	if (pos != string::npos)
	{
		info.returnType = correctType(info.name.substr((string::size_type) 0,
				(string::size_type) pos));
		info.name = info.name.substr((string::size_type) pos + 1, string::npos);
	}
	//get the params
	info.params = getParams(newSig, openParen + 1, closeParen - 1);

	return info;
}


/**
 * Takes a function signature and does a little formatting on it.
 *
 * This removes consecutive whitespace and adds a space after ,'s.
 *
 * @param signature to correct
 * @param stripNamespace removes namespaces from function/method names and
 * 	parameter names
 *
 * @return corrected signature
 */
std::string ReflectionUtil::correctSignature(std::string signature)
{
//	int openParen;
//	int closeParen;
	boost::regex matcher;
	std::string newSig;
//	std::string returnStr;
//	std::string nameStr;
//	std::string afterParamStr;
	std::string operatorName;
	FunctionInfo info;

	newSig = signature;
	//handle operators
	//find operator token to ()
	//replace with a default value
	//fix up the name so that it has no spaces.

	matcher.assign(OPERATOR_MATCH_REGEX);
	boost::match_results<std::string::const_iterator> matchResults;
	bool operatorMatch = regex_search(newSig, matchResults, matcher,
			boost::match_default);

	if (operatorMatch) {
		operatorName = std::string(matchResults[1].first, matchResults[1].second);

		newSig = boost::regex_replace(newSig, matcher, OPERATOR_MATCH_REPLACE,
				boost::match_default | boost::format_all);

		//remove spaces in the operator
		boost::regex opNameMatcher("(\\s+)");
		operatorName = boost::regex_replace(operatorName, opNameMatcher, "(?1)",
				boost::match_default | boost::format_all);

	}

	info = divideFunctionSignature(newSig);
//	matcher.assign(SIGNATURE_CLEAN_REGEX);
//	newSig = boost::regex_replace(newSig, matcher, SIGNATURE_CLEAN_REPLACE,
//			boost::match_default | boost::format_all);
//
//	findParams(newSig, openParen, closeParen);
//	//get anything after the parameters (i.e. const)
//	afterParamStr = StringUtil::trim(newSig.substr(
//			(string::size_type) closeParen + 1,
//			(string::size_type) string::npos));
//	//get the name and return value
//	nameStr = StringUtil::trim(newSig.substr((string::size_type) 0,
//			(string::size_type) openParen));
//	size_t pos;
//	//find the name
//	for (pos = nameStr.length() - 1; pos != string::npos && nameStr[pos] != ' '; pos--)
//	{
//		//do nothing
//	}
//	if (pos != string::npos)
//	{
//		returnStr = correctType(nameStr.substr((string::size_type) 0,
//				(string::size_type) pos));
//		nameStr = nameStr.substr((string::size_type) pos + 1, string::npos);
//	}
//	//get the params
//	std::vector<std::string> params = getParams(newSig, openParen + 1,
//			closeParen - 1);
//
	if (info.returnType == "" || isDestructor(info.name))
	{
		newSig = info.name + "(";
	}
	else
	{
		newSig = info.returnType + " " + info.name + "(";
	}


//	for (size_t i = 0; i < params.size(); i++)
//	{
//		if (i != 0)
//		{
//			newSig += ", ";
//		}
//		newSig += params[i];
//	}

	newSig += buildStr(info.params, ", ") + ")";
//	if (afterParamStr != "")
//	{
//		newSig += " " + afterParamStr;
//	}

	//add the const to the signature if needed
	if ((info.modifiers & cpgf::metaModifierConst) != 0) {
		newSig += " const";
	}

	//strip out extra ( )
	//	go backwards counts ( ) until matching found
	// remove rest of them
	//break function into return, name, parameters
	// name is right before (
	// return type before name
	// params in ( ) delimited by ,

//	boost::regex matcher;


//	if (stripNamespace)
//	{
//		matcher.assign(SIGNATURE_CLEAN_NS_REGEX);
//	    newSig = boost::regex_replace(signature, matcher,
//	    		SIGNATURE_CLEAN_NS_REPLACE,
//	boost::match_default | boost::format_all);
//	}
//	else
//	{
//		matcher.assign(SIGNATURE_CLEAN_REGEX);
//	    newSig = boost::regex_replace(signature, matcher,
//	    		SIGNATURE_CLEAN_REPLACE,
//	boost::match_default | boost::format_all);
//	}

	matcher.assign(CLEAN_SPACES);
	newSig = boost::regex_replace(newSig, matcher, CLEAN_SPACES_REPLACE,
			boost::match_default | boost::format_all);

	//restore name
	if (operatorMatch) {
//		matcher.assign(FAKE_OPERATOR_NAME);
//		newSig = boost::regex_replace(newSig, matcher, operatorName,
//				boost::match_default | boost::format_all);
		newSig.replace(newSig.find(FAKE_OPERATOR_NAME),
				FAKE_OPERATOR_NAME.length(), operatorName);

	}

	return newSig;
}



/**
 * Determines, based on the signatures, the modifiers for this class.
 *
 * Attempts to find the following modifiers
 * 	virutal (for base classes)
 * 	[template] - not supported
 *
 *
 * @param sig Signature of the class
 * @return modifiers for this class
 */
int ReflectionUtil::getClassModifiers(std::string sig) {
	return getAndRemoveClassModifiers(sig);
}

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
 * @param sig Signature of the class
 * @return modifiers for this class
 */
int ReflectionUtil::getAndRemoveClassModifiers(std::string & sig) {
	int modifiers = cpgf::metaModifierNone;
	//virtual, static, inline, extern
	if (findModifier("virtual", sig)) {
		modifiers |= cpgf::metaModifierVirtual;
	}

	//TODO template - not handled yet
	return modifiers;
}

/**
 * Divides a class into name, enclosing class, and modifiers.
 *
 *
 * @param sig Signature of the class
 * @return Info on the class
 */
ClassInfo ReflectionUtil::divideClassSignature(std::string sig) {
	ClassInfo info;

	info.modifiers = getAndRemoveClassModifiers(sig);
	info.enclosingClass = "";
	info.name = sig;
	size_t pos = info.name.rfind("::");
	if (pos != std::string::npos) {
		info.enclosingClass = info.name.substr(0, pos);
		info.name = info.name.substr(pos + 2, string::npos);
	}

	return info;
}

/**
 * Utility method that corrects the formatting of
 * (inner) classes so they are in
 * a standard format and can be compared.
 *
 * @param signature class signature
 * @return Corrected signature
 *
 */
std::string ReflectionUtil::correctClassSignature(std::string signature) {
//	size_t pos = signature.rfind("::");
//	if (pos != std::string::npos) {
//		return signature.substr(pos + 2, string::npos);
//	}

	ClassInfo info = divideClassSignature(signature);
	return info.name;
}



std::string ReflectionUtil::createFieldSignature(const std::string & fieldType,
		const std::string & name)
{
	return correctSignature(fieldType + " " + name);
}



//methods

//std::string ReflectionUtil::createFunctionSignature(std::string returnType, std::string name,
//		int paramCount, std::string params...)
//{
//	std::list<std::string> paramList;
//
//	va_list paramArgs;
//	va_start(paramArgs, params);
//
//	for (int i = 0; i < paramCount; i++) {
//		paramList.push_back(va_arg(paramArgs, std::string));
//	}
//
//	va_end(paramArgs);
//
//	return createFunctionSignature(returnType, name, paramList);
//}


std::string ReflectionUtil::createFunctionSignature(std::string returnType, std::string name,
		const std::vector<std::string> & params, bool isConst)
{
	std::string sig = returnType + " " + name + "(";

	for (std::vector<std::string>::const_iterator i = params.begin();
			i != params.end(); i++)
	{
		if (i != params.begin())
		{
			sig += ", ";
		}
		sig += *i;

	}

	sig += ")";

//	if (isConst)
//	{
//		sig += " const";
//	}

	return correctSignature(sig);
}



//constructors
//std::string ReflectionUtil::createConstructorSignature(std::string name,
//		int paramCount, std::string params...)
//{
//	std::list<std::string> paramList;
//
//	va_list paramArgs;
//	va_start(paramArgs, params);
//
//	for (int i = 0; i < paramCount; i++) {
//		paramList.push_back(va_arg(paramArgs, std::string));
//	}
//
//	va_end(paramArgs);
//
//	return createConstructorSignature(name, paramList);
//}


std::string ReflectionUtil::createConstructorSignature(std::string name,
		const std::vector<std::string> & params)
{
	std::string sig = name + "(";

	for (std::vector<std::string>::const_iterator i = params.begin();
			i != params.end(); i++)
	{
		if (i != params.begin())
		{
			sig += ", ";
		}
		sig += *i;

	}

	sig += ")";

	return correctSignature(sig);
}



//operators
//std::string ReflectionUtil::createOperatorSignature(std::string returnType, std::string name,
//		int paramCount, std::string params...)
//{
//	std::list<std::string> paramList;
//
//	va_list paramArgs;
//	va_start(paramArgs, params);
//
//	for (int i = 0; i < paramCount; i++) {
//		paramList.push_back(va_arg(paramArgs, std::string));
//	}
//
//	va_end(paramArgs);
//
//	return createOperatorSignature(returnType, name, paramList);
//}


//std::string ReflectionUtil::createOperatorSignature(std::string returnType, std::string name,
//		const std::vector<std::string> & params)
//{
//	std::string sig = returnType + " " + name + "(";
//
//		for (std::vector<std::string>::const_iterator i = params.begin();
//				i != params.end(); i++)
//		{
//			if (i != params.begin())
//			{
//				sig += ", ";
//			}
//			sig += *i;
//
//		}
//
//		sig += ")";
//
//		return correctSignature(sig);
//}



/**
 * Determines if the operator is one of the postfix operators
 * (x++ or x--)
 *
 * @param sig signature to check
 * @return true if it is the signature of an postfix operator
 */
bool ReflectionUtil::isPostFixOperator(std::string sig) {
	boost::regex matcher;
		matcher.assign(POSTFIX_OPERATOR_PATTERN);
		return boost::regex_match(correctSignature(sig), matcher);
}



/**
 * Determines if this signature matches the pattern of names for auto-
 * generated array setters (see createArraySetterSignature). Signatures
 * should have the form:
 * 	 opErAToRWrapper_<class name>_<return type>_array_<params>
 * 	 where spaces are replaced with _, & is replaced with ref, and
 * 	 is replaced with ptr.
 * This method checks the signature against the regex
 * 	 opErAToRWrapper_.*_array_.*
 * to see if it is valid.
 *
 * @param sig signature to check
 * @return true if it is the signature of an array setter method
 */
bool ReflectionUtil::isArraySetterSignature(std::string sig) {
	boost::regex matcher;
	matcher.assign(ARRAY_WRAPPER_PATTERN);
	return boost::regex_match(correctSignature(sig), matcher);
}

/**
 * Determines if this signature matches the operator[] method and that it
 * does not return a const result.
 *
 * @param sig signature to check
 * @return true if it is the signature of an array setter method
 */
bool ReflectionUtil::isSettableArrayOperator(std::string sig) {
	boost::regex nonConstMatcher(ARRAY_OPERATOR_PATTERN);
	boost::regex constMatcher(ARRAY_OPERATOR_CONST_PATTERN);
	std::string cSig = correctSignature(sig);
	return boost::regex_match(cSig, nonConstMatcher)
		&& !boost::regex_match(cSig, constMatcher);
}



/**
 * Utility method that creates a method signature for the auxilary function
 * used to simulate setting a value with the [] operator
 * (i.e. x[2] = 4)
 *
 * @param className Name of the class
 * @param arrayOpSignature signature of the operator being called
 * 	(i.e. string & operator[](int) )
 */
std::string ReflectionUtil::createArraySetterSignature(std::string className,
		std::string arrayOpSignature) {

	std::string cleanedName = correctSignature(arrayOpSignature);
	std::string name = ARRAY_WRAPPER_PREFIX + className + "_" +
			correctSignature(arrayOpSignature);
	//opErAToRWrapper_Operators_int_ref_array_int

	//get rid of ,'s, ( ), &, *
	boost::regex matcher;
	matcher.assign(CLEAN_OPNAME);
	name = boost::regex_replace(name, matcher, CLEAN_OPNAME_REPLACE,
			boost::match_default | boost::format_all);

	matcher.assign("[\\s_]+");
	name = boost::regex_replace(name, matcher, "_",
			boost::match_default | boost::format_all);

	matcher.assign(ARRAY_OPERATOR_REGEX);
	std::string newSig = boost::regex_replace(cleanedName,
			matcher, "(?1" + name + ")",
			boost::match_default | boost::format_all);

	return newSig;
}



/**
 * Takes a function/method signature and returns the return type
 *
 * @param signature signature of the function/method
 * @return the return type of the function/method as a string
 */
std::string ReflectionUtil::getReturnType(std::string signature) {
	std::string newSig = correctSignature(signature);
	int openParen;
	int closeParen;
	std::string returnStr = "";

	findParams(newSig, openParen, closeParen);

	//get the name and return value
	std::string nameStr = StringUtil::trim(newSig.substr((string::size_type) 0,
			(string::size_type) openParen));
	size_t pos;
	//find the name
	for (pos = nameStr.length() - 1; pos != string::npos
		&& nameStr[pos] != ' '; pos--)
	{
		//do nothing
	}
	if (pos != string::npos)
	{
		returnStr = correctType(nameStr.substr((string::size_type) 0,
				(string::size_type) pos));
	}

	return returnStr;
}




/**
 * Given a string representation of a type, returns if it is a pointer.
 *
 * @param type Type signature
 * @return true if it is a pointer
 */
bool ReflectionUtil::isPointerType(std::string type) {
	return type.find("*") != std::string::npos;
}

/**
 * Given a string representation of a type, returns if it is a reference.
 *
 * @param type Type signature
 * @return true if it is a reference
 */
bool ReflectionUtil::isReferenceType(std::string type) {
	return type.find("&") != std::string::npos;
}


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
bool ReflectionUtil::isCopyFunctionSignature(std::string sig) {
	boost::regex matcher;
	matcher.assign(COPY_WRAPPER_PATTERN);
	return boost::regex_match(correctSignature(sig), matcher);
}


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
std::string ReflectionUtil::createCopyFunctionSignature(std::string className) {
	std::string name = COPY_WRAPPER_PREFIX + className;

//cpgf::GVariant cOpYwRaPpEr_Operators(Operators * self)
	return className + " * " + name + "(" + className + " *)";
}


/**
 * Determines if the name of this method refers to a destructor
 *
 * @param name Name of the method
 * @return true if it is a destructor
 */
bool ReflectionUtil::isDestructor(std::string name) {
	return (name != "" && name[0] == '~');
}


///**
// * Gets the name from a non-reflected enumeration which has the form
// *  <name> {<enum> [= <value>], ...}
// * @param signature Signature of the enum
// * @return name of the enum
// */
//std::string ReflectionUtil::getEnumName(std::string signature) {
//	size_t openPos = signature.find("{", 0);
//
//	std::string name;
//	if (openPos != std::string::npos) {
//		name = StringUtil::trim(signature.substr(0, openPos - 1));
//	}
//	else {
//		name = signature;
//	}
//
//	return name;
//}
//
///**
// * Gets the values from a non-reflected enumeration which has the form
// *  <name> {<enum> [= <value>], ...}
// * @param signature Signature of the enum
// * @return map with the name of the value and the value
// */
//std::map<std::string, int> ReflectionUtil::getEnumValues(
//		std::string signature) {
//	size_t openPos = signature.find("{", 0);
//	size_t closePos = signature.find("}", 0);
//	std::map<std::string, int> data;
//	std::string values;
//	if (openPos != std::string::npos) {
//		values = StringUtil::trim(signature.substr(openPos + 1, closePos - 1 - openPos));
//
//		std::vector<std::string> tokens;
//		std::vector<std::string> nameValue;
//		std::string name;
//		std::string token;
//		int value;
//
//		Tokenizer::tokenize(values, tokens, "([^,]+)");
//		int lastValue = -1;
//		for (size_t i = 0; i < tokens.size(); i++) {
//			token = StringUtil::trim(tokens[i]);
//
//			nameValue.clear();
//			//divide into name and value
//			Tokenizer::tokenize(token, nameValue, "([^\\s=]+)");
//			if (nameValue.size() >= (size_t) 1) {
//				name = nameValue[0];
//				if (nameValue.size() >= (size_t) 2) {
//					std::stringstream s;
//					s << nameValue[1];
//					s >> value;
//					lastValue = value;
//				}
//				else {
//					lastValue++;
//					value = lastValue;
//				}
//				//add to map
//				data[name] = value;
//			}
//
//		}
//	}
//
//	return data;
//}



}

