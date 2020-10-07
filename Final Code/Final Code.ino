
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Blynk.h>
#include <dht.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// define your values here
//############################################################
#define WIFI_SSID     "rohan"
#define WIFI_PASSWORD "rohan123"
char auth[] = "3710992c39ec4131b22de90744f2ef92";
const int trigPin1 =16;//d0
const int echoPin1 =5;//d1
const int trigPin2 =2;//d4
const int echoPin2=14;//d5
long duration1;
int distance1;
long duration2;
int distance2; 
const int sensor_pin = A0;  /* Connect Soil moisture analog sensor pin to A0 of NodeMCU */
int outputvalue=0 ;
float moisture_percentage;
int LED = 12; //D6
#define DHTPIN 0          // D3
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
 
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);
  if(t>34)
  {
     Blynk.email("irohan90839@gmail.com", "Temperature Alert", "Temperature over 34C!");
    delay(2000);
    }
  else
  {
    delay(2000);
    }
    
}

void setup() {
  Serial.begin(115200);
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(sensor_pin, INPUT);
  pinMode(LED,OUTPUT);
  
  //calling the connectToWifi() function
  connectToWifi();

  Blynk.begin(auth, WIFI_SSID ,WIFI_PASSWORD );
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8442);
 
  dht.begin();
 
  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop() {
  if(WiFi.status() != WL_CONNECTED){
    connectToWifi();        //reconnect in case of connection loss
  }
 ultra1();
 ultra2();
 moist();
 Blynk.run();
 timer.run();
}


   


void connectToWifi(){
  // funtion to connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("\n\nconnecting");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED, !digitalRead(LED));   // blink internal LED
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nconnected: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED, HIGH);
}

   
  

void ultra1(){
  // Clears the trigPin1
digitalWrite(trigPin1, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin1, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin1, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration1 = pulseIn(echoPin1, HIGH);

// Calculating the distance
distance1 = (duration1)*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance1(Dry): ");
Serial.println(distance1);
delay(1000);
/* Loop for an incoming message if needed.
 if(distance1<10)
{
  pinMode(LED,HIGH);
  notifier.sendNotification(DEVICE_ID,"heads up","Incoming!");
  delay(500);
}
  else
  {
    pinMode(LED,LOW);
  }
  delay(500);*/
if(distance1<10)
{
   Blynk.notify("Bin(A) is full,Clear trash");
  delay(1000);

}

else
{
  
  delay(1000);
}

}
void ultra2(){
  // Clears the trigPin2
digitalWrite(trigPin2, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin2, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin2, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration2 = pulseIn(echoPin2, HIGH);

// Calculating the distance
distance2 = (duration2)*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance2(Wet): ");
Serial.println(distance2);
delay(1000);
if(distance2<15)
{
  Blynk.notify("Bin(B) is full,Clear trash");
delay(1000);
}
else
{
  delay(1000);
}
}
void moist(){
  moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) );

  Serial.print("Soil Moisture(in Percentage) = ");
  Serial.print(moisture_percentage);
  Serial.println("%");

  delay(3000);
  if( moisture_percentage>= 15 )
   { digitalWrite(12, HIGH);
   }else
   {digitalWrite(12,LOW);
   }
   delay(1000);
  }




