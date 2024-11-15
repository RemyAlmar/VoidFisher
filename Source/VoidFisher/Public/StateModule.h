// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraDataInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "StateModule.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, Category = "Niagara", meta = (DisplayName = "StateBoids"))
class VOIDFISHER_API UStateModule : public UNiagaraDataInterface
{
	GENERATED_BODY()
public:
	virtual void PostInitProperties() override;

	// Implémentations pour que Niagara utilise ce module
	virtual bool Equals(const UNiagaraDataInterface* Other) const override;
	virtual bool CopyTo(UNiagaraDataInterface* Destination) const override;
	virtual bool CanExecuteOnTarget(ENiagaraSimTarget Target) const override;
};
