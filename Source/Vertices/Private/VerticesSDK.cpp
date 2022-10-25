
#include "VerticesSDK.h"
//#include "Misc/Paths.h"
#include "Misc/MessageDialog.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FVerticesModule"

namespace algorand {
    namespace vertices {

        VerticesSDK::VerticesSDK() {
            // Get the base directory of this plugin
            FString BaseDir = IPluginManager::Get().FindPlugin("Algorand")->GetBaseDir();

            // Add on the relative location of the third party dll and load it
            FString VerticesPath, SodiumPath;

            VerticesPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/AlgorandLibrary/Win64/vertices.dll"));
            SodiumPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/AlgorandLibrary/Win64/libsodium.dll"));

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
        }

        VerticesSDK::~VerticesSDK () {

            FPlatformProcess::FreeDllHandle(VerticesHandle);
            FPlatformProcess::FreeDllHandle(SodiumHandle);

            VerticesHandle = nullptr;
            SodiumHandle = nullptr;
        }
    }
}


#undef LOCTEXT_NAMESPACE