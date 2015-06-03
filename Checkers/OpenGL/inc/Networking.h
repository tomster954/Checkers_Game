//-----------------------------------------------------------------------------
//Author: Tommas Solarino
//Description:	this is the newtworking class, here is the code that send and 
//				recives all the nesseasry info from the other user
//-----------------------------------------------------------------------------

#ifndef NETWORKING
#define NETWORKING

#include "RakNetTypes.h"

class Networking
{
public:

	Networking();
	~Networking();

	void ServerLoop();

	void SetUp();
	
	bool IsServer(){ return m_isServer; }
	void SetWhosTurn(bool _whosTurnIsIt);
	bool BluesTurn(){ return m_serversTurn; }

private:
	RakNet::Packet* m_packet;
	RakNet::RakPeerInterface *m_peer;

	RakNet::SystemAddress m_systemAddress;

	bool m_isServer;
	bool m_serversTurn;

	char m_name[255];
};
#endif