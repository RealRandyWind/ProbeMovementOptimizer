#include "pch.h"

#include "PMOMovement2D.h"
#include "NDevPoint.h"
#include "NDevMathPoint.h"
#include "NDevMathExtend.h"

using namespace ProbeMovementOptimizer;

FVoid FMovement2D::UseDefaultParameters()
{
	Parameters.Start = 0.0;
	Parameters.End = 0.0;
	Parameters.Radius = 1.0;
	Parameters.Alpha = 0.01;
	Parameters.Beta = TLimit<FReal>::Infinity();
}

FVoid FMovement2D::_Use(const FShape &Shape, FPath &Path)
{
	if (Shape.Type != FShape::EType::Boundaries) { return; }

	_Coverage(Shape, Path);
	_Optimize(Path);

	if (this->OnUse) { this->OnUse(Shape, Path); }
}

FVoid FMovement2D::_Coverage(const FShape &Shape, FPath &Path)
{

}

FVoid FMovement2D::_Optimize(FPath &Path)
{
	FSize End, Index;
	FShape::FPoint From, To;
	FReal Lenght, DLenght, Error;

	From = Parameters.Start;
	Lenght = DLenght = 0;
	End = Path.Size();
	for (const auto &Point : Path)
	{
		Lenght += Norm(To - From);
		From = To;
		To = Point;
	}
	Lenght += Norm(Parameters.End - From);
}

FVoid FMovement2D::_Touch(FShape::FPoint &, FPath &)
{

}