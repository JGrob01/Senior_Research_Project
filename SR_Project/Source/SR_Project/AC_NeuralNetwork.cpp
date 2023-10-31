#include "AC_NeuralNetwork.h"

// Sets default values for this component's properties
UAC_NeuralNetwork::UAC_NeuralNetwork()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.
	PrimaryComponentTick.bCanEverTick = true;

	//set a learning rate, this can change based on tests
	learnRate = 0.05f;
}

//------------------------------------------------------------------------------------------------------------INITIALIZE NEURAL NETWORK---------------------------------------------------------------------------------------------------------------

/* Initializes the neural network with the 3 layers, each layer is given a set number of inputs and outputs.
* int32 numIn: The number of input nodes needed in layer
* int32 numHid:The number of hidden nodes needed in layer (currently 2/3's of input+output nodes)
* int32 numOut: The number of output nodes needed in layer (one node only)
* Return bool to see if network was setup properly; needs a return for blueprint callable 
*/
void UAC_NeuralNetwork::InitNeuralNetwork(int32 numIn, int32 numHid, int32 numOut, float expected, E_NeuralNetwork type)
{
	// Initialize input, hidden, and output layers
	inputLayer = GenLayer(numIn, 0);
	hiddenLayer = GenLayer(numHid, numIn);
	outputLayer = GenLayer(numOut, numHid);
	GenExpectedValue(expected);
	NNType = type;
	return;				
}

/* Will generate the layers within the 3 main layers
* int32 nodes: The number of nodes in layer
* int32 prevLayerSize: The number of nodes in the privous layer
* Returns TArray<FNeuralLayer>: which can be used to assign input/hidden/output global variables
*/
TArray<FNeuralLayer> UAC_NeuralNetwork::GenLayer(int32 nodes, int32 prevLayerSize)
{
	TArray<FNeuralLayer> layer;
	FNeuralLayer newNode;
	for (int32 i = 0; i < nodes; i++)
	{
		newNode.Values.Add(0.0f);
		newNode.Weights.Add(GenWeights(prevLayerSize));
	}
	layer.Add(newNode);
	return layer;
}

/* Called when weights are needed to be given to node, will be random at least (between -1 and 1)
* int32 num: the number of nodes
* Returns TArray<FWeights>: An array of random floats used to assign input/hidden/output weights
*/
FWeights UAC_NeuralNetwork::GenWeights(int32 num)
{
	FWeights weightValues;
	for (int32 i = 0; i < num; i++)
	{
		weightValues.weightValues.Add(FMath::FRandRange(-1.0f, 1.0f));
	}
	return weightValues;
}

void UAC_NeuralNetwork::GenExpectedValue(float expected)
{
	expectedValue = expected;
}

//------------------------------------------------------------------------------------------------------------PROPOGATIONS---------------------------------------------------------------------------------------------------------------

/* Will Forward propogate through the network
* TArray<float> inputValues: When given the input layer values, forward prop through the network and make the error values
* Returns float: the total error value to be used in BP for AI movement
*/
float UAC_NeuralNetwork::FwdPropNN(TArray<float> inputValues)
{
	InitInputLayer(inputValues);
	FwdPropLayer(inputLayer[0], hiddenLayer[0]);
	FwdPropLayer(hiddenLayer[0], outputLayer[0]);
	return outputLayer[0].Values[0];
}

/* Initialize the input layer
* TArray<float> values: Assigns given input values to global input layer values
*/
void UAC_NeuralNetwork::InitInputLayer(TArray<float> values)
{
	for (int32 i = 0; i < values.Num(); i++)
	{
		inputLayer[0].Values[i] = values[i];
	}
}

