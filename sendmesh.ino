

TCPServer server = TCPServer(23);
TCPClient client;

char c1;
int counter = 0 ;
int counter1 = 0;
bool flag = false;
char string[1024];



char array[1024];



void setup()
{
  // start listening for clients
      server.begin();
    
      // Make sure your Serial Terminal app is closed before powering your device
      Serial.begin(9600);
      // Wait for a USB serial connection for up to 30 seconds
      waitFor(Serial.isConnected, 30000);
    
    // Argon Local IP , WiFi name.
      Serial.println(WiFi.localIP());
      Serial.println(WiFi.subnetMask());
      Serial.println(WiFi.gatewayIP());
      Serial.println(WiFi.SSID());
      Serial.println("Argon Ready!");
      
}

char sendToBeacons(){
    
    // convert char array to String.
    String str(array);
    
    Serial.print("str: ");
    Serial.print(str);
    Serial.print("Section number is: ");
    Serial.print(array[8]);
    Serial.print(array[9]);
    
    // sending the incident to the correct beacon.
   if(array[8] == '0' || (array[8] == '1' && array[9] == '0')){
       
           Particle.publish("beacon1", str);
           Serial.print("\nsent to B1\n");
        //   str = "";
          emptyArray();

           
   }else if(array[8] == '1' || (array[8] == '2' && array[9] == '0')){
       
           Particle.publish("beacon2", str);
           Serial.print("\nsent to B2\n");
        //   str = "";
          emptyArray();

   }else if(array[8] == '2' || (array[8] == '3' && array[9] == '0')){
       
           Particle.publish("beacon3", str);
           Serial.print("\nsent to B3\n");
        //   str = "";
          emptyArray();

   }else if(array[8] == '3' || (array[8] == '4' && array[9] == '0')){
       
           Particle.publish("beacon3", str);
           Serial.print("\nsent to B4\n");
        //   str = "";
          emptyArray();

   }
   
   
   
   
}

void emptyArray() {
    
    
    
            for(int i = 0; i < sizeof(array); i++ ){
            
                array[i] = {0};
       
        }
    
    
}

void loop()
{
  if (client.connected()) {
   

    while (client.available()) {
        
        
        flag = true;
        char c = client.read();
        
        //filling the array with the data.
        array[counter] = c;
        counter ++;
 
    }
    
    // printing array content!
    if (flag == true){
         Serial.print("\nmessage received: ");
        for(int i = 0; i < sizeof(array); i++ ){
            if(array[i] != NULL){
                 Serial.print(array[i]);
                 
         }
        }
        counter = 0;
        sendToBeacons();
         flag = false;
        
    }
  
  } else {
    // if no client is yet connected, check for a new connection
    client = server.available();
  }
  
}
