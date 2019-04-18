/*
 * Copyright (c) 2016 RedBear
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 */

/* 
 * Defaultly disabled. More details: https://docs.particle.io/reference/firmware/photon/#system-thread 
 */
//SYSTEM_THREAD(ENABLED);

/*
 * Defaultly disabled. If BLE setup is enabled, when the Duo is in the Listening Mode, it will de-initialize and re-initialize the BT stack.
 * Then it broadcasts as a BLE peripheral, which enables you to set up the Duo via BLE using the RedBear Duo App or customized
 * App by following the BLE setup protocol: https://github.com/redbear/Duo/blob/master/docs/listening_mode_setup_protocol.md#ble-peripheral 
 * 
 * NOTE: If enabled and upon/after the Duo enters/leaves the Listening Mode, the BLE functionality in your application will not work properly.
 */
//BLE_SETUP(ENABLED);
 
/*
 * SYSTEM_MODE:
 *     - AUTOMATIC: Automatically try to connect to Wi-Fi and the Particle Cloud and handle the cloud messages.
 *     - SEMI_AUTOMATIC: Manually connect to Wi-Fi and the Particle Cloud, but automatically handle the cloud messages.
 *     - MANUAL: Manually connect to Wi-Fi and the Particle Cloud and handle the cloud messages.
 *     
 * SYSTEM_MODE(AUTOMATIC) does not need to be called, because it is the default state. 
 * However the user can invoke this method to make the mode explicit.
 * Learn more about system modes: https://docs.particle.io/reference/firmware/photon/#system-modes .
 */
#if defined(ARDUINO) 
SYSTEM_MODE(SEMI_AUTOMATIC); 
#endif
#include <math.h>
/* 
 * BLE scan parameters:
 *     - BLE_SCAN_TYPE     
 *           0x00: Passive scanning, no scan request packets shall be sent.(default)
 *           0x01: Active scanning, scan request packets may be sent.
 *           0x02 - 0xFF: Reserved for future use.
 *     - BLE_SCAN_INTERVAL: This is defined as the time interval from when the Controller started its last LE scan until it begins the subsequent LE scan.
 *           Range: 0x0004 to 0x4000
 *           Default: 0x0010 (10 ms)
 *           Time = N * 0.625 msec
 *           Time Range: 2.5 msec to 10.24 seconds
 *     - BLE_SCAN_WINDOW: The duration of the LE scan. The scan window shall be less than or equal to the scan interval.
 *           Range: 0x0004 to 0x4000
 *           Default: 0x0010 (10 ms)
 *           Time = N * 0.625 msec
 *           Time Range: 2.5 msec to 10240 msec
 */
#define BLE_SCAN_TYPE        0x00   // Passive scanning
#define BLE_SCAN_INTERVAL    0x0060 // 60 ms
#define BLE_SCAN_WINDOW      0x0030 // 30 ms
int distance1; int distance2; int distance3; int distance4; int smallestDistance;
/*
 * Advertising Event Type:
 *     - (0x00)BLE_GAP_ADV_TYPE_ADV_IND          : Connectable undirected.
 *     - (0x01)BLE_GAP_ADV_TYPE_ADV_DIRECT_IND   : Connectable directed.
 *     - (0x02)BLE_GAP_ADV_TYPE_ADV_SCAN_IND     : Scannable undirected.
 *     - (0x03)BLE_GAP_ADV_TYPE_ADV_NONCONN_IND  : Non connectable undirected.
 *     - (0x04)BLE_GAP_ADV_TYPE_SCAN_RSP         : Scan response.
 */
