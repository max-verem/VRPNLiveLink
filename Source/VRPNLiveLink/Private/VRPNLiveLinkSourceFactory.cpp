#include "VRPNLiveLinkSourceFactory.h"
#include "VRPNLiveLinkSource.h"
#include "VRPNLiveLink.h"

#define LOCTEXT_NAMESPACE "UVRPNLiveLinkSourceFactory"

FText UVRPNLiveLinkSourceFactory::GetSourceDisplayName() const
{
    return LOCTEXT("SourceDisplayName", "VRPN LiveLink");
}

FText UVRPNLiveLinkSourceFactory::GetSourceTooltip() const
{
    return LOCTEXT("SourceTooltip", "Creates a connection to a VRPN Server");
}

TSharedPtr<SWidget> UVRPNLiveLinkSourceFactory::BuildCreationPanel(FOnLiveLinkSourceCreated InOnLiveLinkSourceCreated) const
{
    return SNew(VRPNLiveLinkSourceEditor)
        .OnOkClicked(VRPNLiveLinkSourceEditor::FOnOkClicked::CreateUObject(this, &UVRPNLiveLinkSourceFactory::OnOkClicked, InOnLiveLinkSourceCreated));
}

TSharedPtr<ILiveLinkSource> UVRPNLiveLinkSourceFactory::CreateSource(const FString& InConnectionString) const
{
/*
    FIPv4Endpoint DeviceEndPoint;
    if (!FIPv4Endpoint::Parse(InConnectionString, DeviceEndPoint))
    {
        return TSharedPtr<ILiveLinkSource>();
    }
*/
    return MakeShared<FVRPNLiveLinkSource>(FText::FromString(InConnectionString));
}

void UVRPNLiveLinkSourceFactory::OnOkClicked(FText InEndpoint, FOnLiveLinkSourceCreated InOnLiveLinkSourceCreated) const
{
    InOnLiveLinkSourceCreated.ExecuteIfBound(MakeShared<FVRPNLiveLinkSource>(InEndpoint), InEndpoint.ToString());
}

#undef LOCTEXT_NAMESPACE
