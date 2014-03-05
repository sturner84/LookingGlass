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

ReflectionUtil ReflectionUtil::instance;

ReflectionUtil::ReflectionUtil()
{
	loadOperatorNames();
}


void ReflectionUtil::loadOperatorNames()
{
	operatorNameToSym["_opAdd"] = "operator+";
	operatorNameToSym["_opSub"] = "operator-";
	operatorNameToSym["_opMul"] = "operator*";
	operatorNameToSym["_opDiv"] = "operator/";
	operatorNameToSym["_opMod"] = "operator%";
	operatorNameToSym["_opPlus"] = "operator+";
	operatorNameToSym["_opMinus"] = "operator-";
	operatorNameToSym["_opAddAssign"] = "operator+=";
	operatorNameToSym["_opSubAssign"] = "operator-=";
	operatorNameToSym["_opMulAssign"] = "operator*=";
	operatorNameToSym["_opDivAssign"] = "operator/=";
	operatorNameToSym["_opModAssign"] = "operator%=";
	operatorNameToSym["_opAssign"] = "operator=";
	operatorNameToSym["_opEqual"] = "operator==";
	operatorNameToSym["_opNotEqual"] = "operator!=";
	operatorNameToSym["_opGreater"] = "operator>";
	operatorNameToSym["_opLess"] = "operator<";
	operatorNameToSym["_opGreaterEqual"] = "operator>=";
	operatorNameToSym["_opLessEqual"] = "operator<=";
	operatorNameToSym["_opAnd"] = "operator&&";
	operatorNameToSym["_opOr"] = "operator||";
	operatorNameToSym["_opNot"] = "operator!";
	operatorNameToSym["_opBitAnd"] = "operator&";
	operatorNameToSym["_opBitOr"] = "operator|";
	operatorNameToSym["_opBitXor"] = "operator^";
	operatorNameToSym["_opLeftShift"] = "operator<<";
	operatorNameToSym["_opRightShift"] = "operator>>";
	operatorNameToSym["_opBitNot"] = "operator~";
	operatorNameToSym["_opBitAndAssign"] = "operator&=";
	operatorNameToSym["_opBitOrAssign"] = "operator|=";
	operatorNameToSym["_opBitXorAssign"] = "operator^=";
	operatorNameToSym["_opLeftShiftAssign"] = "operator<<=";
	operatorNameToSym["_opRightShiftAssign"] = "operator>>=";
	operatorNameToSym["_opInc"] = "++operator"; //TODO note about operators
	operatorNameToSym["_opDec"] = "--operator";
	operatorNameToSym["_opIncSuffix"] = "operator++";
	operatorNameToSym["_opDecSuffix"] = "operator--";
	operatorNameToSym["_opComma"] = "operator,";
	operatorNameToSym["_opArrayGet"] = "operator[]Get";//TODO note about operators
	operatorNameToSym["_opArraySet"] = "operator[]Set";
	operatorNameToSym["_opFunction"] = "operator()";
	operatorNameToSym["_opAddress"] = "operator&";
	operatorNameToSym["_opDerefer"] = "operator*";

	operatorSymToName["operator+"] = "_opAdd";
	operatorSymToName["operator-"] = "_opSub";
	operatorSymToName["operator*"] = "_opMul";
	operatorSymToName["operator/"] = "_opDiv";
	operatorSymToName["operator%"] = "_opMod";
	operatorSymToName["operator+"] = "_opPlus";
	operatorSymToName["operator-"] = "_opMinus";
	operatorSymToName["operator+="] = "_opAddAssign";
	operatorSymToName["operator-="] = "_opSubAssign";
	operatorSymToName["operator*="] = "_opMulAssign";
	operatorSymToName["operator/="] = "_opDivAssign";
	operatorSymToName["operator%="] = "_opModAssign";
	operatorSymToName["operator="] = "_opAssign";
	operatorSymToName["operator=="] = "_opEqual";
	operatorSymToName["operator!="] = "_opNotEqual";
	operatorSymToName["operator>"] = "_opGreater";
	operatorSymToName["operator<"] = "_opLess";
	operatorSymToName["operator>="] = "_opGreaterEqual";
	operatorSymToName["operator<="] = "_opLessEqual";
	operatorSymToName["operator&&"] = "_opAnd";
	operatorSymToName["operator||"] = "_opOr";
	operatorSymToName["operator!"] = "_opNot";
	operatorSymToName["operator&"] = "_opBitAnd";
	operatorSymToName["operator|"] = "_opBitOr";
	operatorSymToName["operator^"] = "_opBitXor";
	operatorSymToName["operator<<"] = "_opLeftShift";
	operatorSymToName["operator>>"] = "_opRightShift";
	operatorSymToName["operator~"] = "_opBitNot";
	operatorSymToName["operator&="] = "_opBitAndAssign";
	operatorSymToName["operator|="] = "_opBitOrAssign";
	operatorSymToName["operator^="] = "_opBitXorAssign";
	operatorSymToName["operator<<="] = "_opLeftShiftAssign";
	operatorSymToName["operator>>="] = "_opRightShiftAssign";
	operatorSymToName["++operator"] = "_opInc"; //TODO note about operator
	operatorSymToName["--operator"] = "_opDec";
	operatorSymToName["operator++"] = "_opIncSuffix";
	operatorSymToName["operator--"] = "_opDecSuffix";
	operatorSymToName["operator,"] = "_opComma";
	operatorSymToName["operator[]Get"] = "_opArrayGet"; //TODO note about operators
	operatorSymToName["operator[]Set"] = "_opArraySet";
	operatorSymToName["operator()"] = "_opFunction";
	operatorSymToName["operator&"] = "_opAddress";
	operatorSymToName["operator*"] = "_opDerefer";


}



