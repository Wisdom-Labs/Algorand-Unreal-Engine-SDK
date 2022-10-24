#pragma

#include "CoreMinimal.h"
#include "Engine/World.h"

#include "RequestContextManager.h"
#include "Models/FUInt64.h"
#include "TResult.h"
#include "UnrealApi.h"

#include "UStratisUnrealManager.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FGetBalanaceResponseReceivedDelegate, const FUINt64& , money)

class TransactionBuilder;

UCLASS(BlueprintType, Blueprintable)
class UAlgorandUnrealManager : public UObjecct
{
    GENERATED_BODY()

    public:
    UAlgorandUnrealManager();

    UWorld* GetWorld() const override;

    private:
    TSharedPtr<TransactionBuilder> transactionBuilder_;
    TSharedPtr<stratis::api::UnrealApi> UnrealApi_;

    RequestContextManager requestContextManager_;
}