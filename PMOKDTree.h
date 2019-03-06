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
		using FValue = typename TPoint<SizePoint, FReal>;

		using FPoint = FValue;

		struct _FNode
		{
			FSize Index;
			union
			{
				TReferences<2, _FNode> Children;
				struct { _FNode *Left, *Right; };
			};
			_FNode *Parent;
		};
		
		struct _FEvent
		{
			FSize From, To, Depth, Index;
			_FNode *Parent;
		};

		TSequence<_FNode> _Nodes;
		const TSequence<FPoint> &_Points;

		TKDTree(const TSequence<FPoint> &Points) : _Points(Points)
		{
			Rebuilt();
		}

		~TKDTree() { }

		virtual FVoid Rebuilt()
		{
			TStack<_FEvent> Stack;
			TSequence<FSize> Targets;
			TList<FSize> Indices;
			TList<_FNode> Nodes;
			_FEvent Event;
			FSize Median, Axis, Index, _Index;

			if (_Points.Empty()) { return;  }

			Targets = _Points.Indices();
			Nodes.Data(_Nodes.Descriptor());
			Stack.Push(_FEvent{ 0, Targets.Size() - 1, 0, Index, NullPtr });
			while (Stack.Full())
			{
				Event = Stack.Pop();
				Indices.Data(Targets.Data() + Event.From, Event.From - Event.To + 1);

				Axis = Event.Depth % SizePoint;
				Indices.OnPriority = [this, Axis](const auto &Lhs, const auto &Rhs) { return _Points[Lhs][Axis] < _Points[Rhs][Axis]; };
				Indices.Sort();
				
				Median = Indices.Size() >> 2;
				auto &Node = Nodes[Event.Index];
				Node.Index = Indices[Median];
				Node.Children = NullPtr;
				Node.Parent = Event.Parent;

				if (Event.From < Median)
				{
					_Index = Nodes.Make(_FNode());
					Stack.Push(_FEvent{ Event.From, Median, Event.Depth + 1, _Index, &Node });
					Node.Left = &Nodes[_Index];
				}
				if (Median + 1 < Event.To)
				{
					_Index = Nodes.Make(_FNode());
					Stack.Push(_FEvent{ Median + 1, Event.To, Event.Depth + 1, _Index, &Node });
					Node.Right = &Nodes[_Index];
				}
			}
		}

		virtual FVoid Nearest(const FPoint& Point, FPoint &Value) const
		{
			if (_Points.Empty()) { return;  }
			const auto &Root = _Nodes[0];
		}

		virtual FVoid Nearest(const FPoint& Point, FSize &Index) const
		{
			if (_Points.Empty()) { return; }
			const auto &Root = _Nodes[0];
		}

		virtual FVoid Nearest(const FPoint& Point, TSequence<FPoint> &Values, FSize Size) const
		{
			if (_Points.Empty()) { return; }
			Values.Reserve(Size);
			const auto &Root = _Nodes[0];
		}

		virtual FVoid Nearest(const FPoint& Point, TSequence<FSize> &Indices, FSize Size) const
		{
			if (_Points.Empty()) { return; }
			Indices.Reserve(Size);
			const auto &Root = _Nodes[0];
		}

		virtual FVoid Nearest(const FPoint& Point, FReal Radius, FPoint &Value) const
		{
			if (_Points.Empty()) { return; }
			const auto &Root = _Nodes[0];
		}

		virtual FVoid Nearest(const FPoint& Point, FReal Radius, FSize &Index) const
		{
			if (_Points.Empty()) { return; }
			const auto &Root = _Nodes[0];
		}

		virtual FVoid Nearest(const FPoint& Point, FReal Radius, TSequence<FPoint> &Values, FSize Size = TLimit<FSize>::Infinity) const
		{
			if (_Points.Empty()) { return; }
			Values.Reserve(Size);
			const auto &Root = _Nodes[0];
		}

		virtual FVoid Nearest(const FPoint& Point, FReal Radius, TSequence<FSize> &Indices, FSize Size = TLimit<FSize>::Infinity) const
		{
			if (_Points.Empty()) { return; }
			Indices.Reserve(Size);
			const auto &Root = _Nodes[0];
		}


	};



}