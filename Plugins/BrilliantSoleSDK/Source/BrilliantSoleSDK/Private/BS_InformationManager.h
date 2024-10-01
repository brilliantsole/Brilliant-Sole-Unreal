// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BS_BaseManager.h"
#include "BS_DeviceType.h"
#include "BS_InformationMessageType.h"
#include "BS_InformationManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBS_InformationManager, Log, All);

UCLASS()
class UBS_InformationManager : public UBS_BaseManager
{
	GENERATED_BODY()

public:
	void Reset() override;
	bool OnRxMessage(EBS_TxRxMessage MessageType, const TArrayView<const uint8> &Message) override;

	// MTU START
public:
	uint16 GetMTU() const { return MTU; }
	DECLARE_DELEGATE_OneParam(FBS_MTU_CallbackLocal, uint16);
	FBS_MTU_CallbackLocal OnMTU_Update;

	uint16 GetMaxTxMessageLength() const { return MTU == 0 ? 0 : MTU - 3; }

private:
	UPROPERTY()
	uint16 MTU = 0;
	void ParseMTU(const TArrayView<const uint8> &Message);
	// MTU END

	// ID START
public:
	DECLARE_DELEGATE_OneParam(FBS_IdCallbackLocal, const FString &);
	const FString &GetId() const { return Id; }
	FBS_IdCallbackLocal OnIdUpdate;

private:
	UPROPERTY()
	FString Id;
	void ParseId(const TArrayView<const uint8> &Message);
	// ID END

	// NAME START
public:
	const FString &GetName() const { return Name; }
	DECLARE_DELEGATE_OneParam(FBS_NameCallbackLocal, const FString &);
	FBS_NameCallbackLocal OnNameUpdate;

	static const uint8 MinNameLength;
	static const uint8 MaxNameLength;
	void SetName(const FString &NewName);

private:
	UPROPERTY()
	FString Name;
	void ParseName(const TArrayView<const uint8> &Message);
	// NAME END

	// TYPE START
public:
	const EBS_DeviceType &GetType() const { return Type; }
	DECLARE_DELEGATE_OneParam(FBS_TypeCallbackLocal, EBS_DeviceType);
	FBS_TypeCallbackLocal OnTypeUpdate;

	void SetType(const EBS_DeviceType NewType);

private:
	UPROPERTY()
	EBS_DeviceType Type;
	void ParseType(const TArrayView<const uint8> &Message);
	// TYPE END

	// CURRENT TIME START
public:
	const uint64 &GetCurrentTime() const { return CurrentTime; }
	DECLARE_DELEGATE_OneParam(FBS_CurrentTimeCallbackLocal, uint64);
	FBS_CurrentTimeCallbackLocal OnCurrentTimeUpdate;

private:
	UPROPERTY()
	uint64 CurrentTime;
	void ParseCurrentTime(const TArrayView<const uint8> &Message);

	void UpdateCurrentTime();
	// CURRENT TIME END
};
