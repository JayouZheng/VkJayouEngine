/*********************************************************************
 *  Misc.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  A set of utility api may be uesd through out the engine.
 *********************************************************************/

#pragma once

#include "Core/TypeDef.h"

class Misc
{

public:

	/**
	 *  Determine whether the element in the container exists.
	 *
	 *  @param  InVector   the vector to check.
	 *  @param  InElement  the element to check.
	 *  @param  InCompare  the compare function/rule to apply.
	 *
	 *  @return true if element exists, otherwise false.
	 */
	template<typename T, typename TLambda>
	static bool IsVecContain(const std::vector<T>& InVector, const T& InElement, const TLambda& InCompare);
};

#include "Misc.inl"
