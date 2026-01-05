#include <M5Unified.h>
#include <M5GFX.h>

void setup() {
  M5.begin();
  M5.Display.setRotation(0);
  M5.Display.setFont(&fonts::FreeMonoBold18pt7b);

  M5.Display.setCursor(0, 200);

  M5.Display.print("    PaperS3 Realtime\n");
  M5.Display.print("     Battery Status\n\n\n");
  M5.Display.print(" Battery Charging:\n\n");
  M5.Display.print(" Battery    Level:\n\n");
  M5.Display.print(" Battery  Voltage:\n\n");
}

void loop() {
  M5.update();

  bool    isCharging     = M5.Power.isCharging();
  int32_t batteryLevel   = M5.Power.getBatteryLevel();    // 0 - 100 %
  int16_t batteryVoltage = M5.Power.getBatteryVoltage();  // unit: mV

  M5.Display.setCursor(380, 340);
  M5.Display.printf("%s \n\n", isCharging ? "Yes" : "No");

  M5.Display.setCursor(380, 410);
  M5.Display.printf("%d %%  \n\n", batteryLevel);

  M5.Display.setCursor(380, 480);
  M5.Display.printf("%d mV   \n\n", batteryVoltage);

  delay(2000);
} 