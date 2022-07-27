#include <ESP8266WiFi.h>          // Үйдегі wifi - ға қосылуға арналған кітапхана
#include <ESP8266WebServer.h>     // Веб-сервер көтеруге арналған кітапхана
#include <Adafruit_Fingerprint.h> // Саусақ ізін сканерлейтін датчиктің кітапханасы
#include <SoftwareSerial.h>       // Serial порт-пен жұмыс жасайтын кітапхана
#include <WiFiClient.h>           // Wifi  клиент
#include <ESP8266HTTPClient.h>    // Веб-Серверге запрос тастайтын кітапхана

// Реле пині D4 ке жалғанған
#define relay D4

const char* ssid     = "iPhone 13";  // SSID
const char* password = "qwerty123";  // пароль

// Веб-сервердің end-point мекен жайлары
const String url =            "http://172.20.10.2:8090/api/noti";
const String new_finger_url = "http://172.20.10.2:8090/api/noti/new";

// Wifi клиент
WiFiClient client;
HTTPClient httpClient;

// Микробақылаушы үшін 80-ші портқа веб-сервер көтеру
ESP8266WebServer server(80);
SoftwareSerial mySerial(13, 15); // Саусақ ізін сканерлейтін датчиктің порттары

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial); // Serial порт үшін обьект

uint8_t id; // Саусақ ізі үшін айнымалы

//Саусақ таңбасын ауыстырыру 
bool state_finger_sensor = false;

// Саусақ ізі келгенін веб-сервер-ге жіберітін функция 
void send_curl_finger()
{
  String data = "The door is opened!";
  httpClient.begin(client, url);
  httpClient.addHeader("Content-Type", "text/plain");
  int code = httpClient.POST(data);
  String content = httpClient.getString();
  Serial.println(code);
  Serial.println(content);
  httpClient.end();
  
}
// Жаңа саусақ ізі келгенін веб-сервер-ге жіберітін функция
void send_curl_new_finger()
{
  String data = "The new finger is stored successfully";
  httpClient.begin(client, new_finger_url);
  httpClient.addHeader("Content-Type", "text/plain");
  int code = httpClient.POST(data);
  String content = httpClient.getString();
  Serial.println(code);
  Serial.println(content);
  httpClient.end();
}


// Инициялизация жасалынатын функция
void setup()
{  
   Serial.begin(9600);
   while (!Serial);
   delay(100); // 100 милисекунд пауза
   Serial.println("\n\nAdafruit Fingerprint sensor enrollment");
   finger.begin(57600); // саусақ ізін сканерлейтін датчиктің 
  
   if (finger.verifyPassword()) {
     Serial.println("Found fingerprint sensor!");
   } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
   }
   
   if (state_finger_sensor == false){
      finger.getTemplateCount();
      Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
      Serial.println("Waiting for valid finger...");
   }
   
   delay(100);
   Serial.println("Connecting to ");
   Serial.println(ssid);
   // жергілікті Wi-Fi желісіне қосылыңыз
   WiFi.begin(ssid, password);

   // wi-fi модулінің wi-fi желісіне қосылғанын тексеріңіз
   while (WiFi.status() != WL_CONNECTED) 
   {
     delay(1000);
     Serial.print(".");
   }
   Serial.println("");
   Serial.println("WiFi connected..!");
   Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
   
   // NodeMCU микробақылауышында веб-сервер көтеріледі, төмендегілер соның end-point-тары
   server.on("/", on_connect);              // Қосылу end-point-ты
   server.on("/api/on", ON);                // релені қосу end-point-ты
   server.on("/api/off", OFF);              // релені өшіру end-point-ты
   server.on("/api/newFinger", new_finger); // жаңа саусақ ізін қабылдау үшін end-point 
   
   server.onNotFound(handle_NotFound);
   server.begin();                          // Микробақылаушыда веб-серверді көтеру
   Serial.println("HTTP server started");
   pinMode(relay, OUTPUT);                  // Реле пинінің шығыс портқа орнату
   digitalWrite(relay, LOW);
}

// Қосылау handler-рі
void on_connect()
{
  server.send(200, "text/plain", "This is without front-end!");
}

// Жаңа саусақ ізі үшін handler
void new_finger()
{
  state_finger_sensor = true;
  server.send(200, "text/plain", "New finger template is running!");
}

// Реле қосылғанын басқаратын handler
void ON()
{
  digitalWrite(relay, HIGH);
  server.send(200, "text/plain", "The door is opening!");
}

// Реле өшкенін хабарлайтын handler
void OFF()
{
  digitalWrite(relay, LOW);
  server.send(200, "text/plain", "The door is closing!");
}


// Саусақ ізін оқитын функция
uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

// Саусақ ізінің ID-ын алатын функция
uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  // switch операторы
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK сәтті!

  p = finger.image2Tz();
  // switch операторы
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK конвертация жасалды!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 
  return finger.fingerID;
}

// Саусақ ізін нақты уақытта сканерлейтін функция 
int getFingerprintIDez() 
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  if(finger.confidence>100)
  {
    // Саусақ ізі анықталды және веб-серверге жіберу
    send_curl_finger();  
    Serial.println("Success");
  }
  

  return finger.fingerID; 
}

// Саусақ ізін енгізеьін функция
uint8_t Enroll()
{
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) 
  {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK сәтті!
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) 
  {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }
  
  // OK сәтті!
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK конвертация жасалды!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   

  
  // статусты өзгерту!
  send_curl_new_finger();
  state_finger_sensor = false;
  
  Serial.println("The enrolling is stopped!");

  
}

// Веб парақша табылмағанда жіберетін handler
void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}

// микробақылауыштың негізгі шексіз циклі  loop() функция ішінде жүреді 1 секунтда 80 млн такт! 
void loop()
{ 
  // Сервер қосылады
  server.handleClient();
  // Тексеру егер егер жаңа саусақ ізі енгізілмесе онда ол сканер режимін жалғастыра береді!
  if (state_finger_sensor == false){
       getFingerprintIDez();
       delay(50);
  }
  // Егер жаңа саусақ ізі енгізу режимі келсе онда микробақылауыш режимді өзгертеді!  
  else if (state_finger_sensor == true)
  {
      Serial.println("Ready to enroll a fingerprint!");
      Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
      id = readnumber();
      if (id == 0) { // ID #0 not allowed, try again!
         return;
      }
      Serial.print("Enrolling ID #");
      Serial.println(id);
      Enroll();
  }
}
