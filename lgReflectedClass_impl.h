/*
 * lgReflectedClass_impl.h
 *
 *  Created on: Jan 29, 2014
 *      Author: Administrator
 */

#ifndef LGREFLECTEDCLASS_IMPL_H_
#define LGREFLECTEDCLASS_IMPL_H_

#include "lgReflectedClass.h"
#include "lgReflectionSupport.h"

/**
 * Invokes a method of this object with the specified parameters
 * and returns the result
 *  (through the returnVal parameter).
 *  The signature for this method is:
 *  	bool invokeReturn(string methodSignature, <ReturnType> returnVal,
 *  		GVariant p0, ...)
 *  		Most types can be used for GVariant
 *
 * @param methodSignature Method to call (i.e. int foo(double x, char y))
 * @param returnVal Value returned from the method Undefined if success = false
 * @param p<1-28> Parameter for the method.  Currently up to 28 parameters are
 * 		permitted.
 *
 * @return True is the method was successfully called.
 * 		If the method was called but the return value could not be converted
 * 		to the ReturnType, then this is false.
 */
#define REF_INVOKE(N, unused) \
	template <typename ReturnType> \
	bool cpptesting::ReflectedObject::invokeReturn(std::string methodSignature, \
		ReturnType & returnVal GPP_COMMA_IF(N) \
		GPP_REPEAT(N, GPP_COMMA_PARAM, const cpgf::GVariant & p)) \
{ \
	return LookingGlass::invokeInternal(this, methodSignature, true, returnVal, \
		false GPP_COMMA_IF(N) \
		GPP_REPEAT(N, GPP_COMMA_PARAM, p)) == LookingGlass::INVOKE_SUCCESS; \
}

GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE







#endif /* LGREFLECTEDCLASS_IMPL_H_ */
