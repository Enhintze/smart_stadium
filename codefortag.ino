// This #include statement was automatically added by the Particle IDE.
#include <oled-wing-adafruit.h>

//Code for tag

OledWingAdafruit display;

void myHandler(const char *event, const char *data)

{}

void setup() 

{
	display.setup();
	display.clearDisplay();
	display.display();
	Particle.subscribe("tag1",myHandler);   //each tag subscribes to its event stream. In this case Tag1 is this guys event
}

void loop()

{
    display.loop();
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,0);
	display.println("event=%s data=%s", event, data ? data : "NULL"); //beacon passes the message. Tag prints msg on OLED
	//maybe add a button here to clear screen and a light to blink when this thing prints anything.
	display.display();
}


//This code is only for one tag. Each tag should have line 18 Particle.subscribe("tag1",myHandler); changed to its event's name.