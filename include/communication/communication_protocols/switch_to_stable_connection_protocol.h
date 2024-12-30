

#ifndef INCLUDE_COMMUNICATION_COMMUNICATION_PROTOCOLS_SWITCH_TO_STABLE_CONNECTION_PROTOCOL_H
#define INCLUDE_COMMUNICATION_COMMUNICATION_PROTOCOLS_SWITCH_TO_STABLE_CONNECTION_PROTOCOL_H


#include "packet.h"


namespace spyke::communication::communication_protocols {

  class Switch_To_Stable_Connection_Protocol : Packet {

    public:

      Switch_To_Stable_Connection_Protocol( const Switch_To_Stable_Connection_Protocol& ) = delete;

      Switch_To_Stable_Connection_Protocol();

      Switch_To_Stable_Connection_Protocol( Switch_To_Stable_Connection_Protocol&& );

  };

}

#endif
