// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ReadWriteDataFile.generated.h"

/**
 * 
 */
UCLASS()
class SR_PROJECT_API UReadWriteDataFile : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, Category = "Read Write File")
		static FString ReadStringFromFile(FString FilePath, bool& bOutSucess, FString& OutInfoMessage);

	UFUNCTION(BlueprintCallable, Category = "Read Write File")
		static void WriteStringFromFile(FString FilePath, FString String, bool& bOutSucess, FString& OutInfoMessage);
};
