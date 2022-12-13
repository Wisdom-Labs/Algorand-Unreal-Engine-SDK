// Copyright Epic Games, Inc. All Rights Reserved.

#include "Algorand.h"
#include "Core.h"
#include "Modules/ModuleManager.h"


#define LOCTEXT_NAMESPACE "FAlgorandModule"

void FAlgorandModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

}

void FAlgorandModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAlgorandModule, Algorand)
