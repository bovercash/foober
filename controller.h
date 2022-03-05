#include <Wire.h>

#ifndef Controller
#define Controller

#define NUNCHUK_ADDRESS 0x52

struct ControllerState {
  uint8_t posX;
  uint8_t posY;
  bool btnC;
  bool btnZ;
};

class Controller {

    private:
        uint8_t nunchuk_data[6];
        ControllerState state;

    public:
        Controller() {

        };

        void init() {
            Wire.begin();
            Wire.beginTransmissio(NUNCHUK_ADDRESS);
            Wire.write(0xF0);
            Wire.write(0x55);
            Wire.endTransmission(true);

            Wire.beginTransmissio(NUNCHUK_ADDRESS);
            Wire.write(0xFB);
            Wire.write(0x00);
            Wire.endTransmission(true);

        };

        uint8_t readState() {
            uint8_t i;
            Wire.requestFrom(NUNCHUK_ADDRESS, 6);
            #ifdef SAM
                delayMicroseconds(10);
            #endif

            for (i = 0; i < 6 && Wire.available(); i++) {
                nunchuk_data[i] = Wire.read();
            }
            Wire.beginTransmission(NUNCHUK_ADDRESS);
            Wire.write(0x00);
            #ifdef SAM
                delayMicroseconds(100);
            #endif
            Wire.endTransmission(true);
            return i == 6;
        };

        ControllerState getState() {
            state.posX = nunchuk_data[0];
            state.posY = nunchuk_data[1];
            state.btnC = (~nunchuk_data[5] >> 1) & 1;
            state.btnZ = (~nunchuk_data[5] >> 0) & 1;
            return state;
        };
};

#endif