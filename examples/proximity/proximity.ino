#include <SoftwareSerial.h>
#include <CDBLEProx.h>

void ble_event(BLE_PROXIMITY_EVENT eventArgs);
SoftwareSerial sw(D5,D6);
CDBLEProximity ble(&sw,ble_event);

void setup() {
  Serial.begin(57600);
  ble.begin();
}

void loop() {
  ble.update();
}


void ble_event(BLE_PROXIMITY_EVENT eventArgs) {
    if (eventArgs.eventID==BLE_EVENT_ON_DEVICE_LOST) {
        Serial.println("No device");  
        Serial.println("");
    }
    if (eventArgs.eventID==BLE_EVENT_ON_DEVICE_APPROACH) {
        Serial.println("New device");
        Serial.print("UUID: "); Serial.println(eventArgs.device.address);
        Serial.print("MAC: "); Serial.println(eventArgs.device.mac);
        Serial.print("HL : "); Serial.println(eventArgs.device.hilo);
        Serial.print("HI : "); Serial.println(eventArgs.device.hi);
        Serial.print("LO : "); Serial.println(eventArgs.device.lo);
        Serial.print("SIG: "); Serial.println(eventArgs.device.rssi);
        Serial.println("");
    }
    if (eventArgs.eventID==BLE_EVENT_ON_DEVICE_MOVED) {
        Serial.println("Device moved");
        Serial.print("UUID: "); Serial.println(eventArgs.device.address);
        Serial.print("MAC: "); Serial.println(eventArgs.device.mac);
        Serial.print("HL : "); Serial.println(eventArgs.device.hilo);
        Serial.print("HI : "); Serial.println(eventArgs.device.hi);
        Serial.print("LO : "); Serial.println(eventArgs.device.lo);
        Serial.print("SIG: "); Serial.println(eventArgs.device.rssi);
        Serial.println("");
    }
}
