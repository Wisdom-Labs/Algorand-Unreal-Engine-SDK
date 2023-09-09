#include "AlgorandHandler.h"

#include "..\..\Plugins\Algorand\Source\Algorand\Public\Models\FAppNoOpTX.h"

UAlgorandHandler::UAlgorandHandler() { 
	algorandManager = 
		CreateDefaultSubobject<UAlgorandUnrealManager>(TEXT("AlgorandManager"));
}

UWorld* UAlgorandHandler::GetWorld() const { return GetOuter()->GetWorld(); }

void UAlgorandHandler::OnRestoreWalletCallback(const FString& output) {
    UE_LOG(LogTemp, Display, TEXT("Generated address: %s"),
        *output);

    TArray<FAppArg> app_args;
    EAppOnCompleteTX app_complete_tx = EAppOnCompleteTX::NOOP;
    
    app_args.Add(UAppNoOpTXFactory::FAppArgFromString(EAppArgType::ARG_METHOD, "mul(uint64,uint64)uint64"));
    app_args.Add(UAppNoOpTXFactory::FAppArgFromString(EAppArgType::ARG_INT, "1000"));
    app_args.Add(UAppNoOpTXFactory::FAppArgFromString(EAppArgType::ARG_INT, "2"));
    
    FScriptDelegate _delegate3;
    _delegate3.BindUFunction(this, FName("OnSendApplicationCallTransactionCallback"));
    algorandManager->SendApplicationCallTransactionCallback.Add(_delegate3);
    algorandManager->sendApplicationCallTransaction(301624623, app_args, app_complete_tx);
    
    // FScriptDelegate _delegate8;
    // _delegate8.BindUFunction(this, FName("OnSendAssetConfigTransactionCallback"));
    // algorandManager->SendAssetConfigTransactionCallback.Add(_delegate8);
    // algorandManager->sendAssetConfigTransaction(FString("A6KIDEH35E56GWUDYZCDFVTLKDIC7P5HQRHGCIM4PVALCRTE2HZBFE7CKM"),
    //                                             FString(""),
    //                                             FString("A6KIDEH35E56GWUDYZCDFVTLKDIC7P5HQRHGCIM4PVALCRTE2HZBFE7CKM"),
    //                                             FString("A6KIDEH35E56GWUDYZCDFVTLKDIC7P5HQRHGCIM4PVALCRTE2HZBFE7CKM"),
    //                                             0,
    //                                             1000,
    //                                             0,
    //                                             "false",
    //                                             FString("1234567890"),
    //                                             FString("My Asset"),
    //                                             FString("https://myurl.com"),
    //                                             FString("fe"));
    
    /*FScriptDelegate _delegate9;
    _delegate9.BindUFunction(this, FName("OnGetAccountInformationCallback"));
    algorandManager->FetchAccountInformationCallback.Add(_delegate9);
    algorandManager->fetchAccountInformation("6WII6ES4H6UW7G7T7RJX63CUNPKJEPEGQ3PTYVVU3JHJ652W34GCJV5OVY");*/
    
    // FScriptDelegate _delegate1;
    // _delegate1.BindUFunction(this, FName("OnGetBalanceCallback"));
    // algorandManager->GetBalanceCallback.Add(_delegate1);
    // algorandManager->getBalance("LCKVRVM2MJ7RAJZKPAXUCEC4GZMYNTFMLHJTV2KF6UGNXUFQFIIMSXRVM4");

    // FScriptDelegate _delegate2;
    // _delegate2.BindUFunction(this, FName("OnSendPaymentTransactionCallback"));
    // algorandManager->SendPaymentTransactionCallback.Add(_delegate2);
    // algorandManager->sendPaymentTransaction(FString("A6KIDEH35E56GWUDYZCDFVTLKDIC7P5HQRHGCIM4PVALCRTE2HZBFE7CKM"), 100000000, FString("Sent 100 algo to A6KIDEH35E56GWUDYZCDFVTLKDIC7P5HQRHGCIM4PVALCRTE2HZBFE7CKM."));

    // FScriptDelegate _delegate8;
    // _delegate8.BindUFunction(this, FName("OnSendAssetTransferTransactionCallback"));
    // algorandManager->SendAssetTransferTransactionCallback.Add(_delegate8);
    // algorandManager->sendAssetTransferTransaction(FString("SSTIXFVQDJOVYDSFDOPPGL6V2ZE66SWXB7EDJHRI5B4IRHLQTHIEZTP35U"), FString("A6KIDEH35E56GWUDYZCDFVTLKDIC7P5HQRHGCIM4PVALCRTE2HZBFE7CKM"),234664633,"0.1",FString("Sent 100 ERC20 token to A6KIDEH35E56GWUDYZCDFVTLKDIC7P5HQRHGCIM4PVALCRTE2HZBFE7CKM."));   // A6KIDEH35E56GWUDYZCDFVTLKDIC7P5HQRHGCIM4PVALCRTE2HZBFE7CKM
    
}

void UAlgorandHandler::OnInitializeNewWalletCallback(const FString& output) {
    UE_LOG(LogTemp, Display, TEXT("Generated address: %s"),
        *output);
    FScriptDelegate _delegate2;
    _delegate2.BindUFunction(this, FName("OnSendPaymentTransactionCallback"));
    algorandManager->SendPaymentTransactionCallback.Add(_delegate2);
    algorandManager->sendPaymentTransaction(FString("NBRUQXLMEJDQLHE5BBEFBQ3FF4F3BZYWCUBBQM67X6EOEW2WHGS764OQXE"), 100, FString("Sent 100 algo to NBRUQXLMEJDQLHE5BBEFBQ3FF4F3BZYWCUBBQM67X6EOEW2WHGS764OQXE."));
}

