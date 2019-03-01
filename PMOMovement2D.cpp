#include "pch.h"

#include "NDevPoint.h"
#include "NDevMathPoint.h"
#include "NDevMathExtend.h"
#include "PMOMovement2D.h"
#include "PMODefinitions.h"

using namespace ProbeMovementOptimizer;

FVoid FMovement2D::UseDefaultParameters()
{
	Parameters.Start = 0.0;
	Parameters.End = 0.0;
	Parameters.Diameter = 1.0;
	Parameters.Alpha = 0.01;
	Parameters.Beta = TLimit<FReal>::Infinity();
}

FVoid FMovement2D::_Initialize()
{
	State.Probe[0] = { 0.5, -0.5 };
	State.Probe[1] = { 0.5, 0.5 };
	State.Probe[2] = { -0.5, 0.5 };
	State.Probe[3] = { -0.5, -0.5 };
}

FVoid FMovement2D::_Use(const FShape &Shape, FPath &Path)
{
	if (Shape.Boundaries.Empty()) { return; }

	_Coverage(Shape, Path);
	_Optimize(Path);

	if (this->OnUse) { this->OnUse(Shape, Path); }
}

FVoid FMovement2D::_Coverage(const FShape &Shape, FPath &Path)
{
	const FReal Radius = Parameters.Diameter * 0.5;
	TList<FPath::FValue> List;
	TStack<_FSample> Stack;

	const auto &Point = Shape.Points[0];
	Stack.Push(_FSample{ List.Make(Point), 0, 0, Point + Radius, Point - Radius });
	while (!Stack.Empty())
	{
		auto Sample = Stack.Pop();
		_Place(Sample, Shape, List, Stack);
	}
	Path.Data(List.Descriptor());
}

FVoid FMovement2D::_Optimize(FPath &Path)
{
	FShape::FPoint From, To;
	FReal Lenght, DLenght;

	From = Parameters.Start;
	Lenght = DLenght = 0;
	for (const auto &Point : Path)
	{
		Lenght += Norm(To - From);
		From = To;
		To = Point;
	}
	Lenght += Norm(Parameters.End - From);
}

FVoid FMovement2D::_Place(_FSample &Sample, const FShape &Shape, TList<FPath::FValue> &List, TStack<_FSample> &Stack)
{
	const FReal Infinity = TLimit<FReal>::Infinity();
	_FSample Samples[4];
	FShape::FPoint Lower, Upper, Cursor, From, To;
	FReal Radius;
	FSize Index, End;
	FBoolean bFrom, bTo, bContained;
	
	auto &Center = List[Sample.Index];

	bFrom = True;
	bContained = bTo = False;

	End = 4;
	for (Index = 0; Index < End; ++Index)
	{
		auto &_Sample = Samples[Index];
		_Sample.Intersect = 0;
		_Sample.Lower = Infinity;
		_Sample.Upper = -Infinity;
		_Sample.Edge = (Sample.Edge + Index) % End;
	}

	/* find intersections (cliping with probe as clip plane) and find inbound points to
	 * determine lower and upper bounds, trace ancker points remember clockwize priority
	 * for sampling by pusing on the stack.
	 * 
	 * TODO reduce search space of nearest edges
	 */
	for (const auto &Facet : Shape.Facets)
	{
		From = Shape.Points[Facet[0]];
		To = Shape.Points[Facet[1]];

		if (bTo = InBound(To, State.Probe[1], State.Probe[3], False))
		{
			MinInto(Lower, To);
			MaxInto(Upper, To);
		}

		bContained = bFrom && bTo;
		if (!bContained)
		{
			_Intersections(From, To, Center, Samples);
		}
	}
	
	End = 4;
	for (Index = 0; Index < End; ++Index)
	{
		/* add only samples with intersections (edgecases) to stack 
		 *
		 * TODO add corrections of current sample placement
		 */
		auto &_Sample = Samples[Index];
		if (_Sample.Intersect)
		{
			_Sample.Index = List.Make();
			Stack.Push(_Sample);
		}
	}
}

FVoid FMovement2D::_Intersections(const FShape::FPoint &P3, const FShape::FPoint &P4, const FShape::FPoint &Center, _FSample (&Samples)[4])
{
	FSize Index, End, Pivot;
	FBoolean bIntersect, bParallel;
	FShape::FPoint P;

	End = 4;
	Pivot = End - 1;
	for (Index = 0; Index < End; ++Index)
	{
		auto &Sample = Samples[Index];
		const auto P1 = State.Probe[Pivot] + Center;
		const auto P2 = State.Probe[Index] + Center;

		bIntersect = Intersect(P1, P2, P3, P4, P, bParallel, False);
		if (bIntersect)
		{
			++Sample.Intersect;
			MinInto(Sample.Lower, P);
			MaxInto(Sample.Upper, P);
		}
		Pivot = Index;
	}
}