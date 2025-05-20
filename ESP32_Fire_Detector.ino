//#include <Tone32.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#define DHT_SENSOR_PIN  32 // ESP32 pin GPIO23 connected to DHT11 sensor
#define DHT_SENSOR_TYPE DHT11

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
//D22 SCL, D21 SDA
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
#define RXD2 26
#define TXD2 27

#define LED   14
#define LOA   15
#define CB_FIRE  33


void setup() {
  pinMode(CB_FIRE, INPUT);

  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  delay(500);
  dht_sensor.begin(); // initialize the DHT sensor
  lcd.init();         // initialize the lcd
  lcd.backlight();    // open the backlight
  test_sim800_module();

}
void loop() {
  updateSerial();
  // put your main code here, to run repeatedly:
  int FIRE_state = digitalRead(CB_FIRE);

  if (FIRE_state == LOW)
  {
    Serial.println("Nhan tin");
    SendSMS();
    //Call();
  }
  float humi  = dht_sensor.readHumidity();    // read humidity
  float tempC = dht_sensor.readTemperature(); // read temperature
  // check whether the reading is successful or not
  if (isnan(tempC) || isnan(humi)) {
    lcd.setCursor(0, 0);
    lcd.print("Failed");
  } else {
    lcd.setCursor(0, 0);  // display position
    lcd.print("NhietDo: ");
    lcd.print(tempC);     // display the temperature
    lcd.print((char)223);
    lcd.print("C");

    lcd.setCursor(0, 1);  // display position
    lcd.print("DoAm: ");
    lcd.print(humi);      // display the humidity
    lcd.print("%");
  }
}


void test_sim800_module()
{
  Serial2.println("AT");
  updateSerial();
  Serial.println();
  Serial2.println("AT+CSQ");
  updateSerial();
  Serial2.println("AT+CCID");
  updateSerial();
  Serial2.println("AT+CREG?");
  updateSerial();
  Serial2.println("ATI");
  updateSerial();
  Serial2.println("AT+CBC");
  updateSerial();
}

void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (Serial2.available())
  {
    Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
  }
}
void Call()
{
  Serial2.println("ATD0981615912;");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  delay(20000); // wait for 20 seconds...
  Serial2.println("ATH"); //hang up
  updateSerial();
}
void SendSMS()
{
  Serial2.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  Serial2.println("AT+CMGS=\"+84981615912\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  Serial2.print("Phat Hien Chay"); //text content
  updateSerial();
  Serial2.write(26);
}
