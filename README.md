# Brilliant Sole Unreal SDK Demo

In the BleGoodies Plugin, modify BleUtils.cpp's `UBleUtils::HexToUUID` function:

```
FString UBleUtils::HexToUUID(const FString &InHexString)
{
	FString HexString = InHexString.TrimStartAndEnd();
	return HexString;
}
```
