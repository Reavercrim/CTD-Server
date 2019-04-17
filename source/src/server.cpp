#include <iostream>
#include "server.hpp"

Server::Server(int maxClients, int serverPort):m_maxClients(maxClients),
                                                        m_numConnectedClients(0)
{
    m_clientConnected.reserve(maxClients);
    m_clientIpPort.reserve(maxClients);


    std::vector<bool>::iterator it;
    for (it = m_clientConnected.begin(); it != m_clientConnected.end(); ++it)
        *it = false;

    m_socket.bind(serverPort);
    m_socket.setBlocking(false);
}

int Server::FindFreeClientIndex() const
{
    for ( int i = 0; i < m_maxClients; i++ )
    {
        if ( !m_clientConnected[i] )
            return i;
    }
    return -1;
}


int Server::FindExistingClientIndex( const IpPort & ipPort ) const
{

    for ( int i = 0; i < m_maxClients; ++i )
    {
        if ( m_clientConnected[i] && m_clientIpPort[i] == ipPort )
            return i;
    }
    return -1;
}

bool Server::IsClientConnected( int clientIndex ) const
{
    return m_clientConnected[clientIndex];
}

const IpPort & Server::GetClientIpPort( int clientIndex ) const
{
    return m_clientIpPort[clientIndex];
}


void Server::assignClientToSlot(int slotId, IpPort ipPort)
{
    m_clientConnected[slotId] = true;
    m_clientIpPort[slotId] = ipPort;
    std::cout << slotId << " : " << ipPort.first << ":" << ipPort.second << std::endl;
}

void Server::processPacket( IpPort ipPort, sf::Packet packet )
{
    sf::Uint16 code;
    packet >> code;

    switch (code)
    {
        case 1:
            handleNewConnection(ipPort);
            break;
        default:
            break;
    }
}


void Server::handleNewConnection( IpPort ipPort )
{
    int slotId = FindFreeClientIndex();
    if ( slotId != -1 )
        assignClientToSlot(slotId, ipPort);

}

void Server::receivePacket()
{
    sf::Packet packet;
    sf::IpAddress sender;
    unsigned short port;


    if (m_socket.receive(packet, sender, port) != sf::Socket::Done)
    {

    }
    else
        processPacket(IpPort(sender,port),packet);
}

void Server::listen()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time TimePerFrame = sf::seconds(1.f/30);

    while (true)
    {

        bool updateClients = false;

        timeSinceLastUpdate += clock.restart();
        if (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;
            updateClients = true;
        }

        receivePacket();


        if(updateClients)
        {

        }

    }
}
