#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <DHT.h>

// Wifi network station credentials
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN ""

const unsigned long BOT_MTBS = 1000; // mean time between scan messages

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime; // last time messages' scan has been done


// Sensor Pins
#define DHTPIN D7
#define LEDPIN1 D2
#define LEDPIN2 D3
#define LEDPIN3 D5
#define LEDPIN4 D6
#define DHTTYPE DHT11 //Mengatur TYPE DHT (Karena ada 2 jenis [DHT11 & DHT22])
int ledStatus1 = 0;
int ledStatus2 = 0;
int ledStatus3 = 0;
int ledStatus4 = 0;

String user1_chat_id = "";
String user2_chat_id = "";
String user3_chat_id = "";

DHT dht(DHTPIN, DHTTYPE);

void handleNewMessages(int numNewMessages) {
  Serial.print("Handle New Messages: ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    String user_id = bot.messages[i].from_id;
    if (from_name == "")
      from_name = "Guest";

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    
    if (text == "/ledon1") {
      if (user_id == user1_chat_id) {
        digitalWrite(LEDPIN1, HIGH); // turn the LED on (HIGH is the voltage level)
        ledStatus1 = 1;
        bot.sendMessage(chat_id, "LED 1 is ON", "");
      } else {
        bot.sendMessage(chat_id, from_name + ", tidak diizinkan untuk menyalakan LED 1", "");
      }
    }

    if (text == "/ledon2") {
      if (user_id == user2_chat_id) {
        digitalWrite(LEDPIN2, HIGH); // turn the LED on (HIGH is the voltage level)
        ledStatus2 = 1;
        bot.sendMessage(chat_id, "LED 2 is ON", "");
      } else {
        bot.sendMessage(chat_id, from_name + ", tidak diizinkan untuk menyalakan LED 2", "");
      }
    }

    if (text == "/ledon3") {
      if (user_id == user3_chat_id) {
        digitalWrite(LEDPIN3, HIGH); // turn the LED on (HIGH is the voltage level)
        ledStatus3 = 1;
        bot.sendMessage(chat_id, "LED 3 is ON", "");
      } else {
        bot.sendMessage(chat_id, from_name + ", tidak diizinkan untuk menyalakan LED 3", "");
      }
    }

    if (text == "/ledon4") {
      digitalWrite(LEDPIN4, HIGH); // turn the LED on (HIGH is the voltage level)
      ledStatus4 = 1;
      bot.sendMessage(chat_id, "Led is ON", "");
    }

    if (text == "/ledoff1") {
      if (user_id == user1_chat_id) {
        ledStatus1 = 0;
        digitalWrite(LEDPIN1, LOW); // turn the LED off (LOW is the voltage level)
        bot.sendMessage(chat_id, "LED 1 is OFF", "");
      } else {
        bot.sendMessage(chat_id, from_name + ", tidak diizinkan untu mematikan LED 1", "");
      }
    }

    if (text == "/ledoff2") {
      if (user_id == user2_chat_id) {
        ledStatus2 = 0;
        digitalWrite(LEDPIN2, LOW); // turn the LED off (LOW is the voltage level)
        bot.sendMessage(chat_id, "LED 2 is OFF", "");
      } else {
        bot.sendMessage(chat_id, from_name + ", tidak diizinkan untu mematikan LED 2", "");
      }
    }

    if (text == "/ledoff3") {
      if (user_id == user3_chat_id) {
        ledStatus3 = 0;
        digitalWrite(LEDPIN3, LOW); // turn the LED off (LOW is the voltage level)
        bot.sendMessage(chat_id, "LED 3 is OFF", "");
      } else {
        bot.sendMessage(chat_id, from_name + ", tidak diizinkan untu mematikan LED 3", "");
      }
    }

    if (text == "/ledoff4") {
      ledStatus3 = 0;
      digitalWrite(LEDPIN4, LOW); // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "Led is OFF", "");
    }

    if (text == "/ledstatus1") {
      String message = "LED Status: ";
      if(ledStatus1 == 1) {
        message += "ON";
      } else {
        message += "OFF";
      }
      
      bot.sendMessage(chat_id, message, "");
    }

    if (text == "/ledstatus2") {
      String message = "LED Status: ";
      if(ledStatus2 == 1) {
        message += "ON";
      } else {
        message += "OFF";
      }
      
      bot.sendMessage(chat_id, message, "");
    }

    if (text == "/ledstatus3") {
      String message = "LED Status: ";
      if(ledStatus3 == 1) {
        message += "ON";
      } else {
        message += "OFF";
      }
      
      bot.sendMessage(chat_id, message, "");
    }
    
    if (text == "/dhtstatus") {
      String message = "";
      message += "Suhu : "+ String(t) + "°C\n";
      message += "Kelembapan : "+ String(h) + " %";
      bot.sendMessage(chat_id, message, "");
    }

    if (text == "/start") {
      String welcome = "Welcome to IoT Telegram Bot, " + from_name + ".\n";
      welcome += "List of Commands:\n\n";
      welcome += "/ledon1 : to switch the LED 1 to ON by User 1 ON\n";
      welcome += "/ledon2 : to switch the LED 2 to ON by User 2 ON\n";
      welcome += "/ledon3 : to switch the LED 3 to ON by User 3 ON\n";
      welcome += "/ledon4 : to switch the LED 4 to ON by all User\n";
      welcome += "/ledoff1 : to switch the LED 1 to OFF by User 1\n";
      welcome += "/ledoff2 : to switch the LED 2 to OFF by User 2\n";
      welcome += "/ledoff3 : to switch the LED 3 to OFF by User 3\n";
      welcome += "/ledoff4 : to switch the LED 4 to OFF by all User\n";
      welcome += "/ledstatus1 : Returns current status of LED 1\n";
      welcome += "/ledstatus2 : Returns current status of LED 2\n";
      welcome += "/ledstatus3 : Returns current status of LED 3\n";
      welcome += "/ledstatus4 : Returns current status of LED 4\n";
      welcome += "/dhtstatus : Returns temperature and humidity status of DHT11\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}


void setup()
{
  Serial.begin(115200);
  Serial.println();

  // Sensor Pin Mode
  pinMode(LEDPIN1, OUTPUT);
  pinMode(LEDPIN2, OUTPUT);
  pinMode(LEDPIN3, OUTPUT);
  pinMode(LEDPIN4, OUTPUT);
  dht.begin();

  // attempt to connect to Wifi network:
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  // Check NTP/Time, usually it is instantaneous and you can delete the code below.
  Serial.print("Retrieving time: ");
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
}

void loop() {
  if (millis() - bot_lasttime > BOT_MTBS) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    
    while (numNewMessages) {
      Serial.println("Got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
}