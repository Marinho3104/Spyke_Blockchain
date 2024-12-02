

#ifndef INCLUDE_COMMUNICATION_CONNECTION_CONNECTION_H
#define INCLUDE_COMMUNICATION_CONNECTION_CONNECTION_H

namespace spyke::communication::connection {

  enum STATUS_CODE : char { 

    CONNECTED = 0, 
    DISCONNECTED = 1,
  
  };

  template < typename IP_TYPE > 
  class Connection {

    private:

      const int socket_id;    // Socket identification
      const IP_TYPE ip;       // Connection IP
      STATUS_CODE status;     // Current connection status
  
    public:

      ~Connection();

      Connection( const Connection& ) = delete;

      Connection( const int&, const IP_TYPE& );

      Connection( Connection&& );

      Connection();

      const bool is_connected() const;

      const int& get_socket_id() const;

      const bool operator==( const Connection& ) const;

      const bool operator!=( const Connection& ) const;

      
      static Connection< IP_TYPE > connect_to( const IP_TYPE& );

      static Connection< IP_TYPE > create_server( const IP_TYPE& );

  };

}

#endif
