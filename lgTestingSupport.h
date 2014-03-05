/**
 * @file CppTestingSupport.h
 *
 * @brief Contains classes that support testing of c++ code.
 * 
 * The classes in this header allow input/output to be redirected and for
 * the output to be more easily compared.  The Tokenizer class simplifies 
 * converting the output into string, int, or double tokens.
 * 
 * @attention This needs to be linked with the Boost library and the boost 
 *  library should come after the CppTesting library (on the linker command) 
 *  Linux/Unix: boost_regex-mt
 * 
 * @author saturner
 * @date 2013-05-25
 */
//TODO create a testing class to help and that automatically
// prepends hint: to the messages sent to cxx
#ifndef CPPTESTINGSUPPORT_H_
#define CPPTESTINGSUPPORT_H_

#include <iostream>
#include <sstream>
#include <exception>
#include <vector> 
#include <boost/regex.hpp> 
#include <boost/format.hpp>

using std::cout;
using std::cin;
using std::stringstream;
using std::streambuf;
using std::string;
using std::vector;

namespace cpptesting {






/**
 * @class StdIORedirector CppTestingSupport.h
 * @brief Redirects cin/cout to allow i/o to be captured for testing
 * 
 * This class allows cin and/or cout to be redirected and the data to be
 * programmatically supplied or examined.
 * 
 */
class StdIORedirector
{
    private:
    	stringstream outputStream;
    	streambuf* oldOutputStream;
    	bool outputRedirect;
    
    	stringstream inputStream;
    	streambuf* oldInputStream;
    	bool inputRedirect;
    public:
        /** 
         * @brief Sets up the class. No redirection is done.
         */
    	StdIORedirector();
    	
        /** 
         * @brief Cleans up the class. Restores cin/cout if needed.
         */
        virtual ~StdIORedirector();
        
    	/** 
         * @brief Replaces cout with a different output stream. This needs to 
         *  be done before cout is used.
         */
        void redirectCout();
    	
        /** 
         * @brief Restores cout to its original state.
         */
        void restoreCout();
    	
        /** 
         * @brief Resets the redirected output stream so that it is empty and
         *  clears the status flags.
         */
        void resetOutput();
        
    	/** 
         * @brief Gets the output from the redirected stream.
         * 
         * @return The output sent to the stream.
         */
        string getOutput() {return outputStream.str();}
        
    	/** 
         * @brief Gets a reference to the output stream being used in the 
         *  redirection.
         * 
         * @return Output stream being used.
         */
        stringstream& getOutputStream() {return outputStream;}
    	
    	/** 
         * @brief Replaces cin with a different input stream. This needs to 
         *  be done before cin is used.
         */
        void redirectCin();
    	
        /** 
         * @brief Restores cin to its original state.
         */
        void restoreCin();
        
    	/** 
         * @brief Puts the string data into the redirected input stream. Any 
         * data in the string is removed.
         * 
         * @param data New input for the stream. 
         */
        void setInput(string data);
    	
        /** 
         * @brief Gets a reference to the input stream being used in the 
         *  redirection.
         * 
         * @return Input stream being used.
         */
        stringstream& getInputStream() {return inputStream;}
        
        /**
         * @brief Returns true if cout was used (since it was redirected/reset).
         * 
         * @return True if cout was used.
         */
    	bool wasCoutUsed();
        
        /**
         * @brief Returns true if cin was used (since it was redirected/reset).
         * 
         * @return True if cin was used.
         */
        bool wasCinUsed();
};





/**
 * @class Tokenizer CppTestingSupport.h
 * @brief Utility class to help tokenize strings.
 * 
 * This class allows strings to be tokenized and it specifically supports
 * strings, ints, and doubles but it will allow for tokenization with any 
 * regular expression.  
 * 
 * ints and doubles are can be extracted from the string either by looking at
 * any numbers in the text (regardless of their relationship to other 
 * characters) or it can only extract separate values (i.e. surrounded by 
 * whitespace).  
 * 
 * For example, if you had "a5 b6 9 0", the first method would extract
 *  5, 6, 9, 0 while the second (looking for separate numbers) would just 
 *  extract 9, 0.  See INT_MATCH, INT_MATCH_SEPARATE, FLOAT_MATCH,
 *  FLOAT_MATCH_SEPARATE for more details.
 * 
 **/
class Tokenizer
{
    public:
        /**
         * @brief Default tokenization for strings.  Extracts anything not
         * whitespace (i.e. \S).
         */
        static const string MATCH;
        
