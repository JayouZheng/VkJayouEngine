/*********************************************************************
 *  Misc.inl
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 *********************************************************************/

#pragma once

// #include <algorithm>
// std::find(vec.begin(), vec.end(), item) != vec.end(), std::find_if, std::find_if_not
template<typename T, typename TLambda>
bool Misc::IsVecContain(const std::vector<T>& InVector, const T& InElement, const TLambda& InCompare)
{
	for (auto& member : InVector)
	{
		if (InCompare(member, InElement))
		{
			return true;
		}
	}
	return false;
}