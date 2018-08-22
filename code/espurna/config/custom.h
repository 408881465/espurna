
#define ALEXA_SUPPORT               0
#define DEBUG_SERIAL_SUPPORT        0
#define DOMOTICZ_SUPPORT            0
#define THINGSPEAK_SUPPORT          0

#define LED1_MODE                   LED_MODE_OFF

#define ADMIN_PASS                  "asdfqwer"
#define USE_PASSWORD                0

// #define HEARTBEAT_REPORT_IP         0
// #define HEARTBEAT_REPORT_MAC        0
// #define HEARTBEAT_REPORT_RSSI       0
// #define HEARTBEAT_REPORT_UPTIME     0
// #define HEARTBEAT_REPORT_DATETIME   0
// #define HEARTBEAT_REPORT_FREEHEAP   0
// #define HEARTBEAT_REPORT_VCC        0
// #define HEARTBEAT_REPORT_HOSTNAME   0
// #define HEARTBEAT_REPORT_APP        0
// #define HEARTBEAT_REPORT_VERSION    0
// #define HEARTBEAT_REPORT_BOARD      0

#define LOADAVG_REPORT              0

#define BUTTON_DEBOUNCE_DELAY       0
#define BUTTON_DBLCLICK_DELAY       0
#define RELAY_BOOT_MODE             RELAY_BOOT_SAME

#define WIFI_SCAN_NETWORKS          0
#define WIFI1_SSID                  "Router"
#define WIFI1_PASS                  "asdfqwer"
#define WEB_FORCE_PASS_CHANGE       0
#define WS_AUTHENTICATION           0

#define MQTT_ENABLED                1
#define MQTT_SERVER                 "Hassbian"
// #define MQTT_STATUS_ONLINE          "online" 
// #define MQTT_STATUS_OFFLINE         "offline"

#define NTP_TIME_OFFSET             480
#define NTP_DAY_LIGHT               0
#define NTP_DST_REGION              1

#define IR_BUTTON_SET               0

#define NODEMCU_ID                  1
#define SONOFF_ID                   0
#define HASSMART_ID                 0

#define MAKE_STRING(e)              #e
#define MAKE_HOSTNAME(a, b)         MAKE_STRING(a) MAKE_STRING(b)

#if NODEMCU_ID
    #define HOSTNAME                MAKE_HOSTNAME(NodeMCU, NODEMCU_ID)
#elif SONOFF_ID
    #define HOSTNAME                MAKE_HOSTNAME(Sonoff, SONOFF_ID)
#elif HASSMART_ID
    #define HOSTNAME                MAKE_HOSTNAME(Hassmart, HASSMART_ID)
#endif

#if NODEMCU_ID == 1
    #define IR_SUPPORT              1
    #define IR_TO_MQTT              1
    #define MQTT_TO_IR              1
#elif NODEMCU_ID == 2
    #define PMSX003_SUPPORT         1
    #define PMS_TYPE                PMS_TYPE_5003T
    #define PMS_SMART_SLEEP         1
#elif NODEMCU_ID == 3
    #define RELAY1_PIN              15
    #define RELAY1_RESET_PIN        15
    #define RELAY1_TYPE             RELAY_TYPE_LATCHED
    #define RELAY_LATCHING_PULSE    200

    #define RELAY_PROVIDER          RELAY_PROVIDER_JBL
#elif NODEMCU_ID == 4
    #define RELAY1_PIN              14
    #define RELAY2_PIN              12
    #define RELAY3_PIN              13
    #define RELAY_PROVIDER          RELAY_PROVIDER_SHARP4
    #define MHZ19_SUPPORT           1
    #define MHZ19_RX_PIN            3
    #define MHZ19_TX_PIN            1    
#elif NODEMCU_ID == 5
    #define RELAY1_MODE_PIN         5
    #define RELAY1_PIN              4
    #define RELAY2_PIN              0
    #define RELAY1_OFF_PIN          2
    #define RELAY_PROVIDER          RELAY_PROVIDER_SHARP
    #define RELAY_PROVIDER_SHARP_TRIGGER LOW
