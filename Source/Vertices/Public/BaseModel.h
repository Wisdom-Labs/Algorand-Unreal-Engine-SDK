#pragma once

namespace algorand {
namespace vertices {

    class Request
    {
    public:
        virtual ~Request() {}
    };

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