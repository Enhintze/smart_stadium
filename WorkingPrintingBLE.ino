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
SYSTEM_MODE(AUTOMATIC); //ALERT: this should be set to automatic or code wont work.
#endif
#include <math.h>

//distance = 10 * pow((txPower-RSSI)/20)
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
#define BLE_SCAN_INTERVAL    0x0200 // 60 ms
#define BLE_SCAN_WINDOW      0x0200 // 30 ms
#define MAX_TAGS       20   // Allow for a total of 20 tags.

// Use an array to store the distances of all tags.
float distance[MAX_TAGS];

float distance1 = 50.00, distance2 = 50.00,distance3=50.00,distance4=50.00,smallestDistance;

float avgDistance1 = 20.00, avgDistance2 = 20.00, avgDistance3 = 20.00, avgDistance4 = 20.00;

String eventOne = "tag1tobeacon";String eventTwo = "tag2tobeacon";
String eventThree = "tag3tobeacon"; String eventFour = "tag4tobeacon";
//subscribe to these events in the codeForTag

boolean isAvailable1 = true; boolean isAvailable2 = true;
boolean isAvailable3 = true; boolean isAvailable4 = true;

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

    /*Find the addresses of all BLES and attach them to corresponding wearables. Note: Only Tag1 address is correct*/
    uint8_t tag1Compare1, tag1Compare2, tag1Compare3, tag1Compare4;
    tag1Compare1 = 232;
    tag1Compare2 = 45;
    tag1Compare3 = 202;
    tag1Compare4 = 113;
    uint8_t tag2Compare1, tag2Compare2, tag2Compare3, tag2Compare4;
    tag2Compare1 = 227;
    tag2Compare2 = 149;
    tag2Compare3 = 239;
    tag2Compare4 = 202;
    uint8_t tag3Compare1, tag3Compare2, tag3Compare3, tag3Compare4;
    tag3Compare1 = 253;
    tag3Compare2 = 202;
    tag3Compare3 = 84;
    tag3Compare4 = 176;
    uint8_t tag4Compare1, tag4Compare2, tag4Compare3, tag4Compare4;
    tag4Compare1 = 0;
    tag4Compare2 = 0;
    tag4Compare3 = 0;
    tag4Compare4 = 0;
   
    
  if ((address1==tag1Compare1) && (address2==tag1Compare2) && (address3 == tag1Compare3) && (address4 == tag1Compare4))
  {
    //FOR TAG1
    float txPower=-60;
    float RSSI = report->rssi;
    float raiseIt = ((txPower-RSSI)/20);
    // Tag1 distance will be stored in the first index of the array.
    distance1 = pow(10,raiseIt);
    float distArray[4];


    for (int i=0;i<=4;i++)
    {
      if (distArray[i] != distance1)
      {
        distArray[i]=distance1;
      }
      avgDistance1 += distArray[i];
    }

    avgDistance1 /=5;
    
    Serial.print("TAG1 Distance: ");
    Serial.println(avgDistance1);
    Serial.print("The peerAddr: ");
    
        for (index = 0; index < 6; index++) 
        {
          Serial.print(report->peerAddr[index],HEX);
          Serial.print(" ");
        }
        
    Serial.print("\nThe rssi: ");
    Serial.println(report->rssi);
    Serial.println();
    
    
  }
    if ((address1==tag2Compare1) && (address2==tag2Compare2) && (address3 == tag2Compare3) && (address4 == tag2Compare4))
  {
    
    float txPower=-60;
    float RSSI = report->rssi;
    float raiseIt = ((txPower-RSSI)/20);
    
    
    // Tag 2 distance will be placed at the second index of the distance array.
    distance2 = pow(10,raiseIt);
    
    float distArray[4];


    for (int i=0;i<=4;i++)
    {
      if (distArray[i] != distance2)
      {
        distArray[i]=distance2;
      }
      avgDistance2 += distArray[i];
    }

    avgDistance2 /=5;
    
    Serial.print("TAG2 Distance: ");
    Serial.println(distance2);
    Serial.print("The peerAddr: ");
    
        for (index = 0; index < 6; index++) 
        {
          Serial.print(report->peerAddr[index],HEX);
          Serial.print(" ");
        }
        
    Serial.print("\nThe rssi: ");
    Serial.println(report->rssi);
    Serial.println();
    
    
  }
    if ((address1==tag3Compare1) && (address2==tag3Compare2) && (address3 == tag3Compare3) && (address4 == tag3Compare4))
  { 
    
    float txPower=-60;
    float RSSI = report->rssi;
    float raiseIt = ((txPower-RSSI)/20);
    
    
    // Tag 2 distance will be placed at the second index of the distance array.
    distance3 = pow(10,raiseIt);

    float distArray[4];


    for (int i=0;i<=4;i++)
    {
      if (distArray[i] != distance3)
      {
        distArray[i]=distance3;
      }
      avgDistance3 += distArray[i];
    }

    avgDistance3 /=5;

    
    Serial.print("TAG3 Distance:");
    Serial.println(distance3);
    Serial.print("The peerAddr: ");
    
        for (index = 0; index < 6; index++) 
        {
          Serial.print(report->peerAddr[index],HEX);
          Serial.print(" ");
        }
        
    Serial.print("\nThe rssi: ");
    Serial.println(report->rssi);
    Serial.println();
    
    
  }
    if ((address1==tag4Compare1) && (address2==tag4Compare2) && (address3 == tag4Compare3) && (address4 == tag4Compare4))
  {
    
    float txPower=-60;
    float RSSI = report->rssi;
    float raiseIt = ((txPower-RSSI)/20);
    
    
    // Tag 2 distance will be placed at the second index of the distance array.
    distance4 = pow(10,raiseIt);

    float distArray[4];


    for (int i=0;i<=4;i++)
    {
      if (distArray[i] != distance4)
      {
        distArray[i]=distance4;
      }
      avgDistance4 += distArray[i];
    }

    avgDistance4 /=5;

    Serial.print("TAG4 Distance:" );
    Serial.println(distance4);
    Serial.print("The peerAddr: ");
    
        for (index = 0; index < 6; index++) 
        {
          Serial.print(report->peerAddr[index],HEX);
          Serial.print(" ");
        }
        
    Serial.print("\nThe rssi: ");
    Serial.println(report->rssi);
    Serial.println();
    
    
  }

}


