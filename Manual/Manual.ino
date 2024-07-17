#define BLYNK_TEMPLATE_ID "TMPL3FRBWbTB1"
#define BLYNK_TEMPLATE_NAME "plant watering system"
#define BLYNK_AUTH_TOKEN "xLkpcP_-UIZ3CiwW4G1riG1dK9C91zfx"
//Include the library files
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define sensor 33
#define relay 4

//Initialize the LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);

BlynkTimer timer;

// Enter your Auth token
char auth[] = "xLkpcP_-UIZ3CiwW4G1riG1dK9C91zfx";

//Enter your WIFI SSID and password
char ssid[] = "Shashank H";
char pass[] = "7829838331";
bool alertFlag = false;
bool highMoistureAlertFlag = false;
void setup() {
  // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  lcd.init();
  lcd.backlight();
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);

  lcd.setCursor(1, 0);
  lcd.print("System Loading");
  for (int a = 0; a <= 15; a++) {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(200);
  }
  lcd.clear();


}

//Get the ultrasonic sensor values
void soilMoisture() {
  int value = analogRead(sensor);
  value = map(value, 0, 4095, 0, 100);
  value = (value - 100) * -1;
  Blynk.virtualWrite(V0, value);
  Serial.println(value);
  lcd.setCursor(0, 0);
  lcd.print("Moisture :");
  lcd.print(value);
  lcd.print(" ");
  if (value < 21 && !alertFlag) {
    Blynk.logEvent("mless_moisture");
    alertFlag = true;
  } else if (value >= 21) {
    alertFlag = false;
  } if (value > 40 && !highMoistureAlertFlag) {
    Blynk.logEvent("msufficient_moisture");
    highMoistureAlertFlag = true;
  } else if (value <= 40) {
    highMoistureAlertFlag = false;
  }

}


//Get the button value
BLYNK_WRITE(V1) {
  bool Relay = param.asInt();
  if (Relay == 1) {
    digitalWrite(relay, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Motor is ON ");
  } else {
    digitalWrite(relay, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Motor is OFF");
  }
}

void loop() {
  soilMoisture();
  Blynk.run();//Run the Blynk library

  delay(200);

}