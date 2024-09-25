// WARNING: THIS HAS NOT YET BEEN TESTED!

// ESPHome Custom Component for the ST STLED316 6 Digit 7-Segment LED Controller Chip.
// This initial version just controlls 2 digits, however it can easily be adapted to control up to 6 digits

#include "esphome.h"
#include "Wire.h"

class STLED316Display : public PollingComponent, public Sensor {
 public:
   Sensor *get_display_value_sensor() { return display_value_sensor_; }

   STLED316Display() : PollingComponent(1000) {}

   void setup() override {
     // Nothing to do here
     Wire.begin();
   }

   void update() override {
     // Read the display value from STLED316
     int display_value = readSTLED316Value();
     if (display_value != -1) {
       publish_state(display_value);
     }
   }

 private:
   Sensor *display_value_sensor_ = new Sensor();

   int readSTLED316Value() {
     int digit1 = readSTLED316Digit(0x01);  // First 7-segment digit
     int digit2 = readSTLED316Digit(0x02);  // Second 7-segment digit

     if (digit1 != -1 && digit2 != -1) {
       return digit1 * 10 + digit2;  // Combine digits to form the number
     } else {
       return -1;  // Error reading value
     }
   }

   int readSTLED316Digit(byte reg) {
     Wire.beginTransmission(0x38);  // Use your STLED316 I²C address
     Wire.write(reg);
     Wire.endTransmission();

     Wire.requestFrom(0x38, 1);
     if (Wire.available()) {
       byte data = Wire.read();
       return decode_digit(data);  // Function to decode 7-segment data to digit
     }
     return -1;
   }

   int decode_digit(byte data) {
     // Add logic to decode the 7-segment display data into digits (0-9)
     switch (data) {
       case 0b00111111: return 0;
       case 0b00000110: return 1;
       case 0b01011011: return 2;
       case 0b01001111: return 3;
       case 0b01100110: return 4;
       case 0b01101101: return 5;
       case 0b01111101: return 6;
       case 0b00000111: return 7;
       case 0b01111111: return 8;
       case 0b01101111: return 9;
       default: return -1;  // Unknown data
     }
   }
};
