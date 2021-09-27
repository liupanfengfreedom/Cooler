// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class CoolerItem
{
public:
	int priority;
	FString id;
	float timer;
	float coolertime;
	void* obj;
	TFunction<void(float)> OnTimerChange;
	CoolerItem(int priority,const FString& id, float coolertime,TFunction<void(float)>ontimerchangefunction ,void* obj=nullptr)
	{
		this->id = id;
		this->priority = priority;
		this->timer = this->coolertime = coolertime;
		this->obj = obj;
		this->OnTimerChange = ontimerchangefunction;
	}
};
class InvisibleCoolerItem
{
public:

	FString id;
	float timer;
	float coolertime;

	InvisibleCoolerItem(const FString& id, float coolertime)
	{
		this->id = id;
		this->timer = this->coolertime = coolertime;
	}
};
class COOLER_API CoolerManager
{
	TArray<CoolerItem*>CoolingItems;
	TArray<CoolerItem*>ReadyItems;
// ///////////////////////////////////
	TArray<InvisibleCoolerItem*>InvisibleCoolingItems;
	TArray<InvisibleCoolerItem*>InvisibleReadyItems;
	template <class T>
	static void sortarrray(TArray<T*>& arr, TFunction<bool(T* A, T* B)> compare)
	{
		for (int i = 0; i < arr.Num() - 1; i++)
		{
			T* flag = (T*)arr[i];
			int ti = i;
			for (int j = i + 1; j < arr.Num(); j++)
			{
				if (compare(arr[j], flag))
				{
					flag = (T*)arr[j];
					ti = j;
				}
			}
			arr[ti] = arr[i];
			arr[i] = flag;
		}
	};
public:
	CoolerManager();
	~CoolerManager();
	void tick(float delta);
	void Add(CoolerItem* ci,bool readyimediatly=true);
	bool CheckReady(const FString& id);
	void UseAndReCoolDown(const FString& id);
	void SetNewCoolDownTime(const FString& id, float time);
	CoolerItem* GetApreferReady();
//////////////////////////////////////////
	void Add_Invisible(InvisibleCoolerItem* ci, bool readyimediatly = true);
	bool CheckReady_Invisible(const FString& id);
	void UseAndReCoolDown_Invisible(const FString& id);
	void SetNewCoolDownTime_Invisible(const FString& id, float time);

};
