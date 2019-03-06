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
		
		TEST_METHOD(TestMovement2D)
		{
			FShape2D Shape;
			FShape2D::FPath Path;

			

			Shape.Points.Reserve(4, True);
			Shape.Points[0] = { 0.5, -0.5 };
			Shape.Points[1] = { 0.5, 0.5 };
			Shape.Points[2] = { -0.5, 0.5 };
			Shape.Points[3] = { -0.5, -0.5 };
			Shape.Facets.Reserve(4, True);
			Shape.Facets[0] = { 0, 1 };
			Shape.Facets[1] = { 1, 2 };
			Shape.Facets[2] = { 2, 3 };
			Shape.Facets[3] = { 3, 0 };

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

		TEST_METHOD(TestFractal)
		{
			// TODO: Your test code here
		}

		TEST_METHOD(TTestFractalInverted)
		{
			// TODO: Your test code here
		}

	};



}