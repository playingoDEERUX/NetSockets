# NetSockets

A fast and clean, universal C++20 wrapper for cross-platform sockets.
This wrapper uses ENet by lsalzman (https://github.com/lsalzman/enet) solely for cross-platform socket management.

(The ENet protocol itself is not employed, neither is any ENet code beyond the **internal socket API**, or anything else other than **helper functions** being leveraged)

Usage example is in NetSockets/Main.cpp
The actual wrapper module itself is located in NetSockets/NetSocket.ixx

Can be used for server/client deployment with any protocol layer! (UDP & TCP if favoured)
