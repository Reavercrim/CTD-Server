#include <iostream>
#include "server.hpp"

Server::Server(int maxClients, int serverPort):m_maxClients(maxClients),
                                               m_numConnectedClients(0),
                                               m_clientConnected(maxClients,false),
                                               m_clientIpPort(maxClients)
{

    m_rectVect.reserve(maxClients);

    std::vector<bool>::iterator it;
    for (it = m_clientConnected.begin(); it != m_clientConnected.end(); ++it)
        *it = false;

    m_socket.bind(serverPort);
    m_socket.setBlocking(false);
}

int Server::findFreeClientIndex() const
{
    for ( int i = 0; i < m_maxClients; i++ )
    {
        if ( !m_clientConnected[i] )
            return i;
    }
    return -1;
}


int Server::findExistingClientIndex( const IpPort & ipPort ) const
{

    for ( int i = 0; i < m_maxClients; ++i )
    {
        if ( m_clientConnected[i] && m_clientIpPort[i] == ipPort )
            return i;
    }
    return -1;
}

bool Server::isClientConnected( int clientIndex ) const
{
    return m_clientConnected[clientIndex];
}

const IpPort & Server::getClientIpPort( int clientIndex ) const
{
    return m_clientIpPort[clientIndex];
}


void Server::assignClientToSlot(int slotId, IpPort ipPort)
{
    m_clientConnected[slotId] = true;
    m_clientIpPort[slotId] = ipPort;
    std::cout << slotId << " : " << ipPort.first << ":" << ipPort.second << std::endl;


    m_rectVect.push_back(sf::Vector2f(2048,2048));
    std::cout << m_rectVect.size() << std::endl;
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
        case 10:
            handleInputs(ipPort,packet);
            break;
        default:
            break;
    }
}


void Server::handleNewConnection( IpPort ipPort )
{
    int slotId = findFreeClientIndex();
    if ( slotId != -1 )
        assignClientToSlot(slotId, ipPort);

}

void Server::handleInputs( IpPort ipPort, sf::Packet packet)
{
    int index = findExistingClientIndex( ipPort );

    if ( index > -1 )
    {
        std::string input;
        packet >> input;

        if (input == "u")
                m_rectVect.at(index).y -= 5;
        else if (input == "d")
                m_rectVect.at(index).y += 5;
        else if (input == "l")
                m_rectVect.at(index).x -= 5;
        else if (input == "r")
                m_rectVect.at(index).x += 5;

    }
}

void Server::receivePacket()
{
    sf::Packet packet;
    sf::IpAddress sender;
    unsigned short port;


    if (m_socket.receive(packet, sender, port) == sf::Socket::Done)
        processPacket(IpPort(sender,port),packet);
}

void Server::listen()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time TimePerFrame = sf::seconds(1.f/30);

    while (true)
    {

        bool clientsUpdate = false;

        timeSinceLastUpdate += clock.restart();
        if (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;
            clientsUpdate = true;
        }

        receivePacket();

        if(clientsUpdate)
        {
            updateClients();
        }
    }
}

void Server::updateClients()
{
    sf::Packet p;
    sf::Uint16 numberObject(m_rectVect.size());
    p << numberObject;


    for (auto it = m_rectVect.begin(); it != m_rectVect.end(); ++it)
    {
            p << it->x;
            p << it->y;
    }

    sf::Packet cp;
    IpPort ipPort;

    for ( sf::Uint16 i = 0; i < m_maxClients; ++i )
    {
        if ( m_clientConnected[i])
        {
            cp = p;
            cp << i;

            ipPort = getClientIpPort(i);

            m_socket.send(cp,ipPort.first,ipPort.second);
        }
    }

}
