// The original version of this file is located at https://github.com/sktometometo/smart_device_protocol/blob/develop/arduino_lib/ArduinoAtomS3Hardware.h by sktometometo
#include <Arduino.h>  // Arduino 1.0

#define SERIAL_CLASS HWCDC

class ArduinoAtomS3Hardware {
 public:
  ArduinoAtomS3Hardware(SERIAL_CLASS* io, long baud = 57600) {
    iostream = io;
    baud_ = baud;
  }
  ArduinoAtomS3Hardware() {
    iostream = &Serial;
    baud_ = 57600;
  }
  ArduinoAtomS3Hardware(ArduinoAtomS3Hardware& h) {
    this->iostream = h.iostream;
    this->baud_ = h.baud_;
  }

  void setBaud(long baud) { this->baud_ = baud; }

  int getBaud() { return baud_; }

  void init() { iostream->begin(baud_); }

  int read() { return iostream->read(); };
  void write(uint8_t* data, int length) {
    for (int i = 0; i < length; i++) iostream->write(data[i]);
  }

  unsigned long time() { return millis(); }

 protected:
  SERIAL_CLASS* iostream;
  long baud_;
};
