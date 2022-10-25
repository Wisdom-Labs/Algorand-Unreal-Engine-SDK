// Copyright Epic Games, Inc. All Rights Reserved.

#define WIN32_LEAN_AND_MEAN

#include "Algorand.h"
#include "Core.h"
#include "Modules/ModuleManager.h"


#define LOCTEXT_NAMESPACE "FAlgorandModule"

void FAlgorandModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	

	/*VerticesSDK vertices;
	vertices.testAlgorand();*/
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
	/*set_http_init(&http_init);
	set_http_get(&http_get);
	set_http_post(&http_post);
	set_http_close(&http_close);*/
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAlgorandModule, Algorand)
