# Algorand Unreal Engine Plugin
Official Unreal Engine plugin for Algorand Blockchain Platform.

> If you are looking for a repo for this c++ sdk, [check this repository](https://github.com/Wisdom-Labs/Algorand-CPlusPlus-SDK.git).

This plugin provides functionallity for building different types of transactions and managing  wallet on algorand chain.

## 📙 1. Overview:
ALgorand Unreal Engine Plugin

This UE plugin includes:
- Root Folder: Unreal-Plugin - Put this plugin in your new or existing project to utilize.
- Example Folder: Unreal Plugin Example Project - Use this project to start your new game or demo/test Algorand System

This sdk contains a setting option to set RPC Info.
* RPC URL : You should set algorand node you want to work on with this url.
  ` setAlgoRpc("https://node.testnet.algoexplorerapi.io"); `
* RPC Port : set port which can connect into server supporting algorand node with.
  ` setAlgoPort(0); `
* RPC TokenHeader : You should set Header to request apis according to algorand node type.
  ` setAlgoTokenHeader("x-api-key:bLcs4F2SyGY0InF9M6Vl9piFTIZ8Ww281OjKXyE1"); `
  This param has a pair made of key and value.

RPC types you can select are like following:
* Algoexplorerapi
* Purestake
* Local Algorand Node, (You should install new algorand node on your local)

## 📑 2. Prerequisites:

To use this UE plugin : ✅
- Unreal Engine 5.x is supported.
- Windows: Visual Studio 2019 or newer, JetBrains Rider 2020.x
- macOS: Xcode 13.2.1
> In order to build project using Unreal Engine 5.0+, use a branch `master`.

## ⚙️ 3. Installation:
1. In this Github repository, clone this repo with `git` command or download as a zip file type.
2. You can copy AlgorandUnrealEngineSDK folder to the `Plugins/Algorand` of your UE project directory.
> In this example, the project directory is `C:\Unreal Projects\MyGameProject\Plugins\Algorand`
3. You should register the plugin name (Algorand) into Build.cs on the Source 
directory of your project and rebuild your project.
    ```csharp  
    public UnrealAlgorandPlugin(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Algorand" });

        PrivateDependencyModuleNames.AddRange(new string[] {  });
    }
    ```
4. Right click on YourProject.uproject, and "Generate Visual Studio project files" - This process may take some time.
5. Once finished, open your *.sln project or *.uproject file with Visual Studio or JetBrains Rider.

## 🧭 4. Modules reference
* **Algorand** - entrypoint module, provides `C++`- & `Blueprints`- friendly interfaces for plugin's users. This module should be considered as the only module you need to depend on.
* **AlgorandAPI** - This module is one to make api request and process its result and send it to entrypoint module.
* **Blockchain** - This module contains core logic for building and signing txs from tx types and params.
* **Wallet** - This module is one related to manage wallet and wallet connect provider.
* **Vertices** - This module play an important role as an interface and router in connecting to our algorand c++ sdk (Vertices shared library).

## 🚀 5. Common Usage
`UAlgorandUnrealManager` stored on `Algorand` module should be used as the main entry-point for all Algorand-related actions.

Check our [demo project](https://github.com/Wisdom-Labs/Algorand-Unreal-Engine-SDK/tree/master/example/unreal-algorand-demo) made with this UE plugin.
