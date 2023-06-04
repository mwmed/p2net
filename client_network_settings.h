#pragma once
#include "c_array.h"




namespace p2net {
	namespace client {
		class client_network_settings {
		public:
            bool AllowServerAsP2PGroupMember = false;
            BYTE DirectP2PStartCondition = 0;
            uint32_t EmergencyLogLineCount = 0;
            bool EnableLookaheadP2PSend = false;
            bool EnableNagleAlgorithm = true;
            bool EnableP2PEncryptedMessaging = false;
            bool EnablePingTest = false;
            bool EnableServerLog = false;
            uint32_t EncryptedMessageKeyLength = 128;
            BYTE FallbackMethod = 0;
            uint32_t FastEncryptedMessageKeyLength = 0;
            double IdleTimeout = 900;
            uint32_t MessageMaxLength = 1048576;
            uint32_t OverSendSuspectingThresholdInBytes = 15360;
            bool UpnpDetectNatDevice = true;
            bool UpnpTcpAddrPortMapping = true;

            core::data::c_array write() {
                core::data::c_array message;
                message.write((std::int8_t)EnableServerLog);
                message.write(FallbackMethod);
                message.write(MessageMaxLength);
                message.write(IdleTimeout);
                message.write(DirectP2PStartCondition);
                message.write(OverSendSuspectingThresholdInBytes);
                message.write((std::int8_t)EnableNagleAlgorithm);
                message.write(EncryptedMessageKeyLength);
                message.write(FastEncryptedMessageKeyLength);
                message.write((std::int8_t)AllowServerAsP2PGroupMember);
                message.write((std::int8_t)EnableP2PEncryptedMessaging);
                message.write((std::int8_t)UpnpDetectNatDevice);
                message.write((std::int8_t)UpnpTcpAddrPortMapping);
                message.write((std::int8_t)EnableLookaheadP2PSend);
                message.write((std::int8_t)EnablePingTest);
                message.write(EmergencyLogLineCount);

                return message;
            }
		};
	}
}