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

and add to BleDeviceDelegate.cpp (be sure to replace the existing `didUpdateValueForCharacteristic` method):

```
#define BUFFER_SIZE 10

@implementation BleDeviceDelegate
{
	NSMutableArray *notificationQueue; // Queue for handling notifications
	BOOL isProcessingNotification;	   // Flag to check if currently processing

	NSUInteger readIndex;
	NSUInteger writeIndex;
	NSUInteger itemCount; // Tracks how many items are currently in the buffer
}

- (instancetype)init
{
	self = [super init];
	if (self)
	{
		notificationQueue = [[NSMutableArray alloc] initWithCapacity:BUFFER_SIZE]; // Correct allocation
		for (int i = 0; i < BUFFER_SIZE; i++)
		{
			[notificationQueue addObject:[NSNull null]]; // Pre-fill with placeholders
		}
		readIndex = 0;
		writeIndex = 0;
		itemCount = 0;
		isProcessingNotification = NO;
	}
	return self;
}

- (void)processNextNotification
{
	if (itemCount == 0)
	{
		// UE_LOG(LogBleGoodies, Log, TEXT("finished processing notifications"));
		isProcessingNotification = NO;
		return;
	}

	// UE_LOG(LogBleGoodies, Log, TEXT("processing next notification"));

	NSDictionary *notification = notificationQueue[readIndex];
	// notificationQueue[readIndex] = [NSNull null];
	readIndex = (readIndex + 1) % BUFFER_SIZE;
	itemCount--;

	NSString *serviceUuid = notification[@"ServiceUuid"];
	NSString *characteristicUuid = notification[@"CharacteristicUuid"];
	NSData *data = notification[@"Data"];

	const FString ServiceUuid(serviceUuid);
	const FString CharacteristicUuid(characteristicUuid);

	// UE_LOG(LogBleGoodies, Log, TEXT("about to notify %u bytes from %s..."), data.length, *CharacteristicUuid);

	AsyncTask(ENamedThreads::GameThread, [=]()
			  {
				  //UE_LOG(LogBleGoodies, Log, TEXT("notifying %u bytes from %s"), data.length, *CharacteristicUuid);
				  self->_onNotification(ServiceUuid, CharacteristicUuid, data);
				  [self processNextNotification]; });
}

- (void)addNotification:(NSDictionary *)notification
{
	if (itemCount < BUFFER_SIZE)
	{
		// UE_LOG(LogBleGoodies, Log, TEXT("Adding notification"));
		notificationQueue[writeIndex] = notification;
		writeIndex = (writeIndex + 1) % BUFFER_SIZE;
		itemCount++;
	}
	else
	{
		UE_LOG(LogBleGoodies, Warning, TEXT("Buffer is full - overwriting older indices"));
		readIndex = (readIndex + 1) % BUFFER_SIZE;
		notificationQueue[writeIndex] = notification;
		writeIndex = (writeIndex + 1) % BUFFER_SIZE;
	}
}

- (void)enqueueNotificationWithServiceUuid:(NSString *)serviceUuid
						characteristicUuid:(NSString *)characteristicUuid
									  data:(NSData *)data
{
	const FString CharacteristicUuid(characteristicUuid);
	// UE_LOG(LogBleGoodies, Log, TEXT("Adding message with %u bytes from %s to queue"), data.length, *CharacteristicUuid);

	[self addNotification:@{@"ServiceUuid" : serviceUuid, @"CharacteristicUuid" : characteristicUuid, @"Data" : data}];

	if (!isProcessingNotification)
	{
		// UE_LOG(LogBleGoodies, Log, TEXT("starting to process notifications..."));
		isProcessingNotification = YES;
		[self processNextNotification];
	}
	else
	{
		// UE_LOG(LogBleGoodies, Log, TEXT("waiting to process notification..."));
	}
}

- (void)peripheral:(CBPeripheral *)peripheral didUpdateValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error
{
	const FString ServiceUuid(characteristic.service.UUID.UUIDString);
	const FString CharacteristicUuid(characteristic.UUID.UUIDString);

	// UE_LOG(LogBleGoodies, Log, TEXT("Got %u bytes from %s"), characteristic.value.length, *CharacteristicUuid);

	if (error)
	{
		const FString ErrorMessage([error localizedDescription]);
		AsyncTask(ENamedThreads::GameThread, ^{
		  self->_onError(ServiceUuid, CharacteristicUuid, ErrorMessage);
		});
		return;
	}

	if (characteristic.isNotifying)
	{
		// UE_LOG(LogBleGoodies, Log, TEXT("About to add message with %u bytes from %s to queue"), characteristic.value.length, *CharacteristicUuid);

		NSString *serviceUuid = characteristic.service.UUID.UUIDString;
		NSString *characteristicUuid = characteristic.UUID.UUIDString;

		[self enqueueNotificationWithServiceUuid:serviceUuid
							  characteristicUuid:characteristicUuid
											data:characteristic.value];
	}
	else
	{
		// Directly process read events
		AsyncTask(ENamedThreads::GameThread, [=]()
				  { self->_onRead(ServiceUuid, CharacteristicUuid, characteristic.value); });
	}
}
```
