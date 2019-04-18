void myHandler(const char *event, const char *data)
{
  Serial.printlnf("event=%s data=%s", event, data ? data : "NULL");
}

void setup()
{
  Particle.subscribe("mason", myHandler);
  Serial.begin(9600);
}
