#ifndef CONSTANTES_H
#define CONSTANTES_H


#define SERVIDOR 1
#define BLUETOOTH 1
#define LORA 0
#define WIFI 0
#define ZIGBEE 0



#define TAMANHO_PACOTE 10
#define NUMERO_PACOTE 1
#define CONTEUDO_ALEATORIO 2

#if CONTEUDO_ALEATORIO == 1
    #define VALOR "String constante\n"
#endif

#define ESP 1

//conexões do bluetooth
#ifndef ESP
    #define BT_RX 2 
    #define BT_TX 3
#endif


#endif