void setup() {
  Serial.begin(115200);
  delay(500);
  //Serial.println("Scanning for BLE started.");
  Particle.subscribe("beacon1",myHandler);
  Particle.subscribe(eventOne,myHandler);
  Particle.subscribe(eventTwo,myHandler);
  Particle.subscribe(eventThree,myHandler);
  Particle.subscribe(eventFour,myHandler);
  
  //This event name and code is only for Beacon1. 
  //Upload this code to each beacon and change event to beacon#
  
  ble.init();

  ble.onScanReportCallback(reportCallback);

  // Set scan parameters.
  ble.setScanParams(BLE_SCAN_TYPE, BLE_SCAN_INTERVAL, BLE_SCAN_WINDOW);
  
  ble.startScanning();
  Serial.println("BLE scan start.");
}

void myHandler(const char *event, const char *data) 
//This should execute whenever any msg is received. The only message received should be from the Argon.
{
  Serial.printlnf("Message received from: %s data=%s", event, data ? data : "NULL");

/*
Comparing distance of each tag that is being scanned by the beacon.
The closest will be chosen. The chosen tag will receive a message, print on serial, and flash its built-in led.
*/
      if(eventOne == event)
      {
        isAvailable1==true;
      }
      else if (eventTwo==event)
      {
        isAvailable2=true;
      }
      else if (eventThree==event)
      {
        isAvailable3=true;
      }
      else if (eventFour==event)
      {
        isAvailable4=true;
      }

      if ((isAvailable1==true)&&(avgDistance1 <= avgDistance2) && (avgDistance1 <= avgDistance3) && (avgDistance1 <= avgDistance4))
      {
      smallestDistance = avgDistance1;
      isAvailable1=false;
      Serial.print("Closest tag is:1 Distance: ");
      Serial.println(avgDistance1);
      Serial.println();
      Particle.publish("tag1", data);
      }
      else if ((isAvailable2==true)&&(avgDistance2 <= avgDistance1) && (avgDistance2 <= avgDistance3) && (avgDistance2 <= avgDistance4))
      {
      smallestDistance = avgDistance2;
      isAvailable2=false;
      Serial.print("Closest tag is:2 Distance: ");
      Serial.println(avgDistance2);
      Serial.println();
      Particle.publish("tag2", data);
      }
      else if ((isAvailable3==true)&&(avgDistance3 <= avgDistance1) && (avgDistance3 <= avgDistance2) && (avgDistance3 <= avgDistance4))
      {
      smallestDistance = avgDistance3;
      isAvailable3=false;
      Serial.print("Closest tag is:3 Distance: ");
      Serial.println(avgDistance3);     
      Serial.println(); 
      Particle.publish("tag3", data);
      }
      else if ((isAvailable4==true)&&(avgDistance4 <= avgDistance1) && (avgDistance4 <= avgDistance2) && (avgDistance4 <= avgDistance3))
      {
      smallestDistance = avgDistance4;
      isAvailable4=false;
      Serial.print("Closest tag is:4 Distance: ");
      Serial.println(avgDistance4);
      Serial.println();
      Particle.publish("tag4", data);
      }
}
