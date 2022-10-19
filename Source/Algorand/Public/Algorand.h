// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FAlgorandModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual void setHTTPCURLs();

private:
	/** Handle to the test dll we will load */
	void*	VerticesHandle;
	void*	SodiumHandle;
};
