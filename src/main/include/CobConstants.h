#pragma once

namespace ohs2020 {

enum class CobKey {
    MODE, ROTATION, TIME_LEFT, IS_RED, FLYWHEEL_WU, FLYWHEEL_STATUS, IN_USE_AUTO, VISION_ANGLE, VISION_X, VISION_Y
};

enum class CobMessageOut {
    PING, GYRO_RESET_CONFIRM
};

enum class CobMessageIn {
    GNIP, GYRO_RESET, RECEIVE_AUTO
};

}//namespace
