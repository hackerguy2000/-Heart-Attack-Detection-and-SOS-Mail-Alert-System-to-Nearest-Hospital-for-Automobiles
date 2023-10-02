#include <TinyGPSPlus.h>
#include <Arduino.h>
#include <ESP_Mail_Client.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ThingSpeak.h>

const char* ssid = "Tenda_431B88";
const char* password = "12345678";
const char* server = "api.thingspeak.com";
const char* apiKey_thingspeak = "WJTRZCS0R39VBQV4"; 



// The TinyGPSPlus object
char LatStr[20]; // Adjust the array size based on your required precision
char LngStr[20]; // Adjust the array size based on your required precision

int sensorPin = 34; // A0 is the input pin for the heart rate sensor
 

float sensorValue = 0; // Variable to store the value coming from the sensor

int count = 9;

unsigned long starttime = 0;

int heartrate = 0;

boolean counted = false;
const int MAX_HOSPITALS =4;
String names[MAX_HOSPITALS];
long double latitudes_arr[MAX_HOSPITALS];
long double longitudes_arr[MAX_HOSPITALS];
long double Distance_arr[MAX_HOSPITALS];


bool performAction = true;
TinyGPSPlus gps;
WiFiClient client;


void setup(void)
{

    pinMode(2, OUTPUT); // D13 LED as Status Indicator

    Serial.begin(9600);
  // Serial.begin(9600);
  Serial.println();
  Serial.print("Connecting to AP");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

    Serial2.begin(9600);
    

   delay(3000); // Start Serial Communication @ 9600
    
  ThingSpeak.begin(client); 
}

void loop()

