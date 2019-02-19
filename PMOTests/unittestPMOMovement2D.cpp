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
			TData<FShape2D> Shapes;
			TData<FShape2D::FPath> Paths;

			auto Movement2D = FMovement2D();
			Movement2D.Initialize();
			Movement2D.Use(Shapes, Paths);
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