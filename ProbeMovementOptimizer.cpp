#include "pch.h"

#include "PMOShape.h"
#include "PMOMovement2D.h"

using namespace ProbeMovementOptimizer;

FReturn main()
{
	TData<FShape2D> Shapes;
	TData<FShape2D::FPath> Paths;

	/*Generate(Shapes, 8)*/
	auto Movement = FMovement2D();
	Movement.Use(Shapes, Paths);

	return Success;
}
