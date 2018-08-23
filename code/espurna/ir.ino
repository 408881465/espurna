/*

IR MODULE

Copyright (C) 2016-2018 by Xose Pérez <xose dot perez at gmail dot com>
Copyright (C) 2017-2018 by François Déchery

*/

#if IR_SUPPORT

#include <IRremoteESP8266.h>
#include <IRrecv.h>

IRrecv * _ir_recv;
decode_results _ir_results;
unsigned long _ir_last_toggle = 0;

// -----------------------------------------------------------------------------
// PRIVATE
// -----------------------------------------------------------------------------

#ifndef IR_TO_MQTT
#define IR_TO_MQTT 0
#endif
#if IR_TO_MQTT
#define MQTT_TOPIC_IR "ir"
#include <libb64/cencode.h>
#include <base64.h>
void _irToMQTT() {
    DEBUG_MSG_P(PSTR("[IR] decode_type: %d, bits: %d, value: %016llX, rawlen: %d\n"), (int)_ir_results.decode_type, (int)_ir_results.bits, _ir_results.value, (int)_ir_results.rawlen);

    int size = _ir_results.rawlen * RAWTICK;
    char *payload = (char *)malloc(size * 97 / 72 + 16);
    base64_encode_chars((const char *)_ir_results.rawbuf, size, payload);
    char *last = payload;
    for (char *p = payload; *p; p++) {
        if (*p != '\n')
            *last++ = *p;
    }
    *last = 0;
    DEBUG_MSG_P(PSTR("[IR] rawbuf: %s\n"), payload);
    mqttSend(MQTT_TOPIC_IR, _ir_results.decode_type, payload);
    free(payload);
}
#endif

#ifndef IR_FROM_MQTT
#define IR_FROM_MQTT 0
#endif
#ifndef IR_SEND_PIN
#define IR_SEND_PIN 5
#endif
#if IR_FROM_MQTT
#include <IRsend.h>
#include <libb64/cdecode.h>
IRsend *_ir_send;
#define MQTT_TOPIC_IR "ir"
void _irFromMQTTCallback(unsigned int type, const char * topic, const char * payload) {
   if (type == MQTT_CONNECT_EVENT) {
        mqttSubscribe(MQTT_TOPIC_IR "/+");
    } else  if (type == MQTT_MESSAGE_EVENT) {
        String t = mqttMagnitude((char *) topic);
        if (!t.startsWith("ir")) {
            return;
        }

        DEBUG_MSG_P(PSTR("[IR] %s=>%s\n"), topic, payload);
        int len = strlen(payload);
        char *data = (char *)malloc(len * 3 / 4 + 16);
        int size = base64_decode_chars(payload, len, data);
        if (size < 2) {
            DEBUG_MSG_P(PSTR("[IR] Invalid payload"));
            free(data);
            return;
        } else {
            DEBUG_MSG_P(PSTR("[IR] payload: %d bytes\n"), size);
            _ir_send->sendRaw((uint16_t *)data, size/2, 38);
            _ir_recv->enableIRIn(); // ReStart the IR receiver (if not restarted it is not able to receive data)
        }
        free(data);
    }
}
#endif

#if IR_BUTTON_SET != 0
void _irProcessCode(unsigned long code) {

    static unsigned long last_code;
    boolean found = false;

    // Repeat last valid code
    DEBUG_MSG_P(PSTR("[IR] Received 0x%08X\n"), code);
    if (code == 0xFFFFFFFF) {
        DEBUG_MSG_P(PSTR("[IR] Processing 0x%08X\n"), code);
        code = last_code;
    }

    for (unsigned char i = 0; i < IR_BUTTON_COUNT ; i++) {

        unsigned long button_code = pgm_read_dword(&IR_BUTTON[i][0]);
        if (code == button_code) {

            unsigned long button_mode = pgm_read_dword(&IR_BUTTON[i][1]);
            unsigned long button_value = pgm_read_dword(&IR_BUTTON[i][2]);

            if (button_mode == IR_BUTTON_MODE_STATE) {
                relayStatus(0, button_value);
            }

            if (button_mode == IR_BUTTON_MODE_TOGGLE) {

                if (millis() - _ir_last_toggle > 250){
                    relayToggle(button_value);
                    _ir_last_toggle = millis();
                } else {
                    DEBUG_MSG_P(PSTR("[IR] Ignoring repeated code\n"));
                }
            }

            #if LIGHT_PROVIDER != LIGHT_PROVIDER_NONE

                if (button_mode == IR_BUTTON_MODE_BRIGHTER) {
                    lightBrightnessStep(button_value ? 1 : -1);
                    nice_delay(150); //debounce
                }

                if (button_mode == IR_BUTTON_MODE_RGB) {
                    lightColor(button_value);
                }

                /*
                #if LIGHT_PROVIDER == LIGHT_PROVIDER_FASTLED
                    if (button_mode == IR_BUTTON_MODE_EFFECT) {
                        _buttonAnimMode(button_value);
                    }
                #endif
                */

                /*
                if (button_mode == IR_BUTTON_MODE_HSV) {
                    lightColor(button_value);
                }
                */

                lightUpdate(true, true);

            #endif

            found = true;
            last_code = code;
            break;

		}

	}

	if (!found) {
		DEBUG_MSG_P(PSTR("[IR] Ignoring code\n"));
	}
}
#endif


// -----------------------------------------------------------------------------
// PUBLIC API
// -----------------------------------------------------------------------------

void irSetup() {

    _ir_recv = new IRrecv(IR_PIN);
    _ir_recv->enableIRIn();

#if IR_FROM_MQTT
    _ir_send = new IRsend(IR_SEND_PIN);
    mqttRegister(_irFromMQTTCallback);
#endif

    // Register loop
    espurnaRegisterLoop(irLoop);

}

void irLoop() {
    if (_ir_recv->decode(&_ir_results)) {
		unsigned long code = _ir_results.value;
#if IR_BUTTON_SET != 0
		_irProcessCode(code);
#endif
#if IR_TO_MQTT
        _irToMQTT();
#endif
		_ir_recv->resume(); // Receive the next value
	}
}

#endif // IR_SUPPORT
