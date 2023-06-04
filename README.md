# P2Net - Secure TCP Library
P2Net is a secure TCP library that provides client-server communication capabilities over TCP. It aims to ensure secure and reliable data transmission between client and server applications.

# Getting Started
To use P2Net in your project, follow the steps below:

Include the necessary P2Net library headers in your code.
Create a derived class from c_tcpclient to handle client events and packet processing.
Create a derived class from c_tcpserver to handle server events and packet processing.
Implement the virtual functions in your derived classes to customize the behavior according to your requirements.
Instantiate the client or server objects in your application code.
Connect the client to the server using the appropriate connection parameters.
Handle the connection event and perform any necessary setup or send initial messages.
Handle other events and process incoming packets as needed.
Build and run your application.
Here's an example of how to initialize the client and server objects:


```
// Implement a derived class from c_tcpclient
class net_client : public c_tcpclient {
public:
      inline net_client(std::string ip, std::uint16_t p) : p2net::core::c_tcpclient(ip, p) {
		
		}
    // Override virtual functions as needed
    // ...

    // Implement custom behavior for handling packets
    virtual void on_handle_packet(p2net::core::c_socket& client_socket, p2net::core::data::c_packet& message) override {
        // Handle packet based on opcode and perform actions
        // ...
    }

    // Implement custom behavior for disconnection event
    virtual void on_disconnect() override {
        // Handle client disconnection event
        // ...
    }
};

// Create an instance of the TCP client class
net_client* client = new net_client("127.0.0.1", 8777);

// Connect to the server
if (client->connect()) {
    // Handle the connection event
    client->on_connected();
}


// Implement a derived class from c_tcpserver
class net_server : public c_tcpserver {
public:
     net_server(std::uint16_t p) : p2net::core::c_tcpserver(p) {}
    // Override virtual functions as needed
    // ...

    // Implement custom behavior for handling packets
    virtual void on_handle_packet(p2net::core::c_socket& client_socket, p2net::core::data::c_packet& message) override {
        // Handle packet based on opcode and perform actions
        // ...
    }

    // Implement custom behavior for client connection event
    virtual bool on_client_connected(p2net::core::c_socket& client_socket) override {
        // Handle client connection event
        // ...

        return true;
    }

    // Implement custom behavior for successful client connection
    virtual void on_client_connection_success(p2net::core::c_socket& client_socket) override {
        // Handle successful client connection
        // ...
    }
};

// Create an instance of the TCP server class
net_server* server = new net_server(server_port);

// Begin listening for incoming connections
if (server->begin_listening()) {
    // Server started successfully
    // Perform additional setup or handle other events
}
```
# Contributions
Contributions to the P2Net library are welcome! If you find any issues or have suggestions for improvement, please open an issue or submit a pull request on the project's GitHub repository.
