#include <SoftwareSerial.h>
#include <TinyGPS.h>


 // RX, TX do arduino 10 - RX, 11 - TX
 // Serial para GPS
SoftwareSerial GpsSerial(10, 11);
//Lib para usar os dados do GPS
TinyGPS gps1;


String apn = "";                    //APN (provedora de internet do seu chip)
String apnUser = "";                     //APN-Username
String apnPass = "";                     //APN-Password
String url = "";  //URL da sua Api, se tiver ssl (https) mude para http


char PAYLOAD[90] ;
//Serial para GSM
SoftwareSerial GsmSerial(7, 8); // RX, TX

float lastLat, lastLng = 0;

void setup() {
      GpsSerial.begin(9600);
      Serial.begin(115200);
      Serial.println("Init Setup");
      GsmSerial.begin(9600);
      GsmSerial.listen();
      delay(15000);
      while (GsmSerial.available()) {
        Serial.write(GsmSerial.read());
      }
      delay(2000);
      gsmConfigGprs();

      Serial.println("Setup completo");
}

void loop() {
  bool received = false;
  GpsSerial.listen();
  while (GpsSerial.available()) {
     char dataSerial = GpsSerial.read();
     received = gps1.encode(dataSerial);
  }

  if (received) {
     long latitude, longitude;
     float lat, lng;
     unsigned long infoAge;

     //Armazena os valores de latitude, longitude e idade nas variáveis abaixo
     gps1.get_position(&latitude, &longitude, &infoAge);     

     if (latitude != TinyGPS::GPS_INVALID_F_ANGLE) {
        lat = float(latitude) / 100000;
     }

     if (longitude != TinyGPS::GPS_INVALID_F_ANGLE) {
         lng = float(longitude) / 100000;
     }

    if(lat != lastLat || lng != lastLng){
         Serial.println(lat,6);
         Serial.println(lng,6);

         lastLat = lat;
         lastLng = lng;

          sendPost("{\"latitude\": \"" + String(lat, 6) +"\", \"longitude\": \""+ String(lng, 6) +"\"}");
      }

     delay(50000);

  }
}


void sendPost( String postdata) {
  Serial.println(" --- Start GPRS & HTTP --- ");
  gsmSendAtCommand("AT+SAPBR=1,1");
  gsmSendAtCommand("AT+HTTPINIT");
  gsmSendAtCommand("AT+HTTPPARA=URL," + url);
  gsmSendAtCommand("AT+HTTPPARA=CONTENT,application/json");
  gsmSendAtCommand("AT+HTTPDATA=137,5000");
  gsmSendAtCommand(postdata);
  gsmSendAtCommand("AT+HTTPACTION=1");
  gsmSendAtCommand("AT+HTTPREAD");
  gsmSendAtCommand("AT+HTTPTERM");
  gsmSendAtCommand("AT+SAPBR=0,1");
}

void gsmConfigGprs() {
  Serial.println(" --- CONFIG GPRS --- ");
  gsmSendAtCommand("AT+SAPBR=3,1,Contype,GPRS");
  gsmSendAtCommand("AT+SAPBR=3,1,APN," + apn);
  if (apnUser != "") {
    gsmSendAtCommand("AT+SAPBR=3,1,USER," + apnUser);
  }
  if (apnPass != "") {
    gsmSendAtCommand("AT+SAPBR=3,1,PWD," + apnPass);
  }
}

void gsmSendAtCommand(String command) {
  GsmSerial.listen();
  Serial.println("Send ->: " + command);
  GsmSerial.println(command);
  long wtimer = millis();
  while (wtimer + 3000 > millis()) {
    while (GsmSerial.available()) {
      Serial.write(GsmSerial.read());
    }
  }
  Serial.println();
}
