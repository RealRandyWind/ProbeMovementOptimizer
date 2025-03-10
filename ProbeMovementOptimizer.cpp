#include "pch.h"

#include "ProbeMovementOptimizer.h"
#include "PMOShape.h"
#include "PMOMovement2D.h"

using namespace ProbeMovementOptimizer;

FReturn main()
{
	TData<FShape2D> Shapes;
	TData<FShape2D::FPath> Paths;

	Generate(Shapes, EGenerate::Square);

	auto Movement2D = FMovement2D();
	Movement2D.Use(Shapes, Paths);
	
	return Success;
}
