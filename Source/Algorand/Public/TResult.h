// Copyright © 2022 Stratis Platform.
//
// This file is part of Stratis Plugin for Unreal Engine. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "CoreMinimal.h"

#include <type_traits>

struct TError {
    TError(const FString& errorMessage) : message(errorMessage) {}
    FString message;
};

template <class T>
using TResult = TVariant<T, TError>;

namespace result {

template <class T>
inline TResult<typename std::decay<T>::type> ok(T&& value)
{
    return TResult<typename std::decay<T>::type>(
        TInPlaceType<typename std::decay<T>::type>(), value);
}

template <class T>
TResult<T> error(const TError& error)
{
    return TResult<T>(TInPlaceType<TError>(), error);
}

template <class T>
bool isSuccessful(const TResult<T>& result)
{
    return result.template IsType<T>();
}

template <class T>
const T& getValue(const TResult<T>& result)
{
    return result.template Get<T>();
}

template <class T>
const TError& getError(const TResult<T>& result)
{
    return result.template Get<TError>();
}

template <class T>
const FString& getErrorMessage(const TResult<T>& result)
{
    return result.template Get<TError>().message;
}

template <class T, class R>
inline TResult<R> transform(const TResult<T>& value,
                            TFunction<TResult<R>(const T&)> transformation)
{
    if (isSuccessful(value)) {
        return transformation(getValue(value));
    } else {
        return error<R>(getError(value));
    }
}

template <class T, class R>
inline TResult<R> transformValue(const TResult<T>& value,
                                 TFunction<R(const T&)> transformation)
{
    if (isSuccessful(value)) {
        return ok<R>(transformation(getValue(value)));
    } else {
        return error<R>(getError(value));
    }
}

} // namespace result
