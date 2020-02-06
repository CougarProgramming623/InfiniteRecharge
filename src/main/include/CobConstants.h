#pragma once

namespace ohs2020 {

enum class CobKey {
    MODE, ROTATION, TIME_LEFT, IS_RED, FLYWHEEL_WU, FLYWHEEL_STATUS
};

enum class CobMessageOut {
    PING
};

enum class CobMessageIn {
    GNIP
};

}//namespace
