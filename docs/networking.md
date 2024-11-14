
# Networking

The networking layer is based on **UDP** using **Asio**. It handles the following tasks:

- **Client-Server Connection**: The client connects to the server using the `ConnectionProtocol` class, which manages the UDP socket and asynchronous communication.
- **Handling Game States**: The server updates clients on game events like entity spawns, updates, and destruction. Clients send their own state (position, actions) back to the server.
- **Multithreaded Processing**: Networking runs on a separate thread from the game loop to allow non-blocking communication.

## Key Files
- **connection_protocol.hpp**: Manages all network-related communication between client and server.
- **server.hpp**: Contains the server-side logic, including handling client connections, game updates, and broadcasting messages.
- **protocol.hpp**: Defines all message structures and types used for communication.
