#include <SFML/Network.hpp>
#include <thread>
#include <iostream>
#include "server.hpp"




// void listen()
// {
//
//     sf::Packet packet;
//
//     sf::IpAddress sender;
//     unsigned short port;
//
//     std::string s;
//
//     sf::Vector2i pos(0,0);
//
//     while(true)
//     {
//         if (socket.receive(packet, sender, port) != sf::Socket::Done)
//         {
//             // error...
//         }
//         else
//         {
//
//             packet >> s;
//
//             if(s == "u")
//                 pos.y--;
//             if(s == "d")
//                 pos.y++;
//             if(s == "l")
//                 pos.x--;
//             if(s == "r")
//                 pos.x++;
//
//             std::cout << pos.x << " : " << pos.y << std::endl;
//             sf::Packet packet;
//             float x(pos.x);
//             float y(pos.y);
//             packet << x << y;
//             socket.send(packet,"127.0.0.1",port);
//         }
//     }
// }



int main()
{
    Server server(64,5000);

    server.listen();

    return 0;
}
