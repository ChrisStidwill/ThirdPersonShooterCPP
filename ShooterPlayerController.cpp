// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/SlateWrapperTypes.h" // Not sure if this include is needed.


void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    CreateCrosshairsHUD();
    CreatePlayerHUD();
}

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    if (bIsWinner)
    {
        UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
        if (WinScreen != nullptr)
        {
            WinScreen->AddToViewport();
        }
    }
    else
    {
        UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
        if (LoseScreen != nullptr)
        {
            LoseScreen->AddToViewport();
            Crosshairs->SetVisibility(ESlateVisibility::Collapsed);
            PlayerHUD->SetVisibility(ESlateVisibility::Collapsed);
        }
    }

    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}

void AShooterPlayerController::CreateCrosshairsHUD()
{
    if (CrosshairsClass == nullptr) return;
    Crosshairs = CreateWidget(this, CrosshairsClass);
    Crosshairs->AddToViewport();
    Crosshairs->SetVisibility(ESlateVisibility::Collapsed);
}

void AShooterPlayerController::CreatePlayerHUD()
{
    if (PlayerHUDClass == nullptr) return;
    PlayerHUD = CreateWidget(this, PlayerHUDClass);
    PlayerHUD->AddToViewport();
}

void AShooterPlayerController::SetCrosshairsHidden(bool HideCrosshairs)
{
    if (Crosshairs == nullptr) return;
    if (HideCrosshairs)
    {
        Crosshairs->SetVisibility(ESlateVisibility::Collapsed);
    } 
    else
    {
        Crosshairs->SetVisibility(ESlateVisibility::Visible);
    }
}


