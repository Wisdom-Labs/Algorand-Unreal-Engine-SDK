#include "AlgorandHandler.h"

UAlgorandHandler::UAlgorandHandler() { 
	algorandManager = 
		CreateDefaultSubobject<UAlgorandUnrealManager>(TEXT("AlgorandManager"));
}

UWorld* UAlgorandHandler::GetWorld() const { return GetOuter()->GetWorld(); }

void UAlgorandHandler::OnRestoreWalletCallback(const FString& output) {
    UE_LOG(LogTemp, Display, TEXT("Generated address: %s"),
        *output);
    
    // FScriptDelegate _delegate1;
    // _delegate1.BindUFunction(this, FName("OnGetBalanceCallback"));
    // algorandManager->GetBalanceCallback.Add(_delegate1);
    // algorandManager->getBalance();

    // FScriptDelegate _delegate2;
    // _delegate2.BindUFunction(this, FName("OnSendPaymentTransactionCallback"));
    // algorandManager->SendPaymentTransactionCallback.Add(_delegate2);
    // algorandManager->sendPaymentTransaction(FString("NBRUQXLMEJDQLHE5BBEFBQ3FF4F3BZYWCUBBQM67X6EOEW2WHGS764OQXE"), 100, FString("Sent 100 algo to NBRUQXLMEJDQLHE5BBEFBQ3FF4F3BZYWCUBBQM67X6EOEW2WHGS764OQXE."));
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

void UAlgorandHandler::OnSendApplicationCallTransactionCallback(const FString& txID) {
    UE_LOG(LogTemp, Display, TEXT("Application Call TX ID: %s"),
        *txID);
}

void UAlgorandHandler::RunSomeLogic() {
    // FScriptDelegate _delegate4;
    // _delegate4.BindUFunction(this, FName("OnRestoreWalletCallback"));
    // algorandManager->RestoreWalletCallback.Add(_delegate4);
    // FString mnemonics = "base giraffe believe make tone transfer wrap attend typical dirt grocery distance outside horn also abstract slim ecology island alter daring equal boil absent carpet";
    // algorandManager->restoreWallet(mnemonics);

    FScriptDelegate _delegate5;
    _delegate5.BindUFunction(this, FName("OnInitializeNewWalletCallback"));
    algorandManager->InitializeNewWalletCallback.Add(_delegate5);
    algorandManager->initializeNewWallet();

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

    // FScriptDelegate _delegate3;
    // _delegate3.BindUFunction(this, FName("OnSendApplicationCallTransactionCallback"));
    // algorandManager->SendApplicationCallTransactionCallback.Add(_delegate3);
    // algorandManager->sendApplicationCallTransaction(16037129);

}