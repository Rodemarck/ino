#ifndef PISCA_INO
#define PISCA_INO

#include <string.h>

//bluetooth
#ifdef ESP
	//código para modulo esp nativo
	#include "BluetoothSerial.h"
	#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
		#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
	#endif

	BluetoothSerial SerialBT;
	
	#define BT_begin() SerialBT.begin("MODULO-BT IOT!")
	#define BT_available() SerialBT.available()
	#define BT_read() SerialBT.read()
	#define BT_write(x) SerialBT.write(x)
#else
	//#include <SoftwareSerial.h>

	SoftwareSerial hc06(BT_RX, BT_TX);

	#define BT_available() hc06.available()
	#define BT_write(x) SerialBT.write(x)
	#define BT_read() SerialBT.read()
	#define BT_begin(x); hc06.begin(9600);\
		{char *txt;\
			sprintf(txt,"AT+NAME%s",x);\
			while(1){\
				if(BT_available()){\
					BT_write(txt);\
					break;\
				}\
				else delay(1);\
			}\
		}
#endif

#include <SPI.h>
//#include <LoRa.h>



#include "constantes.h"
#include "funcoes.h"

#include <stdlib.h>
#include<time.h>



void setup() {
	randomSeed(analogRead(5))
	BT_begin("MODULO-BT IOT!");
	if (!LoRa.begin(915E6)) {
		Serial.println("Starting LoRa failed!");
		while (1);
	}
}
void loop() {
	#if SERVIDOR == 1
		#if BLUETOOTH == 1
			BlueToothServer();
		#endif
		#if LORA == 1
			LoraServer();
		#endif
	#else
		#if BLUETOOTH == 1
			BlueToothClient();
		#endif
		#if LORA == 1
			LoraClient();
		#endif
	#endif
}

#endif