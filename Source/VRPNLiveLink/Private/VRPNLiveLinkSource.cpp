#include "VRPNLiveLinkSource.h"

#include "ILiveLinkClient.h"
#include "LiveLinkTypes.h"
#include "Roles/LiveLinkTransformRole.h"
#include "Roles/LiveLinkTransformTypes.h"
#include "Roles/LiveLinkCameraRole.h"
#include "Roles/LiveLinkCameraTypes.h"

#include "Async/Async.h"
#include "Common/UdpSocketBuilder.h"
#include "HAL/RunnableThread.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

#include "GenericPlatform/GenericPlatformProcess.h"

#define LOCTEXT_NAMESPACE "FVRPNLiveLinkSource"

#include "vrpn_Tracker.h"

FVRPNLiveLinkSource::FVRPNLiveLinkSource(FText InEndpoint)
: Stopping(false)
, Client(nullptr)
, Thread(nullptr)
{
	// defaults
	DeviceEndpoint = InEndpoint;

	SourceStatus = LOCTEXT("SourceStatus_DeviceNotFound", "Device Not Found");
	SourceType = LOCTEXT("VRPNLiveLinkSource", "VRPN LiveLink");
    SourceMachineName = DeviceEndpoint; // LOCTEXT("VRPNLiveLinkSourceMachineName", "DeviceEndpoint");

    Start();

    SourceStatus = LOCTEXT("SourceStatus_Receiving", "Receiving");
}

FVRPNLiveLinkSource::~FVRPNLiveLinkSource()
{
	Stop();
}

void FVRPNLiveLinkSource::ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid)
{
	Client = InClient;
	SourceGuid = InSourceGuid;
}


bool FVRPNLiveLinkSource::IsSourceStillValid() const
{
	// Source is valid if we have a valid thread and socket
	bool bIsSourceValid = !Stopping && Thread != nullptr;
	return bIsSourceValid;
}


bool FVRPNLiveLinkSource::RequestSourceShutdown()
{
	Stop();

	return true;
}
// FRunnable interface

void FVRPNLiveLinkSource::Start()
{
    Stopping = false;

    if (Thread == nullptr)
    {
        ThreadName = "VRPN Receiver ";
        ThreadName.AppendInt(FAsyncThreadIndex::GetNext());
        Thread = FRunnableThread::Create(this, *ThreadName, 128 * 1024, TPri_AboveNormal, FPlatformAffinity::GetPoolThreadMask());
    }
}

void FVRPNLiveLinkSource::Stop()
{
    Stopping = true;

    if (Thread != nullptr)
    {
        Thread->WaitForCompletion();
        Thread = nullptr;
    }
}

void FVRPNLiveLinkSource::HandleTrackerCallback(void* p)
{
    vrpn_TRACKERCB* t = (vrpn_TRACKERCB*)p;

    if (!IsSourceStillValid())
        return;

    FScopeLock lock(&currentLock);

    currentLocation.X = t->pos[0] * 100;
    currentLocation.Y = t->pos[1] * 100;
    currentLocation.Z = t->pos[2] * 100;

    currentQuat.X = t->quat[0];
    currentQuat.Y = t->quat[1];
    currentQuat.Z = t->quat[2];
    currentQuat.W = t->quat[3];
}

void VRPN_CALLBACK handle_tracker(void* userData, const vrpn_TRACKERCB t)
{
    FVRPNLiveLinkSource* ctx = (FVRPNLiveLinkSource*)userData;

    ctx->HandleTrackerCallback((void*)&t);
}

uint32 FVRPNLiveLinkSource::Run()
{
    vrpn_Tracker_Remote *tkr;
    FString conn_fs = DeviceEndpoint.ToString();
    std::string conn_std = std::string(TCHAR_TO_UTF8(*conn_fs));
        
    tkr = new vrpn_Tracker_Remote(conn_std.c_str());

    tkr->register_change_handler(this, handle_tracker);

    while (!Stopping)
        tkr->mainloop();

    delete tkr;

    return 0;
}

void FVRPNLiveLinkSource::Update()
{
    if (Client)
    {
        FName SubjectName = FName(DeviceEndpoint.ToString());

        FVector tScale = FVector(1.0, 1.0, 1.0);
        FTransform tTransform;
        
        {
            FScopeLock lock(&currentLock);
            tTransform = FTransform(currentQuat.Inverse(), currentLocation, tScale);
        };

        FLiveLinkStaticDataStruct TransformStaticDataStruct = FLiveLinkStaticDataStruct(FLiveLinkTransformStaticData::StaticStruct());
        FLiveLinkTransformStaticData& TransformStaticData = *TransformStaticDataStruct.Cast<FLiveLinkTransformStaticData>();
        Client->PushSubjectStaticData_AnyThread({ SourceGuid, SubjectName }, ULiveLinkTransformRole::StaticClass(), MoveTemp(TransformStaticDataStruct));

        FLiveLinkFrameDataStruct TransformFrameDataStruct = FLiveLinkFrameDataStruct(FLiveLinkTransformFrameData::StaticStruct());
        FLiveLinkTransformFrameData& TransformFrameData = *TransformFrameDataStruct.Cast<FLiveLinkTransformFrameData>();
        TransformFrameData.Transform = tTransform;
        Client->PushSubjectFrameData_AnyThread({ SourceGuid, SubjectName }, MoveTemp(TransformFrameDataStruct));
    }
}

#undef LOCTEXT_NAMESPACE
