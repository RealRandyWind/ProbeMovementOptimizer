#pragma once

#include "NDevTypes.h"
#include "NDevPoint.h"
#include "NDevSequence.h"
#include "NDevList.h"
#include "NDevStack.h"
#include "NDevReferences.h"
#include "NDevMathExtend.h"

namespace ProbeMovementOptimizer
{
	using namespace NDev;
	using namespace NDev::Types;

	template<FSize SizePoint>
	struct TKDTree
	{
		using FPoint = typename TPoint<SizePoint, FReal>;

		struct FNode
		{
			FPoint Position;
			union
			{
				TReferences<2, FNode> Children;
				struct { FNode *Left, *Right; };
			};
		};
		
		struct _FEvent
		{
			FSize From, To, Depth, Index;
		};

		TSequence<FNode> _Nodes;
		TSequence<FPoint> _Points;

		TKDTree(TSequence<FPoint> &Points)
		{
			_Points.Data(Points.Descriptor());
			_Nodes.Reserve(Points.Size(), True);
		}

		~TKDTree() { }

		virtual FVoid Rebuilt()
		{
			TStack<_FEvent> Stack;
			TList<FPoint> List;
			_FEvent Event;
			FSize Median, Axis, Index, _Index;

			if (_Points.Empty()) { return;  }

			Index = 0;
			Stack.Push(_FEvent{ 0, _Points.Size() - 1, 0, Index });
			while (Stack.Full())
			{
				Event = Stack.Pop();
				List.Data(_Points.Data() + Event.From, Event.To + 1);

				Axis = Event.Depth % SizePoint;
				List.OnPriority = [Axis](const auto &Lhs, const auto &Rhs) { return Lhs[Axis] < Rhs[Axis]; };
				List.Sort();
				
				Median = List.Size() >> 2;
				auto &Node = _Nodes[Event.Index];
				Node.Position = List[Median];
				Node.Left = Node.Right = NullPtr;

				if (Event.From < Median)
				{
					_Index = ++Index;
					Stack.Push(_FEvent{ Event.From, Median, Event.Depth + 1, _Index });
					Node.Left = &_Nodes[_Index];
				}
				if (Median + 1 < Event.To)
				{
					_Index = ++Index;
					Stack.Push(_FEvent{ Median + 1, Event.To, Event.Depth + 1, _Index });
					Node.Right = &_Nodes[_Index];
				}
			}
		}

		virtual FVoid Nearest(const FPoint& Point, FSize &Index) const;

		virtual FVoid Nearest(const FPoint& Point, TSequence<FSize> &Indices, FSize) const;

		virtual FVoid Nearest(const FPoint& Point, FReal Radius, FSize &Index) const;

		virtual FVoid Nearest(const FPoint& Point, FReal Radius, TSequence<FSize> &Indices, FSize = TLimit<FSize>::Infinity) const;


	};



}