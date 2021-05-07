#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <FirebaseESP8266.h>

// Set these to run example.
#define FIREBASE_HOST "hackoverflow-2f96e-default-rtdb.firebaseio.com/Users/ODV4hKZzizYg71j2r30ZK6XSaSF2/Sensor%20List/1"
#define FIREBASE_AUTH "ppyrNZdnDuJMcrbaEutQudvb8qGnWnlzv3D9CNMn"
#define WIFI_SSID "Virus.exe"
#define WIFI_PASSWORD "whatever@9211"
#define sID "Team#51_Sensor#01"

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

   while ( status != WL_CONNECTED) {
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network:
        status = WiFi.begin(ssid, pass);
        // wait 10 seconds for connection:
        delay(10000);
    }
    // you're connected now, so print out the data:
  Serial.println("You're connected to the network");


  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
    int val = digitalRead(inputPin);
    if (val == HIGH) {
        if (pirState == LOW) {
            Serial.println("Fire detected");
            pirState = HIGH;
            sendDataToFirebase();
            // Increase in Temperature detected
        }
    }
  else {
    if (pirState == HIGH) {
        Serial.println("Safe");
        pirState = LOW;
        // Temperature has decreased
        }
    }
    delay(5000);
}

void sendDataToFirebase() {
    String data = "{" ;
    data = data + "\"to\": \"your_smartphone_id\"," ;
    data = data + "\"notification\": {" ;
    data = data + "\"body\": \"Motion detected\"," ;
    data = data + "\"title\" : \"Alarm\" " ;
    data = data + "} }" ;

    Serial.println("Send data...");
    if (client.connect(fcmServer, 80)) {
        Serial.println("Connected to the server..");
        client.println("POST /fcm/send HTTP/1.1");
        client.println("Authorization: key=auth_key");
        client.println("Content-Type: application/json");
        client.println("Host: fcm.googleapis.com");
        client.print("Content-Length: ");
        client.println(data.length());
        client.print("\n");
        client.print(data);
        Firebase.pushString("/Users/Sensor List/"+sID+"Status", "1");    
    }
    Serial.println("Data sent...Reading response..");
    while (client.available()) {
        char c = client.read();
        Serial.print(c);
    }
    Serial.println("Finished!");
    client.flush();
    client.stop();
}
