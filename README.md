# BLE_PROXIMITY
Arduino + ESP8266 BLE Proximity Library with HM-10 CC2541 CC2540

This library provides proximity functions to ARDUINO or ESP8266. The peripheral used is HM-10.

You may use CC2541 but you have to "burn" the HM-10 firmware in order to respond to AT Commands.
Step 0
Connect HM-10 Vcc to 3.3V 
Connect HM-10 GND to GND
Connect HM-10 RX  to D6   You may use any pin
Connect HM-10 TX  to D5   You may use any pin

**********************************************************************************************
Step 1   declare includes and objects
**********************************************************************************************
#include <SoftwareSerial.h>
#include <CDBLEProx.h>
SoftwareSerial sw(D5,D6);                           // use the pins specified in Step 0
void ble_event(BLE_PROXIMITY_EVENT eventArgs);      // define your handler interface
CDBLEProximity ble(&sw,ble_event);                  // pass byref your SoftwareSerial object and your callback handler


**********************************************************************************************
Step 2  setup and run
**********************************************************************************************
void setup() {
  ble.begin();
}

void loop() {
  ble.update();
}

**********************************************************************************************
Step 3  define your event handler
**********************************************************************************************

void ble_event(BLE_PROXIMITY_EVENT eventArgs) {
    if (eventArgs.eventID==BLE_EVENT_ON_DEVICE_LOST) {   // called once when no device is in range
        Serial.println("No device");  
        Serial.println("");
    }
    if (eventArgs.eventID==BLE_EVENT_ON_DEVICE_APPROACH) {  // called one when a new device is the nearest to your HM-10
        Serial.println("New device");
        Serial.print("MAC: "); Serial.println(eventArgs.device.mac);
        Serial.print("HI : "); Serial.println(eventArgs.device.hi);
        Serial.print("LO : "); Serial.println(eventArgs.device.lo);
        Serial.print("SIG: "); Serial.println(eventArgs.device.rssi);
        Serial.println("");
    }
    if (eventArgs.eventID==BLE_EVENT_ON_DEVICE_MOVED) {   // called any time when the closer device is moved and it is still the nearest
        Serial.println("Device moved");
        Serial.print("MAC: "); Serial.println(eventArgs.device.mac);
        Serial.print("HL : "); Serial.println(eventArgs.device.hilo);
        Serial.print("LO : "); Serial.println(eventArgs.device.lo);
        Serial.print("SIG: "); Serial.println(eventArgs.device.rssi);
        Serial.println("");
    }
}

