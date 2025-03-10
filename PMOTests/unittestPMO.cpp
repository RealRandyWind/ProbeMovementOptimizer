#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "NDev.h"
#include "PMOShape.h"
#include "PMOKDTree.h"
#include "PMODefinitions.h"

namespace PMOTests
{	
	using namespace ProbeMovementOptimizer;
	using namespace NDev;
	using namespace NDev::Types;

	TEST_CLASS(UnitTestPMO)
	{
	public:
		
		TEST_METHOD(TestKDTree)
		{
			
			const FSize H1D = 1, H2D = 2, H3D = 3, HND = 128, N = 1024, M = 32;
			const FReal Zero = 0, One = 1, Two = 2, Three = 3, Four = 4, Radius = 1, Alpha = Radius * (1.0 / (M + 1.0));
			FSize Index, End;
			FReal Beta;
			TNormal<FReal> Distribution;

			{
				using FKDTree = TKDTree<HND>;

				TSequence<FKDTree::FPoint> Points(N * M, True), Targets(M, True), Values;
				FKDTree::FPoint Value;

				Distribution.Seed(HND);
				End = M;
				for (Index = 0; Index < End; ++Index)
				{
					auto &Target = Targets[Index];
					Distribution(Target);
					NormalizeInto(Target);
					Target *= Radius * Index;
				}

				Index = 0;
				for (auto &Target : Targets)
				{
					End += N;
					Beta = Alpha;
					for (; Index < End; ++Index)
					{
						auto &Point = Points[Index];
						Distribution(Point);
						NormalizeInto(Point);
						Point += Target;
						Point *= Beta;
						Beta += Alpha;
					}
				}
				
				FKDTree KDTree(Points);

				for (const auto &Target : Targets)
				{
					KDTree.Nearest(Target, Value);
					//Assert::AreEqual(Alpha, Norm(Value), NullPtr, LINE_INFO());
					/*
					KDTree.Nearest(Target, Alpha, Values)
					Assert::IsTrue(Values.Empty(), NullPtr, LINE_INFO());
					
					Values.Reset();
					KDTree.Nearest(Target, Radius, Values);
					for (const auto &_Value : Values)
					{
						Assert::IsTrue(Norm(Value) < Radius, NullPtr, LINE_INFO());
					}
					
					Values.Reset();
					KDTree.Nearest(Target, Values, M / 2);
					for (const auto &_Value : Values)
					{
						Assert::IsTrue(Norm(Value) < (Radius / 2.0), NullPtr, LINE_INFO());
					}
					
					Values.Reset();
					KDTree.Nearest(Target, Radius, Values, M / 2);
					for (const auto &_Value : Values)
					{
						Assert::IsTrue(Norm(Value) < (Radius / 2.0), NullPtr, LINE_INFO());
					}
					*/
				}
				
			}

		}

		TEST_METHOD(TestShape)
		{
			const FReal Zero = 0, One = 1, Two = 2, Three = 3, Four = 4;
			const FSize H1D = 1, H2D = 2, H3D = 3, HND = 2048, HKD = 128;
			FSize Index, End;

			{
				using FShape = TShape<HND>;
				FShape Shape;
				FShape::FPath Path;
				FShape::FPoint Center, Lower, Upper;
				Shape.Points.Reserve(HKD, True);
				for (auto &Point : Shape.Points) { Point = Two; }
				Shape.Mean(Center);
				for (const auto &Value : Center)
				{
					Assert::AreEqual(Two, Value, NullPtr, LINE_INFO());
				}
				Shape.Bounds(Lower, Upper);
				for (const auto &Value : Lower)
				{
					Assert::AreEqual(Two, Value, NullPtr, LINE_INFO());
				}
				for (const auto &Value : Upper)
				{
					Assert::AreEqual(Two, Value, NullPtr, LINE_INFO());
				}
				Shape.Boundaries.Reserve(HKD, True, True);
				for (auto &Boundary : Shape.Boundaries)
				{
					Boundary.Operation = FShape::FBoundary::EOperation::Inclusion;
					Boundary.Indices.Reserve(HKD, True);
					End = HKD;
					for (Index = 0; Index < End; ++Index) { Boundary.Indices[Index] = Index; }
				}
			}
			
			
		}

		TEST_METHOD(TestIntersect)
		{
			const FSize K = 2;
			TPoint<K, FReal> P = { 1.0, 1.0 };
			FBoolean bParallel, bIntersect;

			{ // parallel no overlap test
				const TPoint<K, FReal> P1 = { 1.0, 0.0 }, P2 = { 1.0, 1.0 }, P3 = { 0.5, -0.5 }, P4 = { 0.5, 0.5 };
				bIntersect = Intersect(P1, P2, P3, P4, P, bParallel);
				Assert::IsTrue(bParallel, NullPtr, LINE_INFO());
				Assert::IsFalse(bIntersect, NullPtr, LINE_INFO());
				bIntersect = Intersect(P1, P2, P3, P4, P, bParallel, False);
				Assert::IsTrue(bIntersect, NullPtr, LINE_INFO());
			}

			{ // parallel overlap test
				const TPoint<K, FReal> P1 = { 0.5, 0.0 }, P2 = { 0.5, 1.0 }, P3 = { 0.5, -0.5 }, P4 = { 0.5, 0.5 };
				bIntersect = Intersect(P1, P2, P3, P4, P, bParallel);
				Assert::IsTrue(bParallel, NullPtr, LINE_INFO());
				Assert::IsTrue(bIntersect, NullPtr, LINE_INFO());
				bIntersect = Intersect(P1, P2, P3, P4, P, bParallel, False);
				Assert::IsTrue(bIntersect, NullPtr, LINE_INFO());
			}

			{ // intersect 90, 0.0
				const TPoint<K, FReal> P1 = { 0.0, -0.5 }, P2 = { 0.0, 0.5 }, P3 = { -0.5, 0.0 }, P4 = { 0.5, 0.0 };
				bIntersect = Intersect(P1, P2, P3, P4, P, bParallel);
				Assert::IsFalse(bParallel, NullPtr, LINE_INFO());
				Assert::IsTrue(bIntersect, NullPtr, LINE_INFO());
				bIntersect = Intersect(P1, P2, P3, P4, P, bParallel, False);
				Assert::IsTrue(bIntersect, NullPtr, LINE_INFO());
				Assert::IsTrue(Equal({ 0.0, 0.0 }, P));
			}

			{ // intersect 90, 0.5
				const TPoint<K, FReal> P1 = { 0.5, -1.0 }, P2 = { 0.5, 1.0 }, P3 = { -1.0, 0.5 }, P4 = { 1.0, 0.5 };
				bIntersect = Intersect(P1, P2, P3, P4, P, bParallel);
				Assert::IsFalse(bParallel, NullPtr, LINE_INFO());
				Assert::IsTrue(bIntersect, NullPtr, LINE_INFO());
				bIntersect = Intersect(P1, P2, P3, P4, P, bParallel, False);
				Assert::IsTrue(bIntersect, NullPtr, LINE_INFO());
				Assert::IsTrue(Equal({ 0.5, 0.5 }, P));
			}

			{ // intersect 45, 0.0
				const TPoint<K, FReal> P1 = { -0.5, 0.5 }, P2 = { 0.5, -0.5 }, P3 = { -0.5, 0.0 }, P4 = { 0.5, 0.0 };
				bIntersect = Intersect(P1, P2, P3, P4, P, bParallel);
				Assert::IsFalse(bParallel, NullPtr, LINE_INFO());
				Assert::IsTrue(bIntersect, NullPtr, LINE_INFO());
				bIntersect = Intersect(P1, P2, P3, P4, P, bParallel, False);
				Assert::IsTrue(bIntersect, NullPtr, LINE_INFO());
				Assert::IsTrue(Equal({ 0.0, 0.0 }, P));
			}

			{ // no intersect
				const TPoint<K, FReal> P1 = { 0.0, 0.0 }, P2 = { 0.0, -1.0 }, P3 = { -0.5, 0.5 }, P4 = { 0.5, 0.5 };
				bIntersect = Intersect(P1, P2, P3, P4, P, bParallel);
				Assert::IsFalse(bParallel, NullPtr, LINE_INFO());
				Assert::IsFalse(bIntersect, NullPtr, LINE_INFO());
				bIntersect = Intersect(P1, P2, P3, P4, P, bParallel, False);
				Assert::IsFalse(bIntersect, NullPtr, LINE_INFO());
			}

		}

	};
}