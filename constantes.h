#ifndef CONSTANTES_H
#define CONSTANTES_H

#include "import.h"

#define SERVIDOR            1
#define BLUETOOTH           1
#define LORA                0
#define WIFI                0
#define RADIO               1
#define ZIGBEE              0


#define TAMANHO_PACOTE      10
#define NUMERO_PACOTE       1
#define CONTEUDO_ALEATORIO  2

#define CLIENTE_UUID        "a123650d-4191-4fcb-8ba7-e45b35884187"
#define SERVER_UUID         "059328e1-b9e6-4df6-9790-41dd3a9b0570"
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define RX_UUID             "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define TX_UUID             "28072946-2647-11ed-a261-0242ac120002"


// The remote service we wish to connect to.
static BLEUUID serviceUUID(SERVICE_UUID);
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID(RX_UUID);

#endif