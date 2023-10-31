//Fill out your copyright notice in the Description page of Project Settings.

#include "ReadWriteDataFile.h"
#include "HAL/PlatformFileManager.h" //core
#include "Misc/FileHelper.h"

FString UReadWriteDataFile::ReadStringFromFile(FString FilePath, bool& bOutSucess, FString& OutInfoMessage)
{
	//Check if File exists
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath)) {
		bOutSucess = false;
		//OutInfoMessage = FString::Printf("File does not exist.");
		return "";
	}
	
	FString RetString = "";

	//Try and read the file, output goes into RetString
	if (!FFileHelper::LoadFileToString(RetString, *FilePath)) {
		bOutSucess = false;
		//OutInfoMessage = FString::Printf("Unable to load file.");
		return "";
	}
	
	bOutSucess = true;
	//OutInfoMessage = FString::Printf("Able to find and load file.");
	return RetString;
}

void UReadWriteDataFile::WriteStringFromFile(FString FilePath, FString String, bool& bOutSucess, FString& OutInfoMessage)
{
	//Try and write string to file
	if (!FFileHelper::SaveStringToFile(String, *FilePath)) {
		bOutSucess = false;
		//OutInfoMessage = FString::Printf("Unable to write String to file.");
		return;
	}

	bOutSucess = true;
	//OutInfoMessage = FString::Printf("Able to write String file.");
}
