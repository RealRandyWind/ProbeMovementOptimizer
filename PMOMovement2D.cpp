#include "pch.h"

#include "PMOMovement2D.h"
#include "NDevPoint.h"
#include "NDevMathPoint.h"
#include "NDevMathExtend.h"

using namespace ProbeMovementOptimizer;

FVoid FMovement2D::UseDefaultParameters()
{
	Parameters.Start = 0;
	Parameters.Radius = 0;
	Parameters.Alpha = 0.01;
	Parameters.Beta = TLimit<FReal>::Infinity();
}

FVoid FMovement2D::_Use(const FShape &Shape, typename FPath &Path)
{
	if (Shape.Type != FShape::EType::Manifolds) { return; }

	for (const auto &Boundary : Shape.Boundaries)
	{
		const FBoolean bExclude = Boundary.Operation == FShape::FBoundary::EOperation::Exclustion;
		const FReal Alpha = bExclude ? -Parameters.Alpha : Parameters.Alpha;

		for (const auto &Point : Boundary.Points)
		{

		}
	}

	if (this->OnUse) { this->OnUse(Shape, Path); }
}