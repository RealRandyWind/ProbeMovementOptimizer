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
	TList<FPath::FValue> List;
	TStack<_FSample> Stack;

	const auto &Point = Shape.Points[0];
	Stack.Push(_FSample{ List.Make(Point), 0, Point - Parameters.Diameter, Point + Parameters.Diameter, False });
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
	_FSample Samples[4];
	FShape::FPoint Lower, Upper, Cursor, Previous;
	FReal Radius;
	FSize Index, End;
	FBoolean bPrevious, bCursor, bContained;
	
	auto &Center = List[Sample.Index];

	bPrevious = True;
	bContained = bCursor = False;
	Radius = Parameters.Diameter * 0.5;

	End = 4;
	for (Index = 0; Index < End; ++Index)
	{
		State.Probe[Index] += Center;
		auto &_Sample = Samples[Index];
		_Sample.bIntersect = False;
		_Sample.Lower = TLimit<FReal>::Infinity();
		_Sample.Upper = -TLimit<FReal>::Infinity();
		_Sample.Edge = (Sample.Edge + Index) % End;
	}

	/* find intersections (cliping with probe as clip plane) and find inbound points to
	 * determine lower and upper bounds, trace ancker points remember clockwize priority
	 * for sampling by pusing on the stack.
	 * 
	 * TODO reduce search space of nearest edges
	 */
	for (const auto &Boundary : Shape.Boundaries)
	{
		Previous = Shape.Points[Boundary.Indices.Size() - 1];
		for (const auto &Index : Boundary.Indices)
		{
			Cursor = Shape.Points[Index];
			if (bCursor = InBound(Cursor, State.Probe[1], State.Probe[3], False))
			{
				MinInto(Lower, Cursor);
				MaxInto(Upper, Cursor);
			}
			
			bContained = bCursor && bPrevious;
			/* TODO flag isolated edges to reduce search space for later placements
			 */
			if (!bContained)
			{
				_Intersections(Previous, Cursor, Center, Samples);
			}
			bPrevious = bCursor;
			Previous = Cursor;
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
		if (_Sample.bIntersect)
		{
			_Sample.Index = List.Make();
			Stack.Push(_Sample);
		}
	}
}

FVoid FMovement2D::_Intersections(const FShape::FPoint &P3, const FShape::FPoint &P4, const FShape::FPoint &Center, _FSample (&Samples)[4])
{
	const FReal Eps = TLimit<FReal>::Epsilon();
	FSize Index, End, Pivot;
	FBoolean bIntersect;
	FShape::FPoint Point, D34, D13, D12, D21, C;
	FReal Alpha;

	D34 = P3 - P4;
	End = 4;
	Pivot = End - 1;
	for (Index = 0; Index < End; ++Index)
	{
		auto &Sample = Samples[Index];
		const auto &P1 = State.Probe[Pivot] + Center;
		const auto &P2 = State.Probe[Index] + Center;

		D13 = P1 - P3;
		D12 = P1 - P2;
		D21 = P2 - P1;

		// Alpha = Det(D12 D34)
		Alpha = D12[0] * D34[1] - D12[1] * D34[0];
		if (IsZeroEps(Alpha, Eps))
		{
			// paralel lines TODO two cases, on paralel non overlap return
			// on overlap
		}
		// C = { Det(D13 D34), Det(D12 D13) } * (1.0 / Alpha)
		C[0] = (D13[0] * D34[1] - D13[1] * D34[0]) * (1.0 / Alpha);
		C[1] = -(D12[0] * D13[1] - D12[1] * D13[0]) * (1.0 / Alpha);

		Point = P1 + C * D21;

		bIntersect = All(Between(C, 0.0, 1.0));
		if (bIntersect)
		{
			Sample.bIntersect = True;
			MinInto(Sample.Lower, Point);
			MaxInto(Sample.Upper, Point);
		}
		Pivot = Index;
	}
}