// -----------------------------------------------------------------------------
// ADS115 ADC sensor over I2C
// Copyright (C) 2018 by Yonsm Guo <http://yonsm.net>
// -----------------------------------------------------------------------------

#if SENSOR_SUPPORT && ADS115_SUPPORT

#pragma once

#undef I2C_SUPPORT
#define I2C_SUPPORT 1 // Explicitly request I2C support.

#include "Arduino.h"
#include "I2CSensor.h"

class ADS1115Sensor : public I2CSensor {

    public:

        // ---------------------------------------------------------------------
        // Public
        // ---------------------------------------------------------------------

        ADS1115Sensor(): I2CSensor() {
            _sensor_id = SENSOR_ADS1115_ID;
            _count = 4;
        }

        // ---------------------------------------------------------------------

        void setMode(unsigned char mode) {
            if (_mode == mode) return;
            _mode = mode;
            _dirty = true;
        }

        // ---------------------------------------------------------------------

        unsigned char getMode() {
            return _mode;
        }

        // ---------------------------------------------------------------------
        // Sensor API
        // ---------------------------------------------------------------------

        // Initialization method, must be idempotent
        void begin() {

            if (!_dirty) return;

            // I2C auto-discover
            unsigned char addresses[] = {0x48, 0x49, 0x4A, 0x4B};
            _address = _begin_i2c(_address, sizeof(addresses), addresses);
            if (_address == 0) return;

            // Run configuration on next update
            _run_configure = true;
            _ready = true;
            _dirty = false;

        }

        // Descriptive name of the sensor
        String description() {
            char buffer[25];
            snprintf(buffer, sizeof(buffer), "ADS1115 @ I2C (0x%02X)", _address);
            return String(buffer);
        }

        // Type for slot # index
        unsigned char type(unsigned char index) {
            return MAGNITUDE_DIGITAL;
        }

        // Pre-read hook (usually to populate registers with up-to-date data)
        void pre() {
            _error = SENSOR_ERROR_OK;
            _read();
        }

        // Current value for slot # index
        double value(unsigned char index) {
            return _values[index];
        }

    protected:

        void _read() {

            // For one-shot modes reconfigure sensor & wait for conversion
            if (_run_configure) {

                // Configure mode
                i2c_write_uint8(_address, _mode);

                // According to datasheet
                // conversion time is ~16ms for low resolution
                // and ~120 for high resolution
                // but more time is needed
                unsigned long wait = (_mode & 0x02) ? 24 : 180;
                unsigned long start = millis();
                while (millis() - start < wait) delay(1);

                // Keep on running configure each time if one-shot mode
                _run_configure = _mode & 0x20;

            }

            uint16_t value = i2c_read_uint16(_address);
            if (level == 0xFFFF) {
                _error = SENSOR_ERROR_CRC;
                _run_configure = true;
                _values[0] = 0;
                return 0;
            }
            _values[0] = value;
        }

        unsigned char _mode;
        bool _run_configure = false;
        uint16_t _values[4] = 0;

};

#endif // SENSOR_SUPPORT && ADS115_SUPPORT
