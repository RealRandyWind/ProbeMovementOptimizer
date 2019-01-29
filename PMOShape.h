#pragma once

#include "NDevTypes.h"
#include "NDevPoint.h"
#include "NDevSequence.h"
#include "NDevMathExtend.h"

namespace ProbeMovementOptimizer
{
	using namespace NDev;
	using namespace NDev::Types;

	template<FSize SizePoint>
	struct TShape
	{
		enum class EType
		{
			_Unknown = Unknown,
			Boundaries,
			Simplices,
			Facets,
			_Size
		};

		using FPoint = TPoint<SizePoint, FReal>;

		using FPath = TSequence<FPoint>;

		using FFacet = TPoint<SizePoint, FSize>;

		using FSimplex = TPoint<SizePoint + 1, FSize>;

		struct FBoundary
		{
			enum class EOperation
			{
				_Unknown = Unknown,
				Inclusion,
				Exclusion,
				Intersection,
				_Size
			};

			EOperation Operation;
			TSequence<FSize> Indices;
		};

		EType Type;
		union
		{
			TSequence<FBoundary> Boundaries;
			TSequence<FSimplex> Simplices;
			TSequence<FFacet> Facets;
		};
		TSequence<FPoint> Points;

		TShape()
		{
			Type = EType::_Unknown;
		}

		~TShape() { }

		virtual FVoid Bounds(FPoint &Lower, FPoint &Upper, FBoolean bInitialize = True)
		{
			if (bInitialize)
			{
				Lower = TLimit<FReal>::Infinity();
				Upper = -TLimit<FReal>::Infinity();
			}

			for (auto &Point : Points)
			{
				MinInto(Lower, Point);
				MaxInto(Upper, Point);
			}
		};

		virtual FVoid Mean(FPoint &Center, FBoolean bInitialize = True, FSize K = 0)
		{
			if (bInitialize) { Center = 0; }
			else { Center *= (1.0 / K); }

			for (auto &Point : Points)
			{
				Center += Point;
			}
			Center *= (1.0 / (K + Points.Size()))
		};


	};

	using FShape2D = TShape<2>;

	using FShape3D = TShape<3>;

	using FShape4D = TShape<4>;

	using FShape5D = TShape<5>;



}