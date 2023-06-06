// Fill out your copyright notice in the Description page of Project Settings.


#include "UAlgorandDownloader.h"
#include <cassert>
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/KismetRenderingLibrary.h"

FString UAlgorandDownloader::UserAgent =
    TEXT("CronosPlay-UnrealEngine-Agent");
// ipfs:// -> https://ipfs.io/ipfs/
FString UAlgorandDownloader::convert_ipfs_url(const FString &src) {
  FString dst = src.Replace(*SrcIpfs, *DstIpfs);
  return dst;
}

void UAlgorandDownloader::SetupIpfsConverting(FString src, FString dst) {
  SrcIpfs = src;
  DstIpfs = dst;
}

// Sets default values
UAlgorandDownloader::UAlgorandDownloader() {
  HttpModule = &FHttpModule::Get();
  SrcIpfs = TEXT("ipfs://");
  DstIpfs = TEXT("https://ipfs.io/ipfs/");
}

void UAlgorandDownloader::OnAlograndDownloadProgressCallback(
    FHttpRequestPtr RequestPtr, int32 SendBytes, int32 RecvBytes) {

  int32 TotalSize = RequestPtr->GetResponse()->GetContentLength();
  float Percent = (float)RecvBytes / TotalSize;

  AlgorandDownloadProgressCallback.Broadcast(RecvBytes, TotalSize, Percent);
}

void UAlgorandDownloader::GetNftImageFromUrl(FString imageurl, bool &success,
                                               FString &output_message) {

  if (!GetNftImageFromUrlCallback.IsBound()) {
    success = false;
    output_message = "GetNftImageFromUrlCallback is not bound";
    return;
  }

  assert(HttpModule != NULL);
  TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httprequest =
      HttpModule->CreateRequest();
  httprequest->SetVerb(TEXT("GET"));
  httprequest->SetHeader(TEXT("User-Agent"), UserAgent);
  httprequest->SetURL(convert_ipfs_url(imageurl));

  httprequest->OnRequestProgress().BindUObject(
      this, &UAlgorandDownloader::OnAlograndDownloadProgressCallback);

  httprequest->OnProcessRequestComplete().BindUObject(
      this, &UAlgorandDownloader::OnGetNftImageFromUrlCompleteCallback);
  httprequest->ProcessRequest();
  output_message = TEXT("");
}

void UAlgorandDownloader::OnGetNftImageFromUrlCompleteCallback(
    FHttpRequestPtr httprequest, FHttpResponsePtr httpresponse,
    bool connectedSuccessfully) {
  UTexture2D *nftimage = NULL;
  FString result;
  if (connectedSuccessfully) {
    // dangerous. Validate imageurl and check validity
    nftimage = UKismetRenderingLibrary::ImportBufferAsTexture2D(
        NULL, httpresponse->GetContent());
    if (NULL == nftimage) {
      result = TEXT("Failed to import image.");
    } else {
      result = TEXT("");
    }
  } else {
    switch (httprequest->GetStatus()) {
    case EHttpRequestStatus::Failed_ConnectionError:
      result = TEXT("Connection failed.");
    default:
      result = TEXT("Request failed.");
    }
  }
  GetNftImageFromUrlCallback.Broadcast(nftimage, result);
}
