/**
 * @file CppTestingSupport.cpp
 *
 * @brief Contains classes that support testing of c++ code.
 * 
 * The classes in this header allow input/output to be redirected and for
 * the output to be more easily compared.  The Tokenizer class simplifies 
 * converting the output into string, int, or double tokens.
 * 
 * @attention This needs to be linked with the Boost library 
 *  Linux/Unix: boost_regex-gcc-mt-s
 * 
 * @author saturner
 * @date 2013-05-25
 */
#include "lgTestingSupport.h"
#include <cmath>
#include <algorithm>

namespace cpptesting {


CompareOutput::CompareOutput(string expected, string result, 
    string pattern)
{
	location = 0;
	lastExpectedToken = "";
	lastResultToken = "";
	comparedSize = 0;

    vector<string> expVec;
    vector<string> reVec;

    Tokenizer::tokenize(expected, expVec, pattern);
    Tokenizer::tokenize(result, reVec, pattern);
    
    for (location = 0; location < expVec.size() && location < reVec.size(); 
        location++) 
    {
        if (expVec[location] != reVec[location])
        {
            lastExpectedToken = expVec[location];
            lastResultToken = reVec[location];
            success = false;
            return;
        }
        
    }

	if (expVec.size() != reVec.size()) 
    {
        if (expVec.size() > reVec.size())
        {
            comparedSize = -1;
            lastExpectedToken = expVec[location];
            lastResultToken = "";        
        }   
        else 
        {
            comparedSize = 1;
            lastExpectedToken = "";
            lastResultToken = reVec[location];        
        }
        location++;
        success = false;
        return;
    }

	
    success = true;
}


string CompareOutput::getStdMessage()
{
	stringstream message;

	if (wasSuccessful())
	{
		message << "The output is the same.";
	}
	else
	{
		if (areEqualLength())
		{
            DiffStrings ds(getLastResultToken(), getLastExpectedToken());
			message << "The output did not match.  On word " <<
			getLocationOfMismatch() + 1 << ", found \"" <<
			ds.getAString() << "\" when \"" <<
			ds.getBString() << "\" was expected.";
		}

		if (isExpectedLonger())
		{
			message << "The output was shorter than expected.  Expected \"" <<
			getLastExpectedToken() << "\" as word " <<
			getLocationOfMismatch() << " in the output.";
		}
		if (isResultLonger())
		{
			message << "The output was longer than expected.  Found \"" <<
			getLastResultToken() << "\" as word " <<
			getLocationOfMismatch() << ".";
		}
	}

	return message.str();
}






StdIORedirector::StdIORedirector()
{
	oldOutputStream = NULL;
	oldInputStream = NULL;
	outputRedirect = inputRedirect = false;
}

StdIORedirector::~StdIORedirector()
{
	restoreCout();
	restoreCin();
}

void StdIORedirector::redirectCout() {

	if (!outputRedirect)
	{
		oldOutputStream = cout.rdbuf();
		//redirect
		resetOutput();
		cout.rdbuf(outputStream.rdbuf());
		outputRedirect = true;
	}
}

void StdIORedirector::resetOutput()
{
	cout.clear();
	outputStream.str("");
	outputStream.clear();
}

void StdIORedirector::restoreCout()
{
	if (outputRedirect)
	{
		cout.rdbuf(oldOutputStream);
		outputRedirect = false;
	}
}


bool StdIORedirector::wasCoutUsed()
{
	return (outputStream.tellp() > 0);
}


void StdIORedirector::redirectCin()
{
	if (!inputRedirect)
	{
		inputRedirect = true;
		oldInputStream = cin.rdbuf();

		//redirect
		setInput("");
		cin.rdbuf(inputStream.rdbuf());
	}
}



void StdIORedirector::restoreCin()
{
	if (inputRedirect)
	{
		cin.rdbuf(oldInputStream);
		inputRedirect = false;
	}
}


void StdIORedirector::setInput(string data)
{
	if (!inputRedirect)
	{
		redirectCin();
	}
	cin.clear(); //clears the state of cin
				 //which is separate from inputStream
	inputStream.clear();
	inputStream.seekg(0);
	inputStream.str(data);
}

bool StdIORedirector::wasCinUsed()
{
	//need to check both for failure
	//cin could be set and inputStream not be
	return ((inputStream.tellg() > 0) || (inputStream.fail())
			|| (cin.fail()));
}

//This matches anything not a space.
const string Tokenizer::MATCH("(\\S+)");

//specifies the symbols allowed to come before a number if looking for a 
// separate token.
// i.e. $, #, +, etc.
const string Tokenizer::NUM_PREFIX("[\\[{\\s$#+(*:;/,\\\\]|\\A");
//specifies the symbols allowed to come after a number if looking for a 
// separate token.
// i.e. ,, !, ., etc.
const string Tokenizer::NUM_POSTFIX("(?=(?=[\\]}\\s!?%#/+*,\\-:;\\)\\\\]|\\Z)|(?:\\.(?:\\s|\\Z)))");
//"(?=([\\]}\\s!?%#/+*-:;,\\\\]|\\Z|\\))|(?:\\.(?:\\s|\\Z)))");
//This matches integers that may be embedded in other text.
// i.e. this should match "5", "6,", ":7", "a9b", etc.  
//The expressions must start with with something that is not a ., a number,
// or a - sign (or the beginning of the expression)
//The value captured is simpily an optional - sign and one or more digits
//The expression must not end with a decimal followed by a number.  It can 
// end with a decimal followed by something not a number (i.e. a period at the
// end of a sentence).
const string Tokenizer::INT_MATCH("(?:[^.\\d-]|\\A)(-?\\d+)"
    "(?=(?:[^.\\d]|\\Z)|(?:\\.(?:\\D|\\Z)))");
 
const string Tokenizer::INT_MATCH_SEPARATE_BASE("(?:%s)(-?\\d+)(?:%s)");
const string Tokenizer::INT_MATCH_SEPARATE(createMatchString(NUM_PREFIX, 
    INT_MATCH_SEPARATE_BASE, NUM_POSTFIX));

const string Tokenizer::FLOAT_MATCH("(?:(?:\\A|\\$)|(?:[^-\\d\\.]))+"
    "(-?(?:(?:\\d+(\\.\\d)?\\d*)|(?:(\\.\\d)+\\d*)))");
const string Tokenizer::FLOAT_MATCH_SEPARATE_BASE("(?:%s)"
    "(-?(?:(?:\\d+(\\.\\d)?\\d*)|(?:(\\.\\d)?\\d*)))(?:%s)");
const string Tokenizer::FLOAT_MATCH_SEPARATE(createMatchString(NUM_PREFIX, 
    FLOAT_MATCH_SEPARATE_BASE, NUM_POSTFIX));



vector<string>& Tokenizer::tokenize(string data,
            vector<string> &matches, string pattern)
 {
        boost::regex matcher;
        matcher.assign(pattern, boost::regex_constants::icase);

        boost::sregex_token_iterator i(data.begin(), data.end(), matcher, 1);
        boost::sregex_token_iterator j;

        while(i != j)
        {
            matches.push_back(*i);
            i++;
        }

        return matches;
 }


string Tokenizer::createMatchString(string  pre, string  base, 
    string post)
{
    return str(boost::format(base) % pre % post); 
}


bool Tokenizer::findInTokens(string data, double value, double delta,
            string pattern)
{
	//TS_TRACE(ToDoubleVector());
	vector<double> dValues;
    tokenize(data, dValues, pattern);

	return findInTokens(dValues, value, delta);

}

bool Tokenizer::findInTokens(vector<double> dValues, double value, double delta)
{
    for (vector<double>::iterator i = dValues.begin(); i != dValues.end(); ++i)
    {
        if (fabs(*i - value) < delta)
        {
            return true;
        }
    }

    return false;

}

bool Tokenizer::findInTokens(string data, int value,
            string pattern)
{
	//TS_TRACE(ToIntVector());
	vector<int> iValues;
    tokenize(data, iValues, pattern);

	return findInTokens(iValues, value);
}

bool Tokenizer::findInTokens(vector<int> iValues, int value)
{
    for (vector<int>::iterator i = iValues.begin(); i != iValues.end(); ++i)
    {
        if (*i == value)
        {
            return true;
        }
    }

    return false;
}

bool Tokenizer::findInTokens(string data, string value, 
    bool ignoreCase, bool partial, string pattern)
{
    vector<string> sValues;
    tokenize(data, sValues, pattern);

    return findInTokens(sValues, value, ignoreCase, partial);
}

bool Tokenizer::findInTokens(vector<string> sValues, string value, 
    bool ignoreCase, bool partial)
{
	string eValue = value;
	if (ignoreCase)
	{
		eValue = StringUtil::toUpper(eValue);
	}

	for (vector<string>::iterator i = sValues.begin(); i != sValues.end(); ++i)
	{
		string aValue = *i;
		if (ignoreCase)
		{
			aValue = StringUtil::toUpper(aValue);
		}
		//TS_TRACE(aValue + " " + eValue);
		if (partial)
		{
			if (aValue.find(eValue) != string::npos)
			{
				return true;
			}
		}
		else
		{

			if (eValue.compare(aValue) == 0)
			{
				return true;
			}
		}
	}

	return false;
}


string StringUtil::toUpper(string str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		str[i] = toupper((int) str[i]);
	}

