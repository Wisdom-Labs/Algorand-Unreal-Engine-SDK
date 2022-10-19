// Copyright Epic Games, Inc. All Rights Reserved.

#define WIN32_LEAN_AND_MEAN

#include "Algorand.h"
#include "VerticesSDK.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"


#define LOCTEXT_NAMESPACE "FAlgorandModule"

void FAlgorandModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("Algorand")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString VerticesPath, SodiumPath;
#if PLATFORM_WINDOWS
	VerticesPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/AlgorandLibrary/Win64/vertices.dll"));
	SodiumPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/AlgorandLibrary/Win64/libsodium.dll"));
#elif PLATFORM_MAC
    LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/AlgorandLibrary/Mac/Release/libExampleLibrary.dylib"));
#elif PLATFORM_LINUX
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/AlgorandLibrary/Linux/x86_64-unknown-linux-gnu/libExampleLibrary.so"));
#endif // PLATFORM_WINDOWS

	VerticesHandle = !VerticesPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*VerticesPath) : nullptr;
	SodiumHandle = !SodiumPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*SodiumPath) : nullptr;
	
	if (VerticesHandle && SodiumHandle)
	{
		// Call the test function in the third party library that opens a message box
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Third Party Plugin", "Loaded C-Vertices-sdk.dll & libsodium.dll from Third Party Plugin sample."));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load example third party library"));
	}

	this->setHTTPCURLs();

	VerticesSDK vertices;
	vertices.testAlgorand();
}

void FAlgorandModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
	FPlatformProcess::FreeDllHandle(VerticesHandle);
	FPlatformProcess::FreeDllHandle(SodiumHandle);
	VerticesHandle = nullptr;
	SodiumHandle = nullptr;
}

void FAlgorandModule::setHTTPCURLs() {
	set_http_init(&http_init);
	set_http_get(&http_get);
	set_http_post(&http_post);
	set_http_close(&http_close);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAlgorandModule, Algorand)
