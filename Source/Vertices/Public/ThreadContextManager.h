#pragma once

#include "CoreMinimal.h"
#include "ThreadContext.h"
#include "RequestThread.h"
#include <functional>

namespace {
	using RequestWorker = algorand::vertices::FRequestWorker;
	using Vertices = algorand::vertices::VerticesSDK;
}

namespace algorand {
namespace vertices {

	class ThreadContextManager
	{
	public:
		template <typename DelegateType, typename RequestType , typename CallbackFunctionType>
		FString createContext(const RequestType& request, TFunction<void(const RequestType&, DelegateType)> requestMethod, CallbackFunctionType&& callback);   //, TFunction<void(const RequestType&, DelegateType)> requestMethod

		bool disposeContext(const FString& id);

		ThreadContext& getContext(const FString& id);
		const ThreadContext& getContext(const FString& id) const;

		void createVertices();

		TSharedPtr<algorand::vertices::VerticesSDK> getVertices();

		void _transactionMethod(const FString& str);

		TSharedPtr<algorand::vertices::VerticesSDK> vertices_;

	private:
		FString createUniqueID() const;

		TMap<FString, ThreadContext> contextStorage_;
	};

	inline void ThreadContextManager::_transactionMethod(const FString& threadID) {
		UE_LOG(LogTemp, Warning, TEXT("%s"), *(threadID));
		ThreadContext _context = this->getContext(threadID);
		switch (_context.method_type)
		{
			case 0: 
				this->vertices_->VerticesGetaddressbalanceGet((const Vertices::VerticesGetaddressbalanceGetRequest&)*(_context.request), _context.delegate->Get<Vertices::FVerticesGetaddressbalanceGetDelegate>());
				break;
		}
	}

	inline void ThreadContextManager::createVertices() {
		vertices_ = MakeShared<algorand::vertices::VerticesSDK>();
	}

	inline TSharedPtr<algorand::vertices::VerticesSDK> ThreadContextManager::getVertices() {
		return vertices_;
	}

	template <typename DelegateType , typename RequestType , typename CallbackFunctionType>
	inline FString ThreadContextManager::createContext(const RequestType& request, TFunction<void(const RequestType&, DelegateType)> requestMethod , CallbackFunctionType&& callback)  //
	{
		FString id(createUniqueID());
		
		TSharedPtr<VerticesDelegate> delegatePtr(MakeShared<VerticesDelegate>(TInPlaceType<DelegateType>()));
		auto& delegate = delegatePtr->Get<DelegateType>();  
		delegate.BindLambda(callback);
		
		contextStorage_.Add(id, { (algorand::vertices::Request*)&request, delegatePtr, delegatePtr->GetIndex() });  //delegate, requestMethod

		FRequestWorker requestWorker = FRequestWorker(
			std::bind(&ThreadContextManager::_transactionMethod, this,
			std::placeholders::_1), 
			id
		);
		requestWorker.Init();
		//_transactionMethod(id);
		
		//requestMethod(request, delegate);

		return id;
	}

	inline bool ThreadContextManager::disposeContext(const FString& id)
	{
		if (contextStorage_.Contains(id)) {
			contextStorage_.Remove(id);
			return true;
		}

		return false;
	}

	inline ThreadContext& ThreadContextManager::getContext(const FString& id)
	{
		return contextStorage_[id];
	}

	inline const ThreadContext& ThreadContextManager::getContext(const FString& id) const
	{
		return contextStorage_[id];
	}

	inline FString ThreadContextManager::createUniqueID() const
	{
		while (true) {
			FString id(FGuid::NewGuid().ToString());
			if (!contextStorage_.Contains(id)) return id;
		}
	}


}
}
