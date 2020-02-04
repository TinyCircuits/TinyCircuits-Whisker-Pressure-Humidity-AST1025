/*************************************************************************
 * Pressure & Humidity Wireling Tutorial: 
 * This program prints the temperature(C), altitude, pressure(hPa), and 
 * humidity(%) readings from the BME280 sensor. Results will be printed
 * to the Serial Monitor.
 * 
 * Hardware by: TinyCircuits
 * BME280 Library by: Adafruit
 * Code by: Laveréna Wienclaw for TinyCircuits
 * 
 * NOTE: Adafruit Sensor Library Modified: change device address from 0x77 
 *       to 0x76 to work with TinyCircuits hardware
 *
 * Initiated: 11/29/2017 
 * Updated: 01/10/2020
 ************************************************************************/

// This library is used for communication with I2C devices, such as the BME280 board
#include <Wire.h>
#include <Wireling.h>

// These libraries are used to interface with the BME280 Sensor
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"

// Global Sensor Variables 
#define SEALEVELPRESSURE_HPA (1013.25) // used to find approximate altitude 
Adafruit_BME280 bme; // I2C

// Used to control how often sensor values are updated in the main loop()
unsigned long delayTime = 1000;

// Make Serial Monitor compatible for all TinyCircuits processors
#if defined(ARDUINO_ARCH_AVR)
  #define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
  #define SerialMonitorInterface SerialUSB
#endif


void setup() {
  Wire.begin();
  SerialMonitorInterface.begin(9600); // Bandwidth for our communication
  // Print to Serial Monitor
  // You can pass flash-memory based strings to SerialMonitorInterface.print() by wrapping them with F(). 
  // This means you're using flash memory instead of RAM to print stuff
  SerialMonitorInterface.println(F("BME280 test"));

  // Enable and Power Wirelings
  Wireling.begin();
  Wireling.selectPort(0);

  // If the bme sensor is not found, throw statement and stop program
  // If you end up here, check to make sure your value in selectPort() is correct!
  if (!bme.begin()) {
    SerialMonitorInterface.println("Could not find a valid BME280 sensor, check wiring!"); // Printed to Serial Monitor
    while (1); // loop forever, because the rest of the program means nothing without the sensor
  }

}

// Forever looping the following logic
void loop() {
  printValues(); // Print to serial monitor
  delay(delayTime); // How often values are updated and printed
}

// This function prints out the values from the sensor to the Serial Monitor
void printValues() {
  SerialMonitorInterface.print("Temperature = ");
  SerialMonitorInterface.print(bme.readTemperature());
  SerialMonitorInterface.println("°C");

  SerialMonitorInterface.print("Pressure = ");
  SerialMonitorInterface.print(bme.readPressure() / 100.0F);
  SerialMonitorInterface.println("hPa");

  SerialMonitorInterface.print("Approx. Altitude = ");
  SerialMonitorInterface.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  SerialMonitorInterface.println("m");

  SerialMonitorInterface.print("Humidity = ");
  SerialMonitorInterface.print(bme.readHumidity());
  SerialMonitorInterface.println("%");

  SerialMonitorInterface.println();
}
