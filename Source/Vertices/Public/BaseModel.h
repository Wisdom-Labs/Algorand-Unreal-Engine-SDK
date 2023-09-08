// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once

enum EAppCompleteTX
{
    NOOP = 0,
    OPT_IN = 1,
    CLOSE_OUT = 2,
    CLEAR_STATE = 3,
    UPDATE_APP = 4,
    DELETE_APP = 5
};

namespace algorand {
namespace vertices {

    // Base Request class
    class Request
    {
    public:
        virtual ~Request() {}
    };

    // Base Response class
    class Response
    {
    public:
        virtual ~Response() {}

        void SetSuccessful(bool InSuccessful) { Successful = InSuccessful; }
        bool IsSuccessful() const { return Successful; }

        void SetResponseString(const FString& InResponseString) { ResponseString = InResponseString; }
        const FString& GetResponseString() const { return ResponseString; }

    private:
        bool Successful;
        FString ResponseString;
    };

}
}