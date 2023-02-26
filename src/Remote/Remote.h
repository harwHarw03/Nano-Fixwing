#pragma once

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte r_address[6] = "00001";

int inputAileron;
int inputRudder;
int inputElevator;
int remoteThrottle; 

void remote_init(){
    radio.begin();
    radio.openReadingPipe(0, r_address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
}

void remote_update(){
    if(radio.available()){
        radio.read(&remoteThrottle, sizeof(remoteThrottle));
        radio.read(&inputAileron, sizeof(inputAileron));
        radio.read(&inputRudder, sizeof(inputRudder));
    }
}

// void setup() {
//   Serial.begin(9600);
//   radio.begin();
//   radio.openWritingPipe(address);
//   radio.setPALevel(RF24_PA_MIN);
//   radio.setDataRate(RF24_250KBPS);
// }

// void loop() {
//   const char* message = "Hello, world!";
//   radio.write(&message, sizeof(message));
//   Serial.println("Sent message: ");
//   Serial.println(message);
//   delay(1000);
// }