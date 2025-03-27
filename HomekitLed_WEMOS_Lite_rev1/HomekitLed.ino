void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  homeSpan.setPairingCode("11122333");
  homeSpan.begin(Category::Lighting, "Home LED");   

  new SpanAccessory();
  new Service::AccessoryInformation();
  new Characteristic::Identify();
  new Service::LightBulb();
  new Characteristic::On();
  // new LED(LED_BUILTIN); 
}


void loop() {
  homeSpan.poll();
}