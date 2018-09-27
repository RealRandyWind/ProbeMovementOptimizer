#pragma once

#include "PMOMovement.h"

namespace ProbeMovementOptimizer
{
	using namespace NDev;
	using namespace NDev::Types;

	struct FMovement2D : public TMovement<2>
	{

		virtual FVoid UseDefaultParameters() override;

	protected:

		virtual FVoid _Use(const FShape &Shape, typename FPath &Path) override;
		

	};
}