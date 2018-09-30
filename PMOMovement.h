#pragma once

#include "PMOAlgorithm.h"
#include "PMOShape.h"

namespace ProbeMovementOptimizer
{
	using namespace NDev;
	using namespace NDev::Types;

	template<FSize SizePoint>
	struct TMovement : public TAlgorithm<TShape<SizePoint>, typename TShape<SizePoint>::FPath>
	{
		using FAlgorithm = TAlgorithm<TShape<SizePoint>, typename TShape<SizePoint>::FPath>;

		using FShape = typename FAlgorithm::FInput;

		using FPath = typename FAlgorithm::FResult;

		struct FParameters
		{
			typename FShape::FPoint Start;
			FReal Radius, Alpha, Beta;
		};

		struct FState
		{
			TSequence<FSize> ExclusionIndices;
		};

		FParameters Parameters;
		FState State;


	};
}
