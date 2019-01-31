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

		struct _FNode
		{
			FPoint Position;
			_FNode *Parent;
			union
			{
				TReferences<2, _FNode> Children;
				struct { _FNode *Left, *Right; };
			};
		};
		
		struct _FEvent
		{
			FSize From, To, Depth, Index;
			_FNode *Parent;
		};

		TSequence<_FNode> _Nodes;
		TSequence<FPoint> _Points;

		TKDTree()
		{
			
		}

		TKDTree(TSequence<FPoint> &Points)
		{
			Rebuilt(Points);
		}

		~TKDTree() { }

		FVoid Rebuilt(TSequence<FPoint> &Points)
		{
			_Points.Data(Points.Descriptor());
			_Nodes.Reserve(Points.Size(), True);
			Rebuilt();
		}

		virtual FVoid Rebuilt()
		{
			TStack<_FEvent> Stack;
			TList<FPoint> List;
			_FEvent Event;
			FSize Median, Axis, Index, _Index;

			if (_Points.Empty()) { return;  }

			Index = 0;
			Stack.Push(_FEvent{ 0, _Points.Size() - 1, 0, Index, NullPtr });
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
				Node.Parent = Event.Parent;

				if (Event.From < Median)
				{
					_Index = ++Index;
					Stack.Push(_FEvent{ Event.From, Median, Event.Depth + 1, _Index, &Node });
					Node.Left = &_Nodes[_Index];
				}
				if (Median + 1 < Event.To)
				{
					_Index = ++Index;
					Stack.Push(_FEvent{ Median + 1, Event.To, Event.Depth + 1, _Index, &Node });
					Node.Right = &_Nodes[_Index];
				}
			}
		}

		virtual FVoid Nearest(const FPoint& Point, FPoint &Value) const
		{
			if (_Points.Empty()) { return;  }
			auto &Root = _Nodes[0];


		}

		virtual FVoid Nearest(const FPoint& Point, TSequence<FPoint> &Values, FSize Size) const
		{
			Values.Reserve(Size);
			if (_Points.Empty()) { return; }
			auto &Root = _Nodes[0];
			
		}

		virtual FVoid Nearest(const FPoint& Point, FReal Radius, FPoint &Value) const
		{
			if (_Points.Empty()) { return; }
			auto &Root = _Nodes[0];
		}

		virtual FVoid Nearest(const FPoint& Point, FReal Radius, TSequence<FPoint> &Values, FSize Size = TLimit<FSize>::Infinity) const
		{
			Values.Reserve(Size);
			if (_Points.Empty()) { return; }
			auto &Root = _Nodes[0];
		}


	};



}