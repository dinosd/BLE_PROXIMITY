#include "CDBLEProx.h"
CDBLEProximity::CDBLEProximity(SoftwareSerial *softserial,CDBLEProximity_event_handler handler) {
 ss = softserial;
 callback_event_handler = handler;
}

void CDBLEProximity::begin() {
  LAST_DEV_COUNT = 0;
  
 
  
  ss->begin(9600);
  ss->print("AT");
  delay(100);
  while (ss->available() ) {
     String res = ss->readString();
     Serial.println(res);
  }

  ss->print("AT+ROLE1");
  delay(100);
  while (ss->available() ) {
     String res = ss->readString();
     Serial.println(res);
  }
 
  ss->print("AT+IMME1");
  delay(100);
  while (ss->available() ) {
     String res = ss->readString();
     Serial.println(res);
  }
  ss->print("AT+RESET");
  delay(2000);
  while (ss->available() ) {
     String res = ss->readString();
     Serial.println(res);
  }
  sendEmptyEvent();
}

void CDBLEProximity::update() {
  static int BLE_mode = 1;
  static String response = "";
  static int devices_found = 0;  
  static int strongest_rssi = -1000;
  static String strongest_hilo = "";
  static String strongest_mac = "";
  if (BLE_mode==1) {
      ss->print("AT+DISI?");
      response = "";
      BLE_mode=2;
      devices_found = 0;
      strongest_rssi = -1000;
      return;
  }
  if (BLE_mode==2) {
     while (ss->available()) {
       #ifdef ESP8266
          yield();
       #endif
       String str_temp=ss->readString();
       if (str_temp.startsWith("OK+DISIS")) {
          str_temp.replace("OK+DISIS","");
       }
       response += str_temp;
       while (response.indexOf("OK+DISC")>=0 && response.length()>=78 ) {
           #ifdef ESP8266
              yield();
           #endif

           String record = response.substring(0,78);
           response = response.substring(78);
         
           String address = str_token(record,':',2); 
           String hilo = str_token(record,':',3); 
           String mac  = str_token(record,':',4); 
           String rssi = str_token(record,':',5); 
           int temp_rssi = rssi.toInt();
           if (address=="00000000000000000000000000000000") {
              devices_found++;
               if (temp_rssi > strongest_rssi) {
                  strongest_mac = mac;
                  strongest_rssi = temp_rssi;
                  strongest_hilo = hilo;
               }
           }
           
       }
       if (response.indexOf("OK+DISCE")>=0) {
          
          BLE_mode=3;
          return;
       }
     }
  }
  if (BLE_mode==3) {
     BLE_mode=1;
     if (devices_found==0 && LAST_DEV_COUNT>0 ) {
        sendEmptyEvent();
     }
     if (devices_found>0 && LAST_DEVICE.mac != strongest_mac) {
        LAST_DEVICE.mac = strongest_mac;
        LAST_DEVICE.rssi = strongest_rssi;
        LAST_DEVICE.hilo = strongest_hilo;
        String h = LAST_DEVICE.hilo.substring(0,4);
        String l = LAST_DEVICE.hilo.substring(4,8);
        LAST_DEVICE.hi = (int)strtol(h.c_str(),NULL,16);
        LAST_DEVICE.lo = (int)strtol(l.c_str(),NULL,16);
        BLE_PROXIMITY_EVENT event;
        event.eventID = BLE_EVENT_ON_DEVICE_APPROACH;
        event.device = LAST_DEVICE;
        callback_event_handler(event);
     }
     if (devices_found>0 && LAST_DEVICE.mac == strongest_mac && LAST_DEVICE.rssi != strongest_rssi) {
        LAST_DEVICE.rssi = strongest_rssi;
        BLE_PROXIMITY_EVENT event;
        event.eventID = BLE_EVENT_ON_DEVICE_MOVED;
        event.device = LAST_DEVICE;
        callback_event_handler(event);
     }
     LAST_DEV_COUNT = devices_found;
  }
  
}
void CDBLEProximity::sendEmptyEvent() {
    LAST_DEVICE.mac = "";
    LAST_DEVICE.rssi = -1000;
    LAST_DEVICE.hi = 0;
    LAST_DEVICE.lo = 0;
    LAST_DEVICE.hilo = "";

    BLE_PROXIMITY_EVENT event;
    event.eventID = BLE_EVENT_ON_DEVICE_LOST;
    event.device = LAST_DEVICE;
    callback_event_handler(event);
}

String CDBLEProximity::str_token(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

