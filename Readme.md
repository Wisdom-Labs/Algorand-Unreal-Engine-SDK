# Algorand Unreal Engine Plugin
Official Unreal Engine plugin for Algorand Blockchain Platform.

> If you are looking for a repo for this c++ sdk, [check this repository](https://github.com/Wisdom-Labs/Algorand-CPlusPlus-SDK.git).

This plugin provides functionallity for building different types of transactions and managing  wallet on algorand chain.

## 📙 1. Overview:
ALgorand Unreal Engine Plugin

We made a [Doc](https://github.com/Wisdom-Labs/Algorand-Unreal-Engine-SDK/blob/master/Algorand-Unreal-SDK-Documentation.pdf) which shows overview of our plugin usage and you can check this doc at start of dev with this plugin.
 
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
  + mainnet: https://node.mainnet.algoexplorerapi.io
  + testnet: https://node.testnet.algoexplorerapi.io
  + betanet: https://node.betanet.algoexplorerapi.io
* Purestake
  + mainnet: https://mainnet-algorand.api.purestake.io/ps2
  + testnet: https://testnet-algorand.api.purestake.io/ps2
  + betanet: https://betanet-algorand.api.purestake.io/ps2
* Sandbox Algorand Node
  + algod: http://localhost:4001 
  + kmd: http://localhost:4002
  + indexer: http://localhost:8980
* Local Algorand Node, (You should install new algorand node on your local)

 👔 You should add token for above some urls and can reference this [link](https://github.com/Wisdom-Labs/Algorand-Unreal-Engine-SDK/tree/master/example/unreal-algorand-demo#how-to-set-rpc-info).

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

## 🧭 4. Modules reference
* **Algorand** - entrypoint module, provides `C++`- & `Blueprints`- friendly interfaces for plugin's users. This module should be considered as the only module you need to depend on.
* **AlgorandAPI** - This module is one to make api request and process its result and send it to entrypoint module.
* **Blockchain** - This module contains core logic for building and signing txs from tx types and params.
* **Wallet** - This module is one related to manage wallet and wallet connect provider.
* **Vertices** - This module play an important role as an interface and router in connecting to our algorand c++ sdk (Vertices shared library).

## 🚀 5. Common Usage
`UAlgorandUnrealManager` stored on `Algorand` module should be used as the main entry-point for all Algorand-related actions.

Check our [demo project](https://github.com/Wisdom-Labs/Algorand-Unreal-Engine-SDK/tree/master/example/unreal-algorand-demo) made with this UE plugin.
