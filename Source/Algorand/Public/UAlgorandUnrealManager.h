#pragma

#include "CoreMinimal.h"
#include "Engine/World.h"

#include "RequestContextManager.h"
#include "Models/FUInt64.h"
#include "TResult.h"
#include "UnrealApi.h"

#include "UStratisUnrealManager.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FGetBalanceResponseReceivedDelegate, const FUINt64& , money)
DECLARE_DYNAMIC_DELEGATE_OneParam(FErrorReceivedDelegate, const FError&, error)

class TransactionBuilder;

UCLASS(BlueprintType, Blueprintable)
class UAlgorandUnrealManager : public UObjecct
{
    GENERATED_BODY()

    public:
    UAlgorandUnrealManager();

    UFUNCTION(BlueprintCallable, Category = "StratisUnrealManager")
    void getBalance(const FGetBalanceResponseReceivedDelegate& delegate,
                    const FErrorReceivedDelegate& errorDelegate);
    void getBalance(TFunction<void(const TResult<int64>&)> callback);

    UWorld* GetWorld() const override;

    private:
    TSharedPtr<TransactionBuilder> transactionBuilder_;
    TSharedPtr<stratis::api::UnrealApi> UnrealApi_;

    RequestContextManager requestContextManager_;
}