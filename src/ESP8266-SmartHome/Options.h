//Wifi Credentials
#define WIFI_SSID ""  // Wifi Name
#define WIFI_PASSWORD ""  // Wifi Password

//Telegram Bot
#define BOT_TOKEN ""  // Bots token, found from the BotFather
#define CHAT_ID ""  // Your chat id from IDBot

//Other Variables
const unsigned long BOT_MTBS = 1000; // Mean Time Between Scan messages
const bool do_Serial = true;

// TEMP
const int ledPin = LED_BUILTIN;
int ledStatus = 0;

// Command array
const int arrayLength = 4;
String Array[4] =
  { 
  "/ledon",
  "/ledoff",
  "/status",
  "/start"
  };