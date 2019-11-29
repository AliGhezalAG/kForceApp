#ifndef INIT_H
#define INIT_H

#include <QObject>

using namespace std;

#define RESULTS_DIR_PATH                    "./results/"

// Kinvent KForce params

#define SERVICE_UUID                        "{49535343-fe7d-4ae5-8fa9-9fafd205e455}"
#define CHARACTERISTIC_UUID                 "{49535343-1e4d-4bd9-ba61-23c647249616}"
#define GET_REAL_TIME_CLOCK_COMMAND         "70"
#define SET_TIME_CLOCK_COMMAND              "71"
#define GET_MEASURE_MULYIPLIER_COMMAND      "21"
#define RESET_BASELINE_COMMAND              "42"
#define GET_BASELINE_COMMAND                "43"
#define GET_MEMORY_DUMP_COMMAND             "67"
#define RESET_MEMORY_COMMAND                "68"
#define GET_BATTERY_LEVE_COMMAND            "81"
#define GET_FIRMWARE_VERSION_COMMAND        "09"
#define MEASUREMENT_MULTIPLIER_DENOMINATOR  2000000.0
#define TIMESTAMP_OFFSET                    946684800
#define MEMORY_SIZE                         131071

// UTT board params
#define PACKET_SIZE                         12
#define REQUEST_DATA_COMMAND                "5253"
#define END_OF_DATA_CODE                    "5250"
#define OPENING_PACKET_START_PREFIX         "5049"
#define OPENING_PACKET_END_PREFIX           "4950"
#define CLOSING_PACKET_START_PREFIX         "5052"

#endif // INIT_H
