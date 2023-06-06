// Copyright 2023, Wisdom Labs. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameFramework/Actor.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "UAlgorandDownloader.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAlgorandDownloadProgress, int32,
                                               RecvSize, int32, TotalSize,
                                               float, Percent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAlgorandGetArcAssetImageFromUrl,
                                             UTexture2D *, NftImage, FString,
                                             Result);

UCLASS(BlueprintType)
class ALGORAND_API UAlgorandDownloader : public UObject {
  GENERATED_BODY()

protected:
  FHttpModule *HttpModule;

public:
  FString SrcIpfs;          // "ipfs://"
  FString DstIpfs;          // "https://ipfs.io/ipfs/"
  static FString UserAgent; // "CronosPlay-UnrealEngine-Agent"

  UAlgorandDownloader();
  FString convert_ipfs_url(const FString &src);

  UPROPERTY(BlueprintAssignable)
  FAlgorandDownloadProgress AlgorandDownloadProgressCallback;
  void OnAlograndDownloadProgressCallback(FHttpRequestPtr RequestPtr,
                                      int32 SendBytes, int32 RecvBytes);

  /**
   * SetupIpfsConverting
   * @param src ipfs uri "ipfs://"
   * @param dst ipfs url "https://ipfs.io/ipfs/"
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "SetupIpfsConverting",
                    Keywords = "Nft,Ipfs,Uri,Url"),
            Category = "UAlgorandDownloader")

  void SetupIpfsConverting(FString src, FString dst);

  void OnGetNftImageInfoFromUriCompleteCallback(FHttpRequestPtr httprequest,
                                                FHttpResponsePtr httpresponse,
                                                bool connectedSuccessfully);

  /**
   * GetNftImageFromUrl
   * CAUTION: This function is dangerous. Validate imageurl and check , because
   * it can download any data from arbitrary url. before calling this api,
   * please check the image url is valid. validity check references:
   * @param imageurl  imageurl to get nft image
   * @param success success of apicall
   * @param output_message  error message of apicall
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetNftImageFromUrl",
                    Keywords = "Nft,Ipfs,Image,Texture,Url"),
            Category = "AlgorandDownloader")

  void GetNftImageFromUrl(FString imageurl, bool &success,
                          FString &output_message);
  UPROPERTY(BlueprintAssignable)
  FAlgorandGetArcAssetImageFromUrl GetNftImageFromUrlCallback;

  void OnGetNftImageFromUrlCompleteCallback(FHttpRequestPtr httprequest,
                                            FHttpResponsePtr httpresponse,
                                            bool connectedSuccessfully);
};
