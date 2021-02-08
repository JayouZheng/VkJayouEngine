/*********************************************************************
 *  RenderEnum.h
 *  Copyright (C) 2021 Jayou. All Rights Reserved.
 * 
 *  A set of Utility enums uesd for rendering.
 *********************************************************************/

#pragma once

namespace Render
{
	enum class Sampler
	{	
		PointWrap = 0,
		PointClamp,
		LinearWrap,
		LinearClamp,
		AnisotropicWrap,
		AnisotropicClamp,
		PCF,

		Max = 0xff
	};
}
