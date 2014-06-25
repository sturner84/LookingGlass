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


//ReflectionUtil ReflectionUtil::instance;
//
//ReflectionUtil::ReflectionUtil()
//{
////	loadOperatorNames();
//}


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
bool Signature::findModifier(std::string modifierToFind, std::string & sig) const {
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


std::string Signature::buildStr(std::vector<std::string> v,
		std::string delim) const
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


std::vector<std::string> & Signature::correctModifiers(
		std::vector<std::string> &v)
{

	//fix up modifiers
	int cPos = -1;
	int vPos = -1;
	int mPos = -1;
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
				else {
					if (v[i] == "mutable")
					{
						if (mPos == -1)
						{
							mPos = i;
							i++;
						}
						else { //if it already exists, delete it
							v.erase(v.begin() + i);
						}
					}
				}
			}
		}
		//		//if out of order, swap strings and positions
		//		if (cPos != -1 && vPos != -1 && cPos > vPos) {
		//			std::string swap = v[cPos];
		//			v[cPos] = v[vPos];
		//			v[vPos] = swap;
		//
		//			int swapPos = cPos;
		//			cPos = vPos;
		//			vPos = swapPos;
		//		}
		reorderModifiers(v, vPos, cPos, mPos);
	}

	return v;
}


void Signature::swapModifiers(std::vector<std::string> & values,
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
void Signature::reorderModifiers(std::vector<std::string> & values,
		int & volatilePos, int & constPos, int & mutablePos) {

	//out of order
//	CPGF_TRACE("before swap: V" << volatilePos << " "
//			<< (volatilePos != -1 ? values[volatilePos] : "n/a")
//			<< " C" << constPos << " "
//			<< (constPos != -1 ? values[constPos] : "n/a")
//			<< " M" << mutablePos << " "
//			<< (mutablePos != -1 ? values[mutablePos] : "n/a")
//			<< "\r\n")
	swapModifiers(values, volatilePos, constPos);
	swapModifiers(values, volatilePos, mutablePos);
	swapModifiers(values, constPos, mutablePos);
//	CPGF_TRACE("before swap: V" << volatilePos << " "
//				<< (volatilePos != -1 ? values[volatilePos] : "n/a")
//				<< " C" << constPos << " "
//				<< (constPos != -1 ? values[constPos] : "n/a")
//				<< " M" << mutablePos << " "
//				<< (mutablePos != -1 ? values[mutablePos] : "n/a")
//				<< "\r\n")

}



/**
 * Creates a type and formats it for comparison
 */
void TypeSignature::createType(const std::string & type) {
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
		if (StringUtil::trim(*it) != "") {
			if (typeModifiers.count(*it) > 0)
			{
				modifiers.push_back(*it);
			}
			else
			{
				typeTokens.push_back(*it);
			}
		}
	}

	// fix up modifiers
	correctModifiers(modifiers);

	//create string from type and then modifiers
	resultStr = buildStr(typeTokens, " ") + " " + buildStr(modifiers, " ");
//	CPGF_TRACE("built string: " << resultStr << "\r\n");
	name = StringUtil::trim(resultStr);
	signature = name;
}

/**
 * Gets the string version of the cpgf type
 *
 * @param type cpgf reflected type info
 */
TypeSignature::TypeSignature(const cpgf::GMetaType &type) : Signature()
{
	if (type.getBaseName()) {
		createType(type.getBaseName());
	}
	else {
		createType(type.getBaseType().getStdTypeInfo().name());
	}

	//signature = name;

}


/**
 * Given a string representation of a type, returns if it is a pointer.
 *
 * @return true if it is a pointer
 */
bool TypeSignature::isPointerType() const {
	return signature.find("*") != std::string::npos;
}

/**
 * Given a string representation of a type, returns if it is a reference.
 *
 * @return true if it is a reference
 */