	return str;
}


string StringUtil::toLower(string str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		str[i] = tolower((int) str[i]);
	}

	return str;
}

string StringUtil::replaceChars(string str, string removeChars,
		char replacement)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (removeChars.find((char) str[i]) != string::npos)
		{
			str[i] = replacement;
		}
	}

	return str;
}


const size_t DiffStrings::MAX_CONTEXT_LEN = 20;
const string DiffStrings::ELLIPSIS = "...";
const string DiffStrings::DIFF_START = "[";
const string DiffStrings::DIFF_END = "]";

DiffStrings::DiffStrings(string a, string b, size_t maxSize)
{
    aStr = a;
    bStr = b;
    if (aStr != bStr)
    {
        compareStrings(maxSize);
    }
}

void DiffStrings::compareStrings(size_t maxSize)
{
    size_t pre = getPrefix();
    size_t post = getPostfix(aStr.substr(pre), bStr.substr(pre));
//    size_t min = aStr.length() < bStr.length() ? aStr.length() : bStr.length();
//	//check for pre, post overlap
//	if (pre + post > min) {
//		post = pre - min;
//	}

    aStr = getDiffString(aStr, pre, post, maxSize);
    bStr = getDiffString(bStr, pre, post, maxSize);
}

size_t DiffStrings::getPrefix()
{
    size_t pre = 0;
    
    while (pre < aStr.length() && pre < bStr.length() && aStr[pre] == bStr[pre])
    {
        pre++;
    }
    
    return pre;
}

