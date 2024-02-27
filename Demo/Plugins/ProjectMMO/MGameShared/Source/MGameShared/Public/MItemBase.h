#pragma once
#include "MItemBase.generated.h"

UCLASS()
class MGAMESHARED_API UMItemBase : public UObject
{
	GENERATED_BODY()
	
public:

	UMItemBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	UMItemBase(FVTableHelper& Helper);
	virtual ~UMItemBase() override;

	virtual bool Init();

	
};

