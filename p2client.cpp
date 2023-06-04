#include "p2client.h"

p2net::core::data::c_packet p2net::p2client::make_packet(packet_core_opcode opcode, core::data::c_array message)
{

    core::data::c_packet packet;


    packet.body.opcode = opcode;
    packet.body.message.init(message.get_buffer(), message.get_buffer_length());
    packet.header.magic_id = 0x5713;
    packet.fix_size();


    return packet;
}
