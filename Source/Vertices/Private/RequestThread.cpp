
#include "RequestThread.h"
#include "include/vertices/vertices.h"

#pragma region Main Thread Code

namespace algorand {
namespace vertices {

	FRequestWorker::FRequestWorker(TFunction<void(const FString&)> _method, const FString& _threadID)
	{
		transactionMethod = _method;
		threadID = _threadID;
		Thread = FRunnableThread::Create(this, TEXT("FRequestWorker"), 0, TPri_BelowNormal);
	}

	FRequestWorker::~FRequestWorker()
	{
		if (Thread)
		{
			// Kill() is a blocking call, it waits for the thread to finish.
			// Hopefully that doesn't take too long
			Thread->Kill();
			delete Thread;
		}
	}

#pragma endregion
	// The code below will run on the new thread.
	bool FRequestWorker::Init()
	{
		UE_LOG(LogTemp, Warning, TEXT("My custom thread has been initialized"));

			// Return false if you want to abort the thread
			return true;
	}

	uint32 FRequestWorker::Run()
	{
		// Peform your processor intensive task here. In this example, a neverending
		// task is created, which will only end when Stop is called.
		while (bRunThread)
		{
			bool boo = false;
			UE_LOG(LogTemp, Warning, TEXT("My custom thread: %s is running!"), *(threadID));
			FPlatformProcess::Sleep(0.03);
			boo = vertices_check_writable();
			if (vertices_check_writable())
			{
				transactionMethod(threadID);
				break;
			}
				
		}

		return 0;
	}


	// This function is NOT run on the new thread!
	void FRequestWorker::Stop()
	{
		// Clean up memory usage here, and make sure the Run() function stops soon
		// The main thread will be stopped until this finishes!

		// For this example, we just need to terminate the while loop
		// It will finish in <= 1 sec, due to the Sleep()
		bRunThread = false;
	}

}
}

