// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BS_BaseManager.h"
#include "BS_DeviceType.h"
#include "BS_InformationManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_InformationManager, Log, All);

DECLARE_DELEGATE_OneParam(FMTU_CallbackLocal, uint16);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMTU_Callback, uint16, MTU);

DECLARE_DELEGATE_OneParam(FIdCallbackLocal, FString &);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIdCallback, FString &, Id);

DECLARE_DELEGATE_OneParam(FNameCallbackLocal, FString &);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNameCallback, FString &, Name);

DECLARE_DELEGATE_OneParam(FTypeCallbackLocal, EBS_DeviceType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTypeCallback, EBS_DeviceType, Type);

DECLARE_DELEGATE_OneParam(FCurrentTimeCallbackLocal, uint64);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrentTimeCallback, float, CurrentTime);

#define EBS_MTU_Message BS_MessageGetMTU
#define EBS_DeviceIdMessage BS_MessageGetId
#define EBS_DeviceNameMessage BS_MessageGetName, \
							  BS_MessageSetName
#define EBS_DeviceTypeMessage BS_MessageGetType, \
							  BS_MessageSetType
#define EBS_CurrentTimeMessage BS_MessageGetCurrentTime, \
							   BS_MessageSetCurrentTime

UCLASS()
class UBS_InformationManager : public UBS_BaseManager
{
	GENERATED_BODY()

public:
	void Reset() override;
	bool OnRxMessage(uint8 MessageType, const TArray<uint8> &Message) override;

	// MTU START
public:
	uint16 GetMTU() const { return MTU; }
	FMTU_CallbackLocal OnMTU_Update;

	uint16 GetMaxTxMessageLength() const { return MTU == 0 ? 0 : MTU - 3; }

private:
	uint16 MTU = 0;
	void ParseMTU(const TArray<uint8> &Message);
	// MTU END

	// ID START
public:
	const FString &GetId() const { return Id; }
	FIdCallbackLocal OnIdUpdate;

private:
	FString Id;
	void ParseId(const TArray<uint8> &Message);
	// ID END

	// NAME START
public:
	const FString &GetName() const { return Name; }
	FNameCallbackLocal OnNameUpdate;

	static const uint8 MinNameLength;
	static const uint8 MaxNameLength;
	void SetName(const FString &NewName);

private:
	FString Name;
	void ParseName(const TArray<uint8> &Message);
	// NAME END

	// TYPE START
public:
	const EBS_DeviceType &GetType() const { return Type; }
	FTypeCallbackLocal OnTypeUpdate;

	void SetType(const EBS_DeviceType NewType);

private:
	EBS_DeviceType Type;
	void ParseType(const TArray<uint8> &Message);
	// TYPE END

	// CURRENT TIME START
public:
	const uint64 &GetCurrentTime() const { return CurrentTime; }
	FCurrentTimeCallbackLocal OnCurrentTimeUpdate;

private:
	uint64 CurrentTime;
	void ParseCurrentTime(const TArray<uint8> &Message);

	void UpdateCurrentTime();
	// CURRENT TIME END
};
