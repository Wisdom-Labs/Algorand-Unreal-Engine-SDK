#include "AlgorandHandler.h"

UAlgorandHandler::UAlgorandHandler() { 
	algorandManager = 
		CreateDefaultSubobject<UAlgorandUnrealManager>(TEXT("AlgorandManager"));
}

UWorld* UAlgorandHandler::GetWorld() const { return GetOuter()->GetWorld(); }

void UAlgorandHandler::OnGenerateWalletCallback(const FString& address) {
    UE_LOG(LogTemp, Display, TEXT("Generated address in satoshis: %s"),
        *address);
}

void UAlgorandHandler::OnGetBalanceCallback(const FUInt64& amount) {
    UE_LOG(LogTemp, Display, TEXT("Balance in satoshis: %llu"),
        amount.Value);
}

void UAlgorandHandler::OnSendPaymentTransactionCallback(const FString& txID) {
    UE_LOG(LogTemp, Display, TEXT("Payment TX ID in satoshis: %s"),
        *txID);
}

void UAlgorandHandler::OnSendApplicationCallTransactionCallback(const FString& txID) {
    UE_LOG(LogTemp, Display, TEXT("Application Call TX ID in satoshis: %s"),
        *txID);
}

void UAlgorandHandler::RunSomeLogic() {
    /*FScriptDelegate _delegate1;
    _delegate1.BindUFunction(this, FName("OnGetBalanceCallback"));
    algorandManager->GetBalanceCallback.Add(_delegate1);
    algorandManager->getBalance();*/
    /*FScriptDelegate _delegate2;
    _delegate2.BindUFunction(this, FName("OnSendPaymentTransactionCallback"));
    algorandManager->SendPaymentTransactionCallback.Add(_delegate2);
    algorandManager->sendPaymentTransaction(FString("NBRUQXLMEJDQLHE5BBEFBQ3FF4F3BZYWCUBBQM67X6EOEW2WHGS764OQXE"), 100, FString("Sent 100 algo to NBRUQXLMEJDQLHE5BBEFBQ3FF4F3BZYWCUBBQM67X6EOEW2WHGS764OQXE."));*/

    FScriptDelegate _delegate3;
    _delegate3.BindUFunction(this, FName("OnSendApplicationCallTransactionCallback"));
    algorandManager->SendApplicationCallTransactionCallback.Add(_delegate3);
    algorandManager->sendApplicationCallTransaction(16037129);

    /*FScriptDelegate _delegate4;
    _delegate4.BindUFunction(this, FName("OnGenerateWalletCallback"));
    algorandManager->GenerateWalletCallback.Add(_delegate4);
    algorandManager->generateWallet();*/

}