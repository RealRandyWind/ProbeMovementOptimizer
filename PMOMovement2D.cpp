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
	if (Shape.Facets.Empty()) { return; }

	_Coverage(Shape, Path);
	_Optimize(Path);

	if (this->OnUse) { this->OnUse(Shape, Path); }
}

FVoid FMovement2D::_Coverage(const FShape &Shape, FPath &Path)
{
	const FReal Infinity = TLimit<FReal>::Infinity();
	TList<FPath::FValue> List;
	TStack<_FSample> Stack;
	_FSample Sample;

	const auto &Point = Shape.Points[0];
	Sample.Index = List.Make(Point);
	Sample.Intersect = 0;
	Sample.Frame[0] = Infinity;
	Sample.Frame[1] = -Infinity;
	Sample.Anchor[0] = Sample.Anchor[1] = Point;

	Stack.Push(Sample);
	while (!Stack.Empty())
	{
		Sample = Stack.Pop();
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
	FSize Index, End;
	FBoolean bInBound, bContained, bClockwise;
	
	auto &Center = List[Sample.Index];

	End = 4;
	for (Index = 0; Index < End; ++Index)
	{
		auto &_Sample = Samples[Index];
		_Sample.Intersect = 0;
		_Sample.Anchor[0] = _Sample.Frame[0] = Infinity;
		_Sample.Anchor[1] = _Sample.Frame[1] = -Infinity;
	}

	const auto Lower = State.Probe[1] + Center;
	const auto Upper = State.Probe[3] + Center;

	/* find intersections (cliping with probe as clip plane) and find inbound points to
	 * determine lower and upper bounds, trace ancker points remember clockwize priority
	 * for sampling by pusing on the stack.
	 * 
	 * TODO reduce search space of nearest facet
	 */
	for (const auto &Facet : Shape.Facets)
	{
		bContained = True;
		for (const auto &Index : Facet)
		{
			const auto &P = Shape.Points[Index];
			bInBound = InBound(P, Lower, Upper, False);
			if (bInBound) { BoundInto(Sample.Frame, P); }
			bContained &= bInBound;
		}
		if (!bContained) { _Intersections(Facet, Center, Samples, Shape); }
	}
	
	End = 4;
	for (Index = 0; Index < End; ++Index)
	{
		/* add only samples with intersections (edgecases) to stack 
		 *
		 * TODO add corrections of current sample placement before
		 */
		auto &_Sample = Samples[Index];
		if (_Sample.Intersect)
		{
			_Sample.Index = List.Make();
			Stack.Push(_Sample);
		}
	}
}

FVoid FMovement2D::_Intersections(const FShape::FFacet &Facet, const FShape::FPoint &Center, _FSample (&Samples)[4], const FShape &Shape)
{
	FSize Index, End, Pivot;
	FBoolean bIntersect, bParallel;
	FShape::FPoint P;

	const auto &P3 = Shape.Points[Facet[0]];
	const auto &P4 = Shape.Points[Facet[1]];

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
			BoundInto(Sample.Anchor, P);
		}
		Pivot = Index;
	}
}