#elif NODEMCU_ID == 6
    #define RELAY1_MODE_PIN         5
    #define RELAY1_PIN              4
    #define RELAY2_PIN              0
    #define RELAY1_OFF_PIN          2
    #define RELAY_PROVIDER          RELAY_PROVIDER_SHARP
    #define RELAY_PROVIDER_SHARP_TRIGGER LOW

    #define MHZ19_SUPPORT           1
    #define MHZ19_RX_PIN            3
    #define MHZ19_TX_PIN            1    
    #define MHZ19_CORRECTION        -440
#elif NODEMCU_ID == 7
    #define RELAY1_MODE_PIN         5
    #define RELAY1_PIN              4
    #define RELAY2_PIN              0
    #define RELAY1_OFF_PIN          2
    #define RELAY_PROVIDER          RELAY_PROVIDER_SHARP
    #define RELAY_PROVIDER_SHARP_TRIGGER LOW

    #define PMSX003_SUPPORT         1
    #define PMS_TYPE                PMS_TYPE_5003ST
    #define PMS_SMART_SLEEP         1

    #define SENSEAIR_SUPPORT        1
    #define SENSEAIR_RX_PIN         1
    #define SENSEAIR_TX_PIN         3
#elif SONOFF_ID == 1
    #define BUTTON3_PIN             3
    #define BUTTON3_MODE            BUTTON_SWITCH | BUTTON_SET_PULLUP | BUTTON_DEFAULT_HIGH
    #define BUTTON3_RELAY           2
    #define RELAY2_PIN              20
    #define RELAY2_TYPE             RELAY_TYPE_NORMAL
#elif SONOFF_ID == 3
    #undef BUTTON2_PIN
    #undef BUTTON2_MODE
    #undef BUTTON2_RELAY
    #define RELAY2_PIN              14
    #define RELAY2_TYPE             RELAY_TYPE_NORMAL
    
    #define I2C_SDA_PIN             1
    #define I2C_SCL_PIN             3
    #define BH1750_SUPPORT          1
#elif HASSMART_ID

    #define MANUFACTURER            "HASSMART"

    // Buttons
    #define BUTTON1_PIN             0
    #define BUTTON2_PIN             14
    #define BUTTON1_RELAY           1
    #define BUTTON2_RELAY           2
    #define BUTTON1_MODE            BUTTON_PUSHBUTTON | BUTTON_DEFAULT_HIGH
    #define BUTTON2_MODE            BUTTON_PUSHBUTTON | BUTTON_DEFAULT_HIGH

    // Relays
    #define RELAY1_PIN              12
    #define RELAY2_PIN              5
    #define RELAY1_TYPE             RELAY_TYPE_NORMAL
    #define RELAY2_TYPE             RELAY_TYPE_NORMAL

    // LEDs
    #define LED1_PIN                13
    #define LED1_PIN_INVERSE        0

    #if HASSMART_ID == 2 || HASSMART_ID == 4 || HASSMART_ID == 6
        #define DEVICE              "DUAL"
        #undef BUTTON3_PIN
        #undef BUTTON3_RELAY
        #undef BUTTON3_MODE
    #elif HASSMART_ID == 1 || HASSMART_ID == 3 || HASSMART_ID == 5
        #define DEVICE              "TRIPPLE"
        #define BUTTON3_PIN         2
        #define BUTTON3_RELAY       3
        #define BUTTON3_MODE        BUTTON_PUSHBUTTON | BUTTON_DEFAULT_HIGH
        #define RELAY3_PIN          4
        #define RELAY3_TYPE         RELAY_TYPE_NORMAL
        #define _Hassmart5
        #if HASSMART_ID == 5
            #undef RELAY2_PIN
            #undef RELAY3_PIN
            #define RELAY2_PIN      22 // FAKE
            #define RELAY3_PIN      23 // FAKE
            #define RELAY4_PIN      4
            #define RELAY4_TYPE     RELAY_TYPE_NORMAL
        #endif
    #endif

#endif

//#define SENSOR_DEBUG              1
//#define SENSOR_READ_INTERVAL      10
//#define SENSOR_REPORT_EVERY       6

#define RELAY_PROVIDER_JBL          5
#define RELAY_PROVIDER_SHARP4       6
#define RELAY_PROVIDER_SHARP        7
