# Algorand Unreal Engine Plugin
Official Unreal Engine plugin for Algorand Blockchain Platform.

> If you are looking for a repo for this c++ sdk, [check this repository](https://github.com/Wisdom-Labs/Algorand-CPlusPlus-SDK.git).

This plugin provides functionallity for building different types of transactions and managing  wallet on algorand chain.

### UE5 support
In order to build project using Unreal Engine 5.0+, use a branch `master`.

### Modules reference
* **Algorand** - entrypoint module, provides `C++`- & `Blueprints`- friendly interfaces for plugin's users. This module should be considered as the only module you need to depend on.
* **AlgorandAPI** - This module is one to make api request and process its result and send it to entrypoint module.
* **Blockchain** - This module contains core logic for building and signing txs from tx types and params.
* **Wallet** - This module is one related to manage wallet and wallet connect provider.
* **Vertices** - This module play an important role as an interface and router in connecting to our algorand c++ sdk (Vertices shared library).

### Basic usage

`UAlgorandUnrealManager` should be used as the main entry-point for all Algorand-related actions.

### Demo Project

Check our [demo project](https://github.com/Wisdom-Labs/Algorand-Unreal-Engine-SDK/tree/master/example/unreal-algorand-demo).
