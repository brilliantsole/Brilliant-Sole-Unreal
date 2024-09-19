// Copyright 2024 Zack Qattan @ Brilliant Sole. All Rights Reserved

#include "UDP_Utils.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

DEFINE_LOG_CATEGORY(LogUDP_Utils);

bool UUDP_Utils::FindAvailableUdpPort(int32 &OutPort)
{
    // Start searching from a specific port number (you can adjust the range as needed)
    constexpr int32 StartPort = 12345;
    constexpr int32 MaxPort = 65535; // Maximum UDP port number

    for (int32 Port = StartPort; Port <= MaxPort; ++Port)
    {
        // Create a socket to test the port
        FSocket *TestSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_DGram, TEXT("PortTestSocket"), false);

        if (!TestSocket)
        {
            // Handle socket creation error
            continue; // Try the next port
        }

        // Create an internet address for binding
        TSharedPtr<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

        if (InternetAddr.IsValid())
        {
            InternetAddr->SetAnyAddress(); // Bind to any available address
            InternetAddr->SetPort(Port);

            if (TestSocket->Bind(*InternetAddr))
            {
                // Port is available
                OutPort = Port;
                UE_LOGFMT(LogUDP_Utils, Verbose, "OutPort: {0}", OutPort);

                // Clean up the test socket
                TestSocket->Close();
                ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(TestSocket);

                return true;
            }
        }

        // Clean up the test socket
        TestSocket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(TestSocket);
    }

    UE_LOGFMT(LogUDP_Utils, Verbose, "Couldn't find Available port");

    // No available port found
    return false;
}