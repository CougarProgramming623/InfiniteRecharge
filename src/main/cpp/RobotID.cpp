#include "RobotID.h"
#include "ohs/Formatter.h"

#include <algorithm>
#include <sys/ioctl.h>
#include <net/if.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

namespace ohs623{

    BotType RobotID::s_CurrentBot = BotType::BOT_TYPE_COUNT;
    int IDs [BotType::BOT_TYPE_COUNT][Motor::MOTOR_COUNT];

    int RobotID::GetID(Motor id) {
        
        if (s_CurrentBot == BotType::BOT_TYPE_COUNT) {
            std::fill(IDs, IDs + sizeof(IDs)/sizeof(int), -1);

            IDs[BotType::MARK][Motor::FRONT_LEFT] =  6;
            IDs[BotType::MARK][Motor::FRONT_RIGHT] =  2;
            IDs[BotType::MARK][Motor::BACK_LEFT] =  1;
            IDs[BotType::MARK][Motor::BACK_RIGHT] =  5;

            IDs[BotType::SABER][Motor::FRONT_LEFT] =  31;
            IDs[BotType::SABER][Motor::FRONT_RIGHT] =  32;
            IDs[BotType::SABER][Motor::BACK_LEFT] =  34;
            IDs[BotType::SABER][Motor::BACK_RIGHT] =  33;     


            // MAC address detection

            struct ifreq ifr;
            struct ifconf ifc;
            char buf[1024];
            int success = 0;

            int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
            if (sock == -1) { /* handle error*/ };

            ifc.ifc_len = sizeof(buf);
            ifc.ifc_buf = buf;
            if (ioctl(sock, SIOCGIFCONF, &ifc) == -1) { /* handle error */ }

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

            ohs623::DefaultFormatter formatter;

            
            formatter.Write("Current MAC address: ")
            for (int i = 0; i < sizeof(mac_address); i++)
                formatter.Base(mac_address[i], 16);
            frc::DriverStation::ReportError(formatter.c_str);
        }

        int actualID = IDs[s_CurrentBot][id];
        if (actualID == -1) 
            frc::DriverStation::ReportError("Invalid ID Number: " + std::to_string(actualID));
        return actualID;
    }

} // namespace