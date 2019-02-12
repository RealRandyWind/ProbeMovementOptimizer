#pragma once

#include "PMOMovement.h"
#include "NDevMathExtend.h"

namespace ProbeMovementOptimizer
{
	using namespace NDev;
	using namespace NDev::Types;

	struct FMovement2D : public TMovement<2>
	{
		virtual FVoid UseDefaultParameters() override;

	protected:

		struct _FSample
		{
			FShape::FPoint &Center, Lower, Upper;
		};

		virtual FVoid _Use(const FShape &, FPath &) override;

		FVoid _Coverage(const FShape &, FPath &);

		FVoid _Optimize(FPath &);

		FVoid _Place(FShape::FPoint &, FShape::FPoint &, const FShape::FBoundary &, const FShape &, const FShape::FPath &, FSize &);

		FVoid _InBound(FShape::FPoint &, FShape::FPoint &, FShape::FPoint &, const FShape::FPoint &, const FShape::FPoint &, FBoolean &);

	};



}