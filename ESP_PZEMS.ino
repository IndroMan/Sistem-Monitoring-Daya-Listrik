#include <PZEM004Tv30.h>
#include <WiFi.h>
#include <AsyncMqttClient.h>
#include <Ticker.h>
#include <NTPClient.h>  //untuk mengakses dan mengambil waktu dari server NTP (Network Time Protocol).
#include <WiFiUdp.h>    //digunakan untuk mengatur dan mengelola komunikasi melalui protokol UDP (User Datagram Protocol) menggunakan koneksi WiFi.

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 8 * 3600);  // Adjust the time zone offset accordingly

// Update these with values suitable for your network.


const char* ssid = "Monitoring Skripsi";
const char* password = "";
const char* mqtt_server = "tunnel.kazuyosan.my.id";


WiFiClient espClient;
AsyncMqttClient MqttClient;
Ticker mqttReconnectTimer;
unsigned long lastMsg = 0;
#define RXD2 16  // Pin RX untuk PZEM
#define TXD2 17  // Pin TX untuk PZEM

// Inisialisasi PZEM dengan Serial2 pada ESP32
PZEM004Tv30 pzem(&Serial2, RXD2, TXD2);

void setup_wifi() {  //perintah koneksi wifi
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);         //setting wifi chip sebagai station/client
  WiFi.begin(ssid, password);  //koneksi ke jaringan wifi

  while (WiFi.status() != WL_CONNECTED) {  //perintah tunggu esp32 sampi terkoneksi ke wifi
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void connectToMqtt() {
  Serial.println("Mencoba menghubungkan ke MQTT...");
  MqttClient.connect();
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("MQTT terputus. Mencoba reconnect dalam 5 detik...");
  mqttReconnectTimer.once(5, connectToMqtt);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  Serial.print("Pesan diterima di topik: ");
  Serial.println(topic);
  Serial.print("QoS: ");
  Serial.println(properties.qos);
  Serial.print("Pesan: ");
  Serial.write(payload, len);
  Serial.println();
}

void setup() {
  Serial.begin(9600);                           // Mulai komunikasi serial untuk debugging
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);  // Inisialisasi Serial2 untuk komunikasi dengan PZEM
  setup_wifi();                                 //memanggil void setup_wifi untuk dieksekusi
  MqttClient.onMessage(onMqttMessage);
  MqttClient.onDisconnect(onMqttDisconnect);
  MqttClient.setServer(mqtt_server, 8081);
  connectToMqtt();
  
  MqttClient.subscribe("monitoring/listrik/harga", 1);  // Subscribe dengan QoS 2
}

void loop() {
  float voltage = pzem.voltage();
  if (voltage != NAN) {
    Serial.print("Voltage: ");
    Serial.print(voltage);
    Serial.println("V");
  } else {
    Serial.println("Error reading voltage");
  }

  float current = pzem.current();
  if (current != NAN) {
    Serial.print("Current: ");
    Serial.print(current);
    Serial.println("A");
  } else {
    Serial.println("Error reading current");
  }

  float power = pzem.power();
  if (current != NAN) {
    Serial.print("Power: ");
    Serial.print(power);
    Serial.println("W");
  } else {
    Serial.println("Error reading power");
  }

  float energy = pzem.energy();
  if (current != NAN) {
    Serial.print("Energy: ");
    Serial.print(energy, 3);
    Serial.println("kWh");

  } else {
    Serial.println("Error reading energy");
  }

  float frequency = pzem.frequency();
  if (current != NAN) {
    Serial.print("Frequency: ");
    Serial.print(frequency, 1);
    Serial.println("Hz");
  } else {
    Serial.println("Error reading frequency");
  }

  float pf = pzem.pf();
  if (current != NAN) {
    Serial.print("PF: ");
    Serial.println(pf);
  } else {
    Serial.println("Error reading power factor");
  }
  //  memanggil waktu secara update
  timeClient.update();

  // deklarasi untuk mengatur waktu
  time_t epochTime = timeClient.getEpochTime();

  // deklarasi untuk mendapatk waktu
  String formattedTime = timeClient.getFormattedTime();
  MqttClient.publish("monitoring/listrik/waktu", 1, false, formattedTime.c_str());  // Publish data ke broker MQTT

  // Proses mereset energy sesuai dengan waktu ditentukan
  if (formattedTime >= "01:15:00" && formattedTime <= "01:15:10") {
    pzem.resetEnergy();
  }

  unsigned long sekarang = millis();
  if (sekarang - lastMsg > 2000) {  //perintah publish data
    lastMsg = sekarang;

    String tegangan = String(voltage, 2);  //membuat variabel tegangan untuk di publish ke broker mqtt
    tegangan += " V";
    MqttClient.publish("monitoring/listrik/tegangan", 1, false, tegangan.c_str());  //publish data dari varibel tegangan ke broker mqtt
    String arus = String(current * 100);                                 //membuat variabel arus untuk di publish ke broker mqtt
    arus += " A";
    MqttClient.publish("monitoring/listrik/arus", 1, false, arus.c_str());  //publish data dari varibel arus ke broker mqtt
    Serial.println(arus);
    String daya = String(energy, 3);                          // Membuat variabel daya
    daya += " KwH";                                           // Menambahkan " KwH" ke string daya
    MqttClient.publish("monitoring/listrik/daya", 1, false, daya.c_str());  // Publish data ke broker MQTT
    String Harga_listrik = String(energy * 1352);
    String harga_final = "Rp. " + Harga_listrik;
    MqttClient.publish("monitoring/listrik/harga", 1, false, harga_final.c_str());  // Publish harga listrik

    Serial.println();
    delay(2000);
  }
}
