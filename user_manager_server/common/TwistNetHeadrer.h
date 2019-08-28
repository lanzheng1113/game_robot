#pragma once

enum TwistClientType{
	EGameClient,
	EAnswerClient,
};

struct TwistGameClientHeader
{
	TwistClientType eClientType;
	DWORD dwUserId;
	DWORD dwVersion;
	DWORD dwTotalPacketSize;
	int PacketIndex;
};

#define MoreDataAck				0x10010010
#define DataAllRecvedAck		0x20010010

#define LAST_PACKET_INDEX -1