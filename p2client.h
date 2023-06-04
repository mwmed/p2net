#pragma once
#include "c_packet.h"





namespace p2net {
	namespace p2client {
		core::data::c_packet make_packet(packet_core_opcode opcode, core::data::c_array message);
	}
}