size_t DiffStrings::getPostfix(string a, string b)
{
    size_t post = 0;
    
    while (a.length() > post &&  b.length() > post
        && a[a.length() - 1 - post] == b[b.length() - 1 - post])
    {
        post++;
    }
    
    return post;
}   

string DiffStrings::getDiffString(const string& str, size_t pre, size_t post, 
    size_t maxSize)
{
    return addEllipsis(str.substr((string::size_type) 0,
    		(string::size_type) pre), maxSize, true) + DIFF_START
        + str.substr((string::size_type) pre, (
        		string::size_type) str.length() - pre - post) + DIFF_END
        + addEllipsis(str.substr(str.length() - post, string::npos), maxSize,
             false);
}

string DiffStrings::addEllipsis(const string& str, size_t maxSize, 
            bool atBegin)
{
    string nStr = str;
    
    if (nStr.length() > maxSize) // too big
    {
        if (atBegin) 
        {
            nStr = ELLIPSIS + nStr.substr((string::size_type) nStr.length() - maxSize,
            		(string::size_type) maxSize);
        }
        else
        {
            nStr = nStr.substr((string::size_type) 0,
            		(string::size_type) maxSize) + ELLIPSIS;
        }
    }
    
    return nStr;
} 


//int* StringDistance::distanceTable;
//std::map<char, int> StringDistance::letters;
//int StringDistance::size;

const int StringDistance::INIT_SIZE = 50;

StringDistance StringDistance::strDistance;

StringDistance::StringDistance() {
	// do nothing
	size = 0;
	distanceTable = NULL;
}

StringDistance::~StringDistance() {
	delete[] distanceTable;
}

void StringDistance::updateData(int length) {

	const int newSize = std::max(length + 2, INIT_SIZE);

	if (!distanceTable) {
		distanceTable = new int[newSize * newSize];
		size = newSize;
	}
	else {
		if (newSize > size) {
			delete[] distanceTable;
			distanceTable = new int[newSize * newSize];
			size = newSize;
		}
	}
	letters.clear();
}

