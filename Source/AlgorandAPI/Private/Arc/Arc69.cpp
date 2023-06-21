//
// Created by Bruno on 6/5/2023.
//

#include "Arc/Arc69.h"
#include <regex>
#include <math.h>

const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string atob(const std::string &encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]) & 0xff;

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;

        for (j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]) & 0xff;

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}

std::string trim(std::string str)
{
    // Trim leading spaces
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start != std::string::npos)
        str = str.substr(start);

    // Trim trailing spaces
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    if (end != std::string::npos)
        str = str.substr(0, end + 1);

    return str;
}

Arc69::Arc69(const arc_asset& asset_, FString algoRpc, uint64_t algoPort, FString algoToken) {
    // set rpc info
    myAlgoRpc = algoRpc;
    myAlgoPort = algoPort;
    myAlgoTokenHeader = algoToken;

    asset = asset_;
    this->getAssetByID(asset.index);
}

bool Arc69::IsVerify() {
    return tx.tx_type == "acfg" && tx.note.Len() > 0;
}
void Arc69::from_tx_note(std::string& tx_note) {
    std::string note = atob(tx_note);
    note = trim(note);

    std::regex pattern("/[^ -~]+/g");
    note = std::regex_replace(note, pattern, "");

    auto json_note = json_parse(note);

    try {

        if(json_note.IsValid())
        {
            for (auto itr : json_note->Values) {
                if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"standard") == 0)
                    metadata.standard = itr.Value->AsString();
                if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"description") == 0)
                    metadata.description = itr.Value->AsString();
                if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"external_url") == 0)
                    metadata.external_url = itr.Value->AsString();
                if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"media_url") == 0)
                    metadata.media_url = itr.Value->AsString();
                if (std::strcmp(TCHAR_TO_UTF8(*itr.Key) ,"mime_type") == 0)
                    metadata.mime_type = itr.Value->AsString();
            }

            if (json_note->HasField("authors")) {
                TArray<TSharedPtr<FJsonValue>> json_authors = json_note->GetArrayField("authors");
                for (TSharedPtr<FJsonValue> JsonValue : json_authors)
                {
                    if(JsonValue->Type == EJson::String)
                        metadata.authors.Add(JsonValue->AsString());
                    if(JsonValue->Type == EJson::Number)
                        metadata.authors.Add(FString::Printf(TEXT("%.*f"),0, JsonValue->AsNumber()));
                }
            }

            if (json_note->HasField("properties")) {
                TSharedPtr<FJsonObject> json_properties = json_note->GetObjectField("properties");
                if(json_properties.IsValid()) {
                    for (auto itr : json_properties->Values) {
                        if(itr.Value->Type == EJson::String)
                            metadata.properties.Add(itr.Key, itr.Value->AsString());
                        if(itr.Value->Type == EJson::Number)
                            metadata.properties.Add(itr.Key, FString::FromInt(itr.Value->AsNumber()));
                    }
                }
            }
        }
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}
