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
	Parameters.Diameter = 1.0;
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
	TStack<_FSample> Stack;

	Stack.Push(_FSample{ List[Stack.Size()], });
	while (Stack.Full())
	{
		auto Sample = Stack.Pop();
		_Place(Sample, Shape, Stack);
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

FVoid FMovement2D::_Place(_FSample &Sample, const FShape &Shape, TStack<_FSample> &Stack)
{
	FShape::FPoint Probe[2];
	FShape::FPoint Lower, Upper, Cursor, Previous, Point;
	FReal Radius;
	FSize Index;
	FBoolean bInBound;

	bInBound = False;
	Radius = Parameters.Diameter * 0.5;
	Probe[0] = Sample.Center - Radius;
	Probe[1] = Sample.Center + Radius;

	/* find intersections (cliping with probe as cip plane) and find inbound points to
	 * determine lower and upper bounds, trace ancker points remember clockwize priority
	 * for sampling by pusing on the stack.
	 * 
	 * later optimize neast intersections lookup
	 */
	for (const auto &Boundary : Shape.Boundaries)
	{
		Previous = Shape.Points[Boundary.Indices.Size()];
		for (const auto &Index : Boundary.Indices)
		{
			Cursor = Shape.Points[Index];
			if (bInBound = InBound(Cursor, Probe))
			{
				MinInto(Lower, Cursor);
				MaxInto(Upper, Cursor);
			}
			if (_Intersection(Previous, Cursor, Probe, Point))
			{
				
			}
			Previous = Cursor;
		}
	}
}

FBoolean FMovement2D::_Intersection(const FShape::FPoint &Previous, const FShape::FPoint &Cursor, const FShape::FPoint (&Probe)[2], FShape::FPoint &Point)
{
	return False;
}