bool TypeSignature::isReferenceType() const {
	return signature.find("&") != std::string::npos;
}



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
int FieldSignature::removeModifiers(std::string & sig) {

	boost::regex matcher;
	matcher.assign(TYPE_CLEAN_REGEX_NO_ARRAY);
	sig = StringUtil::trim(boost::regex_replace(sig, matcher,
			TYPE_CLEAN_REPLACE_NO_ARRAY,
			boost::match_default | boost::format_all));

	int tModifiers = cpgf::metaModifierNone;
	//static, extern, volatile, mutable
	if (findModifier("mutable", sig)) {
		tModifiers |= cpgf::metaModifierMutable;
	}
	if (findModifier("static", sig)) {
		tModifiers |= cpgf::metaModifierStatic;
	}
	if (findModifier("volatile", sig)) {
		tModifiers |= cpgf::metaModifierVolatile;
	}
	if (findModifier("extern", sig)) {
		tModifiers |= cpgf::metaModifierExtern;
	}
	//TODO can we assume that there is no name here??
	//const
//	std::string tName = "";
//	size_t arrayPos = sig.find("[");
//	size_t namePos = sig.rfind(" ", arrayPos);
//	if (namePos != string::npos) {
//		tName = StringUtil::trim(sig.substr(namePos, string::npos));
//		sig = StringUtil::trim(sig.substr(0, namePos));
//
//	}

	matcher.assign("(?:\\s|^|\\*)(const)(?:\\s|&|$)(?:[^*]*$)");
	if (boost::regex_search(sig, matcher)) {
//		CPGF_TRACE("sig const match before: " << sig << "\r\n");

		tModifiers |= cpgf::metaModifierConst;
		sig = StringUtil::trim(boost::regex_replace(sig, matcher, "(?1)",
				boost::match_default | boost::format_all));
//		CPGF_TRACE("sig const match after: " << sig << "\r\n");


	}

	//TODO template - not handled yet


//	if (tName != "") {
//		sig = sig + " " + tName;
//	}
	matcher.assign(CLEAN_SPACES);
	sig = StringUtil::trim(boost::regex_replace(sig, matcher,
			CLEAN_SPACES_REPLACE, boost::match_default | boost::format_all));


	return tModifiers;
}

/**
 * Divides a field/variable into type, name
 *
 * @param sig Signature of the field/variable
 *
 */
void FieldSignature::divideSignature(const std::string & sig) {
	boost::regex matcher;
	std::string newField;
	std::string nType;

	newField = sig;

	matcher.assign(TYPE_CLEAN_REGEX_NO_ARRAY);
	newField = StringUtil::trim(boost::regex_replace(newField, matcher,
			TYPE_CLEAN_REPLACE_NO_ARRAY,
			boost::match_default | boost::format_all));

	//assumes it has a name
	size_t pos = newField.rfind(" ");

	if (pos != string::npos)
	{
		nType = newField.substr((string::size_type) 0,
				(string::size_type) pos);
		name = newField.substr((string::size_type) pos,
				(string::size_type) string::npos);

		matcher.assign(ARRAY_MATCH_REGEX);
		boost::match_results<std::string::iterator> match;
		//		std::string::const_iterator start = ;
		//		std::string::const_iterator end = ;

		while (boost::regex_search(name.begin(), name.end(), match, matcher))
		{
			nType += std::string(match[0].first, match[0].second);
			name = std::string(name.begin(), match[0].first) +
					std::string(match[0].second, name.end());
		}

		//use this to correct the type and change it back to a string
		nType = TypeSignature(nType);
	}

	//this expects a name, so add a fake one and remove it afterwards
	//TODO get rid of this
	//nType += " x";
	modifiers = removeModifiers(nType);
	//nType = nType.substr(0, nType.length() - 2);

	//clean out any extra spaces in there
	matcher.assign(CLEAN_SPACES);
	name = StringUtil::trim(boost::regex_replace(name, matcher,
			CLEAN_SPACES_REPLACE, boost::match_default | boost::format_all));
	nType = StringUtil::trim(boost::regex_replace(nType, matcher,
			CLEAN_SPACES_REPLACE, boost::match_default | boost::format_all));

	type = TypeSignature(nType);
}


/**
 * Creates a field signature from reflected data
 *
 * @param field Field to get signature of
 */
