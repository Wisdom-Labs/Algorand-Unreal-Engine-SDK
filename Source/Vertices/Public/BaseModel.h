// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once

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