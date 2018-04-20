#include "Arduino.h"
#include "SoftwareSerial.h"

#ifndef CDBLEProx_h
#define CDBLEProx_h
struct BLE_DEVICE {
  String address;
  String mac;
  int rssi;
  uint8_t hi;
  uint8_t lo;
  String hilo;
};
struct BLE_PROXIMITY_EVENT {
  uint8_t eventID;
  BLE_DEVICE device;
};


#define BLE_EVENT_ON_DEVICE_LOST 1
#define BLE_EVENT_ON_DEVICE_APPROACH 2
#define BLE_EVENT_ON_DEVICE_MOVED 3

typedef void(*CDBLEProximity_event_handler)(BLE_PROXIMITY_EVENT eventArgs);

class CDBLEProximity {
  public:
    CDBLEProximity(SoftwareSerial *softserial,CDBLEProximity_event_handler handler);
    void begin();
    void update();
    String str_token(String data, char separator, int index);
  private:
    SoftwareSerial *ss;
    CDBLEProximity_event_handler callback_event_handler;
    void sendEmptyEvent();
    
    BLE_DEVICE LAST_DEVICE;
    int    LAST_DEV_COUNT;
};
#endif
