#pragma once

namespace ohs2020 {

enum class CobKey {
    MODE, ROTATION, TIME_LEFT, IS_RED, FLYWHEEL_WU, FLYWHEEL_STATUS, IN_USE_AUTO, LIMELIGHT_TOGGLE,
    TEST1, TEST2, TEST3
};

enum class CobMessageOut {
    PING, GYRO_RESET_CONFIRM
};

enum class CobMessageIn {
    GNIP, GYRO_RESET, RECEIVE_AUTO
};

}//namespace