        /**
         *  @brief This matches integers that may be embedded in other text.
         * 
         * This matches integers that may be embedded in other text.
         *  i.e. this should match "5", "6,", ":7", "a9b", etc.
         * The expressions must start with with something that is not 
         * a ., a number, or a - sign (or the beginning of the expression)
         * The value captured is simpily an optional - sign and one or more
         * digits. The expression must not end with a decimal followed by 
         * a number.  It can end with a decimal followed by something not 
         * a number (i.e. a period at the end of a sentence).
         */
        static const string INT_MATCH;
        
        /**
         * @brief Regular expression for matching ints with printf like 
         *  parameters for adding the set of characters before and after the 
         *  int. 
         * 
         * Regular expression for matching ints with printf like 
         *  parameters for adding the set of characters before and after the 
         *  int. This is used with NUM_PREFIX and NUM_POSTFIX to create
         *  INT_MATCH_SEPARATE and it can be used in the createMatchString 
         *  method to generate a regular expression with different 
         *  pre/postfixes.
         * 
         */
        static const string INT_MATCH_SEPARATE_BASE;
        
        /**
         * @brief Regular expression for matching ints that are considered
         * separate tokens.  This relies on NUM_PREFIX and NUM_POSTFIX and
         *  INT_MATCH_SEPARATE_BASE to determine what constitutes a separate
         *  token.
         * 
         */
        static const string INT_MATCH_SEPARATE;
        
        /**
         *  @brief This matches doubles that may be embedded in other text.
         * 
         * This matches doubles that may be embedded in other text.
         *  i.e. this should match "5.5", "6,", ":7.3", "a9.74b", etc.
         * The expressions must start with with something that is not 
         * a ., a number, or a - sign (or the beginning of the expression)
         * The value captured is an optional - sign and one or more
         * digits and a decimal point. 
         */
        static const string FLOAT_MATCH;
        
        /**
         * @brief Regular expression for matching double with printf like 
         *  parameters for adding the set of characters before and after the 
         *  double. 
         * 
         * Regular expression for matching double with printf like 
         *  parameters for adding the set of characters before and after the 
         *  double. This is used with NUM_PREFIX and NUM_POSTFIX to create
         *  FLOAT_MATCH_SEPARATE and it can be used in the createMatchString 
         *  method to generate a regular expression with different 
         *  pre/postfixes.
         * 
         */
        static const string FLOAT_MATCH_SEPARATE_BASE;
        
        /**
         * @brief Regular expression for matching doubles that are considered
         * separate tokens.  This relies on NUM_PREFIX and NUM_POSTFIX and
         *  FLOAT_MATCH_SEPARATE_BASE to determine what constitutes a separate
         *  token.
         * 
         */
        static const string FLOAT_MATCH_SEPARATE;
        
        
        /** 
         * @brief This specifies the symbols allowed to come before a number
         *  if looking for a separate token.
         * 
         *  This specifies the symbols allowed to come before a number
         *  if looking for a separate token. By default, the allows characters
         *  are:
         *      whitespace              i.e. " 5"
         *      beginning of the line   i.e. "5"
         *      $                       i.e. "$5"
         *      #                       i.e. "#5"
         *      +                       i.e. "+5"
         *      (                       i.e. "(5"
         *      *                       i.e. "*5"
         *      :                       i.e. ":5"
         *      ;                       i.e. ";5"
         *      /                       i.e. "/5"
         *      ,                       i.e. ",5"
         *      \                       i.e. "\5"
         *      [                       i.e. "[5"
         *      {                       i.e. "{5"
         */     
        static const string NUM_PREFIX;
        
        /** 
         * @brief This specifies the symbols allowed to come after a number
         *  if looking for a separate token.
         * 
         *  This specifies the symbols allowed to come after a number
         *  if looking for a separate token. By default, the allows characters
         *  are:
         *      whitespace              i.e. "5 "
         *      end of the line         i.e. "5"
         *      a period                i.e. "5." or "5. "
         *      !                       i.e. "5!"
         *      ?                       i.e. "5?"
         *      %                       i.e. "5%"
         *      #                       i.e. "5#"
         *      /                       i.e. "5/"
         *      +                       i.e. "5+"
         *      *                       i.e. "5*"
         *      )                       i.e. "5)"
         *      :                       i.e. "5:"
         *      ;                       i.e. "5;"
         *      ,                       i.e. "5,"
         *      -                       i.e. "5-"
         *      \                       i.e. "5\"
         *      }                       i.e. "5}"
         *      ]                       i.e. "5]"
         */     
        static const string NUM_POSTFIX;
        
