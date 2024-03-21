#include <ThingSpeak.h> //library platform thingspeak
#include <ESP8266WiFi.h> //library Wifi NodeMCU ESP8266
#include <WiFiClient.h> //library client
#include <DHT.h> //library DHT
#include<ESP8266WebServer.h> //library webserver


#define DHTPIN D5 // mendefinisikan pin D4 sebagai pin DHT
#define DHTTYPE DHT11 //mendefinisikan type dari DHT
DHT dht(DHTPIN, DHTTYPE);


ESP8266WebServer server(80);

unsigned int value;

int Led_Merah = 0;
int Led_Kuning = 0;
int Led_Hijau = 0;
int BUZZER = 0;

float Celsius;
float Fahrenheit;
float Kelvin;

const int LEDPIN_MERAH = D0;
const int LEDPIN_KUNING = D1;
const int LEDPIN_HIJAU = D2;
const int BUZZERPIN = D3;

const char *ssid = "Xiaomi 12T"; //setting nama wifi 
const char *password = "sebentar"; //password wifi 
WiFiClient client; //mengecek jika client sudah terhubung

unsigned long channel = 2471705; //ID akun ThingSpeak
const char * myWriteAPIKey = "TJJPBYH5C8VSPV36"; //memasukan apikey

void setup() {
  Serial.begin(9600); //memulai komunikasi serial dengan baud rate 115200
  dht.begin(); //memulai sensor DHT.
  pinMode(BUZZERPIN, OUTPUT);
  pinMode(LEDPIN_MERAH, OUTPUT);
  pinMode(LEDPIN_KUNING, OUTPUT);
  pinMode(LEDPIN_HIJAU, OUTPUT);

  digitalWrite(LEDPIN_MERAH, 0);
  digitalWrite(LEDPIN_KUNING, 0);
  digitalWrite(LEDPIN_HIJAU, 0);

  ThingSpeak.begin(client);
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); //menghubungkan dengan wifi router
  while (WiFi.status() != WL_CONNECTED){ //Cek koneksi Wifi.
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);

  server.on("/", handleRoot);
  
  server.begin();
}

void handleRoot() {
  String content = "<h1> Monitor Value Komponen Kelompok 3 IOT B </h1> <br> Suhu Celsius = " + String(Celsius) + "<br>" + "Suhu Fahrenheit = " + String(Fahrenheit) + "<br>" + "Suhu Kelvin = " + String(Kelvin);
  server.send(200, "text/html", content);
}

void loop() {
  server.handleClient(); 
  Celsius = dht.readTemperature();
  Fahrenheit = (Celsius * 9 / 5) + 32;
  Kelvin = Celsius + 273.15;

  if (Celsius > 36) {
    digitalWrite(LEDPIN_MERAH, 1);
    digitalWrite(LEDPIN_KUNING, 0);
    digitalWrite(LEDPIN_HIJAU, 0);
    Led_Merah = 1;
    Led_Kuning = 0;
    Led_Hijau = 0;
    BUZZER = 10;
    tone(BUZZERPIN, BUZZER);
  } else if (Celsius <= 36 && Celsius >= 30) {
    digitalWrite(LEDPIN_MERAH, 0);
    digitalWrite(LEDPIN_KUNING, 1);
    digitalWrite(LEDPIN_HIJAU, 0);
    Led_Merah = 0;
    Led_Kuning = 1;
    Led_Hijau = 0;
    BUZZER = 0;
    noTone(BUZZERPIN);
  } else if (Celsius < 30) {
    digitalWrite(LEDPIN_MERAH, 0);
    digitalWrite(LEDPIN_KUNING, 0);
    digitalWrite(LEDPIN_HIJAU, 1);
    Led_Merah = 0;
    Led_Kuning = 0;
    Led_Hijau = 1;
    BUZZER = 0;
    noTone(BUZZERPIN);
  }

  ThingSpeak.writeField(channel, 1, BUZZER, myWriteAPIKey); // tulis isi field (Channel, field number, data input, apikey) = Buzzer
  ThingSpeak.writeField(channel, 2, Celsius, myWriteAPIKey); // tulis isi field (Channel, field number, data input, apikey) = Suhu Celsius
  ThingSpeak.writeField(channel, 3, Fahrenheit, myWriteAPIKey); // tulis isi field (Channel, field number, data input, apikey) = Suhu Fahrenheit
  ThingSpeak.writeField(channel, 4, Kelvin, myWriteAPIKey); // tulis isi field (Channel, field number, data input, apikey) = Suhu Kelvin
  ThingSpeak.writeField(channel, 5, Led_Merah, myWriteAPIKey); // tulis isi field (Channel, field number, data input, apikey) = Led Merah
  ThingSpeak.writeField(channel, 6, Led_Kuning, myWriteAPIKey); // tulis isi field (Channel, field number, data input, apikey) = Led Kuning
  ThingSpeak.writeField(channel, 7, Led_Hijau, myWriteAPIKey); // tulis isi field (Channel, field number, data input, apikey) = Led Hijau

  delay(1000);
}