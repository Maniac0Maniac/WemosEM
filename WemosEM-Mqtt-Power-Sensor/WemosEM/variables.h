
/*
  Global variables and constants

  Alfonso C. Alvarez (Alcar), 14nd September 2019

  @author <a href="mailto:alcar21@gmail.com">Alfonso Carlos Alvarez Reyes</a>

  Compile with Arduino 2.4.2
*/

//Debugging
bool Debugging = false; 

// I/O
#define Status_LED 2 // D4
#define Status_LED_On digitalWrite(Status_LED, HIGH)
#define Status_LED_Off digitalWrite(Status_LED, LOW)

#define MIN_READ_EM_INTERVAL 1000        // Minimum seconds read sensor Clamp of energy meter
#define NUM_SAMPLES 1480 // measured by Tom in Mui Wo

#define MAXLEN_MQTT_ENABLED 1
#define MAXLEN_MQTT_SERVER 100
#define MAXLEN_MQTT_PORT 5
#define MAXLEN_MQTT_USERNAME 30
#define MAXLEN_MQTT_PASSWORD 30
#define MAXLEN_SYSTEM_PASSWORD 30

#define MAXLEN_VOLTAGE 7
#define MAXLEN_ICAL 6
#define MAXLEN_TIMEZONE 2
#define MIN_MESSAGE_INTERVAL 5000
#define MAXLEN_MESSAGE_INTERVAL 7

#define HOSTNAME_PREFIX "wemosEM-"

#define TOPIC_VOLTAGE "voltage"
#define TOPIC_STATUS "status"
#define TOPIC_RESET_KWH "resetkWh"

// BLYNK Virtual Pins
#define VPIN_VOLTAGE V0
#define VPIN_CURRENT V1
#define VPIN_watts V2
#define VPIN_ENERGY V3
#define VPIN_ENERGY_LAST_RESET V4

// THINGSPEAK
#define TIME_BEETWEEN_REQUEST 15000 //15 seconds

const PROGMEM char TOPIC_HA_CURRENT[] = "homeassistant/sensor/%s/current/config";
const PROGMEM char TOPIC_HA_POWER[] = "homeassistant/sensor/%s/power/config";
const PROGMEM char TOPIC_HA_KWH[] = "homeassistant/sensor/%s/kWh/config";
const PROGMEM char TOPIC_HA_RSSI[] = "homeassistant/sensor/%s/rssi/config";
// const PROGMEM char TOPIC_HA_KWTOTAL[] = "homeassistant/sensor/%s/kwTotal/config";

//const PROGMEM char MESSAGE_HA_CURRENT[] = "{\"unit_of_measurement\":\"A\",\"state_topic\":\"wemos/%s/power\",\"value_template\":\"{{ value_json.current }}\",\"json_attributes_topic\":\"wemos/%s/power\",\"name\":\"%s-current\",\"unique_id\":\"%s-current-wemosem\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"sw_version\":\"WemosEM 1.1\",\"model\":\"Wemos D1 Mini + SCT013\",\"manufacturer\":\"Wemos\"},\"icon\":\"mdi:counter\",\"availability_topic\":\"wemos/%s/status\"}";
//const PROGMEM char MESSAGE_HA_POWER[] = "{\"unit_of_measurement\":\"w\",\"state_topic\":\"wemos/%s/power\",\"device_class\":\"power\",\"value_template\":\"{{ value_json.watts }}\",\"json_attributes_topic\":\"wemos/%s/power\",\"name\":\"%s-watts\",\"unique_id\":\"%s-watts-wemosem\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"sw_version\":\"WemosEM 1.1\",\"model\":\"Wemos D1 Mini + SCT013\",\"manufacturer\":\"Wemos\"},\"icon\":\"mdi:counter\",\"availability_topic\":\"wemos/%s/status\"}";
//const PROGMEM char MESSAGE_HA_KWH[] = "{\"unit_of_measurement\":\"kWh\",\"state_topic\":\"wemos/%s/power\",\"device_class\":\"power\",\"value_template\":\"{{ value_json.kWh }}\",\"json_attributes_topic\":\"wemos/%s/power\",\"name\":\"%s-kWh\",\"unique_id\":\"%s-kWh-wemosem\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"sw_version\":\"WemosEM 1.1\",\"model\":\"Wemos D1 Mini + SCT013\",\"manufacturer\":\"Wemos\"},\"icon\":\"mdi:counter\",\"availability_topic\":\"wemos/%s/status\"}";
// const PROGMEM char MESSAGE_HA_KWTOTAL[] = "{\"unit_of_measurement\":\"kwTotal\",\"state_topic\":\"wemos/%s/power\",\"device_class\":\"power\",\"value_template\":\"{{ value_json.kwTotal }}\",\"json_attributes_topic\":\"wemos/%s/power\",\"name\":\"%s-kwTotal\",\"unique_id\":\"%s-kwTotal-wemosem\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"sw_version\":\"WemosEM 1.1\",\"model\":\"Wemos D1 Mini + SCT013\",\"manufacturer\":\"Wemos\"},\"icon\":\"mdi:counter\",\"availability_topic\":\"wemos/%s/status\"}";

