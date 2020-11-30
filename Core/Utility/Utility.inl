/*********************************************************************
 *  Utility.inl
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 *********************************************************************/

#pragma once

template<typename T, typename TLambda>
bool Utility::IsVecContain(const std::vector<T>& InVector, const T& InElement, const TLambda& InCompare)
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