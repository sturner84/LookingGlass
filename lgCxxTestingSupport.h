#ifndef CXXTESTINGSUPPORT_H_
#define CXXTESTINGSUPPORT_H_

#include "lgTestingSupport.h"
#include <string>

namespace cpptesting {


//TODO complete cxxtest support

class CxxStdIORedirector : public StdIORedirector
{
    private: 
        std::string prefix;
    
    public:
    
        static const std::string DEFAULT_HINT_PREFIX;
        static const std::string DEFAULT_CIN_MESSAGE;
        static const std::string DEFAULT_COUT_MESSAGE;
          
        /** 
         * @brief Sets up the class. No redirection is done.
         * @param hintPrefix prefix to be added to the beginning of the messages
         * sent to the CXX tests.  By default this is "".
         */
        CxxStdIORedirector(std::string hintPrefix = DEFAULT_HINT_PREFIX);
        
        //TODO Allow testing of the output against a regex?
        //TODO Add a test that tries to call main and return the output?
        //TODO make these all threaded?
        
        /** 
         * @brief Cleans up the class. Restores cin/cout if needed.
         */
        virtual ~CxxStdIORedirector();
    

        /**
         * @brief Tests if cin was used in since the input was redirected.
         *
         * The assumption is that the student should not be using cin/cout
         * in most methods but should be relying on parameters and returns
         * to pass data.  If cin was used, this fails with an error message.
         *
         */
        void testIfCinUsed(std::string message = DEFAULT_CIN_MESSAGE);

        /**
         * @brief Tests if cout was used in since the output was redirected.
         *
         * The assumption is that the student should not be using cin/cout
         * in most methods but should be relying on parameters and returns
         * to pass data.  If cout was used, this fails with an error message.
         *
         */
        void testIfCoutUsed(std::string message = DEFAULT_COUT_MESSAGE);
};



/**
 *
 * TODO to be implemented as tests
 *
 * namespaces
 *  testForNamespace(string name)
 *
 * Global Variables
 * Functions
 *   testForGlobalVariables()
 *  testForFunction(string functionSignature,
 * 		 int maxSimilar = 5)
 *
 *  testForOperator(string methodSignature,
 * 		int maxSimilar = 5)
 *
 *
 * Classes
 * testIfClassExists(string className)
 * testForPublicVariables(string className)
 * //needs private/protected to be reflected
 * testForVariableCount(String className, int maxCount)
 * testInheritsFrom(string className, string parentClassName, string message)
 *
 * testIfClassHasConstructor(string cClass, string constructSignature,
 * 	int maxSimilar = 5)
 *
 * 	testIfClassHasMethod(string cClass, string methodSignature,
 * 		bool isAccessible, int maxSimilar = 5)
 *
 *  testIfClassHasOperator(string cClass, string methodSignature,
 * 		bool isAccessible, int maxSimilar = 5)
 */



}
#endif /*CXXTESTINGSUPPORT_H_*/
