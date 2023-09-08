# Algorand SDK for Unreal Engine
Official Unreal Engine Plugin for Algorand Blockchain Platform.

## 📙 1. Overview:

This plugin provides the functionallity for building different types of transactions, wallet management and NFT Viewer on the Algorand blockchain.
- For indepth video demo on using the plugin watch here: [Video](https://youtu.be/xXlqLxUo3Uo)

- If you are looking for a repo for this c++ sdk, [check this repository](https://github.com/Wisdom-Labs/Algorand-CPlusPlus-SDK.git).

## 📑 2. Prerequisites:

To use this UE plugin : ✅
- Unreal Engine 5.x is supported.
  https://docs.unrealengine.com/5.1/en-US/installing-unreal-engine/
- Windows: Visual Studio 2019 or newer, JetBrains Rider 2020.x
- macOS: Xcode 13.2.1
> In order to build project using Unreal Engine 5.0+, use a branch `master`.

## ⚙️ 3. Installation:
1. In this Github repository, clone this repo with `git` command or download as a zip file type and rename cloned folder name `AlgorandUnrealEngineSDK`.
2. You can find a folder called example, which is our test project.
3. Then you should create a folder called `Plugins/Algorand` on `example/unreal-algorand-demo` folder.
4. You should copy all except for example folder on `AlgorandUnrealEngineSDK` folder to the `Plugins/Algorand` folder of our test project.
> After you complete above action, this plugin path is `example\unreal-algorand-demo\Plugins\Algorand`.
5. You should register the plugin name (Algorand) into Build.cs on the Source 
directory of out test project (`example\unreal-algorand-demo`) and rebuild your project.
    ```csharp  
    public UnrealAlgorandPlugin(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Algorand" });

        PrivateDependencyModuleNames.AddRange(new string[] {  });
    }
    ```
>Right click on UnrealAlgorandPlugin.uproject on `example/unreal-algorand-demo` folder, and "Generate Visual Studio project files" - This process may take some time.
6. Once finished, open your *.sln project or *.uproject file with Visual Studio or JetBrains Rider.

## 🧭 4. Modules Reference:
* **Algorand** - entrypoint module, provides `C++`- & `Blueprints`- friendly interfaces for plugin's users. This module should be considered as the only module you need to depend on.
* **AlgorandAPI** - This module is one to make api request and process its result and send it to entrypoint module.
* **Blockchain** - This module contains core logic for building and signing txs from tx types and params.
* **Wallet** - This module is one related to manage wallet and wallet connect provider.
* **Vertices** - This module play an important role as an interface and router in connecting to our algorand c++ sdk (Vertices shared library).

## 🚀 5. Get Started

Read [Getting Started](docs/getting_started.md) to learn the basic workflows for developing on Algorand.

### Demo Project
 Check an [Example Folder](https://github.com/Wisdom-Labs/Algorand-Unreal-Engine-SDK/tree/master/example/unreal-algorand-demo) made with this UE plugin.