#include "ohs/RobotID.h"
#include "ohs/Formatter.h"
#include "ohs/Assert.h"
#include "ohs/Log.h"

#include <algorithm>
#include <sys/ioctl.h>
#include <net/if.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <stdint.h>

namespace ohs623 {

	BotType RobotID::s_CurrentBot = BotType::BOT_TYPE_COUNT;
	MotorData IDs [BotType::BOT_TYPE_COUNT][Motor::MOTOR_COUNT];

	const uint8_t SABER_MAC_ADDRESS[6] = { 0x00, 0x80, 0x2F, 0x27, 0x20, 0x48 };
	const uint8_t MARK_MAC_ADDRESS [6] = { 0x00, 0x80, 0x2F, 0x17, 0xEE, 0xE9 };

	bool AreMacAddressEqual(const uint8_t (&a)[6], const uint8_t (&b)[6]) {
		static_assert(sizeof(a) == sizeof(b));
		for (unsigned int i = 0; i < sizeof(a); i++) {
			if (a[i] != b[i]) return false;
		}
		return true;
	}

	int RobotID::GetID(Motor id) {
		const MotorData& data = Get(id);
		return data.ID;
	}

	BaseTalon* RobotID::InitMotor(Motor motor) {
		const MotorData& data = Get(motor);
		switch (data.Controller)
		{
			case MotorControllerType::TALON_SRX: 
			{
				OHS_DEBUG([motor](auto& f) { f << "Initalizing Talon SRX id " << motor; });
				return new WPI_TalonSRX(data.ID);
			}
			case MotorControllerType::TALON_FX:
			{
				OHS_DEBUG([motor](auto& f) { f << "Initalizing Talon FX id " << motor; });
				return new WPI_TalonFX(data.ID);
			}
			default: OHS_ASSERT(false, "Motor controller type not recognised!!");
		}
		return nullptr;
	}

	const MotorData& RobotID::Get(Motor motorID) {
		if (s_CurrentBot == BotType::BOT_TYPE_COUNT) {
			MotorData defaultData = { -1, MotorControllerType::TALON_SRX };
			std::fill(IDs[0], IDs[0] + sizeof(IDs) / sizeof(IDs[0]), defaultData);

			IDs[BotType::MARK][Motor::FRONT_LEFT] = { 6, MotorControllerType::TALON_SRX };
			IDs[BotType::MARK][Motor::FRONT_RIGHT] = { 2, MotorControllerType::TALON_SRX };
			IDs[BotType::MARK][Motor::BACK_LEFT] = { 1, MotorControllerType::TALON_SRX };
			IDs[BotType::MARK][Motor::BACK_RIGHT] = { 5, MotorControllerType::TALON_SRX };

			IDs[BotType::SABER][Motor::FRONT_LEFT] = { 31, MotorControllerType::TALON_FX };
			IDs[BotType::SABER][Motor::FRONT_RIGHT] = { 32, MotorControllerType::TALON_FX };
			IDs[BotType::SABER][Motor::BACK_LEFT] = { 34, MotorControllerType::TALON_FX };
			IDs[BotType::SABER][Motor::BACK_RIGHT] = { 33, MotorControllerType::TALON_FX };     
			IDs[BotType::SABER][Motor::CLIMB_LEFT] = { 4, MotorControllerType::TALON_SRX };     
			IDs[BotType::SABER][Motor::CLIMB_RIGHT] = { 17 , MotorControllerType::TALON_SRX };     
			IDs[BotType::SABER][Motor::FLYWHEEL] = { 35, MotorControllerType::TALON_SRX };     
			IDs[BotType::SABER][Motor::FEEDER] = { 3, MotorControllerType::TALON_SRX };     
			IDs[BotType::SABER][Motor::LOW_TRANSPORT] = { 7 , MotorControllerType::TALON_SRX };     
			IDs[BotType::SABER][Motor::HIGH_TRANSPORT] = { 15 , MotorControllerType::TALON_SRX };     
			IDs[BotType::SABER][Motor::INTAKE_POSITIONER] = { 11 , MotorControllerType::TALON_SRX };     
			IDs[BotType::SABER][Motor::INTAKE_SPIN] = { 14 , MotorControllerType::TALON_SRX };     

			//Default to saber if the bot type cant be found
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

		if (IDs[s_CurrentBot][motorID].ID == -1) {
			frc::DriverStation::ReportError("Invalid ID Number: " + std::to_string(motorID) + " For bot " + std::to_string(s_CurrentBot));
		}

		return IDs[s_CurrentBot][motorID];
	}

} // namespace