FieldSignature::FieldSignature(const cpgf::GMetaField *field) : type("")
{
	type = TypeSignature(field->getItemType());
//	CPGF_TRACE("MetaField Before: " << type.getSignature() << "\r\n");
	std::string tempType = type.getSignature();
//	CPGF_TRACE(" MetaField After: " << tempType << "\r\n");
	modifiers = removeModifiers(tempType);
//	CPGF_TRACE(" MetaField No Mods: " << tempType << "\r\n");
	type = TypeSignature(tempType);
	name = field->getName();
	modifiers |= field->getModifiers();
	signature = type.getSignature() + " " + name;
}
/**
 * Creates a field signature from a type and name. The type is normalized
 * (somewhat) so that it is consistent with other signatures.
 *
 * @param fieldType Type of the field (i.e. const int)
 * @param name Name of the field
 * @param modifiers Modifiers for this field
 */
void FieldSignature::createField(const std::string & fieldType,
		const std::string & nName, int mods) {
	type = TypeSignature(fieldType);
//	CPGF_TRACE("String Field Before: " << type.getSignature() << "\r\n");
	std::string tempType = type.getSignature();
//	CPGF_TRACE("String Field After: " << type.getSignature() << "\r\n");
	modifiers = removeModifiers(tempType);
	type = TypeSignature(tempType);
	name = nName;
	modifiers |= mods;
	signature = type.getSignature() + " " + name;
}


/**
 * Creates a field signature from field declaration. The type is normalized
 * (somewhat) so that it is consistent with other signatures.
 *
 * @param field Field declaration (i.e. const int x)
 * @param name Name of the field
 */
void FieldSignature::createField(const std::string & field) {
	divideSignature(field);
//	CPGF_TRACE("create Field Before: " << field << "\r\n");
	signature = type.getSignature() + " " + name;
//	CPGF_TRACE("create Field After: " << field << "\r\n");
}




