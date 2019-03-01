#pragma once

#include "NDevTypes.h"
#include "NDevData.h"
#include "NDevMathDistributions.h"
#include "PMOShape.h"

namespace ProbeMovementOptimizer
{
	using namespace NDev;
	using namespace NDev::Types;

	enum class EGenerate : FEnum
	{
		_Unknown = Unknown,
		Arch,
		Array,
		Square,
		Spiral,
		Fractal,
		_Size
	};

	template<FSize SizePoint>
	FVoid Generate(TShape<SizePoint> &Item, EGenerate Type, FSize K = 32, FReal Scale = 1.0, FBoolean bInverted = False, FSize Seed = 0)
	{
		switch (Type)
		{
			case EGenerate::Arch: { _GenerateArch(Item, K, Scale, bInverted, Seed); } break;
			case EGenerate::Array: { _GenerateArray(Item, K, Scale, bInverted, Seed); } break;
			case EGenerate::Square: { _GenerateSquare(Item, K, Scale, bInverted, Seed); } break;
			case EGenerate::Spiral: { _GenerateSpiral(Item, K, Scale, bInverted, Seed); } break;
			case EGenerate::Fractal: { _GenerateFractal(Item, K, Scale, bInverted, Seed); } break;
			default: break;
		}
	}

	template<FSize SizePoint>
	FVoid Generate(TData<TShape<SizePoint>> &List, EGenerate Type, FSize N = 8, FSize K = 32, FReal Scale = 1.0, FBoolean bInverted = False, FSize Seed = 0)
	{
		List.Reserve(N, True);
		for (auto &Item : List)
		{
			Generate(Item, Type, K, Scale, bInverted, Seed);
		}
	}
	
	template<FSize SizePoint>
	FVoid _GenerateArch(TShape<SizePoint> &Item,  FSize K, FReal Scale, FBoolean bInverted, FSize Seed)
	{
		using FShape = TShape<SizePoint>;

		//auto Operation = bInverted ? FShape::FBoundary::EOperation::Exclusion : FShape::FBoundary::EOperation::Inclusion;
		Item.Points.Reserve(K, True);
	}

	template<FSize SizePoint>
	FVoid _GenerateArray(TShape<SizePoint> &Item, FSize K, FReal Scale, FBoolean bInverted, FSize Seed)
	{
		using FShape = TShape<SizePoint>;

		//auto Operation = bInverted ? FShape::FBoundary::EOperation::Exclusion : FShape::FBoundary::EOperation::Inclusion;
		Item.Points.Reserve(K, True);
	}

	template<FSize SizePoint>
	FVoid _GenerateSquare(TShape<SizePoint> &Item, FSize K, FReal Scale, FBoolean bInverted, FSize Seed)
	{
		using FShape = TShape<SizePoint>;

		//auto Operation = bInverted ? FShape::FBoundary::EOperation::Exclusion : FShape::FBoundary::EOperation::Inclusion;
		Item.Points.Reserve(K, True);
	}

	template<FSize SizePoint>
	FVoid _GenerateSpiral(TShape<SizePoint> &Item, FSize K, FReal Scale, FBoolean bInverted, FSize Seed)
	{
		using FShape = TShape<SizePoint>;

		//auto Operation = bInverted ? FShape::FBoundary::EOperation::Exclusion : FShape::FBoundary::EOperation::Inclusion;
		Item.Points.Reserve(K, True);
	}

	template<FSize SizePoint>
	FVoid _GenerateFractal(TShape<SizePoint> &Item, FSize K, FReal Scale, FBoolean bInverted, FSize Seed)
	{
		using FShape = TShape<SizePoint>;

		//auto Operation = bInverted ? FShape::FBoundary::EOperation::Exclusion : FShape::FBoundary::EOperation::Inclusion;
		Item.Points.Reserve(K, True);
	}



}