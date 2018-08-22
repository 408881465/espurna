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
void _irToMQTT() {
    DEBUG_MSG_P(PSTR("decode_type: %d\n"), _ir_results.decode_type);
    DEBUG_MSG_P(PSTR("value: %08X\n"), _ir_results.value);
    DEBUG_MSG_P(PSTR("bits: %s\n"), String(_ir_results.bits).c_str());

    String rawCode = "";
    for (uint16_t i = 1;  i < _ir_results.rawlen;  i++) {
        if (i % 100 == 0) yield();  // Preemptive yield every 100th entry to feed the WDT.
        rawCode = rawCode + (_ir_results.rawbuf[i] * RAWTICK);
        if ( i < _ir_results.rawlen-1 ) rawCode = rawCode + ","; // ',' not needed on last one
    }
    DEBUG_MSG_P(PSTR("rawbuf: %s\n"), rawCode.c_str());
}
#endif

#ifndef MQTT_TO_IR
#define MQTT_TO_IR 0
#endif
#if MQTT_TO_IR
  #include <IRsend.h>
  IRsend irsend(3);
#define IRbitsKey "IRBITS_" // bits  will be defined if a subject contains IRbitsKey followed by a value of 2 digits
#define IRRptKey "RPT_" // repeats  will be defined if a subject contains IRRptKey followed by a value of 1 digit

void _mqttToIR(char * topicOri, char * datacallback) {
  
  // IR DATA ANALYSIS    
  //send received MQTT value by IR signal
  boolean signalSent = false;
  uint64_t data = 0;
  String strcallback = String(datacallback);
  //trc(datacallback);
  int s = strcallback.length();
  //number of "," value count
  int count = 0;
  for(int i = 0; i < s; i++)
  {
   if (datacallback[i] == ',') {
    count++;
    }
  }
  if(count == 0){
    data = strtoul(datacallback, NULL, 10); // standard sending with unsigned long, we will not be able to pass values > 4294967295
  }
    
    //We look into the subject to see if a special Bits number is defined 
  String topic = topicOri;
  unsigned int valueBITS  = 0;
  int pos = topic.lastIndexOf(IRbitsKey);       
  if (pos != -1){
    pos = pos + +strlen(IRbitsKey);
    valueBITS = (topic.substring(pos,pos + 2)).toInt();
    //trc(F("Bits nb:"));
    //trc(valueBITS);
  }
  //We look into the subject to see if a special repeat number is defined 
  uint16_t  valueRPT = 0;
  int pos2 = topic.lastIndexOf(IRRptKey);
  if (pos2 != -1) {
    pos2 = pos2 + strlen(IRRptKey);
    valueRPT = (topic.substring(pos2,pos2 + 1)).toInt();
    //trc(F("IR repeat:"));
    //trc(valueRPT);
  }

    if (valueBITS == 0) valueBITS = NEC_BITS;
    irsend.sendNEC(data, valueBITS, valueRPT);

   //irrecv.enableIRIn(); // ReStart the IR receiver (if not restarted it is not able to receive data)
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
