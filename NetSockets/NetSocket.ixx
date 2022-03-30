#include <enet/enet.h>
import <vector>;
import <string>;
using std::string;
using std::vector;

static bool g_bInitialized = false;
export module NetSocket;

export class NetSocket
{
public:
	bool bConnected = false;

	NetSocket(const char* host = NULL, enet_uint16 port = 0, enet_uint8 eSocketType = ENET_SOCKET_TYPE_STREAM)
	{
		if (!g_bInitialized)
			g_bInitialized = (enet_initialize() == 0);

		if (eSocketType == 0)
			return;

		if (Init((ENetSocketType)eSocketType))
		{
			if (host)
				Connect(host, port);
		}
	}
	~NetSocket()
	{
		Kill();
	}

	bool Init(ENetSocketType eSocketType)
	{
		if (m_Socket != ENET_SOCKET_NULL)
			Kill();

		return (m_Socket = enet_socket_create(eSocketType)) !=
			ENET_SOCKET_NULL;
	}

	int SetSocketOption(ENetSocketOption eSocketOption, int value)
	{ // For TCP_NODELAY, please use this with ENET_SOCKET_OPTION_NODELAY.
		return enet_socket_set_option(m_Socket, eSocketOption, value);
	}

	bool Connect(const char* host, enet_uint16 port)
	{
		if (m_Socket == ENET_SOCKET_NULL || enet_address_set_host(&m_Address, host) != 0)
			return false;

		m_Address.port = port;
		bConnected = (enet_socket_connect(m_Socket, &m_Address) == 0);
		return bConnected;
	}

	int Receive(vector<enet_uint8>& bytes, int len = ENET_PROTOCOL_MAXIMUM_MTU)
	{
		ENetBuffer buffer;

		buffer.data = m_PacketData;
		buffer.dataLength = len;

		while (len > 0)
		{
			int read = enet_socket_receive(m_Socket, NULL, &buffer, 1);
			if (read < 0)
				return -1;

			if (read == 0)
			{
				Kill();
				return -1;
			}

			size_t prevSize = bytes.size();
			bytes.insert(bytes.end(), (enet_uint8*)buffer.data, (enet_uint8*)buffer.data + read);
		}

		return (int)bytes.size();
	}

	bool Send(void* data, size_t dataLen)
	{
		ENetBuffer buffer;
		buffer.data = data;
		buffer.dataLength = dataLen;

		enet_uint8* dataPtr = (enet_uint8*)buffer.data;

		int sent;
		while (buffer.dataLength > 0)
		{
			sent = enet_socket_send(m_Socket, NULL, &buffer, 1); // keep sending until we hit the first 0, so that we know all data has been sent.

			if (sent <= 0)
				return false;

			dataPtr += sent;
			buffer.data = dataPtr;
			buffer.dataLength -= sent;
		}

		return true;
	}

	// Further implementations can be customized upon here:
	bool Send(const string& s) { return Send((void*)s.c_str(), s.length()); }

private:
	void Kill()
	{
		enet_socket_destroy(m_Socket);
		m_Socket = ENET_SOCKET_NULL;
	}

	enet_uint8 m_PacketData[ENET_PROTOCOL_MAXIMUM_MTU];
	ENetSocket m_Socket = ENET_SOCKET_NULL;
	ENetAddress m_Address;
};