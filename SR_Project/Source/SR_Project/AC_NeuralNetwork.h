// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_NeuralNetwork.generated.h"

USTRUCT(BlueprintType)
struct FWeights																						//struct that holds an array of float
{
	GENERATED_BODY()

		UPROPERTY()
		TArray<float> weightValues;

};

USTRUCT(BlueprintType)
struct FNeuralLayer																					//struct that holds node values(array of floats for each node);
{																									//and each nodes weight values(array of array of floats)
	GENERATED_BODY()

		UPROPERTY()
		TArray<float> Values;

		UPROPERTY()
		TArray<FWeights> Weights;
};

UENUM(BlueprintType)
enum class E_NeuralNetwork : uint8 {
	E_Sigmoid UMETA(DisplayName = "Sigmoid"),
	E_ReLU UMETA(DisplayName = "ReLU"),
	E_tanH UMETA(DisplayName = "tanH"),
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SR_PROJECT_API UAC_NeuralNetwork : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)														//holds expected error values for each network to be used during back propogation 
		float expectedValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)																						//holds the input layer nodes weights and values
		TArray<FNeuralLayer> inputLayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)																						//holds the hidden layer nodes weights and values
		TArray<FNeuralLayer> hiddenLayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)																						//holds the output layer nodes weights and values
		TArray<FNeuralLayer> outputLayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		E_NeuralNetwork NNType;

	UAC_NeuralNetwork();

private:

	UPROPERTY()																						//The learn rate for back propogation; can change later
		float learnRate;

	UFUNCTION(BlueprintCallable)																	//initialize neural network with set input, hidden and output node
	void InitNeuralNetwork(int32 numIn, int32 numHid, int32 numOut, float expected, E_NeuralNetwork type);

	TArray<FNeuralLayer> GenLayer(int32 nodes, int32 prevLayerSize);								//generate a neural netwrok layer

	FWeights GenWeights(int32 num);																	// Generate random weights for a layer

	void GenExpectedValue(float expected);															// Generate random weights for a layer																			 

	//------------------------------------------------------------------------------------------------------------

	UFUNCTION(BlueprintCallable)
	float FwdPropNN(TArray<float> inputValues);														//Forward prop through layers; can be called via blueprint

	void InitInputLayer(TArray<float> values);														//Give the input layer values; initial wieghts are random

	void FwdPropLayer(FNeuralLayer& fromLayer, FNeuralLayer& toLayer);								//Actual forward propagation calculations 
	
	UFUNCTION(BlueprintCallable)																	//Back prop through layes and changes thier weight values; can be called via blueprint
	void BackPropNN();

	// Backpropagation for a layer
	void BackPropLayer(FNeuralLayer& fromLayer, FNeuralLayer& toLayer, float error);				//Actual back propagation calculations 

	//------------------------------------------------------------------------------------------------------------

	float Sigmoid(float value);																		//Helper function to calcuate sigmoid function

	float SigmoidDerivative(float value);

	float Tanh(float value);																		//Helper function to calcuate Tanh function

	float TanhDerivative(float value);

	float ReLU(float value);																		//Helper function to calcuate ReLU function
	
	float RelUDerivative(float value);
};