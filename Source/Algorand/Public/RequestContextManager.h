// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "CoreMinimal.h"
#include "RequestContext.h"
#include "VerticesSDK.h"
#include <functional>

/**
 * @brief request context manager
 * how context works for request and response of api
 * template function is used for build api request
 * ue5 async task is used for return api response
 */

namespace {
	// Export procedures
	
	using Vertices = algorand::vertices::VerticesSDK;
}

class RequestContextManager
{
public:
	
	/**
	 * @brief generate context for api request and response
	 * @tparam DelegateType 
	 * @tparam RequestType 
	 * @tparam CallbackFunctionType 
	 * @param request this type is defined from Vertices request by api type
	 * @param requestMethod is used to build api request and have delegate to let async task enable after the request 
	 * @param callback is function to process after get response of api request
	 * @return return generated context id as string
	 */
	template <typename DelegateType, typename RequestType, typename CallbackFunctionType>
	FString createContext(const RequestType& request, TFunction<void(const RequestType&, DelegateType)> requestMethod, CallbackFunctionType&& callback);

	/**
	 * @brief dispose context after finished async task of api
	 * @param id is used for dispose processed context
	 * @return reflects dispose status as bool, if true, successful, else not dispose
	 */
	bool disposeContext(const FString& id);

	/**
	 * @brief get context with id
	 * @param id context id to get
	 * @return generated context info as RequestContext
	 */
	RequestContext& getContext(const FString& id);

private:
	
	/**
	 * @brief create unique id for new context 
	 * @return return created ID as string
	 */
	FString createUniqueID() const;

	/// storage to save generated context 
	TMap<FString, RequestContext> contextStorage_;
};

// delegate pointer bind lambda function to access callback function with response data
template <typename DelegateType, typename RequestType, typename CallbackFunctionType>
inline FString RequestContextManager::createContext(const RequestType& request, TFunction<void(const RequestType&, DelegateType)> requestMethod, CallbackFunctionType&& callback)
{
	FString id(createUniqueID());

	TSharedPtr<ApiDelegate> delegatePtr(MakeShared<ApiDelegate>(TInPlaceType<DelegateType>()));

	auto& delegate = delegatePtr->Get<DelegateType>();
	delegate.BindLambda(
		[this, callback = std::move(callback), contextID = id](const auto& response) {
		callback(response);
		this->disposeContext(contextID);
	});

	requestMethod(request, delegate);
	//contextStorage_.Add(id, { requestMethod(request, delegate), delegatePtr });

	return id;
}

// remove context if id exists in context storage
inline bool RequestContextManager::disposeContext(const FString& id)
{
	if (contextStorage_.Contains(id)) {
		contextStorage_.Remove(id);
		return true;
	}

	return false;
}

inline RequestContext& RequestContextManager::getContext(const FString& id)
{
	return contextStorage_[id];
}

// generate unique id as string with FGuid
inline FString RequestContextManager::createUniqueID() const
{
	while (true) {
		FString id(FGuid::NewGuid().ToString());
		if (!contextStorage_.Contains(id)) return id;
	}
}
