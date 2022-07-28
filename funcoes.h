#ifndef FUNCOES_H
#define FUNCOES_H

#include "pisca.ino"




// BlueTooh
void BlueToothServer(){
    if(!BT_available()) return
    String txt = "";
    while(BT_available())
        txt += BT_read();
    Serial.println(txt);
}
void BlueToothClient(){
    for(int np = 0; np < NUMERO_PACOTE; np++){
        for(int tp = 0; tp < TAMANHO_PACOTE; tp++){
            #if CONTEUDO_ALEATORIO == 1
                BT_write(VALOR);
            #else
                BT_write(random(256));
            #endif
        }
    }
}


// Lora
void LoraBegin(){
    if (!LoRa.begin(915E6)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
}
void LoraServer(){
    if(!LoRa.parsePacket()) return;
    String txt = "";
    while (LoRa.available()) 
        txt += (char)LoRa.read();
    Serial.println(txt);
}
void LoraClient(){
	for(int np = 0; np < NUMERO_PACOTE; np++){
        LoRa.beginPacket();
        for(int tp = 0; tp < TAMANHO_PACOTE; tp++){
            #if CONTEUDO_ALEATORIO == 1
                LoRa.print(VALOR);
            #else
                LoRa.print(random(256));
            #endif
        }
        LoRa.endPacket();
    }
}


// Sms
void SmsServer(){}
void SmsClient(){}

#endif