        /**
         * @brief Tokenizes the string are stores it in a vector
         * 
         * This method tokenizes the string and tries to convert the data
         * into the appropriate type. If successful, it is stored in the vector.
         * 
         * @attention This method requires that the >> and << operators be 
         *  defined on the data type.
         * 
         * @attention The tokenizer pulls out the 1st captured group from the 
         * regular expression.  If more than one group is used, make sure that
         * they are marked as uncaptured (i.e. ?:) 
         * 
         * @param data String to tokenize
         * @param matches Vector to store the data
         * @param pattern Pattern used to tokenize the data. This should match
         *  the intended tokens and not describe where the string is split.
         *  (i.e. \s would return a vector of whitespace and not the values
         *  delimited by the whitespace).
         *  The default value is to tokenize it into strings
         * 
         * @return A vector with the tokens.  This is a reference to matches.
         * 
         */
        template <class T> static vector<T>& tokenize(string data, 
            vector<T> &matches, string pattern = MATCH);
        
        /**
         * @brief Creates a regular expression using the prefix, postfix, and 
         * the base.
         * 
         * Creates a regular expression using the prefix, postfix, and 
         * the base. This assumes that base has two printf-like parameters in it
         * that except string values.
         * 
         * This is a convient way to use INT_MATCH_SEPARATE_BASE and
         * FLOAT_MATCH_SEPARATE_BASE with different prefixes and/or postfixes.
         * 
         * @param pre Set of valid characters that can proceed the token.  This
         *  should be written as a regular expression and should use uncaptured
         *  groups.
         * @param base Regular expression that matches the tokens to be found.
         *  This should have two printf-like parameters (%s) for the prefix and
         *  postfix. There should only be one capturing group in this
         *  expression.
         * @param post Set of valid characters that can follow the token.  This
         *  should be written as a regular expression and should use uncaptured
         *  groups.
         * 
         * @return A string with the regular expression contains the prefix, 
         *  postfix, and base.
         * 
         */
        static string createMatchString(string pre, string base,
            string post);

        /**
         * @brief Finds a double value in the data that is within delta of
         *  value.
         * 
         * Convenience method that finds a double value in the data that 
         * is within delta of value.
         * 
         * @param data String to tokenize
         * @param value Value to find
         * @param delta Distance from value allowed.
         * @param pattern Regular expression to tokenize the data.  By default,
         *  it is FLOAT_MATCH (any number regardless of surrounding text).
         * 
         * @return true if the value is found
         * 
         */
        static bool findInTokens(string data, double value, double delta,
            string pattern = FLOAT_MATCH);
        
        /**
         * @brief Finds a double value in the dValues that is within delta of
         *  value.
         * 
         * Convenience method that finds a double value in the data that 
         * is within delta of value.
         * 
         * @param dValues List of tokens
         * @param value Value to find
         * @param delta Distance from value allowed.
         * 
         * @return true if the value is found
         * 
         */
        static bool findInTokens(vector<double> dValues, double value, 
            double delta);
        
        /**
         * @brief Finds an int value in the data.
         * 
         * Convenience method that finds an int value in the data.
         * 
         * @param data String to tokenize
         * @param value Value to find
         * @param pattern Regular expression to tokenize the data.  By default,
         *  it is INT_MATCH (any int regardless of surrounding text).
         * 
         * @return true if the value is found
         * 
         */
        static bool findInTokens(string data, int value,
            string pattern = INT_MATCH);
            
        /**
         * @brief Finds an int value in the data.
         * 
         * Convenience method that finds an int value in the data.
         * 
         * @param iValues list of tokens
         * @param value Value to find
         * 
         * @return true if the value is found
         * 
         */
        static bool findInTokens(vector<int> iValues, int value);
        
        /**
         * @brief Finds a string value in the data.
         * 
         * Convenience method that finds a string value in the data.
         * 
         * @param data String to tokenize
         * @param value Value to find
         * @param ignoreCase True if case should be ignored. By default, this
         *  is true.
         * @param partial Truie is the value to be found can match just part of
         *  a token (i.e. "a" would match "ab").  By default, this is false.
         * @param pattern Regular expression to tokenize the data.  By default,
         *  it is MATCH (any text).
         * 
         * @return true if the value is found
         * 
         */
        static bool findInTokens(string data, string value, 
            bool ignoreCase = true, bool partial = false, 
            string pattern = MATCH);
            
