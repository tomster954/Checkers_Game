#include "Networking.h"
#include <stdio.h>
#include <string.h>

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"

#define MAX_CLIENTS 10
#define SERVER_PORT 60000

enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1
};

Networking::Networking()
{
	m_peer = RakNet::RakPeerInterface::GetInstance();

	SetUp();
}

Networking::~Networking()
{
	RakNet::RakPeerInterface::DestroyInstance(m_peer);
}
void Networking::ServerLoop()
{
	for (m_packet = m_peer->Receive(); m_packet; m_peer->DeallocatePacket(m_packet), m_packet = m_peer->Receive())
	{
		switch (m_packet->data[0])
		{
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
		{
			printf("Another client has disconnected.\n");
		}break;

		case ID_REMOTE_CONNECTION_LOST:
		{
			printf("Another client has lost the connection.\n");
		}break;

		case ID_REMOTE_NEW_INCOMING_CONNECTION:
		{
			printf("Another client has connected.\n");
		}break;

		case ID_CONNECTION_REQUEST_ACCEPTED:
		{
			printf("Our connection request has been accepted.\n");

			// Use a BitStream to write a custom user message
			// Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
			bsOut.Write("Hello world");
			m_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_packet->systemAddress, false);
		}break;

		case ID_NEW_INCOMING_CONNECTION:
		{
			printf("A connection is incoming.\n");
		}break;

		case ID_NO_FREE_INCOMING_CONNECTIONS:
		{
			printf("The server is full.\n");
		}break;

		case ID_DISCONNECTION_NOTIFICATION:
		{
			if (m_isServer)
			  printf("A client has disconnected.\n");
			else
			  printf("We have been disconnected.\n");
		}break;

		case ID_CONNECTION_LOST:
		{
			if (m_isServer)
			   printf("A client lost the connection.\n");
			else
			   printf("Connection lost.\n");
		}break;

		case ID_GAME_MESSAGE_1:
		{
			RakNet::RakString rs;
			RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(rs);
			printf("%s\n", rs.C_String());
		}break;

		default:
		{
				   printf("Message with identifier %i has arrived.\n", m_packet->data[0]);
		}break;
		}
	}
}


void Networking::SetUp()
{
	char str[512];

	printf("(C)onsole or (S)erver?\n");
	gets(str);

	if ((str[0] == 'c') || (str[0] == 'C'))
	{
		RakNet::SocketDescriptor sd;
		m_peer->Startup(1, &sd, 1);
		m_isServer = false;
	}
	else {
		RakNet::SocketDescriptor sd(SERVER_PORT, 0);
		m_peer->Startup(MAX_CLIENTS, &sd, 1);
		m_isServer = true;
	}

	if (m_isServer)
	{
		printf("Starting the server.\n");
		// We need to let the server accept incoming connections from the clients
		m_peer->SetMaximumIncomingConnections(MAX_CLIENTS);
	}
	else {
		printf("Enter server IP or hit enter for 127.0.0.1\n");
		gets(str);
		if (str[0] == 0){
			strcpy(str, "127.0.0.1");
		}
		printf("Starting the client.\n");
		m_peer->Connect(str, SERVER_PORT, 0, 0);

	}
}