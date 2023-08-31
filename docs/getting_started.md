# Getting Started

## How do I connect to an Algorand Network?

Rest API services provide access to an algorand network.

| Service                       | Client                                               | Purpose                                                                                 |
| ----------------------------- | ---------------------------------------------------- | --------------------------------------------------------------------------------------- |
| `C++ Library`         | [`Algorand C++ SDK`](https://github.com/Wisdom-Labs/Algorand-CPlusPlus-SDK)     | Send Transactions and fetch some Details.                       |
| `Algod v2 API`         | [`AlgodClient`](xref:Algorand.Unity.AlgodClient)     | Monitor Transactions.                       |
| `Indexer API`                     | [`IndexerClient`](xref:Algorand.Unity.IndexerClient) | Query details from Algorand  and lookup account, asset, transactions or smart contract information. |

If you want to run sandbox algorand node for local dev, check this tutorial [Access BetaNet Network using Sandbox](https://developer.algorand.org/tutorials/betanet-sandbox/)

## How do I send TX to an Algorand Blockchain?

To make a transaction:

1. Define/construct your transaction using static methods on UAlgorandUnrealManager class.
2. Send and Sign the transaction using `3rd-party` module via [`Algorand C++ SDK`]().
3. Wait for the transaction to be confirmed via its [`Multi-cast Delegate`].

See [Your First Transaction](getting_started/your_first_transaction.md) for an in-depth guide on making your first transaction.

## How do I set rpc info

- Rest API
    * rpc url
        + mainnet: https://mainnet-api.algonode.cloud
        + testnet: https://testnet-api.algonode.cloud
        + betanet: https://betanet-api.algonode.cloud
 - Purestake
    * rpc url
        + mainnet: https://mainnet-algorand.api.purestake.io/ps2
        + testnet: https://testnet-algorand.api.purestake.io/ps2
        + betanet: https://betanet-algorand.api.purestake.io/ps2
    * rpc Port
        should be set port supporting in algorand node.
        should set ```port = 0```
    * rpc TokenHeader
        > Thing that you should know
        + When you set tokenHeader of algorand rpc using purestake, key value of tokenHeader should be started with `x-api-key:`
        + You can get purestake api key using [Purestake Doc](https://developer.algorand.org/tutorials/getting-started-purestake-api-service/)
  - Sandbox Algorand Node

    You can access any algorand network using sandbox. (`mainnet`, `testnet`, `betanet`, or ... )
    > Thing that you should know
    + key value of tokenHeader should be started with `X-Algo-API-Token:`
    + You can see reference Doc to install sandbox. [Sandbox Doc](https://developer.algorand.org/tutorials/betanet-sandbox)

 - Local Algorand Node
 
    After installation of algorand node on local, You should set some rpc info.
    > Thing that you should know
    + key value of tokenHeader should be started with `X-Algo-API-Token:`

## Example for Demo project
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