        /**
         * @brief Finds a string value in the data.
         * 
         * Convenience method that finds a string value in the data.
         * 
         * @param sValues List of tokens
         * @param value Value to find
         * @param ignoreCase True if case should be ignored. By default, this
         *  is true.
         * @param partial Truie is the value to be found can match just part of
         *  a token (i.e. "a" would match "ab").  By default, this is false.
         *
         * @return true if the value is found
         * 
         */
        static bool findInTokens(vector<string> sValues, string value, 
            bool ignoreCase = true, bool partial = false);
};



 template <class T> vector<T>& Tokenizer::tokenize(string data, 
            vector<T> &matches, string pattern) 
 {
        boost::regex matcher;
        matcher.assign(pattern, boost::regex_constants::icase);
        
        boost::sregex_token_iterator i(data.begin(), data.end(), matcher, 1);
        boost::sregex_token_iterator j;
        
        while(i != j)
        {
            stringstream value;
            T cValue;
            
            value << *i++;
            value >> cValue;
            if (!value.fail()) 
            {
                matches.push_back(cValue);
            }
            //*i++;
        }
        
        return matches;
 }



/**
 * @class StringUtil CppTestingSupport.h
 * @brief Support class with some common string utility methods.
 * 
 * This class contains a few common string utility methods to help manipulate
 * text data.  It can trim whitespace, change case, and removed a set of 
 * characters from the string.
 * 
 **/
class StringUtil 
{
    public:
    
        // --------------------------------------------------------------
        /**
         * @brief Removes the whitespace to the left of (before) the string.
         * 
         * Removes the whitespace to the left of (before) the string.
         * 
         * @param s String to be trimmed. 
         * 
         * @return Trimmed string. 
         * 
         */
        static inline std::string ltrim(std::string s)
        {
            size_t i;
            for (i = 0; i < s.length() && std::isspace((int) s[i]); i++)
            {
                //nothing
            }
    
            s.erase(0,i);
    
            return s;
        }
        
    
        // --------------------------------------------------------------
        /**
         * @brief Removes the whitespace to the right of (after) the string.
         * 
         * Removes the whitespace to the right of (after) the string.
         * 
         * @param s String to be trimmed.  This parameter is modified.
         * 
         * @return Trimmed string.  This returns a reference to s.
         * 
         */
        static inline std::string rtrim(std::string s)
        {
            size_t i;
            //i will wrap around to npos
            for (i = s.length() - 1; i != string::npos && std::isspace((int) s[i]); i--)
            {
                //nothing
            }
    
            if (i < s.length())
            {
                s.erase(i+1,s.length()-i);
            }
    
            return s;
        }
       
    
        // --------------------------------------------------------------
        /**
         * @brief Removes the whitespace to before and after the string.
         * 
         * Removes the whitespace to the before and after the string.
         * 
         * @param s String to be trimmed.  This parameter is modified.
         * 
         * @return Trimmed string.  This returns a reference to s.
         * 
         */
        static inline std::string trim(std::string s)
        {
            return ltrim(rtrim(s));
        }
        
        
        /**
         * @brief Converts the string to upper case.
         * 
         * Converts the string to upper case.
         * 
         * @param str String to convert. This parameter is modified.
         * 
         * @return String in all capital letters.  This returns a reference to 
         *  str.
         *  
         */
        static string toUpper(std::string str);
        
         /**
         * @brief Converts the string to lower case.
         * 
         * Converts the string to lower case.
         * 
         * @param str String to convert. This parameter is modified.
         * 
         * @return String in all lower case letters.  This returns a reference
         *  to str.
         *  
         */
        static string toLower(std::string str);
      
        
        /**
         * @brief Replaces a set of characters in the string with another char.
         * 
         * Replaces a set of characters in the string with another char.
         *  If a character in str matches any of the characters in 
         *  removeChars, it is replaced with replacement.
         * 
         * @param str String to modify. This parameter is modified.
         * @param removeChars Set of characters to remove.
         * @param replacement Character to replace the removed characters.
         * 
         * @return String with the characters replaced.  
         *  This returns a reference to str.
         *  
         */
        static string replaceChars(std::string str, std::string removeChars,
            char replacement);
        
};


