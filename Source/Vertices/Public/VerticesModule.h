#pragma once

#include "Modules/ModuleManager.h"

class FVerticesModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
