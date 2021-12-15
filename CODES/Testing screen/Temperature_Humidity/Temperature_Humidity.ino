/* How to use the DHT-22 sensor with Arduino uno
   Temperature and humidity sensor
*/

//Libraries
#include <DHT.h>;

//Constants
#define DHTPIN A7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino


//Variables
int chk;
float hum;  //Stores humidity value
float tempC; //Stores temperature value
float tempF; //Stores temperature value

void setup()
{
  pinMode(A7, INPUT);
  Serial.begin(9600);
  dht.begin();
}

void loop()
{
  //Serial.print("\xC2\xB0"); degree symbol
    delay(2000);
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    tempC = dht.readTemperature();
    tempF = tempC*1.8 + 3.2;
    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(tempC);
    Serial.print("\xC2\xB0"); Serial.print("Celsius ");
    Serial.print(tempF);
    Serial.print("\xC2\xB0"); Serial.println(" Fahrenheit");
    delay(10000); //Delay 2 sec.
}

   
