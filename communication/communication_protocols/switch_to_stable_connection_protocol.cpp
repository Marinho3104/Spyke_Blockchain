
#include "switch_to_stable_connection_protocol.h"


spyke::communication::communication_protocols::Switch_To_Stable_Connection_Protocol::Switch_To_Stable_Connection_Protocol() : Packet( PROTOCOL_ID::SWITCH_TO_STABLE_CONNECTION ) {}

spyke::communication::communication_protocols::Switch_To_Stable_Connection_Protocol::Switch_To_Stable_Connection_Protocol( spyke::communication::communication_protocols::Switch_To_Stable_Connection_Protocol&& other )
  : Packet( std::move( other ) ) {}


