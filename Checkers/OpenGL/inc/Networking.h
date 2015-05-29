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
	
private:
	RakNet::Packet* m_packet;
	RakNet::RakPeerInterface *m_peer;

	bool m_isServer;
};
#endif