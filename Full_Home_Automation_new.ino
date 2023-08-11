//Final 

//Include the library files
#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

//Initialize the LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = "Ap5-ulpx3UYCKseQjTbOFPPRvuYchLU4";//Enter your Auth token
char ssid[] = "MinhDat_2.4GHz";//Enter your WIFI name
char pass[] = "24022001";//Enter your WIFI password

DHT dht(D3, DHT11); //(sensor pin,sensor type)
BlynkTimer timer;
unsigned long times=millis();
bool isPushOn = false;
int Light_button;
WidgetLED led(V0);

// Define component pins
#define Buzzer D4
#define MQ2 A0
#define light D0
#define relay1 D7
#define relay2 D8


// V0 LED Widget - LED connect is blinking
void blinkLedWidget(){
  if (led.getValue()) {
    led.off();
  } else {
    led.on();
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);
  
  
  Blynk.syncVirtual(V9);
  delay(100);
  dht.begin();
  timer.setInterval(100L, blinkLedWidget);

//  timer.setInterval(100L, DHT11sensor);
  
  pinMode(A0,INPUT);
  pinMode(D0,INPUT);
  pinMode(D1,INPUT);
  pinMode(D2,INPUT);
  pinMode(D3,INPUT);
  pinMode(D4,OUTPUT);
  pinMode(D7,OUTPUT);
  pinMode(D8,OUTPUT);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  dht.begin();
  
  lcd.init();                //Khởi tạo LCD
//  lcd.clear();               //Xóa màn hình
  lcd.backlight();           //Bật đèn nền cho LCD
  lcd.setCursor(4,0);        //Cột 2, dòng 0
  lcd.print("Senior 2");    //Ghi chữ bắt đầu từ cột thứ 2 dòng 0
  lcd.setCursor(4,1);        //Cột 0, dòng thứ 1
  lcd.print("Project");
  delay(4000);
  lcd.clear();

//Call the functions
  timer.setInterval(100L, gassensor);
  timer.setInterval(100L, DHT11sensor);
}

//Get the MQ2 sensor values
void gassensor() {
  int value = analogRead(MQ2);
  Serial.println(value);
  value = map(value, 0, 1024, 0, 100);
  if (value <= 65) {
    digitalWrite(Buzzer, LOW);
  } else if (value > 65) {
    Blynk.notify("Warning! Gas leak detected");
    digitalWrite(Buzzer, HIGH);
  }
  Blynk.virtualWrite(V1, value);
  lcd.setCursor(0, 0);
  lcd.print("G:");
  lcd.print(" ");
  lcd.print(value);

}

//Get the DHT11 sensor values
void DHT11sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V2, t);
  Blynk.virtualWrite(V3, h);

  lcd.setCursor(8, 0);
  lcd.print("T:");
  lcd.print(t);

  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.print(h);

}


//Get buttons values
BLYNK_WRITE(V9) {
 Light_button = param.asInt();
 isPushOn = true;
}

//Get buttons 2
BLYNK_WRITE(V10) {
 bool RelayTwo = param.asInt();
  if (RelayTwo == 1) {
    digitalWrite(relay2, LOW);
  } else {
    digitalWrite(relay2, HIGH);
  }
}


void loop() {
  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer

//LDRsensor
  int Light_notification = digitalRead(D0);
    if (Light_button == 0){
    if (Light_notification == 1){
      digitalWrite(D7,HIGH);
      isPushOn = false;
    }
    else if ( Light_notification == 0){
      digitalWrite(D7,LOW);
    }   
  }
    if(isPushOn == true){
      digitalWrite(D7,Light_button);
      isPushOn = false;
    }

}  
