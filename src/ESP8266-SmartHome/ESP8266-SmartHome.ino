/*---------------------------------
  To do:
    
---------------------------------*/

// Libraries
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Config and Options
#include "Options.h"

// Initialization of Core Variables
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime; // Last time message scan occured
  
// Finds the index of the command sent
int findIndex(String Element)
{
  for (int i = 0; i < arrayLength; i++)
  {
     if (Element == Array[i]) return i + 1;
  }
  if (do_Serial) Serial.println("Index 0");
  return 0;
}

// Message Handler
void handleNewMessages(int numNewMessages)
{
  if (do_Serial) Serial.print("handleNewMessages: "); Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++) // Loops till all messages are processed
  {
    String text = bot.messages[i].text; // Message
    String chat_id = bot.messages[i].chat_id; // Chat id the User is connected to 
    String from_name = bot.messages[i].from_name; // Name of User
    
      switch (findIndex(text))
      {
        case 1: // /ledon
          digitalWrite(ledPin, LOW); // turn the LED on (HIGH is the voltage level)
          ledStatus = 1;
          bot.sendMessage(chat_id, "Led is ON", "");
          break;
  
        case 2: // /ledoff
          ledStatus = 0;
          digitalWrite(ledPin, HIGH); // turn the LED off (LOW is the voltage level)
          bot.sendMessage(chat_id, "Led is OFF", "");
          break;
  
        case 3: // /status
          if (ledStatus) {bot.sendMessage(chat_id, "Led is ON", "");}
          else {bot.sendMessage(chat_id, "Led is OFF", "");}
          break;
  
        case 4: // /start
          {
          String welcome = "Welcome to Universal Arduino Telegram Bot library, " + from_name + ".\n";
          welcome += "This is Flash Led Bot example.\n\n";
          welcome += "/ledon : to switch the Led ON\n";
          welcome += "/ledoff : to switch the Led OFF\n";
          welcome += "/status : Returns current status of LED\n";
          bot.sendMessage(chat_id, welcome, "Markdown");
          break;
          }
  
        default:
          if (do_Serial) Serial.println("\nIndex out of bound");
        if (do_Serial) Serial.println("\nDone");
      }
  }
}

//Setup
void setup() 
{
  if (do_Serial) 
  {
    Serial.begin(115200);
    Serial.println("\nStarting...");
  }

  //Initialize Pins
  pinMode(ledPin, OUTPUT); // initialize digital ledPin as an output.
  delay(10);
  digitalWrite(ledPin, HIGH); // initialize pin as off (active LOW)

  // attempt to connect to Wifi network:
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  if (do_Serial) Serial.print("\nConnecting to Wifi SSID: "); Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    if (do_Serial) Serial.print(".");
    delay(500);
  }
  if (do_Serial) Serial.print("\nWiFi connected.\nIP address: "); Serial.print(WiFi.localIP());
}

//Main
void loop() 
{
  if (millis() - bot_lasttime > BOT_MTBS)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      if (do_Serial) Serial.println("\ngot response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    
    bot_lasttime = millis();
  }
}
