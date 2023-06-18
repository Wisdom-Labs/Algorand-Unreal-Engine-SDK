# Algorand Play Unreal SDK Demo

This repository contains a sample project that uses the Algorand Play Unreal SDK. 

## basic flow
 inherit c++ and blueprint functions  
 - GetBalance
 - Send Payment TX
 - Create Asset TX
 - Update Asset TX
 - Transfer Asset TX
 - NFT Viewer
 - Send Application TX
 - Generate wallet

## How to set rpc info
 - Rest API
    * rpc url
        + mainnet: https://mainnet-api.algonode.cloud
        + testnet: https://testnet-api.algonode.cloud
        + betanet: https://betanet-api.algonode.cloud
    > C++ Usage
    ```c++
    setAlgoRpc("https://testnet-api.algonode.cloud");
    setAlgoPort(443);
    setAlgoTokenHeader("");
    ```
 - Purestake
    * rpc url
        + mainnet: https://mainnet-algorand.api.purestake.io/ps2
        + testnet: https://testnet-algorand.api.purestake.io/ps2
        + betanet: https://betanet-algorand.api.purestake.io/ps2
        > C++ Usage
        + ```setAlgoRpc("https://testnet-algorand.api.purestake.io/ps2");```
    * rpc Port
        should be set port supporting in algorand node.
        should set ```port = 0```
        > C++ Usage
        + ```setAlgoPort(0);```
    * rpc TokenHeader
        > Thing that you should know
        + When you set tokenHeader of algorand rpc using purestake, key value of tokenHeader should be started with `x-api-key:`
        + You can get purestake api key using [Reference](##Reference)
        > C++ Usage
        + ```setAlgoTokenHeader("x-api-key:bLcs4F2SyGY0InF9M6Vl9piFTIZ8Ww281OjKXyE1");```
  - Sandbox Algorand Node

    You can access any algorand network using sandbox. (`mainnet`, `testnet`, `betanet`, or ... )
    > Thing that you should know
    + key value of tokenHeader should be started with `X-Algo-API-Token:`
    + You can see reference Doc to install sandbox. [Reference](##Reference)
    > C++ Usage
    ```c++
    setAlgoRpc("http://localhost");
    setAlgoPort(4001);
    setAlgoTokenHeader("X-Algo-API-Token:aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    ```
 - Local Algorand Node
 
    After installation of algorand node on local, You should set some rpc info.
    > Thing that you should know
    + key value of tokenHeader should be started with `X-Algo-API-Token:`
    + You can see reference Doc to install local algorand node. [Reference](##Reference)
    > C++ Usage
    ```c++
    setAlgoRpc("http://localhost");
    setAlgoPort(8080);
    setAlgoTokenHeader("X-Algo-API-Token:77aa632e1a023e6b1c79bbb275645cb0ca7ac82cb9d4e92226d9c0029fe35c1c");
    ```

## How to run
 - Payment TX
    > C++ Usage
    + receiver address: `NBRUQXLMEJDQLHE5BBEFBQ3FF4F3BZYWCUBBQM67X6EOEW2WHGS764OQXE`
 - AssetCreation TX
    > C++ Usage
    + manager address: `Z5C3T63QHLEAAXV2A2L3Y2LY6TURAPEESMQZC7777PGGPPAAGC4F4GWHHA`
    + reserve address: `Z5C3T63QHLEAAXV2A2L3Y2LY6TURAPEESMQZC7777PGGPPAAGC4F4GWHHA`
    + freeze address: `Z5C3T63QHLEAAXV2A2L3Y2LY6TURAPEESMQZC7777PGGPPAAGC4F4GWHHA`
    + clawback address: `Z5C3T63QHLEAAXV2A2L3Y2LY6TURAPEESMQZC7777PGGPPAAGC4F4GWHHA`
    + asset id: `0`
    + total: `10000`
    + decimals: `2`
    + unit name: `AUSD`
    + asset name: `AlgoUSD`
    + url: `https://book.io`
    + notes: `Asset Creation TX`
 - AssetUpdate TX
    > C++ Usage
    + manager address: `A6KIDEH35E56GWUDYZCDFVTLKDIC7P5HQRHGCIM4PVALCRTE2HZBFE7CKM`
    + reserve address: `SSTIXFVQDJOVYDSFDOPPGL6V2ZE66SWXB7EDJHRI5B4IRHLQTHIEZTP35U`
    + freeze address: `A6KIDEH35E56GWUDYZCDFVTLKDIC7P5HQRHGCIM4PVALCRTE2HZBFE7CKM`
    + clawback address: `SSTIXFVQDJOVYDSFDOPPGL6V2ZE66SWXB7EDJHRI5B4IRHLQTHIEZTP35U`
    + asset id: `232082544`
    + total: `1000`
    + decimals: `0`
    + unit name: `WAUSD`
    + asset name: `AlgoUSD`
    + url: `https://ebook.io`
    + notes: `Asset Creation TX`
 - AssetTransfer TX
    > C++ Usage
    + sender address: `Z5C3T63QHLEAAXV2A2L3Y2LY6TURAPEESMQZC7777PGGPPAAGC4F4GWHHA`
    + receiver address: `A6KIDEH35E56GWUDYZCDFVTLKDIC7P5HQRHGCIM4PVALCRTE2HZBFE7CKM`
    + asset ID: `218447260`
- NFT Details
    > C++ Usage
    + wallet address: `HFIB4OCOT3ECQCVVTNROKIWWF7Q4MSWQSIJTB6G3UI3B6E2XBMIS55GCFU`
 - Application Usage
    > C++ Usage
    + App ID: `16037129`
## Reference
 - If you are a game developer, please check this doc.  [How to get purestake key](https://developer.algorand.org/tutorials/getting-started-purestake-api-service/)
 - If you want to run sandbox algorand node, check this tutorial [Access BetaNet Network using Sandbox](https://developer.algorand.org/tutorials/betanet-sandbox/)
 - Also if you want to run local algorand node on `windows`, check this [How to compile and rn the algorand node natively windows](https://developer.algorand.org/tutorials/compile-and-run-the-algorand-node-natively-windows/)

    