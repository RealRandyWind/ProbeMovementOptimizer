#pragma once

#include "NDev.h"

namespace ProbeMovementOptimizer
{
	using namespace NDev;
	using namespace NDev::Types;

	template<FSize SizePoint>
	FBoolean Intersect(const TPoint<SizePoint, FReal> &P1, const TPoint<SizePoint, FReal> &P2, const TPoint<SizePoint, FReal> &P3, const TPoint<SizePoint, FReal> &P4, TPoint<SizePoint, FReal> &P, FBoolean &bParallel, const FBoolean bHandleParallel = True, const FReal Eps = TLimit<FReal>::Epsilon());

	template<>
	inline FBoolean Intersect<2>(const TPoint<2, FReal> &P1, const TPoint<2, FReal> &P2, const TPoint<2, FReal> &P3, const TPoint<2, FReal> &P4, TPoint<2, FReal> &P, FBoolean &bParallel, const FBoolean bHandleParallel, const FReal Eps)
	{
		TPoint<2, FReal> D34, D13, D12, D21, C;
		FReal Alpha;

		D13 = P1 - P3;
		D12 = P1 - P2;
		D21 = P2 - P1;
		D34 = P3 - P4;
		/* Alpha = Det(D12, D34); */
		Alpha = D12[0] * D34[1] - D12[1] * D34[0];
		if (bParallel = IsZeroEps(Alpha, Eps))
		{
			return !bHandleParallel || Any(IsEqualEps(P1, P3, Eps));
		}
		/* C = { Det(D13, D34), Det(D12, D13) }; C *= (1.0 / Alpha); */
		C[0] = (D13[0] * D34[1] - D13[1] * D34[0]) * (1.0 / Alpha);
		C[1] = -(D12[0] * D13[1] - D12[1] * D13[0]) * (1.0 / Alpha);
		P = P1 + C[0] * D21;
		return All(Between(C, 0.0, 1.0));
	}
}