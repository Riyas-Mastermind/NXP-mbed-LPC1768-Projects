#include "mbed.h"
#include "TextLCD.h"

// LCD connections: RS, E, D4, D5, D6, D7
TextLCD lcd(p15, p16, p17, p18, p19, p20);

// Ultrasonic sensor pins
DigitalOut trig(p6);
DigitalIn echo(p7);

// LEDs and Buzzer
DigitalOut redLED(p8);
DigitalOut greenLED(p9);
DigitalOut buzzer(p10);

// Timer for echo pulse measurement
Timer t;

// Function to measure distance safely (with timeout)
float measure_distance_cm() {
    trig = 0;
    wait_us(2);
    trig = 1;
    wait_us(10);
    trig = 0;

    // Wait for echo start (max 30ms)
    int timeout = 30000; // 30ms timeout
    while (!echo.read() && --timeout);
    if (timeout <= 0) return -1; // No echo start

    t.reset();
    t.start();

    // Wait for echo end (max 30ms)
    timeout = 30000;
    while (echo.read() && --timeout);
    t.stop();

    if (timeout <= 0) return -1; // No echo end

    // Distance in cm = time (µs) * 0.01715
    float distance = t.read_us() * 0.01715f;
    return distance;
}

int main() {
    // --- Welcome messages ---
    lcd.cls();
    lcd.printf("Hello Riyas :)");
    thread_sleep_for(2000);

    lcd.cls();
    lcd.printf("Hey LPC1768");
    lcd.locate(0, 1);
    lcd.printf("Project Demo-1");
    thread_sleep_for(3000);

    lcd.cls();
    lcd.printf("HC-SR04 Active");
    thread_sleep_for(1500);
    lcd.cls();

    // --- Main loop ---
    while (true) {
        float distance = measure_distance_cm();

        lcd.locate(0, 0);
        if (distance < 0) {
            lcd.printf("No Echo       ");
        } else {
            int dist_int = (int)distance;
            lcd.printf("Dist: %d cm   ", dist_int);

        }

        // Distance logic
        if (distance > 0 && distance < 10.0f) {
            redLED = 1;
            greenLED = 0;
            buzzer = 1;
        } else if (distance >= 10.0f && distance < 20.0f) {
            redLED = 1;
            greenLED = 0;
            buzzer = 0;
        } else if (distance >= 20.0f) {
            redLED = 0;
            greenLED = 1;
            buzzer = 0;
        } else {
            // No valid reading
            redLED = 0;
            greenLED = 0;
            buzzer = 0;
        }

        thread_sleep_for(500);
    }
}
