#pragma once

#include "NDevTypes.h"
#include "NDevPoint.h"
#include "NDevSequence.h"
#include "NDevData.h"
#include "NDevResource.h"

namespace ProbeMovementOptimizer
{

	using namespace NDev;
	using namespace NDev::Types;

	template<typename TypeInput, typename TypeResult>
	struct TAlgorithm : public FResource
	{
		using FInput = TypeInput;
		
		using FResult = TypeResult;

		using FOnUse = TFunction<FVoid(const FInput &, const FResult &)>;

		FOnUse OnUse;
		FBoolean _bInitialized;

		TAlgorithm()
		{
			_bInitialized = False;
			OnUse = NullPtr;
			UseDefaultParameters();
		}

		~TAlgorithm() { }

		FBoolean Initialized()
		{
			return _bInitialized;
		}

		const FBoolean Initialized() const
		{
			return _bInitialized;
		}

		FSize InputSize()
		{
			return sizeof(FInput);
		}

		const FSize InputSize() const
		{
			return sizeof(FInput);
		}

		FSize ResultSize()
		{
			return sizeof(FResult);
		}

		const FSize ResultSize() const
		{
			return sizeof(FResult);
		}

		FVoid Initialize()
		{
			_bInitialized = False;
			_Initialize();
			_bInitialized = True;
		}

		FVoid Use(const TData<FInput> &Inputs, TData<FResult> &Results)
		{
			FSize Index, End;

			if (!_bInitialized) { return; }

			End = Inputs.Size();
			Results.Reserve(End, True);
			for (Index = 0; Index < End; ++Index)
			{
				_Use(Inputs[Index], Results[Index]);
			}
		}

		FVoid Use(const FInput &Input, FResult &Result)
		{
			if (!_bInitialized) { return; }

			_Use(Input, Result);
		}

		virtual FVoid UseDefaultParameters() { };

	protected:
		virtual FVoid _Initialize() { };

		virtual FVoid _Use(const FInput &, FResult &) = 0;


	};



}