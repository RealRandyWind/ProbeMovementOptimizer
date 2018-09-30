#pragma once

#include "NDevTypes.h"
#include "NDevPoint.h"
#include "NDevSequence.h"

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

		using FSimplex = TPoint<SizePoint + 1, FSize>;

		using FFacet = TPoint<SizePoint, FSize>;

		struct FBoundary
		{
			enum class EOperation
			{
				_Unknown = Unknown,
				Union,
				Exclustion,
				Intersection,
				_Size
			};

			EOperation Operation;
			TSequence<FPoint> Points;
		};

		EType Type;
		union
		{
			TSequence<FBoundary> Boundaries;
			struct
			{
				union
				{
					TSequence<FSimplex> Simplices;
					TSequence<FFacet> Facets;
				};
				TSequence<FPoint> Points;
			};
			FPointer Pointer;
		};

		TShape()
		{
			Pointer = NullPtr;
		}

		~TShape() { }

	};

	using FShape2D = TShape<2>;

	using FShape3D = TShape<3>;

	using FShape4D = TShape<4>;

	using FShape5D = TShape<5>;
}