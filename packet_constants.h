#pragma once
#include <cstdint>






enum class packet_core_opcode : uint8_t
{
	none,

	//client -> C2S
	notify_csnencryptedsessionkey_message,



	//server -> S2C
	notify_serverconnectionhint_message,
	notify_serverconnection_success,

	//common -> C2S | S2C
	rmi,
	encrypyted_reliable
};