#ifndef SERVER_HPP
#define SERVER_HPP

#include <SFML/Network.hpp>
#include "netUtility.hpp"


class Server
{
private:
    int m_maxClients;
    int m_numConnectedClients;
    std::vector<bool> m_clientConnected;
    std::vector<IpPort> m_clientIpPort;
    sf::UdpSocket m_socket;


public:
    Server(int maxClients, int serverPort);
    int FindFreeClientIndex() const;
    int FindExistingClientIndex( const IpPort & ipPort ) const;
    bool IsClientConnected( int clientIndex ) const;
    const IpPort & GetClientIpPort( int clientIndex ) const;
    void assignClientToSlot(int slotId, IpPort ipPort);
    void processPacket( IpPort ipPort, sf::Packet packet);
    void handleNewConnection( IpPort ipPort );
    void receivePacket();
    void listen();



};

#endif //SERVER_HPP
