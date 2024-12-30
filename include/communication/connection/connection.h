

#ifndef INCLUDE_COMMUNICATION_CONNECTION_CONNECTION_H
#define INCLUDE_COMMUNICATION_CONNECTION_CONNECTION_H

#include "packet.h"
#include <semaphore.h>
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
      sem_t write_locker;     // Locker for write operations
      sem_t read_locker;      // Locker for read operations
  
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

      const bool send_packet( std::unique_ptr< communication_protocols::Packet > );

      std::unique_ptr< communication_protocols::Packet > receive_packet();

      
      static Connection< IP_TYPE > connect_to( const IP_TYPE& );

      static Connection< IP_TYPE > create_server( const IP_TYPE& );

  };

}

#endif
