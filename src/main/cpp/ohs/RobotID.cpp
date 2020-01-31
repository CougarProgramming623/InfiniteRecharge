#include "ohs/RobotID.h"
#include "ohs/Formatter.h"
#include "ohs/Assert.h"

#include <algorithm>
#include <sys/ioctl.h>
#include <net/if.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <stdint.h>

namespace ohs623 {

	BotType RobotID::s_CurrentBot = BotType::BOT_TYPE_COUNT;
	int IDs [BotType::BOT_TYPE_COUNT][Motor::MOTOR_COUNT];

	const uint8_t SABER_MAC_ADDRESS[6] = { 0x00, 0x80, 0x2F, 0x27, 0x20, 0x48 };
	const uint8_t MARK_MAC_ADDRESS [6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };//FIXME

	bool AreMacAddressEqual(const uint8_t (&a)[6], const uint8_t (&b)[6]) {
		static_assert(sizeof(a) == sizeof(b));
		for (unsigned int i = 0; i < sizeof(a); i++) {
			if (a[i] != b[i]) return false;
		}
		return true;
	}

	int RobotID::GetID(Motor id) {
		
		if (s_CurrentBot == BotType::BOT_TYPE_COUNT) {
			std::fill(IDs[0], IDs[0] + sizeof(IDs) / sizeof(int), -1);

			IDs[BotType::MARK][Motor::FRONT_LEFT] =  6;
			IDs[BotType::MARK][Motor::FRONT_RIGHT] =  2;
			IDs[BotType::MARK][Motor::BACK_LEFT] =  1;
			IDs[BotType::MARK][Motor::BACK_RIGHT] =  5;

			IDs[BotType::SABER][Motor::FRONT_LEFT] =  31;
			IDs[BotType::SABER][Motor::FRONT_RIGHT] =  32;
			IDs[BotType::SABER][Motor::BACK_LEFT] =  34;
			IDs[BotType::SABER][Motor::BACK_RIGHT] =  33;     

			s_CurrentBot = BotType::SABER;

			// MAC address detection

			struct ifreq ifr;
			struct ifconf ifc;
			char buf[1024];
			int success = 0;

			int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
			OHS_ASSERT(sock != -1, "socket(AF_INET, SOCK_DGRAM, IPPROTO_IP) FAILED!");

			ifc.ifc_len = sizeof(buf);
			ifc.ifc_buf = buf;
			if (ioctl(sock, SIOCGIFCONF, &ifc) == -1) { frc::DriverStation::ReportError("ioctl(sock, SIOCGIFCONF, &ifc) FAILED!"); }

			struct ifreq* it = ifc.ifc_req;
			const struct ifreq* const end = it + (ifc.ifc_len / sizeof(struct ifreq));

			for (; it != end; ++it) {
				strcpy(ifr.ifr_name, it->ifr_name);
				if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
					if (! (ifr.ifr_flags & IFF_LOOPBACK)) { // don't count loopback
						if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
							success = 1;
							break;
						}
					}
				}
				else { /* handle error */ }
			}
			unsigned char mac_address[6];

			if (success) memcpy(mac_address, ifr.ifr_hwaddr.sa_data, 6);
			else frc::DriverStation::ReportError("Unable to find mac address for roborio!");

			if (AreMacAddressEqual(mac_address, SABER_MAC_ADDRESS)) s_CurrentBot = BotType::SABER;
			else if (AreMacAddressEqual(mac_address, MARK_MAC_ADDRESS)) s_CurrentBot = BotType::MARK;
			else {
				ohs623::DefaultFormatter formatter;
				formatter.Write("Failed to match MAC address: ");
				for (unsigned int i = 0; i < sizeof(mac_address); i++) {
					formatter.Write("0x").Base(mac_address[i], 16).Write(' ');
				}
				frc::DriverStation::ReportError(formatter.c_str());
			}
		}

		int actualID = IDs[s_CurrentBot][id];
		if (actualID == -1) {
			frc::DriverStation::ReportError("Invalid ID Number: " + std::to_string(actualID) + " For bot " + std::to_string(s_CurrentBot));
		}
		return actualID;
	}

} // namespace