std::string ReflectionUtil::getOperatorSymbol(std::string opName)
{
	return instance.operatorNameToSym[opName];
}
std::string ReflectionUtil::getOperatorName(std::string opSymbol)
{
	return instance.operatorSymToName[opSymbol];
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
		sig = "void ";
	}


//	if (mClass->getName() != "")
//	{
//		sig += mClass->getQualifiedName() + "::";
//	}

	std::string opName = ReflectionUtil::getOperatorSymbol(method->getName());

	if (opName == "")
	{
		sig += method->getName() + "(";
	}
	else
	{
		sig += opName + "(";
	}

	for (size_t i = 0; i < method->getParamCount(); i++)
	{
		sig += getType(method->getParamType(i));
		if (i != method->getParamCount() - 1)
		{
			sig += ", ";
		}
		else
		{
			if (method->isVariadic())
			{
				sig += "...";
			}
		}
	}

	sig += ")";
//TODO test this
	if (method->getItemType().isConst())
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


std::string ReflectionUtil::createOperatorSignature(const cpgf::GMetaOperator *op)
{
	std::string sig;

	if (op->hasResult())
	{
		sig += getType(op->getResultType());
		sig += " ";
	}
	else
	{
		sig = "void ";
	}
//	if (mClass->getName() != "")
//	{
//		sig += mClass->getName() + "::";
//	}
	sig += op->getName() + "(";

	for (size_t i = 0; i < op->getParamCount(); i++)
	{
		sig += getType(op->getParamType(i));
		if (i != op->getParamCount() - 1)
		{
			sig += ", ";
		}
	}

	sig += ")";

	return sig;
}

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
	for (size_t i = 0; i < v.size(); )
	{
		if (v[i] == "*" || v[i] == "&")
		{
			i++;
			cPos = vPos = -1;
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

	}

	return v;
}

std::string ReflectionUtil::correctField(std::string field)
{

	std::string resultStr;

	boost::regex matcher;
	std::string newField;

	matcher.assign(TYPE_CLEAN_REGEX_NO_ARRAY);
	newField = StringUtil::trim(boost::regex_replace(field, matcher,
			TYPE_CLEAN_REPLACE_NO_ARRAY,
			boost::match_default | boost::format_all));

	//assumes it has a name
	size_t pos = newField.rfind(" ");

	if (pos != string::npos)
	{
		std::string type = newField.substr((string::size_type) 0,
				(string::size_type) pos);
		std::string name = newField.substr((string::size_type) pos,
				(string::size_type) string::npos);

		matcher.assign(ARRAY_MATCH_REGEX);
		boost::match_results<std::string::iterator> match;
//		std::string::const_iterator start = ;
//		std::string::const_iterator end = ;

		while (boost::regex_search(name.begin(), name.end(), match, matcher))
		{
			type += std::string(match[0].first, match[0].second);
			name = std::string(name.begin(), match[0].first) +
					std::string(match[0].second, name.end());
		}

		newField = correctType(type) + " " + name;
	}
	//clean out any extra spaces in there
	//put in other correct* methods - done?
	matcher.assign(CLEAN_SPACES);
	newField = boost::regex_replace(newField, matcher, CLEAN_SPACES_REPLACE,
				boost::match_default | boost::format_all);

	return newField;
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

std::string ReflectionUtil::correctEnum(std::string enumStr)
{
	boost::regex matcher;
	std::string newEnum;
	matcher.assign(SIGNATURE_CLEAN_ENUM_REGEX);
	newEnum = boost::regex_replace(enumStr, matcher, SIGNATURE_CLEAN_ENUM_REPLACE,
			boost::match_default | boost::format_all);

	return newEnum;
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
	int openParen;
	int closeParen;
	boost::regex matcher;
	std::string newSig;
	std::string returnStr;
	std::string nameStr;
	std::string afterParamStr;

	matcher.assign(SIGNATURE_CLEAN_REGEX);
	newSig = boost::regex_replace(signature, matcher, SIGNATURE_CLEAN_REPLACE,
			boost::match_default | boost::format_all);

	findParams(newSig, openParen, closeParen);
	//get anything after the parameters (i.e. const)
	afterParamStr = StringUtil::trim(newSig.substr(
			(string::size_type) closeParen + 1,
			(string::size_type) string::npos));
	//get the name and return value
	nameStr = StringUtil::trim(newSig.substr((string::size_type) 0,
			(string::size_type) openParen));
	size_t pos;
	//find the name
	for (pos = nameStr.length() - 1; pos != string::npos && nameStr[pos] != ' '; pos--)
	{
		//do nothing
	}
	if (pos != string::npos)
	{
		returnStr = correctType(nameStr.substr((string::size_type) 0,
				(string::size_type) pos));
		nameStr = nameStr.substr((string::size_type) pos + 1, string::npos);
	}
	//get the params
	std::vector<std::string> params = getParams(newSig, openParen + 1,
			closeParen - 1);

	if (returnStr == "")
	{
		newSig = nameStr + "(";
	}
	else
	{
		newSig = returnStr + " " + nameStr + "(";
	}


//	for (size_t i = 0; i < params.size(); i++)
//	{
//		if (i != 0)
//		{
//			newSig += ", ";
//		}
//		newSig += params[i];
//	}

	newSig += buildStr(params, ", ") + ")";
	if (afterParamStr != "")
	{
		newSig += " " + afterParamStr;
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

	return newSig;
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
	//TODO handle operator names?
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

	if (isConst)
	{
		sig += " const";
	}

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


std::string ReflectionUtil::createOperatorSignature(std::string returnType, std::string name,
		const std::vector<std::string> & params)
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

		return correctSignature(sig);
}

}
