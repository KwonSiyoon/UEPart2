// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABCharacterStatWidget.h"
#include "Components/TextBlock.h"

void UABCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();


	for (TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct());	PropIt;	++PropIt)
	{
		// 속성의 키값(이름 값).
		const FName PropKey(PropIt->GetName());

		// Base 스탯에 대한 텍스트 블록 이름.
		const FName TextBaseControlName	= *FString::Printf(TEXT("Txt%sBase"), *PropIt->GetName());

		// Modifier 스탯에 대한 텍스트 블록 이름.
		const FName TextModifierControlName	= *FString::Printf(TEXT("Txt%sModifier"), *PropIt->GetName());

		// 이름 값을 사용해 텍스트 블록 로드.
		UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextBaseControlName));

		// 제대로 찾았으면,
		if (BaseTextBlock)
		{
			BaseLookup.Add(PropKey, BaseTextBlock);
		}

		// 이름 값을 사용해 텍스트 블록 로드.
		UTextBlock* ModifierTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextModifierControlName));

		// 제대로 찾았으면,
		if (ModifierTextBlock)
		{
			ModifierLookup.Add(PropKey, ModifierTextBlock);
		}
	}
}

void UABCharacterStatWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	for (TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct());	PropIt;	++PropIt)
	{
		const FName PropKey(PropIt->GetName());

		float BaseData = 0.0f;

		PropIt->GetValue_InContainer((const void*)&BaseStat, &BaseData);

		float ModifierData = 0.0f;

		PropIt->GetValue_InContainer((const void*)&ModifierStat, &ModifierData);

		UTextBlock** BaseTextBlockPtr = BaseLookup.Find(PropKey);
		if (BaseTextBlockPtr)
		{
			(*BaseTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(BaseData)));
		}

		UTextBlock** ModifierTextBlockPtr = ModifierLookup.Find(PropKey);
		if (ModifierTextBlockPtr)
		{
			(*ModifierTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(ModifierData)));
		}
	}
}
