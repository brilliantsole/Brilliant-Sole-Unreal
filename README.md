# Brilliant Sole Unreal SDK Demo

In the BleGoodies Plugin, modify BleUtils.cpp's `UBleUtils::HexToUUID` function:

```
FString UBleUtils::HexToUUID(const FString &InHexString)
{
	FString HexString = InHexString.TrimStartAndEnd();
	return HexString;
}
```

and modify BleGoodiesDevice.java's `BleGoodiesDevice::connect` function:

```
public void connect() {
	mConnection = mDevice.connectGatt(mActivity, false, getGattCallback());
	mConnection.requestMtu(517);
}
```

and modify IOS BleDevice.cpp's `UBleDevice::GetCharacteristic` function:

```
if ([Characteristic.UUID isEqual:CharacteristicCBUUID])
{
	TargetCharacteristic = Characteristic;
	break;
}
if ([[Characteristic.UUID.UUIDString lowercaseString] isEqualToString:[CharacteristicCBUUID.UUIDString lowercaseString]])
{
	TargetCharacteristic = Characteristic;
	break;
}
```

and modify UBleDevice::WriteCharacteristic:

```
void UBleDevice::WriteCharacteristic(const FString &ServiceUUID, const FString &CharacteristicUUID, const TArray<uint8> &Data)
{
	CBCharacteristic *Characteristic = GetCharacteristic(ServiceUUID, CharacteristicUUID);
	if (Characteristic)
	{
		[Device writeValue:[NSData dataWithBytes:(void *)Data.GetData() length:Data.Num()]
			forCharacteristic:Characteristic
						 type:CBCharacteristicWriteWithoutResponse];
	}
}
```

to this:

```
if ([[Characteristic.UUID.UUIDString lowercaseString] isEqualToString:[CharacteristicCBUUID.UUIDString lowercaseString]])
{
	TargetCharacteristic = Characteristic;
	break;
}
```
