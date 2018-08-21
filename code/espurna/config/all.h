/*

    If you want to modify the stock configuration but you don't want to touch
    the repo files you can define USE_CUSTOM_H in your build settings.

    Arduino IDE:
    define it in your boards.txt for the board of your choice.
    For instance, for the "Generic ESP8266 Module" with prefix "generic" just add:

        generic.build.extra_flags=-DESP8266 -DUSE_CUSTOM_H

    PlatformIO:
    add the setting to your environment or just define global PLATFORMIO_BUILD_FLAGS

        export PLATFORMIO_BUILD_FLAGS="'-DUSE_CUSTOM_H'"

    Check https://github.com/xoseperez/espurna/issues/104
    for an example on how to use this file.

*/

//#ifdef USE_CUSTOM_H
#include "custom.h"
//#endif

#include "version.h"
#include "types.h"
#include "arduino.h"
#include "hardware.h"
#include "defaults.h"
#include "general.h"
#include "prototypes.h"
#include "sensors.h"
#include "dependencies.h"
#include "progmem.h"
#include "debug.h"

#ifdef USE_CORE_VERSION_H
#include "core_version.h"
#endif

// Redefine
#define HEARTBEAT_REPORT_IP         0
#define HEARTBEAT_REPORT_MAC        0
#define HEARTBEAT_REPORT_RSSI       0
#define HEARTBEAT_REPORT_UPTIME     0
#define HEARTBEAT_REPORT_DATETIME   0
#define HEARTBEAT_REPORT_FREEHEAP   0
#define HEARTBEAT_REPORT_VCC        0
#define HEARTBEAT_REPORT_HOSTNAME   0
#define HEARTBEAT_REPORT_APP        0
#define HEARTBEAT_REPORT_VERSION    0
#define HEARTBEAT_REPORT_BOARD      0

#define MQTT_STATUS_ONLINE          "online" 
#define MQTT_STATUS_OFFLINE         "offline"

#define SENSOR_DEBUG                1
#define SENSOR_READ_INTERVAL        10
#define SENSOR_REPORT_EVERY         6
