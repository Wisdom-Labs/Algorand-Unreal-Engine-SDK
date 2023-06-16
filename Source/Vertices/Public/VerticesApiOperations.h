// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "CoreMinimal.h"
#include "BaseModel.h"

namespace algorand {
    namespace vertices {
        
        // Request for Restore wallet
        class VerticesSDK::VerticesRestoreWalletGetRequest : public Request
        {
        public:
            virtual ~VerticesRestoreWalletGetRequest() {}
            TOptional<FString> Mnemonics;
        };

        // Response for Restore wallet
        class VerticesSDK::VerticesRestoreWalletGetResponse : public Response
        {
        public:
            ~VerticesRestoreWalletGetResponse() {}
            FString output;
        };

        // Request for Initialize New wallet
        class VerticesSDK::VerticesInitializeNewWalletGetRequest : public Request
        {
        public:
            virtual ~VerticesInitializeNewWalletGetRequest() {}
        };

        // Response for Initialize New wallet
        class VerticesSDK::VerticesInitializeNewWalletGetResponse : public Response
        {
        public:
            ~VerticesInitializeNewWalletGetResponse() {}
            FString output;
        };

        // Request for Get Backup Mnemonic Phrase
        class VerticesSDK::VerticesGetBackupMnemonicPhraseGetRequest : public Request
        {
        public:
            virtual ~VerticesGetBackupMnemonicPhraseGetRequest() {}
        };

        // Response for Get Backup Mnemonic Phrase
        class VerticesSDK::VerticesGetBackupMnemonicPhraseGetResponse : public Response
        {
        public:
            ~VerticesGetBackupMnemonicPhraseGetResponse() {}
            FString output;
        };

        // Request for Generate Mnemonics
        class VerticesSDK::VerticesGenerateMnemonicsGetRequest : public Request
        {
        public:
            virtual ~VerticesGenerateMnemonicsGetRequest() {}
        };

        // Response for Generate Mnemonics
        class VerticesSDK::VerticesGenerateMnemonicsGetResponse : public Response
        {
        public:
            ~VerticesGenerateMnemonicsGetResponse() {}
            FString output;
        };

        // Request for Get Address
        class VerticesSDK::VerticesGetaddressbalanceGetRequest : public Request
        {
        public:
            virtual ~VerticesGetaddressbalanceGetRequest() {}

            TOptional<FString> Address;
        };

        // Response for Get Address
        class VerticesSDK::VerticesGetaddressbalanceGetResponse : public Response
        {
        public:
            ~VerticesGetaddressbalanceGetResponse() {}
            uint64 Amount;
        };

        // Request for Payment TX
        class VerticesSDK::VerticesPaymentTransactionGetRequest : public Request
        {
        public:
            virtual ~VerticesPaymentTransactionGetRequest() {}

            TOptional<FString> senderAddress;
            TOptional<FString> receiverAddress;
            TOptional<uint64_t> amount;
            TOptional<FString> notes;
        };

        // Response for Payment TX
        class VerticesSDK::VerticesPaymentTransactionGetResponse : public Response
        {
        public:
            ~VerticesPaymentTransactionGetResponse() {}
            FString txID;
        };

        // Request for Asset Config TX
        class VerticesSDK::VerticesAssetConfigTransactionGetRequest : public Request
        {
        public:
            virtual ~VerticesAssetConfigTransactionGetRequest() {}

            TOptional<FString>  Creator; 
            TOptional<FString>  Manager;
            TOptional<FString>  Reserve;
            TOptional<FString>  Freeze;
            TOptional<FString>  Clawback;
            TOptional<uint64_t>  AssetId;
            TOptional<uint64_t>  Total;
            TOptional<uint64_t>  Decimals;
            TOptional<uint8_t>  IsFrozen;
            TOptional<FString>  UnitName;
            TOptional<FString>  AssetName;
            TOptional<FString>  Url;
            TOptional<FString>  Notes;
        };

        // Response for Asset Config TX
        class VerticesSDK::VerticesAssetConfigTransactionGetResponse : public Response
        {
        public:
            ~VerticesAssetConfigTransactionGetResponse() {}
            FString txID;
            uint64 assetID;
        };

        // Request for Asset Transfer TX
        class VerticesSDK::VerticesAssetTransferTransactionGetRequest : public Request
        {
        public:
            virtual ~VerticesAssetTransferTransactionGetRequest() {}

            TOptional<FString> senderAddress;
            TOptional<FString> receiverAddress;
            TOptional<uint64_t> asset_id;
            TOptional<double> amount;
            TOptional<FString> notes;
        };

        // Response for Asset Transfer TX
        class VerticesSDK::VerticesAssetTransferTransactionGetResponse : public Response
        {
        public:
            ~VerticesAssetTransferTransactionGetResponse() {}
            FString txID;
        };

        // Request for Application Call TX
        class VerticesSDK::VerticesApplicationCallTransactionGetRequest : public Request
        {
        public:
            virtual ~VerticesApplicationCallTransactionGetRequest() {}

            TOptional<FString> senderAddress;
            TOptional<uint64_t> app_ID;
        };

        // Response for Application Call TX
        class VerticesSDK::VerticesApplicationCallTransactionGetResponse : public Response
        {
        public:
            ~VerticesApplicationCallTransactionGetResponse() {}
            FString txID;
        };

        // Request for Arc Asset Details
        class VerticesSDK::VerticesArcAssetDetailsGetRequest : public Request
        {
        public:
            virtual ~VerticesArcAssetDetailsGetRequest() {}
            
            TOptional<uint64_t> asset_ID;
        };

        // Response for Arc Asset Details
        class VerticesSDK::VerticesArcAssetDetailsGetResponse : public Response
        {
        public:
            ~VerticesArcAssetDetailsGetResponse() {}
            
            uint8 standard;
            FString unit_name;
            uint64_t total;
            uint64_t decimals;
            FString description;
            FString clawback;
            FString creator;
            FString freeze;
            FString manager;
            FString reserve;
            FString media_url;
            FString external_url;
            FString animation_url;
            TMap<FString, FString> properties;
        };
    }
}