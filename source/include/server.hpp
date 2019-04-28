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
    std::vector<sf::Vector2f> m_rectVect;


public:
    Server(int maxClients, int serverPort);
    int findFreeClientIndex() const;
    int findExistingClientIndex( const IpPort & ipPort ) const;
    bool isClientConnected( int clientIndex ) const;
    const IpPort & getClientIpPort( int clientIndex ) const;
    void assignClientToSlot(int slotId, IpPort ipPort);
    void processPacket( IpPort ipPort, sf::Packet packet);
    void handleNewConnection( IpPort ipPort );
    void handleInputs( IpPort ipPort, sf::Packet packet );
    void receivePacket();
    void updateClients();
    void listen();



};

#endif //SERVER_HPP