void UAlgorandHandler::OnGetBackupMnemonicPhraseCallback(const FString& output) {
    UE_LOG(LogTemp, Display, TEXT("Received mnemonics: %s"),
        *output);
}

void UAlgorandHandler::OnGenerateMnemonicsCallback(const FString& output) {
    UE_LOG(LogTemp, Display, TEXT("Generated mnemonics: %s"),
        *output);
}

void UAlgorandHandler::OnGetBalanceCallback(const FUInt64& amount) {
    UE_LOG(LogTemp, Display, TEXT("Balance: %llu"),
        amount.Value);
}

void UAlgorandHandler::OnSendPaymentTransactionCallback(const FString& txID) {
    UE_LOG(LogTemp, Display, TEXT("Payment TX ID: %s"),
        *txID);
}

void UAlgorandHandler::OnSendAssetConfigTransactionCallback(const FString& txID, const FUInt64& assetID) {
    UE_LOG(LogTemp, Display, TEXT("Asset Config TX ID: %s, %llu"),
        *txID, assetID.Value);
}

void UAlgorandHandler::OnSendAssetTransferTransactionCallback(const FString& txID) {
    UE_LOG(LogTemp, Display, TEXT("Asset Transfer TX ID: %s"),
        *txID);
}

void UAlgorandHandler::OnSendApplicationCallTransactionCallback(const FString& txID, const FString& logs) {
    UE_LOG(LogTemp, Display, TEXT("Application Call TX ID & TX LOG -- %s:%s"),
        *txID, *logs);

    FString app_result = UAppNoOpTXFactory::FAppResultFromLogs(EAppLogType::LOG_INT, logs);
}

void UAlgorandHandler::OnGetArcAssetDetailsCallback(const FArcAssetDetails& asset_Details)
{
    UE_LOG(LogTemp, Display, TEXT("Arc Asset Media Url: %s"),
     *asset_Details.media_url);
}

void UAlgorandHandler::OnGetAccountInformationCallback(const TArray<FString>& IDs, const TArray<FString>& Names)
{
    for(int i = 0; i < IDs.Num(); i++) 
        UE_LOG(LogTemp, Display, TEXT("Arc Account Info: %s, %s"),
         *(IDs[i]), *(Names[i]));
}

void UAlgorandHandler::RunSomeLogic() {
    FScriptDelegate _delegate4;
    _delegate4.BindUFunction(this, FName("OnRestoreWalletCallback"));
    algorandManager->RestoreWalletCallback.Add(_delegate4);
    FString mnemonics = "rally relief lucky maple primary chair syrup economy tired hurdle slot upset clever chest curve bitter weekend prepare movie letter lamp alert then able taste";
    algorandManager->restoreWallet(mnemonics);
    
    /*FScriptDelegate _delegate9;
    _delegate9.BindUFunction(this, FName("OnGetArcAssetDetailsCallback"));
    algorandManager->FetchArcAssetDetailsCallback.Add(_delegate9);
    algorandManager->fetchArcAssetDetails(1092400027);// 779312090, 1019478822*/
    
    // FScriptDelegate _delegate5;
    // _delegate5.BindUFunction(this, FName("OnInitializeNewWalletCallback"));
    // algorandManager->InitializeNewWalletCallback.Add(_delegate5);
    // algorandManager->initializeNewWallet();

    // FScriptDelegate _delegate6;
    // _delegate6.BindUFunction(this, FName("OnGetBackupMnemonicPhraseCallback"));
    // algorandManager->GetBackupMnemonicPhraseCallback.Add(_delegate6);
    // algorandManager->getBackupMnemonicPhrase();
    
    /*FScriptDelegate _delegate7;
    _delegate7.BindUFunction(this, FName("OnGenerateMnemonicsCallback"));
    algorandManager->GenerateMnemonicsCallback.Add(_delegate7);
    algorandManager->generateMnemonics();*/
    
    // FScriptDelegate _delegate1;
    // _delegate1.BindUFunction(this, FName("OnGetBalanceCallback"));
    // algorandManager->GetBalanceCallback.Add(_delegate1);
    // algorandManager->getBalance();
    
    // FScriptDelegate _delegate2;
    // _delegate2.BindUFunction(this, FName("OnSendPaymentTransactionCallback"));
    // algorandManager->SendPaymentTransactionCallback.Add(_delegate2);
    // algorandManager->sendPaymentTransaction(FString("NBRUQXLMEJDQLHE5BBEFBQ3FF4F3BZYWCUBBQM67X6EOEW2WHGS764OQXE"), 100, FString("Sent 100 algo to NBRUQXLMEJDQLHE5BBEFBQ3FF4F3BZYWCUBBQM67X6EOEW2WHGS764OQXE."));

    // FScriptDelegate _delegate8;
    // _delegate8.BindUFunction(this, FName("OnSendAssetTransferTransactionCallback"));
    // algorandManager->SendAssetTransferTransactionCallback.Add(_delegate8);
    // algorandManager->sendAssetTransferTransaction(FString("NBRUQXLMEJDQLHE5BBEFBQ3FF4F3BZYWCUBBQM67X6EOEW2WHGS764OQXE"), FString("NBRUQXLMEJDQLHE5BBEFBQ3FF4F3BZYWCUBBQM67X6EOEW2WHGS764OQXE"),100,100,FString("Sent 100 ERC20 token to NBRUQXLMEJDQLHE5BBEFBQ3FF4F3BZYWCUBBQM67X6EOEW2WHGS764OQXE."));
}