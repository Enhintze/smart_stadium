void setup() {
Serial.begin(9600);
}

void loop() {
Particle.publish("mason", "sssss");
}