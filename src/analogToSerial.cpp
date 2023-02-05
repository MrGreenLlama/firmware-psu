#include <Arduino.h>

#define HISTORY_LENGTH 5
#define INPUT12V A0
#define INPUT5V A2
#define INPUT3V3 A1

/**
 * This function analogReads a pin and parses it's value into a float
 * @arg
 * Pin is of course the pin on the arduino, just like a regular analogRead. \n
 * Scaling can be used to get the actual voltage after it's been reduced by a voltage divider. \n
 * Scaling defaults to 1.
 * @example
 * 1 = 100% scale → 0..1023 = 0V..5V \n
 * 10 = 1000% scale → 0..1023 = 0V..50V
 */
float analogReadAndParse(int pin, int scaling = 1) {
    uint16_t analogValues[HISTORY_LENGTH] = {};
    for (unsigned int &value: analogValues) {
        value = analogRead(pin);
        delayMicroseconds(100);
    }

    float calculateAverage = 0.0f;
    for (unsigned int value: analogValues) calculateAverage += value;
    return ((calculateAverage / HISTORY_LENGTH) * (scaling)) * (5.0f / 1023.0f);
}


void setup() {
    Serial.begin(9600);
}


void loop() {
    String text = Serial.readString();

    if (text.length() > 0) {
        // print values in TOML format
        Serial.println("\n[average_voltages]");
        if (text.indexOf("rs_read_voltage_12V") > -1 || text.indexOf("rs_read_voltage_all") > -1) {
            Serial.print("12V = ");
            Serial.println(analogReadAndParse(INPUT12V, 10));
        }
        if (text.indexOf("rs_read_voltage_5V") > -1 || text.indexOf("rs_read_voltage_all") > -1) {
            Serial.print("5V = ");
            Serial.println(analogReadAndParse(INPUT5V));
        }
        if (text.indexOf("rs_read_voltage_3V3") > -1 || text.indexOf("rs_read_voltage_all") > -1) {
            Serial.print("3V3 = ");
            Serial.println(analogReadAndParse(INPUT3V3));
        }
        // TODO ...Continue for current, power, temperature, etc
    }
}