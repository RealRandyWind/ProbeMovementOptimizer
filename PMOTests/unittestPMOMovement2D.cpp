#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "NDev.h"
#include "PMOShape.h"
#include "PMOAlgorithm.h"
#include "PMOMovement2D.h"
#include "ProbeMovementOptimizer.h"

namespace PMOTests
{
	using namespace ProbeMovementOptimizer;
	using namespace NDev;
	using namespace NDev::Types;

	TEST_CLASS(UnitTestPMOMovement2D)
	{
	public:
		
		TEST_METHOD(TestBase)
		{
			FShape2D Shape;
			FShape2D::FPath Path;

			Shape.Points.Reserve(4, True);
			Shape.Boundaries.Reserve(1, True);
			Shape.Boundaries[0] = Shape.Boundaries.Element();
			Shape.Boundaries[0].Indices.Reserve(4, True);
			Shape.Points[0] = { 1.0, 0.0 };
			Shape.Points[1] = { 1.0, 1.0 };
			Shape.Points[2] = { 0.0, 1.0 };
			Shape.Points[3] = { 0.0, 0.0 };
			Shape.Boundaries[0].Operation = FShape2D::FBoundary::EOperation::Inclusion;
			Shape.Boundaries[0].Indices[0] = 0;
			Shape.Boundaries[0].Indices[1] = 1;
			Shape.Boundaries[0].Indices[2] = 2;
			Shape.Boundaries[0].Indices[3] = 3;

			auto Movement2D = FMovement2D();
			Movement2D.Initialize();
			Movement2D.Use(Shape, Path);
		}

		TEST_METHOD(TestSquare)
		{
			// TODO: Your test code here
		}

		TEST_METHOD(TestSquareInverted)
		{
			// TODO: Your test code here
		}

		TEST_METHOD(TestSpiral)
		{
			// TODO: Your test code here
		}

		TEST_METHOD(TestSpiralInverted)
		{
			// TODO: Your test code here
		}

		TEST_METHOD(TestArray)
		{
			// TODO: Your test code here
		}

		TEST_METHOD(TestArrayInverted)
		{
			// TODO: Your test code here
		}

		TEST_METHOD(TestArch)
		{
			// TODO: Your test code here
		}

		TEST_METHOD(TestArchInverted)
		{
			// TODO: Your test code here
		}


	};



}