{

    starttime = millis();

    while (millis() < starttime + 10000) // Reading pulse sensor for 10 seconds

    {

        sensorValue = analogRead(sensorPin);

        if (sensorValue > 550 && counted == false) // Threshold value is 550 (~ 2.7V)

        {

            count++;

            Serial.print("count = ");

            Serial.println(count);

            digitalWrite(2, HIGH);

            delay(50);

            digitalWrite(2, LOW);

            counted = true;
        }

        else if (sensorValue < 550)

        {

            counted = false;

            digitalWrite(2, LOW);
        }
    }

    heartrate = (+count * 6)-140; // Multiply the count by 6 to get beats per minute

    Serial.println();

    Serial.print("BPM = ");

    Serial.println(heartrate); // Display BPM in the Serial Monitor

    Serial.println();
    count = 0;
    // GPS CODE
    while (Serial2.available() > 0)

        if (gps.encode(Serial2.read()))
        {
            displayInfo();
           // long double A=gps.location.lat();
         //   long double B=gps.location.lng();

            
            if (heartrate > 100 && WiFi.status() == WL_CONNECTED)
            {
                  ThingSpeak.setField(1, heartrate); // Field 1: BPM
                  ThingSpeak.setField(2, static_cast<float>(gps.location.lat())); // Field 2: Latitude
                  ThingSpeak.setField(3, static_cast<float>(gps.location.lng())); // Field 3: Longitude
               
              HTTPClient http;
    String url = "https://maps.googleapis.com/maps/api/place/nearbysearch/json?";
    url += "location="+String(gps.location.lat(),6)+","+String(gps.location.lng(),6);
    url += "&radius=7000"; // Set the radius in meters here
    url += "&type=hospital";
    url += "&key=AIzaSyDd577FfREZXORIDQ4iTuOFtJGgMxNQ9VU"; // Replace with your actual Google Maps API key

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        //Serial.println(payload);


        // Parse JSON response
        DynamicJsonDocument doc(6144);
        deserializeJson(doc, payload);

        JsonArray results = doc["results"];
        int counts= results.size();

        Serial.println("Nearby hospitals:");

        for (int i = 0; i < counts && i < MAX_HOSPITALS; i++)
        {
          JsonObject place = results[i];
          String name = place["name"].as<String>();
          long double latitude= place["geometry"]["location"]["lat"].as<long double>();
          long double longitude = place["geometry"]["location"]["lng"].as<long double>();
   

          names[i] = name;
          //String mails[]
          Serial.print("Hospital Name: ");
          Serial.println(name);
          
          latitudes_arr[i]=latitude;
          longitudes_arr[i]=longitude;
          int httpCode = ThingSpeak.writeFields(2128844, apiKey_thingspeak);
           if (httpCode == 200)
    {
      Serial.println("Data sent to ThingSpeak successfully!");
    }
    else
    {
      //Serial.println(httpCode);
    }
        }
     
      }
      else {
        Serial.println("Error on HTTP request");
      }
    }

    http.end();

    delay(5000);
               
                long double lat1 = gps.location.lat();    // MY latitude by gps module
                long double long1 = gps.location.lng();  // MY longitude by gps module
                    ThingSpeak.setField(2, static_cast<long>(gps.location.lat() * 1000000)); // Field 2: Latitude
                    ThingSpeak.setField(3, static_cast<long>(gps.location.lng() * 1000000)); // Field 3: Longitude

                for( int k=0;k<MAX_HOSPITALS;k++)
                {
                 Distance_arr[k] = distance_cal(lat1, long1, latitudes_arr[k], longitudes_arr[k]);
                }

                int n = MAX_HOSPITALS;
                double mini = INT_MAX;
                int index = 0;
                int m;
                int flag=0;
                for (m = 0; m < n; m++)
                {
                    if (Distance_arr[m] < mini)
                    {
                        mini = Distance_arr[m];
                        index = m;
                        flag=1;
                    }
                }
                Serial.print("The nearest hospital is:::"); // print the label
                Serial.println(names[index]);          // print the hospital name on a new line*/
                String emails[MAX_HOSPITALS]={"rahulshawyoyo@gmail.com","rahulshawpersonal@gmail.com","rahulshawyo@gmail.com","aahiri2001@gmail.com"};
                if(flag==1)
                {
                  String final_mail=emails[index];
                  Serial.println("Mail id in which mail is needed to be send");
                  Serial.println(final_mail);
                }
            }
        }

    if (millis() > 5000 && gps.charsProcessed() < 10)

    {

        Serial.println(F("No GPS detected: check wiring."));

        while (true)
            ;
    }
}
long double toRadians(const long double degree)
{
    long double one_deg = (M_PI) / 180;
    return (one_deg * degree);
}

long double distance_cal(long double lat1, long double long1,
                     long double lat2, long double long2)
{
    // Convert the latitudes and longitudes from degree to radians.
    lat1 = toRadians(lat1);
    long1 = toRadians(long1);
    lat2 = toRadians(lat2);
    long2 = toRadians(long2);

    // Haversine Formula
    long double dlong = long2 - long1;
    long double dlat = lat2 - lat1;

    long double ans = pow(sin(dlat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlong / 2), 2);
    ans = 2 * asin(sqrt(ans));

    // Radius of Earth in Kilometers, R = 6371
    // Use R = 3956 for miles
    long double R = 6371;

    // Calculate the result
    ans = ans * R;

    return ans;
}

void displayInfo()

{

    Serial.print(F("Location: "));

    if (gps.location.isValid())
    {

        Serial.print("Lat: ");

        Serial.print(gps.location.lat(), 6);

        Serial.print(F(","));
        Serial.print("Lng: ");

        Serial.print(gps.location.lng(), 6);

        
        Serial.println();
        delay(4000);
    }

    else

    {

        Serial.print(F("INVALID"));
        Serial.println();
    }
}

void updateSerial()

{

    delay(500);

    while (Serial.available())

    {

        Serial2.write(Serial.read()); // Forward what Serial received to Software Serial Port
    }

    while (Serial2.available())

    {

        Serial.write(Serial2.read()); // Forward what Software Serial received to Serial Port
    }
}