void StringDistance::initializeTable(int lenght1, int length2) {

	const int MAX_DIFF = lenght1 + length2;

	distanceTable[0] = MAX_DIFF;
	// initialize the first rows and columns
	for (int i = 0; i <= lenght1; i++) {
		distanceTable[(i + 1) * size] = MAX_DIFF;
		distanceTable[(i + 1) * size + 1] = i;
	}

	for (int k = 0; k <= length2; k++) {
		distanceTable[k + 1] = MAX_DIFF;
		distanceTable[size + k + 1] = k;
	}
}

void StringDistance::initializeMap(std::string str1, std::string str2) {
	for (size_t i = 0; i < str1.length(); i++) {
		letters[str1[i]] = 0;
	}
	for (size_t i = 0; i < str2.length(); i++) {
		letters[str2[i]] = 0;
	}
}

int StringDistance::computeDistance(std::string str1, std::string str2) {

	if (str1 == "") {
		if (str2 == "") {
			return 0;
		}

		return str2.length();
	}

	if (str2 == "") {
		return str1.length();
	}

	int str1Len = str1.length();
	int str2Len = str2.length();
	int currentRun;
	int savedI;
	int savedK;

	updateData(std::max(str1Len, str2Len));
	initializeTable(str1Len, str2Len);
	initializeMap(str1, str2);

	for (int i = 1; i <= str1Len; i++) {

		currentRun = 0;

		for (int k = 1; k <= str2Len; k++) {

			savedI = letters[str2[k - 1]];
			savedK = currentRun;

			if (str1[i - 1] == str2[k - 1]) {
				distanceTable[(i + 1) * size + k + 1] =
						distanceTable[(i) * size + k];
				currentRun = k;
			} else {
				distanceTable[(i + 1) * size + k + 1] =
						std::min(distanceTable[(i) * size + k],
						std::min(distanceTable[(i + 1) * size + k],
								distanceTable[(i) * size + k + 1])) + 1;
			}

			distanceTable[(i + 1) * size + k + 1] = std::min(
					distanceTable[(i + 1) * size + k + 1],
					distanceTable[(savedI) * size + savedK] + (i - savedI -1)
						+ 1 + (k - savedK - 1));
		}
//		std::cout << letters['A'] << " " << letters['B'] << " " << letters['C'] << "\n";
		letters[str1[i - 1]] = i;
//		std::cout << letters['A'] << " " << letters['B'] << " " << letters['C'] << "\n";
//		std::cout.flush();
	}

//	std::cout.width(5);
//	std::cout << " ";
//	for (int j = 0; j < str2Len + 1; j++)
//	{
//		std::cout.width(3);
//		std::cout << j << " ";
//	}
//	std::cout << "\n";
//	for (int i = 0; i <= str1Len + 1; i++)
//	{
//		std::cout.width(3);
//		std::cout << i << ": ";
//		for (int j = 0; j <= str2Len + 1; j++)
//		{
//			std::cout.width(3);
//			std::cout << distanceTable[i * size + j] << " ";
//		}
//		std::cout << "\n";
//	}

	return distanceTable[(str1Len + 1) * size + str2Len + 1];
}

size_t StringDistance::getDistance(std::string str1, std::string str2) {
	return strDistance.computeDistance(str1, str2);
}


std::vector<std::string> StringDistance::getNClosest(std::string str,
		const std::vector<std::string> & values, size_t n)
{
	std::map<int, std::vector<std::string> > distanceMap;
	std::vector<std::string> results;
	int dist;

	//put matches into the map by distance
	for (size_t i = 0; i < values.size(); i++)
	{
		dist = getDistance(str, values[i]);
		distanceMap[dist].push_back(values[i]);
	}

	//take the n best matches and put them into results
	for (std::map<int, std::vector<std::string> >::iterator i = distanceMap.begin();
			results.size() < n && i != distanceMap.end(); i++)
	{
		results.insert(results.end(), i->second.begin(), i->second.end());
	}

	return results;
}


std::string StringDistance::getNClosestAsString(std::string str,
		const std::vector<std::string> & values, size_t n)
{
	std::vector<std::string> results = getNClosest(str, values, n);
	std::string compStr = "";

	for (std::vector<std::string>::iterator i = results.begin();
			i != results.end(); i++)
	{
		DiffStrings diff(str, *i);
		compStr += "Expected: " + diff.getAString() + " Found: "
				+ diff.getBString() + "\n";
	}

	return compStr;
}

}