void reportCallback(advertisementReport_t *report) {
uint8_t index;
//  int* saveAdd; 

    //get the address of the BLE. Convert hex to decimal. Fill in the corresponding spaces address 1,2,3 ....
    //for example if address of BLE is FD:CA:54 ... 0xFD = 253 -> address1 = 253. Do this for each BLE
    //we want to pull information from.

    uint8_t address1, address2, address3, address4;
    address1 = report->peerAddr[0];
    address2 = report->peerAddr[1];
    address3 = report->peerAddr[2];
    address4 = report->peerAddr[3];
    uint8_t tag1Compare1, tag1Compare2, tag1Compare3, tag1Compare4;
    tag1Compare1 = 253;
    tag1Compare2 = 202;
    tag1Compare3 = 84;
    tag1Compare4 = 176;
    uint8_t tag2Compare1, tag2Compare2, tag2Compare3, tag2Compare4;
    tag1Compare1 = 220;
    tag1Compare2 = 202;
    tag1Compare3 = 241;
    tag1Compare4 = 200;
    uint8_t tag3Compare1, tag3Compare2, tag3Compare3, tag3Compare4;
    tag1Compare1 = 0;
    tag1Compare2 = 0;
    tag1Compare3 = 0;
    tag1Compare4 = 0;
    uint8_t tag4Compare1, tag4Compare2, tag4Compare3, tag4Compare4;
    tag1Compare1 = 0;
    tag1Compare2 = 0;
    tag1Compare3 = 0;
    tag1Compare4 = 0;
   

    
  if ((address1==tag1Compare1) && (address2==tag1Compare2) && (address3 == tag1Compare3) && (address4 == tag1Compare4))
  {
    //FOR TAG1
    int txPower=-60;
    int RSSI = report->rssi;
    int ratio = (RSSI*1.0)/txPower;
    
    
    distance1 = (0.899796)*pow(ratio,7.7095)+0.111;
    //not sure of accuracy but works

    Serial.print("TAG1 Distance: ");
    Serial.println(distance1);
    Serial.print("The peerAddr: ");
    
        for (index = 0; index < 6; index++) 
        {
          Serial.print(report->peerAddr[index],HEX);
          Serial.print(" ");
        }
        
    Serial.print("\nThe rssi: ");
    Serial.println(report->rssi); //insert conversion formula to convert RSSI to meters.
    Serial.println();
    
    
  }
    if ((address1==tag2Compare1) && (address2==tag2Compare2) && (address3 == tag2Compare3) && (address4 == tag2Compare4))
  {
    //FOR TAG2
    int txPower=-60;
    int RSSI = report->rssi;
    int ratio = (RSSI*1.0)/txPower;
    
    
    distance2 = (0.899796)*pow(ratio,7.7095)+0.111;
    //not sure of accuracy but works

    Serial.print("TAG1 Distance: ");
    Serial.println(distance1);
    Serial.print("The peerAddr: ");
    
        for (index = 0; index < 6; index++) 
        {
          Serial.print(report->peerAddr[index],HEX);
          Serial.print(" ");
        }
        
    Serial.print("\nThe rssi: ");
    Serial.println(report->rssi); //insert conversion formula to convert RSSI to meters.
    Serial.println();
    
    
  }
    if ((address1==tag3Compare1) && (address2==tag3Compare2) && (address3 == tag3Compare3) && (address4 == tag3Compare4))
  {
    //FOR TAG3
    int txPower=-60;
    int RSSI = report->rssi;
    int ratio = (RSSI*1.0)/txPower;
    
    
    distance3 = (0.899796)*pow(ratio,7.7095)+0.111;
    //not sure of accuracy but works

    Serial.print("TAG1 Distance: ");
    Serial.println(distance1);
    Serial.print("The peerAddr: ");
    
        for (index = 0; index < 6; index++) 
        {
          Serial.print(report->peerAddr[index],HEX);
          Serial.print(" ");
        }
        
    Serial.print("\nThe rssi: ");
    Serial.println(report->rssi); //insert conversion formula to convert RSSI to meters.
    Serial.println();
    
    
  }
    if ((address1==tag4Compare1) && (address2==tag4Compare2) && (address3 == tag4Compare3) && (address4 == tag4Compare4))
  {
    //FOR TAG4
    int txPower=-60;
    int RSSI = report->rssi;
    int ratio = (RSSI*1.0)/txPower;
    
    
    distance4 = (0.899796)*pow(ratio,7.7095)+0.111;
    //not sure of accuracy but works

    Serial.print("TAG1 Distance: ");
    Serial.println(distance1);
    Serial.print("The peerAddr: ");
    
        for (index = 0; index < 6; index++) 
        {
          Serial.print(report->peerAddr[index],HEX);
          Serial.print(" ");
        }
        
    Serial.print("\nThe rssi: ");
    Serial.println(report->rssi); //insert conversion formula to convert RSSI to meters.
    Serial.println();
    
    
  }
        if ((distance1 != 0) && (distance1 <= distance2) && (distance1 <= distance3) && (distance1 <= distance4))
      {
        smallestDistance = distance1;
        //talk to tag1
      }
      else if ((distance2 != 0) && (distance2 <= distance1) && (distance2 <= distance3) && (distance2 <= distance4))
      {
        smallestDistance = distance2;
        //talk to tag2
      }
      else if ((distance3 != 0) && (distance3 <= distance1) && (distance3 <= distance2) && (distance3 <= distance4))
      {
        smallestDistance = distance3;
        //talk to tag3
      }
      else if ((distance4 != 0) && (distance4 <= distance1) && (distance4 <= distance2) && (distance4 <= distance3))
      {
        smallestDistance = distance4;
        //talk to tag4
      }

}


void setup() {
  Serial.begin(115200);
  delay(5000);
  Serial.println("BLE scan demo.");
  Mesh.subscribe("argon", myHandler);
  
  ble.init();

  ble.onScanReportCallback(reportCallback);

  // Set scan parameters.
  ble.setScanParams(BLE_SCAN_TYPE, BLE_SCAN_INTERVAL, BLE_SCAN_WINDOW);
  
  ble.startScanning();
  Serial.println("BLE scan start.");
}

void myHandler(const char *event, const char *data)
{
  Serial.printlnf("event=%s data=%s", event, data ? data : "NULL");
  
   if ((distance1 != 0) && (distance1 <= distance2) && (distance1 <= distance3) && (distance1 <= distance4))
      {
        smallestDistance = distance1;
        Mesh.publish("tag1", "MSG1");
      }
      else if ((distance2 != 0) && (distance2 <= distance1) && (distance2 <= distance3) && (distance2 <= distance4))
      {
        smallestDistance = distance2;
        Mesh.publish("tag2", "MSG2");
      }
      else if ((distance3 != 0) && (distance3 <= distance1) && (distance3 <= distance2) && (distance3 <= distance4))
      {
        smallestDistance = distance3;
        Mesh.publish("tag3", "MSG3");
      }
      else if ((distance4 != 0) && (distance4 <= distance1) && (distance4 <= distance2) && (distance4 <= distance3))
      {
        smallestDistance = distance4;
        Mesh.publish("tag4", "MSG4");
      }
}

void loop() {

}
