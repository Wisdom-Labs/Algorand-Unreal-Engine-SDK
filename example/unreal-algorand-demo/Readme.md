# Algorand Play Unreal SDK Demo

This repository contains a sample project that uses the Algorand Play Unreal SDK. 

## basic flow
 inherit c++ and blueprint functions  
 - GetBalance
 - Send Payment TX
 - Send Application TX
 - Generate wallet

## How to set rpc info
 - Algoexplorerapi
    * rpc url
        + mainnet: https://node.mainnet.algoexplorerapi.io
        + testnet: https://node.testnet.algoexplorerapi.io
        + betanet: https://node.betanet.algoexplorerapi.io
    > C++ Usage
    ```c++
    setAlgoRpc("https://node.testnet.algoexplorerapi.io");
    setAlgoPort(0);
    setAlgoTokenHeader("");
    ```
 - Purestake
    * rpc url
        + mainnet: https://mainnet-algorand.api.purestake.io/ps2
        + testnet: https://testnet-algorand.api.purestake.io/ps2
        + betanet: https://betanet-algorand.api.purestake.io/ps2
        > C++ Usage
        + ```setAlgoRpc("https://betanet-algorand.api.purestake.io/ps2");```
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
 - Local Algorand Node
    After installation of algorand node on local, You should set some rpc info.
    > Thing that you should know
    + key value of tokenHeader should be started with `X-Algo-API-Token:`
    + You can see reference Doc. [Reference](##Reference)
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
 - Application Usage
    > C++ Usage
    + App ID: `16037129`
## Reference
 - If you are a game developer, please check this doc.  [How to get purestake key](https://developer.algorand.org/tutorials/getting-started-purestake-api-service/)
 - Also if you want to run local algorand node on `windows`, check this [How to compile and rn the algorand node natively windows](https://developer.algorand.org/tutorials/compile-and-run-the-algorand-node-natively-windows/)

    