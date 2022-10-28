#pragma once

#include "CoreMinimal.h"
#include <functional>

namespace algorand {
namespace vertices {
    
    class VERTICES_API FRequestWorker : public FRunnable
    {

    public:
        TFunction<void(const FString&)> transactionMethod;
        FString threadID;

        FRequestWorker(TFunction<void(const FString&)>, const FString&);
    
        virtual ~FRequestWorker();

        // Begin FRunnable interface.
        bool Init();
        uint32 Run();
        void Stop();
        // End FRunnable interface

    private:

        FRunnableThread* Thread;

        bool bRunThread;

    };

}
}
