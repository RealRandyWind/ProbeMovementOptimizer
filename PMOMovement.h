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
			typename FShape::FPoint Start, End;
			FReal Diameter, Alpha, Beta;
		};

		struct FState
		{
			typename FShape::FPoint Probe[1 << SizePoint];
		};

		FParameters Parameters;
		FState State;


	};



}
