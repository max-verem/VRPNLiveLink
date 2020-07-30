#include "VRPNLiveLinkSourceEditor.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "VRPNLiveLinkSourceEditor"

void VRPNLiveLinkSourceEditor::Construct(const FArguments& Args)
{
	OkClicked = Args._OnOkClicked;

    FText Endpoint = FText::FromString("virtual/CAMERA-54@127.0.0.1:3885");

	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(450)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.FillWidth(0.25f)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("VRPNAddress", "VRPN address"))
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.FillWidth(0.75f)
				[
					SAssignNew(EditabledText, SEditableTextBox)
					.Text(Endpoint)
					.OnTextCommitted(this, &VRPNLiveLinkSourceEditor::OnEndpointChanged)
				]
			]
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Right)
			.AutoHeight()
			[
				SNew(SButton)
				.OnClicked(this, &VRPNLiveLinkSourceEditor::OnOkClicked)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Ok", "Ok"))
				]
			]
		]
	];
}

void VRPNLiveLinkSourceEditor::OnEndpointChanged(const FText& NewValue, ETextCommit::Type)
{
	TSharedPtr<SEditableTextBox> EditabledTextPin = EditabledText.Pin();
	if (EditabledTextPin.IsValid())
	{
	}
}

FReply VRPNLiveLinkSourceEditor::OnOkClicked()
{
    TSharedPtr<SEditableTextBox> EditabledTextPin = EditabledText.Pin();
    if (EditabledTextPin.IsValid())
    {
        OkClicked.ExecuteIfBound(EditabledTextPin->GetText());
    }
    return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE