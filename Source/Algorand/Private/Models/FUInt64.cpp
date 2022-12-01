// Copyright 2022, Wisdom Labs. All Rights Reserved

#include "Models/FUInt64.h"

FUInt64::operator uint64() const { return this->Value; }

FUInt64::FUInt64() : Value(0) {}

FUInt64::FUInt64(uint64 value) : Value(value) {}

// convert using Strtoui64
FUInt64 UUInt64Factory::UInt64FromString(const FString& value)
{
    return FUInt64(FCString::Strtoui64(*value, NULL, 10));
}
