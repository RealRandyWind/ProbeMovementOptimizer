#include "pch.h"

#include "PMOShape.h"
#include "PMOMovement2D.h"

using namespace ProbeMovementOptimizer;

FReturn main()
{
	TData<FShape2D> Shapes;
	TData<FShape2D::FPath> Paths;

	FMovement2D Movement;
	Movement.UseDefaultParameters();
	Movement.Initialize();
	Movement.Use(Shapes, Paths);

	return Success;
}