/* Forward propogation code; use the set activation function
* FNeuralLayer& fromLayer: The given start layer
* FNeuralLayer& toLayer: The given next layer
*/
void UAC_NeuralNetwork::FwdPropLayer(FNeuralLayer& fromLayer, FNeuralLayer& toLayer)
{
	for (int32 toIndex = 0; toIndex < toLayer.Values.Num(); toIndex++)
	{
		TArray<float> weights = toLayer.Weights[toIndex].weightValues;
		float sum = 0.0f;
		for (int32 index = 0; index < weights.Num(); index++)
		{
			sum += fromLayer.Values[index] * weights[index];
		}
		
		switch (NNType)
		{
		case E_NeuralNetwork::E_Sigmoid:
			toLayer.Values[toIndex] = Sigmoid(sum);
			break;
		case E_NeuralNetwork::E_ReLU:
			toLayer.Values[toIndex] = ReLU(sum);
			break;
		case E_NeuralNetwork::E_tanH:
			toLayer.Values[toIndex] = Tanh(sum);
			break;
		default:
			break;
		}
	}
}

/* Back prop through layers
* float error: given the total error value to be used
* Return void: needed in blueprint callable code
*/
void UAC_NeuralNetwork::BackPropNN()
{
	float error = 0.5 * std::pow(expectedValue - outputLayer[0].Values[0], 2);
	BackPropLayer(outputLayer[0], hiddenLayer[0], error);
	BackPropLayer(hiddenLayer[0], inputLayer[0], error);
	return;
}

/* Back propogation code; will change the weights based on error; remember, goes back to forth
* FNeuralLayer& fromLayer: The given start layer
* FNeuralLayer& toLayer: The given next layer
* TArray<float> error: an array of error values(should be 1 for output to hidden)
*/
void UAC_NeuralNetwork::BackPropLayer(FNeuralLayer& fromLayer, FNeuralLayer& toLayer, float error)
{
	for (int32 fromIndex = 0; fromIndex < fromLayer.Values.Num(); fromIndex++)
	{
		float value = fromLayer.Values[fromIndex];
		float delta = 0.0f;
		
		switch (NNType)
		{
		case E_NeuralNetwork::E_Sigmoid:
			delta = error * SigmoidDerivative(value);
			break;
		case E_NeuralNetwork::E_ReLU:
			delta = error * RelUDerivative(value);
			break;
		case E_NeuralNetwork::E_tanH:
			delta = error * TanhDerivative(value);
			break;
		default:
			break;
		}

		for (int32 index = 0; index < fromLayer.Weights[fromIndex].weightValues.Num(); index++)								//error here				
		{
			fromLayer.Weights[fromIndex].weightValues[index] += learnRate * delta * toLayer.Values[index];
		}
	}
}

//------------------------------------------------------------------------------------------------------------HELPER FUNCTIONS---------------------------------------------------------------------------------------------------------------

/* Helper funtion for sigmod
* float value: given value
* Returns float: crunched down from sigmoid calc
*/
float UAC_NeuralNetwork::Sigmoid(float value)
{
	return (1.0 / (1.0 + std::exp(-value)));
}

/* Helper funtion to calculate the derivative of Sigmoid
* float value: given value
* Returns float: derivative of Sigmoid
*/
float UAC_NeuralNetwork::SigmoidDerivative(float value) {
	float sig = Sigmoid(value);
	return sig * (1.0 - sig);
}

/* Helper funtion for Tanh
* float value: given value
* Returns float: crunched down from Tanh calc
*/
float UAC_NeuralNetwork::Tanh(float value)
{
	return tanh(value);
}

/* Helper funtion to calculate the derivative of Tanh
* float value: given value
* Returns float: derivative of Tanh
*/
float UAC_NeuralNetwork::TanhDerivative(float value) {
	float tanhx = Tanh(value);
	return 1.0 - tanhx * tanhx;
}

/* Helper funtion for ReLU
* float value: given value
* Returns float: crunched down from ReLU calc
*/
float UAC_NeuralNetwork::ReLU(float value)
{
	return value > 0 ? value : 0;
}

/* Helper funtion to calculate the derivative of ReLU
* float value: given value
* Returns float: derivative of ReLU
*/
float UAC_NeuralNetwork::RelUDerivative(float value) {
	return (value > 0) ? 1.0 : 0.0;
}