 /**
 * @class DiffStrings CppTestingSupport.h
 * @brief Visually compares two string  
 * 
 * This class compares two strings and visually shows the differences
 * between them.  That is, it finds the common prefix and postfix (if
 * any) and then displays the difference in [].  So, comparing "abc" and
 * "adc" would yield "a[b]c" and "a[d]c" for getAString, getBString
 * respectively.
 * 
 * If the pre/postfix is too long, it is shortened and an ellipsis (...)
 * is added.
 * 
 * Borrowed liberally from junit.org's org.junit.ComparisonFailure class.
 * https://github.com/junit-team/junit/blob/master/src/main/java/org/junit/ComparisonFailure.java
 * 
 */
class DiffStrings
{
    private:
        string aStr;
        string bStr;
        
        static const string ELLIPSIS;
        static const string DIFF_START;
        static const string DIFF_END;
    
        void compareStrings(size_t maxSize);
        size_t getPrefix();
        size_t getPostfix(string a, string b);
        string getDiffString(const string& str, size_t pre, size_t post, 
            size_t maxSize);
        string addEllipsis(const string& str, size_t maxSize, 
            bool atBegin); 
    
    public:
        /**
         * @brief The maximum number of characters (minus the elipse) to show
         *  in the prefix/postfix when comparing two strings with compareStrings
         */
        static const size_t MAX_CONTEXT_LEN;
    
        /**
         * @brief Visually displays the differences between two strings.
         * 
         * This class compares two strings and visually shows the differences
         * between them.  That is, it finds the common prefix and postfix (if
         * any) and then displays the difference in [].  So, comparing "abc" and
         * "adc" would yield "a[b]c" and "a[d]c" for getAString, getBString
         * respectively.
         * 
         * If the pre/postfix is too long, it is shortened and an ellipsis (...)
         * is added.
         * 
         * @param a The first string to compare.
         * @param b The second string to compare.
         * @param maxSize The maximum number of characters to show in the pre/postfix
         * before adding an ellipsis.  This defaults to 20.
         *      
         */
        DiffStrings(string a, string b, size_t maxSize = MAX_CONTEXT_LEN);
        
        /**
         * @brief Determines if the strings passed to the constructor are equal.
         * 
         * @return true if the strings are equal
         */
        bool areEqual() const {return aStr == bStr;} 
        
        /**
         * @brief Gets the value of the first string passed into the 
         * constructor with the differences shown. If the strings were the 
         * same, this returns a copy of the original string.
         * 
         * @return The first string modified to show the differences.
         */
        const string& getAString() const {return aStr;} 
        
        /**
         * @brief Gets the value of the second string passed into the 
         * constructor with the differences shown. If the strings were the 
         * same, this returns a copy of the original string.
         * 
         * @return The second string modified to show the differences.
         */
        const string& getBString() const {return bStr;} 
        
    
};


 /**
 * @class CompareOutput CppTestingSupport.h
 * @brief Utility class to compare the tokens in two strings.  
 * 
 * This class compare the tokens in two strings and provides a little more 
 * information about how they are different.  This can be used to find the
 * first mismatched token.
 * 
 */   
class CompareOutput
{
    private:
        size_t location;
        int comparedSize;
        bool success;
        string lastExpectedToken;
        string lastResultToken;
    
    public:
        /**
         * @brief Compares expected and result by their tokens.   
         * 
         * Compares expected and result by their tokens.  By default, it 
         * tokenizes all non-whitespace.
         * 
         * The first mismatching token is marked and if there is a difference
         * in the number of tokens, it is noted as well.
         * 
         * @param expected The expected tokens
         * @param result The actual tokens
         * @param pattern The regular expression used to tokenize the data
         */
        CompareOutput(string expected, string result,
            string pattern = Tokenizer::MATCH);
            
        /**
         * @brief Determines if the number of tokens were equal.
         * 
         * @return true if the number of tokens were equal.
         */
        bool areEqualLength() {return comparedSize == 0;};
        
        /**
         * @brief Determines if there were more tokens in the expected value.
         * 
         * @return true if there were more tokens in the expected value.
         */
        bool isExpectedLonger() {return comparedSize == -1;};
        
        /**
         * @brief Determines if there were more tokens in the actual value.
         * 
         * @return true if there were more tokens in the actual value.
         */
        bool isResultLonger() {return comparedSize == 1;};
        
        /**
         * @brief Determines if the expected and actual were the same.
         * 
         * @return true if they were the same.
         */
        bool wasSuccessful() {return success;};
        
