// Fill out your copyright notice in the Description page of Project Settings.


#include "CoolerManager.h"

CoolerManager::CoolerManager()
{
}

CoolerManager::~CoolerManager()
{
    for (int i = 0; i < CoolingItems.Num(); i++)
    {
        delete CoolingItems[i];
    }
    for (int i = 0; i < ReadyItems.Num(); i++)
    {
        delete ReadyItems[i];
    }

    for (int i = 0; i < InvisibleCoolingItems.Num(); i++)
    {
        delete InvisibleCoolingItems[i];
    }
    for (int i = 0; i < InvisibleReadyItems.Num(); i++)
    {
        delete InvisibleReadyItems[i];
    }
}
void CoolerManager::Add(CoolerItem* ci, bool readyimediatly)
{
    if (readyimediatly)
    {
        ReadyItems.AddUnique(ci);
        CoolerManager::sortarrray<CoolerItem>(ReadyItems, [=](CoolerItem* a, CoolerItem* b)->bool {
            return a->priority > b->priority;
        });
    }
    else
    {
        CoolingItems.AddUnique(ci);
    }
}
void CoolerManager::tick(float delta)
{
    for (int i = 0; i < CoolingItems.Num(); i++)
    {
        CoolingItems[i]->timer -= delta;
        CoolingItems[i]->OnTimerChange(CoolingItems[i]->timer);
        //UE_LOG(LogTemp, Warning, TEXT("size : %s"), *FString::FromInt(CoolingItems.Num()));
        if (CoolingItems[i]->timer <= 0)
        {
            ReadyItems.AddUnique(CoolingItems[i]);
            CoolerManager::sortarrray<CoolerItem>(ReadyItems, [=](CoolerItem* a, CoolerItem* b)->bool {
                return a->priority > b->priority;
            });
            CoolingItems.RemoveAt(i);
            //UE_LOG(LogTemp, Warning, TEXT("RemoveAt"));
            //UE_LOG(LogTemp, Warning, TEXT("size : %s"), *FString::FromInt(CoolingItems.Num()));
        }
    }
    for (int i = 0; i < InvisibleCoolingItems.Num(); i++)
    {
        InvisibleCoolingItems[i]->timer -= delta;
        if (InvisibleCoolingItems[i]->timer <= 0)
        {
            InvisibleReadyItems.AddUnique(InvisibleCoolingItems[i]);
            InvisibleCoolingItems.RemoveAt(i);
        }
    }
}
bool CoolerManager::CheckReady(const FString& id)
{
    for (int i = 0; i < ReadyItems.Num(); i++)
    {
        if (ReadyItems[i]->id == id)
        { 
            return true;
        }
    }
    return false;
}
void CoolerManager::SetNewCoolDownTime(const FString& id,float time)
{
    for (int i = 0; i < CoolingItems.Num(); i++)
    {
        if (CoolingItems[i]->id == id)
        {
            CoolingItems[i]->coolertime = time;
        }
    }
    for (int i = 0; i < ReadyItems.Num(); i++)
    {
        if (ReadyItems[i]->id == id)
        {
            ReadyItems[i]->coolertime = time;
        }
    }
}
void CoolerManager::UseAndReCoolDown(const FString& id)
{
    for (int i = 0; i < ReadyItems.Num(); i++)
    {
        if (ReadyItems[i]->id == id)
        {
            ReadyItems[i]->timer = ReadyItems[i]->coolertime;
            CoolingItems.Add(ReadyItems[i]);
            ReadyItems.RemoveAt(i);
            CoolerManager::sortarrray<CoolerItem>(ReadyItems, [=](CoolerItem* a, CoolerItem* b)->bool {
                return a->priority > b->priority;
            });
            return;
        }
    }
}
CoolerItem* CoolerManager::GetApreferReady()
{
    if (ReadyItems.Num() > 0)
    {
        return ReadyItems[0];
    }
    return nullptr;
}
/////////////////////////////////////////////////////////////////////////
void CoolerManager::Add_Invisible(InvisibleCoolerItem* ci, bool readyimediatly)
{
    if (readyimediatly)
    {
        InvisibleReadyItems.AddUnique(ci);
    }
    else
    {
        InvisibleCoolingItems.AddUnique(ci);
    }
}
bool CoolerManager::CheckReady_Invisible(const FString& id)
{
    for (int i = 0; i < InvisibleReadyItems.Num(); i++)
    {
        if (InvisibleReadyItems[i]->id == id)
        {
            return true;
        }
    }
    return false;
}
void CoolerManager::UseAndReCoolDown_Invisible(const FString& id)
{
    for (int i = 0; i < InvisibleReadyItems.Num(); i++)
    {
        if (InvisibleReadyItems[i]->id == id)
        {
            InvisibleReadyItems[i]->timer = InvisibleReadyItems[i]->coolertime;
            InvisibleCoolingItems.Add(InvisibleReadyItems[i]);
            InvisibleReadyItems.RemoveAt(i);
            return;
        }
    }
}
void CoolerManager::SetNewCoolDownTime_Invisible(const FString& id, float time)
{
    for (int i = 0; i < InvisibleCoolingItems.Num(); i++)
    {
        if (InvisibleCoolingItems[i]->id == id)
        {
            InvisibleCoolingItems[i]->coolertime = time;
        }
    }
    for (int i = 0; i < InvisibleReadyItems.Num(); i++)
    {
        if (InvisibleReadyItems[i]->id == id)
        {
            InvisibleReadyItems[i]->coolertime = time;
        }
    }
}

