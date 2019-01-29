#pragma once

#include "NDevTypes.h"
#include "NDevData.h"
#include "NDevMathDistributions.h"
#include "PMOShape.h"

namespace ProbeMovementOptimizer
{
	using namespace NDev;
	using namespace NDev::Types;

	template<FSize SizePoint>
	FVoid Generate(TShape<SizePoint> &Item, FSize K = 32, FSize Seed = 0)
	{
		using FShape = TShape<SizePoint>;
		TNormal<FReal> Distribution;
		Distribution.Seed(Seed);

	}

	template<FSize SizePoint>
	FVoid Generate(TData<TShape<SizePoint>> &List, FSize N = 8, FSize K = 32, FSize Seed = 0)
	{
		List.Reserve(N, True);
		for (auto &Item : List)
		{
			Generate(Item, K, Seed);
		}
	}

}