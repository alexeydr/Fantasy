#include "UI/Emoji/EmojiWidget.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

void UEmojiWidget::SetNewEmoji(UTexture2D* Image)
{
	if (Emoji)
	{
		if (Image)
		{
			Emoji->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Emoji->SetBrushFromTexture(Image);
		}
		else
		{
			Emoji->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}