const PROGMEM char MESSAGE_HA_CURRENT[] = "{\"name\":\"%s Current\",\"unit_of_measurement\":\"A\",\"value_template\":\"{{ value_json.current }}\",\"device_class\":\"current\",\"unique_id\":\"%s_current\",";  //\"icon\":\"mdi:counter\",
const PROGMEM char MESSAGE_HA_POWER[] = "{\"name\":\"%s Watts\",\"unit_of_measurement\":\"W\",\"value_template\":\"{{ value_json.watts }}\",\"device_class\":\"power\",\"unique_id\":\"%s_watts\",";  //,\"icon\":\"mdi:counter\"
const PROGMEM char MESSAGE_HA_KWH[] = "{\"name\":\"%s kWh\",\"unit_of_measurement\":\"kWh\",\"value_template\":\"{{ value_json.kWh }}\",\"device_class\":\"energy\",\"unique_id\":\"%s_kWh\","; //,\"icon\":\"mdi:counter\"
const PROGMEM char MESSAGE_HA_RSSI[] = "{\"name\":\"%s RSSI\",\"unit_of_measurement\":\"%%\",\"value_template\":\"{{ value_json.rssi }}\",\"device_class\":\"power\",\"unique_id\":\"%s_rssi\",\"icon\":\"mdi:access-point\",";

const PROGMEM char MESSAGE_HA_FOOTER[]  = "\"state_topic\":\"wemos/%s/power\",\"json_attributes_topic\":\"wemos/%s/power\",\"availability_topic\":\"wemos/%s/status\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"sw_version\":\"WemosEM 1.1\",\"model\":\"Wemos D1 Mini + SCT013\",\"manufacturer\":\"Wemos\"}}";

// WIFI AND MQTT
WiFiClient espClient;                  // For ESP8266 boards
PubSubClient mqtt_client(espClient);
WiFiManager wifiManager;
WiFiEventHandler gotIpEventHandler, disconnectedEventHandler;  // Event IP Handler
unsigned long uptimeAP = 0;

// UPDATE FIRMWARE
const char* update_path = "/WebFirmwareUpgrade";

// TIME ZONE
//const PROGMEM char ntpServer[] = "pool.ntp.org";
const char *ntpServer = "pool.ntp.org";

// Message timing
int reconnected_count = 0;            // Counter of messages sent
long ms_since_last_message = 0;       // Time elapsed since last message was sent, in ms

// Energy Monitor
EnergyMonitor emon;

// Data of Energy Monitor
double wattsTotal = 0;               // Measured energy in watts/h
double kiloWattHours = 0;             // Measured energy in kWh
double beforeResetKiloWattHours = 0;  // Measured energy in kWh
double rmsCurrent = 0;                // Measured current in A
double rmsPower = 0;                  // Measured power in W
unsigned long lastTimeMeasure = 0;    // Last time measure in ms
boolean isResetEnergyToday = false;    // reset energy on setup day

// ThingSpeak
unsigned long lastTimeMeasureTS = 0;

// NTP
boolean syncEventTriggered = false;   // True if a time even has been triggered
NTPSyncEvent_t ntpEvent;              // Last triggered event

// Web Server
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

String My_MAC = "";                   // MAC address, to be read from ESP8266
String wifi_hostname;
String mqtt_topic = "None";
String mqtt_topic_status = "None";
String mqtt_topic_prefix_subscribe = "";
String mqtt_topic_subscribe = "";
bool wifiFirstConnected = false;

bool resetESP = false;
bool restartESP = false;
