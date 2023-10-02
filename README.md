# -Heart-Attack-Detection-and-SOS-Mail-Alert-System-to-Nearest-Hospital-for-Automobiles
 a complete device with BPM sensor to Detect possible cardiac arrest or heart failure condition of driver with continuous monitoring of important health related issues in real time.
# ESP32 Heart Rate and Hospital Locator and alert sender to the hospital for assitance 

## Description
This project is designed to run on an ESP32 microcontroller and uses various libraries to measure heart rate, obtain GPS coordinates, and find nearby hospitals. It sends heart rate data and GPS coordinates to ThingSpeak and uses the Google Places API to find hospitals in proximity, and send an SOS mail alert to the nearest hospital .

## Prerequisites
- Arduino IDE (version X.X or later)
- ESP32 board support package installed in Arduino IDE
- Required libraries: TinyGPSPlus, ESP_Mail_Client, WiFi, HTTPClient, ArduinoJson, ThingSpeak
( libraries can be installed by library manager in arduino Ide sketch-> Include library -> Manage libraries )

## Hardware Setup
1. Connect a heart rate sensor to the A0 pin on your ESP32.
2. Ensure your ESP32 is connected to a Wi-Fi network.
3. Connect a GPS module to the appropriate serial pins (e.g., Serial2).
4. Adjust any additional hardware connections if necessary.
5. A battery is needed to be connect with positive terminal of GPS module and negative terminal of gps module.
(I have included the circuit diagram and detailed working of the project section in the data folder of the project)

## Software Setup
1. Open the Arduino IDE and install the required libraries via the Library Manager.
2. Configure your Wi-Fi credentials and other settings in the code.
3. Replace `apiKey_thingspeak` with your ThingSpeak API key.
4. Replace the Google Maps API key with your own.

## Usage
1. Upload the code to your ESP32 using the Arduino IDE.
2. Open the Serial Monitor to view heart rate, GPS data, and nearby hospitals.
3. The ESP32 will periodically send heart rate and GPS data to ThingSpeak.
4. If the heart rate exceeds 100, it will also search for nearby hospitals using the Google Places API.
5. The nearest hospital's name and email (if available) will be displayed.
6. Open your THingspeak account to see the live tracking of heartbeat and latitude and longitude of the person.
7. By IFTTT/Thingspeak,the code will send the details (lat ans long) of the patient(driver) to the nearest hospital so that the hsopital can send a medical assistance to the patient as soon as possible.


## Contributing
Contributions to this project are welcome! Please follow our [contribution guidelines](CONTRIBUTING.md) to get started.

## License
This project is licensed under the [MIT License](LICENSE).

## Version History
- 1.0.0 (2023-10-02): Initial release.
