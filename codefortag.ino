// This #include statement was automatically added by the Particle IDE.
#include <oled-wing-adafruit.h>

//Code for tag

OledWingAdafruit display;

//String tagNum = "tag2"

int led2 = 7;

void setup() 

{
    display.setup();
    display.clearDisplay();
    display.display();
    Particle.subscribe("tag2",myHandler);   
    //each tag subscribes to its event stream. In this case Tag1 is this guys event
    pinMode(led2, OUTPUT);
}
    
void myHandler(const char *event, const char *data)
{
    digitalWrite(led2, HIGH);
    delay(100);
    digitalWrite(led2, LOW);
    delay(100);
    display.loop();
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,0);
	display.println(data);
	//beacon passes the message. Tag prints msg on OLED
	//maybe add a button here to clear screen and a light to blink when this thing prints anything.
	display.display();
	
		if (display.pressedA()) 
	{
		Particle.publish("tag2tobeacon","I'm available");
		//after receiving an incident 
    }
}
    


//This code is only for one tag. Each tag should have line 18 
//Particle.subscribe("tag1",myHandler); changed to its event's name.
