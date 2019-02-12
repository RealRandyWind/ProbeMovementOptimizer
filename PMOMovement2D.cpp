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
	TList<FPath::FValue> List;
	FShape::FPoint Cursor, Next;
	FSize Index, End;

	for (const auto &Boundary : Shape.Boundaries)
	{
		Index = 0;
		End = Boundary.Indices.Size();
		Cursor = Shape.Points[Boundary.Indices[Index]];
		while (Index < End)
		{
			/* check if cursor is inside existing sample, if yes we should
			 * continue with this sample
			 */
			_Place(Cursor, Next, Boundary, Shape, Path, Index);
			List.Add(Cursor + Parameters.Radius);
			Cursor = Next;
		}
	}
	Path.Data(List.Descriptor());
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

FVoid FMovement2D::_Place(FShape::FPoint &Cursor, FShape::FPoint &Next, const FShape::FBoundary &Boundary, const FShape &Shape, const FShape::FPath &Path, FSize &Index)
{
	FShape::FPoint Lower, Upper;
	FBoolean bInBound;

	Upper = Lower = Cursor;
	bInBound = True;
	/* compute uper and lower until bounds excede movement freedom with respect to
	 * corner
	 */
	while (bInBound)
	{
		/* best may be to use nearest as next, but each neighbor needs stored information of 
		 * its adjacent edges and direction, but for now its ok to assume a more simple shape
		 * that the next point and previous are always adjacent, nearest and including.
		 */
		const auto &Point = Shape.Points[Boundary.Indices[Index]];
		_InBound(Lower, Upper, Next, Point, Cursor, bInBound);
		/* check if probe can be moved such that containing all previous points
		 */
		++Index;
	}
	/* set intersection probe with  index - 1 to index as Next, need edge enter and
	 * leave information.
	 * 
	 * next placement should always be prioritized in clocwise order.
	 */
}

FVoid FMovement2D::_InBound(FShape::FPoint &Lower, FShape::FPoint &Upper, FShape::FPoint &Next, const FShape::FPoint &Point, const FShape::FPoint &Cursor, FBoolean &InBound)
{
	
}