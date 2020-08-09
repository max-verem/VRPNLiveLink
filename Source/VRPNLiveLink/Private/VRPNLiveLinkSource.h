// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ILiveLinkSource.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"
#include "IMessageContext.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Misc/ScopeLock.h"

class FRunnableThread;
class FSocket;
class ILiveLinkClient;
class ISocketSubsystem;

class VRPNLIVELINK_API FVRPNLiveLinkSource : public ILiveLinkSource, public FRunnable
{
public:

	FVRPNLiveLinkSource(FText Endpoint);

	virtual ~FVRPNLiveLinkSource();

	// Begin ILiveLinkSource Interface
	
	virtual void ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid) override;

	virtual bool IsSourceStillValid() const override;

	virtual bool RequestSourceShutdown() override;

	virtual FText GetSourceType() const override { return SourceType; };
	virtual FText GetSourceMachineName() const override { return SourceMachineName; }
	virtual FText GetSourceStatus() const override { return SourceStatus; }

    virtual void Update() override;
	// End ILiveLinkSource Interface

	// Begin FRunnable Interface

	virtual bool Init() override { return true; }
	virtual uint32 Run() override;
	void Start();
	virtual void Stop() override;
	virtual void Exit() override { }

	// End FRunnable Interface

    void HandleTrackerCallback(void* data);
private:
    FQuat currentQuat;
    FVector currentLocation;
    FCriticalSection currentLock;

	ILiveLinkClient* Client;

	// Our identifier in LiveLink
	FGuid SourceGuid;

	FText SourceType;
	FText SourceMachineName;
	FText SourceStatus;

    FText DeviceEndpoint;

	// Threadsafe Bool for terminating the main thread loop
	FThreadSafeBool Stopping;

	// Thread to run socket operations on
	FRunnableThread* Thread;

	// Name of the sockets thread
	FString ThreadName;
};
