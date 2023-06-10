#include "AlgorandHandler.h"

UAlgorandHandler::UAlgorandHandler() { 
	algorandManager = 
		CreateDefaultSubobject<UAlgorandUnrealManager>(TEXT("AlgorandManager"));
}

UWorld* UAlgorandHandler::GetWorld() const { return GetOuter()->GetWorld(); }

void UAlgorandHandler::OnRestoreWalletCallback(const FString& output) {
    UE_LOG(LogTemp, Display, TEXT("Generated address: %s"),
        *output);

    /*FString("WSD"),
                                                FString("WSDAlgo"),
                                                FString("https://algoexplorer.io/"),
                                                FString("Created a token called WSD.")*/
    
    FScriptDelegate _delegate8;
    _delegate8.BindUFunction(this, FName("OnSendAssetConfigTransactionCallback"));
    algorandManager->SendAssetConfigTransactionCallback.Add(_delegate8);
    algorandManager->sendAssetConfigTransaction(FString("A6KIDEH35E56GWUDYZCDFVTLKDIC7P5HQRHGCIM4PVALCRTE2HZBFE7CKM"),
                                                FString(""),
                                                FString("A6KIDEH35E56GWUDYZCDFVTLKDIC7P5HQRHGCIM4PVALCRTE2HZBFE7CKM"),
                                                FString("A6KIDEH35E56GWUDYZCDFVTLKDIC7P5HQRHGCIM4PVALCRTE2HZBFE7CKM"),
                                                0,
                                                10000,
                                                0,
                                                FString(""),
                                                FString("wef"),
                                                FString("ef"),
                                                FString("fe"));
    
    /*FScriptDelegate _delegate1;
    _delegate1.BindUFunction(this, FName("OnGetBalanceCallback"));
    algorandManager->GetBalanceCallback.Add(_delegate1);
    algorandManager->getBalance("LCKVRVM2MJ7RAJZKPAXUCEC4GZMYNTFMLHJTV2KF6UGNXUFQFIIMSXRVM4");*/

    // FScriptDelegate _delegate2;
    // _delegate2.BindUFunction(this, FName("OnSendPaymentTransactionCallback"));
    // algorandManager->SendPaymentTransactionCallback.Add(_delegate2);
    // algorandManager->sendPaymentTransaction(FString("YZ5IOV3EPSSBRKOLY64BHG2HNF3MUC32Y5BM4ZJSHA5KOSF5OPAIABVSRI"), 100, FString("Sent 100 algo to NBRUQXLMEJDQLHE5BBEFBQ3FF4F3BZYWCUBBQM67X6EOEW2WHGS764OQXE."));

    // FScriptDelegate _delegate8;
    // _delegate8.BindUFunction(this, FName("OnSendAssetTransferTransactionCallback"));
    // algorandManager->SendAssetTransferTransactionCallback.Add(_delegate8);
    // algorandManager->sendAssetTransferTransaction(FString("BT4WXMIULS5OZWRRPDXQJMB3GAGC4A5MITAFQARDPEERQBTNYEMXQXO64A"), FString("A6KIDEH35E56GWUDYZCDFVTLKDIC7P5HQRHGCIM4PVALCRTE2HZBFE7CKM"),218447260,100,FString("Sent 100 ERC20 token to NBRUQXLMEJDQLHE5BBEFBQ3FF4F3BZYWCUBBQM67X6EOEW2WHGS764OQXE."));   // A6KIDEH35E56GWUDYZCDFVTLKDIC7P5HQRHGCIM4PVALCRTE2HZBFE7CKM
    
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

void UAlgorandHandler::OnSendAssetConfigTransactionCallback(const FString& txID) {
    UE_LOG(LogTemp, Display, TEXT("Asset Config TX ID: %s"),
        *txID);
}

void UAlgorandHandler::OnSendAssetTransferTransactionCallback(const FString& txID) {
    UE_LOG(LogTemp, Display, TEXT("Asset Transfer TX ID: %s"),
        *txID);
}

void UAlgorandHandler::OnSendApplicationCallTransactionCallback(const FString& txID) {
    UE_LOG(LogTemp, Display, TEXT("Application Call TX ID: %s"),
        *txID);
}

void UAlgorandHandler::OnGetArcAssetDetailsCallback(const FArcAssetDetails& asset_Details)
{
    UE_LOG(LogTemp, Display, TEXT("Arc Asset Media Url: %s"),
     *asset_Details.media_url);
}

void UAlgorandHandler::RunSomeLogic() {
    FScriptDelegate _delegate4;
    _delegate4.BindUFunction(this, FName("OnRestoreWalletCallback"));
    algorandManager->RestoreWalletCallback.Add(_delegate4);
    FString mnemonics = "afford emerge have market grow elevator tumble crumble smart sting matrix movie custom slice labor dilemma define foster focus vintage aisle inmate veteran abstract sunny";
    algorandManager->restoreWallet(mnemonics);

    // FScriptDelegate _delegate9;
    // _delegate9.BindUFunction(this, FName("OnGetArcAssetDetailsCallback"));
    // algorandManager->FetchArcAssetDetailsCallback.Add(_delegate9);
    // algorandManager->fetchArcAssetDetails(779312090);
    
    // FScriptDelegate _delegate5;
    // _delegate5.BindUFunction(this, FName("OnInitializeNewWalletCallback"));
    // algorandManager->InitializeNewWalletCallback.Add(_delegate5);
    // algorandManager->initializeNewWallet();

    // FScriptDelegate _delegate6;
    // _delegate6.BindUFunction(this, FName("OnGetBackupMnemonicPhraseCallback"));
    // algorandManager->GetBackupMnemonicPhraseCallback.Add(_delegate6);
    // algorandManager->getBackupMnemonicPhrase();
    //
    // FScriptDelegate _delegate7;
    // _delegate7.BindUFunction(this, FName("OnGenerateMnemonicsCallback"));
    // algorandManager->GenerateMnemonicsCallback.Add(_delegate7);
    // algorandManager->generateMnemonics();
    
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

    // FScriptDelegate _delegate3;
    // _delegate3.BindUFunction(this, FName("OnSendApplicationCallTransactionCallback"));
    // algorandManager->SendApplicationCallTransactionCallback.Add(_delegate3);
    // algorandManager->sendApplicationCallTransaction(16037129);
}