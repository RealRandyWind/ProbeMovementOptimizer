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

		/* sample is our event opject, Lower/Upper show the constrain of movement,
		 * Ankers the out edge limitations both points are alwasy on the edge of the
		 * sample, movment of the sample may changes the Ankers.
		 *
		 * idee is to move the next sample ankerd on containing A and B.
		 */
		struct _FSample
		{
			FSize Index, Edge, Intersect;
			FShape::FPoint Lower, Upper;
		};

		virtual FVoid _Initialize() override;

		virtual FVoid _Use(const FShape &, FPath &) override;

		FVoid _Coverage(const FShape &, FPath &);

		FVoid _Optimize(FPath &);

		FVoid _Place(_FSample &, const FShape &, TList<FPath::FValue> &, TStack<_FSample> &);

		FVoid _Intersections(const FShape::FPoint &, const FShape::FPoint &, const FShape::FPoint &, _FSample (&)[4]);


	};



}