void MethodSignature::findParams(const std::string & signature, int &openParen,
		int &closeParen) {
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


std::vector<TypeSignature> MethodSignature::getParams(
		const std::string & signature, int openParen, int closeParen) {
	std::string sig = signature;
	std::vector<TypeSignature> params;
	std::vector<std::string> strParams;

	if (openParen != -1 && closeParen != -1)
	{
		sig = signature.substr((string::size_type) openParen,
				(string::size_type) closeParen - openParen + 1);
	}

	boost::split(strParams, sig, boost::is_any_of(","));

	for (size_t i = 0; i < strParams.size(); i++)
	{
		params.push_back(TypeSignature(strParams[i]));
	}

	return params;
}


/**
 * Creates a string based on the contents of the vector separated by the
 * delimiter. Simple version of a join function.
 *
 * @param v List of strings to join
 * @param delim String to put between the items in the list
 * @return The list in string form separated by delim.
 */
std::string MethodSignature::buildStr(std::vector<TypeSignature> v,
		const std::string & delim)
{
	std::string resultStr;
	for (size_t i = 0; i < v.size(); i++)
	{
		if (i != 0)
		{
			resultStr += delim;
		}
		resultStr += v[i].getSignature();
	}

	return resultStr;
}



/**
 * Determines, based on the signatures, the modifiers for this function/
 * method.
 *
 */
int MethodSignature::removeModifiers(std::string & sig) {
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
 */
void MethodSignature::divideSignature(const std::string & sig) {
	int openParen;
	int closeParen;
	boost::regex matcher;
	std::string newSig;
	std::string afterParamStr;

	newSig = sig;

	matcher.assign(SIGNATURE_CLEAN_REGEX);
	newSig = boost::regex_replace(newSig, matcher, SIGNATURE_CLEAN_REPLACE,
			boost::match_default | boost::format_all);

	findParams(newSig, openParen, closeParen);

	modifiers =removeModifiers(newSig);

	//get the name and return value
	returnType = TypeSignature("");
	name = StringUtil::trim(newSig.substr((string::size_type) 0,
			(string::size_type) openParen));
	size_t pos;
	//find the name
	pos = name.rfind(" ");

	if (pos != string::npos)
	{
		returnType = TypeSignature(name.substr((string::size_type) 0,
				(string::size_type) pos));
		name = name.substr((string::size_type) pos + 1, string::npos);
	}
	//get the params
	params = getParams(newSig, openParen + 1, closeParen - 1);
}


void MethodSignature::createConstructor(const std::string & nName,
		const cpgf::GMetaConstructor * construct) {
	returnType = TypeSignature("");
	name = nName;
	modifiers = construct->getModifiers();
	signature = "";
	TypeSignature param("");

	signature += name + "(";

	for (size_t i = 0; i < construct->getParamCount(); i++)
	{
		param = TypeSignature(construct->getParamType(i));
		params.push_back(param);
		signature += param.getSignature();
		if (i != construct->getParamCount() - 1)
		{
			signature += ", ";
		}
		else
		{
			//TODO not supported
			if (construct->isVariadic())
			{
				signature += "...";
			}
		}
	}

	signature += ")";
}

/**
 * Creates a function/method signature from reflected data
 *
 * @param method Function/method to get signature of
 */
MethodSignature::MethodSignature(const cpgf::GMetaMethod *method) : returnType("") {

	name = method->getName();
	modifiers = method->getModifiers();
	signature = "";

	if (method->hasResult())
	{
		returnType = TypeSignature(method->getResultType());
		signature += returnType.getSignature() + " ";
	}
	else
	{
		//		CPGF_TRACE(method->getName() << " " << !isDestructor(method->getName()) << "\r\n")
		if (isDestructor()) {
			returnType = TypeSignature("");
		}
		else {
			//			sig = "void ";
			returnType = TypeSignature("void");
			signature += returnType.getSignature() + " ";
		}
	}

	signature += name + "(";

	TypeSignature pType("");

	for (size_t i = 0; i < method->getParamCount(); i++)
	{
		pType = TypeSignature(method->getParamType(i));
		if (pType.getSignature() != "") {
			if (i != 0) {
				signature += ", ";
			}
			signature += pType.getSignature();
			params.push_back(pType);
			//TODO not supported
			if (i == method->getParamCount() - 1 && method->isVariadic()) {
				signature += "...";
			}
		}
	}

	signature += ")";

	if ((modifiers & cpgf::metaModifierConst) != 0)
	{
		signature += " const";
	}
}


/**
 * Creates a function/method signature from a return type and name
 * and list of parameter types. The type is normalized
 * (somewhat) so that it is consistent with other signatures.
 *
 * @param returnType Return type of the function/method (i.e. const int)
 * @param name Name of the function/method
 * @param params List of parameter types
 * @param modifiers Modifiers for this method
 */
void MethodSignature::createMethod(const std::string & rType,
		const std::string & nName,
		const std::vector<std::string> & paramStrs, int mods) {
	returnType = TypeSignature(rType);
	modifiers = mods;
	name = nName;

	if (rType == "") {
		signature = name + "(";
	}
	else {
		signature = returnType.getSignature() + " " + name + "(";
	}

	for (std::vector<TypeSignature>::const_iterator i = params.begin();
			i != params.end(); i++)
	{
		if (i != params.begin())
		{
			signature += ", ";
		}
		signature += *i;
		params.push_back(TypeSignature(*i));
	}

	signature += ")";

	if ((modifiers & cpgf::metaModifierConst) != 0)
	{
		signature += " const";
	}
}

//constructors
/**
 * Creates a constructor signature from reflected data
 *
 * @param mClass Class that contains the constructor
 * @param construct Constructor to get signature of
 */
MethodSignature::MethodSignature(const cpgf::GMetaClass *mClass,
		const cpgf::GMetaConstructor *construct) : returnType("") {
	createConstructor(mClass->getName(), construct);
}

/**
 * Creates a constructor signature from reflected data
 *
 * @param nName Name of class that contains the constructor
 * @param construct Constructor to get signature of
 */
MethodSignature::MethodSignature(const std::string & nName,
		const cpgf::GMetaConstructor * construct) : returnType("") {
	createConstructor(nName, construct);
}

/**
 * Creates a constructor signature from a name
 * and list of parameter types. The type is normalized
 * (somewhat) so that it is consistent with other signatures.
 *
 * @param name Name of the function/method
 * @param params List of parameter types
 */
void MethodSignature::createConstructor(const std::string & nName,
		const std::vector<std::string> & paramStrs, int mods) {
	returnType = TypeSignature("");
	modifiers = mods;
	name = nName;

	signature = name + "(";

	for (std::vector<std::string>::const_iterator i = paramStrs.begin();
			i != paramStrs.end(); i++)
	{
		if (i != paramStrs.begin())
		{
			signature += ", ";
		}
		signature += *i;
		params.push_back(TypeSignature(*i));
	}

	signature += ")";
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
 * @param signature functions/methods/operators/constructors signature
 *
 * TODO move to a more robust comparison scheme using clang
 */
void MethodSignature::createMethod(const std::string & sig) {
	boost::regex matcher;
	std::string newSig;
	std::string operatorName;

	newSig = sig;
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

	divideSignature(newSig);

	if (returnType.getSignature() == "" || isDestructor())
	{
		newSig = name + "(";
	}
	else
	{
		newSig = returnType.getSignature() + " " + name + "(";
	}

	newSig += buildStr(params, ", ") + ")";

	//add the const to the signature if needed
	if ((modifiers & cpgf::metaModifierConst) != 0) {
		newSig += " const";
	}

	matcher.assign(CLEAN_SPACES);
	newSig = boost::regex_replace(newSig, matcher, CLEAN_SPACES_REPLACE,
			boost::match_default | boost::format_all);

	//restore name
	if (operatorMatch) {
		newSig.replace(newSig.find(FAKE_OPERATOR_NAME),
				FAKE_OPERATOR_NAME.length(), operatorName);

	}

	signature = newSig;
}


/**
 * Determines if the operator is one of the postfix operators
 * (x++ or x--)
 *
 * @return true if it is the signature of an postfix operator
 */
bool MethodSignature::isPostFixOperator() const {
	boost::regex matcher;
	matcher.assign(POSTFIX_OPERATOR_PATTERN);
	return boost::regex_match(signature, matcher);
}

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
bool MethodSignature::isArraySetterSignature() const {
	boost::regex matcher;
	matcher.assign(ARRAY_WRAPPER_PATTERN);
	return boost::regex_match(signature, matcher);
}

/**
 * Determines if this signature matches the operator[] method and that it
 * does not return a const result.
 *
 * @return true if it is the signature of an array setter method
 */
bool MethodSignature::isSettableArrayOperator() const {
	boost::regex nonConstMatcher(ARRAY_OPERATOR_PATTERN);
	boost::regex constMatcher(ARRAY_OPERATOR_CONST_PATTERN);
	std::string cSig = signature;
	return boost::regex_match(cSig, nonConstMatcher)
	&& !boost::regex_match(cSig, constMatcher);
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
 * @return true if it is the signature of an array setter method
 */
bool MethodSignature::isCopyFunctionSignature() const {
	boost::regex matcher;
	matcher.assign(COPY_WRAPPER_PATTERN);
	return boost::regex_match(signature, matcher);
}


/**
 * Determines if the name of this method refers to a destructor
 *
 * @return true if it is a destructor
 */
bool MethodSignature::isDestructor() const {
	return (name != "" && name[0] == '~');
}


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
MethodSignature MethodSignature::createArraySetterSignature(
		const std::string & className) const {

	if (!isSettableArrayOperator()) {
		return MethodSignature("");
	}

	std::string cleanedName = signature;
	std::string name = ARRAY_WRAPPER_PREFIX + className + "_" +
			signature;
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

	return MethodSignature(newSig);
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
 * @param sig Signature of the function/method
 * @return modifiers for this function
 */
int ClassSignature::removeModifiers(std::string sig) {
	int mods = cpgf::metaModifierNone;
	//virtual
	if (findModifier("virtual", sig)) {
		mods |= cpgf::metaModifierVirtual;
	}

	//TODO template - not handled yet
	return mods;
}

/**
 * Divides a class into name, enclosing class, and modifiers.
 *
 * @param sig Signature of the class
 */
void ClassSignature::divideSignature(const std::string & sig) {
	modifiers = removeModifiers(sig);
	enclosingClass = "";
	name = sig;
	size_t pos = name.rfind("::");
	if (pos != std::string::npos) {
		enclosingClass = name.substr(0, pos);
		name = name.substr(pos + 2, string::npos);
	}
	if (enclosingClass == "") {
		signature = name;
	}
	else {
		signature = enclosingClass + "::" + name;
	}
}



/**
 * Corrects the formatting of
 * (inner) classes so they are in
 * a standard format and can be compared.
 *
 * @param sig class signature
 *
 * TODO move to a more robust comparison scheme using clang
 */
void ClassSignature::createClass(const std::string & sig) {
	divideSignature(sig);
}

/**
 * Corrects the formatting of
 * (inner) classes so they are in
 * a standard format and can be compared.
 *
 * @param signature class signature
 *
 */
ClassSignature::ClassSignature(const cpgf::GMetaClass * cClass) {
	std::string tName = cClass->getName();

	const cpgf::GMetaClass * parent = static_cast<const cpgf::GMetaClass *>(
			cClass->getOwnerItem());

	modifiers = removeModifiers(tName);
	name = tName;
	if (parent != NULL && parent->getName() != ""
			&& parent->getName() != "cpptest") {
		enclosingClass = parent->getName();
		signature =  enclosingClass + "::" + name;

	}
	else {
		signature = name;
	}
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
 * @return signature for the copy function
 */
MethodSignature ClassSignature::createCopyFunctionSignature() const {
	return MethodSignature(name + " * " + COPY_WRAPPER_PREFIX + name
			+ "(" + name + " *)");
}


/**
 * Divides an enum into a name and a list of enums and their values
 *
 * @param sig Signature of the class
 */
void EnumSignature::divideSignature(const std::string & sig) {

	boost::regex matcher;
	std::string newSig;
	matcher.assign(SIGNATURE_CLEAN_ENUM_REGEX);
	newSig = boost::regex_replace(sig, matcher, SIGNATURE_CLEAN_ENUM_REPLACE,
			boost::match_default | boost::format_all);

	size_t openPos = newSig.find("{", 0);

	if (openPos != std::string::npos) {
		name = StringUtil::trim(newSig.substr(0, openPos - 1));

		size_t closePos = newSig.find("}", 0);
		std::string eValues;
		if (openPos != std::string::npos) {
			eValues = StringUtil::trim(newSig.substr(openPos + 1, closePos - 1 - openPos));

			std::vector<std::string> tokens;
			std::vector<std::string> nameValue;
			std::string name;
			std::string token;
			int value;

			Tokenizer::tokenize(eValues, tokens, "([^,]+)");
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
					names[name] = value;
					values[value] = name;
				}

			}
		}
	}
	else {
		name = StringUtil::trim(newSig);
	}

	modifiers = cpgf::metaModifierNone;
}


/**
 * Corrects the formatting of enums so they are in
 * a standard format and can be compared.
 *
 * @param enumStr enum signature
 *
 * TODO move to a more robust comparison scheme using clang
 */
void EnumSignature::createEnum(const std::string & enumStr) {
	divideSignature(enumStr);
	signature = enumStr;
}

/**
 * Corrects the formatting of enums so they are in
 * a standard format and can be compared.
 *
 * @param eEnum reflected enum
 *
 */
EnumSignature::EnumSignature(const cpgf::GMetaEnum * eEnum) {
	name = eEnum->getName();
	modifiers = cpgf::metaModifierNone;
	signature = name + "{";

	int value;
	std::string key;
	std::string valueStr;

	for (size_t i = 0; i < eEnum->getCount(); i++) {
		if (i != 0) {
			signature += ", ";
		}

		key = eEnum->getKey(i);
		value = cpgf::fromVariant<int>(eEnum->getValue(i));
		names[key] = value;
		values[value] = key;
		std::stringstream convert;
		convert << value;
		convert >> valueStr;
		signature += key + " = " + valueStr;
	}

	signature += "}";
}







}