        /**
         * @brief Gets the last compared token in the expected value.
         * 
         * Gets the last compared token in the expected value. If there is a 
         * mismatch, this is the token that did not match.  If expected was 
         * longer than the actual, this is the first token not in the actual 
         * data.  If the result was longer, this is "".
         * 
         * @return The last compared token in the expected value.
         */
        string getLastExpectedToken() {return lastExpectedToken;};
        
        /**
         * @brief Gets the last compared token in the result value.
         * 
         * Gets the last compared token in the result value. If there is a 
         * mismatch, this is the token that did not match.  If expected was 
         * longer than the actual, this is "".  If the result was longer, 
         * this is first token not in the actual data.
         * 
         * @return The last compared token in the result value.
         */
        string getLastResultToken() {return lastResultToken;};
        
        /**
         * @brief Gets the location (of the token) of the first mismatch.
         * 
         * @return The location (of the token) of the first mismatch.
         */
        size_t getLocationOfMismatch() {return location;};
        
        /**
         * @brief Gets the result of the comparison as a standard message. This
         *  could be used as feedback in a test. 
         * 
         * Gets the result of the comparison as a standard message. This
         *  could be used as feedback in a test. This specifies the error and 
         *  the location of the error. 
         * 
         * @return The result of the comparison as a message.
         */
        string getStdMessage();
};


// -------------------------------------------------------------------------
/**
 *  @class StringDistance CppTestingSupport.h
 *  @brief Gets the Damerau–Levenshtein distance between two strings.
 *
 *  @author  Schnook
 *  @version Oct 2, 2012
 */
class StringDistance {

private:
	int* distanceTable;
	std::map<char, int> letters;
	int size;

	static const int INIT_SIZE;
	static StringDistance strDistance;

	StringDistance();


	void updateData(int length);
	void initializeTable(int lenght1, int length2);
	void initializeMap(std::string str1, std::string str2);
	int computeDistance(std::string str1, std::string str2);
public:

	~StringDistance();
	// ----------------------------------------------------------
	/**
	 * Gets the Damerau–Levenshtein distance between two strings.
	 *
	 * The algorithm was taken from:
	 * http://en.wikipedia.org/wiki/Damerau%E2%80%93Levenshtein_distance
	 *
	 * @param str1 First string to compare
	 * @param str2 Second string to compare
	 *
	 * @return The number of edits needed to convert str1 to str2
	 */
	static size_t getDistance(std::string str1, std::string str2);


	/**
	 * @brief Finds the n closest matches to str in values.
	 *
	 * Finds the n closest matches to str in values. The strings are ordered in
	 * the vector by closeness with the closest at the front.  The method tries
	 * to return n values.  If there are less than n strings in values, then
	 * all of them are returned.  If there are multiple strings in values with
	 * the same distance from str, more than n values may be returned (it will
	 * return all of the ties unless it is >= n).
	 *
	 * @param str String to compare to the others
	 * @param values Set of strings to compare with
	 * @param n Number of matches to return
	 *
	 * @return A list of matches ordered by closeness to str
	 */
	static std::vector<std::string> getNClosest(std::string str,
			const std::vector<std::string> & values, size_t n);

	/**
	 * @brief Finds the n closest matches to str in values. The string
	 * returned has the comparisions between str and the other matches.
	 *
	 * Finds the n closest matches to str in values. The strings are ordered in
	 * the vector by closeness with the closest at the front.  The method tries
	 * to return n values.  If there are less than n strings in values, then
	 * all of them are returned.  If there are multiple strings in values with
	 * the same distance from str, more than n values may be returned (it will
	 * return all of the ties unless it is >= n).
	 *
	 * This differs from getNClosest by returning the matches in the form
	 * of a string with comparisons between str and the other matches. So,
	 * if str = "abc" and values = {"abd", "lmc", "abcde"}, it will return
	 * as string in the form:
	 * 	Expected: ab[c] Found: ab[d]
	 * 	Expected: ab[c] Found: [lm]c
	 * 	Expected: abc[] Found: abc[de]
	 *
	 * @param str String to compare to the others
	 * @param values Set of strings to compare with
	 * @param n Number of matches to return
	 *
	 * @return A string with the comparisons between str and the other
	 *  matches.
	 */
	static std::string getNClosestAsString(std::string str,
			const std::vector<std::string> & values, size_t n);
};


}

#endif /* CPPTESTINGSUPPORT_H_ */
