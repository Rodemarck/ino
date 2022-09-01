#include "import.h"

class ClientCallback : public BLEClientCallbacks {
private:
    bool* conectado;
public:
    ClientCallback(bool* _conectado){
        this->conectado =_conectado;
    }
    void onConnect(BLEClient* pclient) {
        *this->conectado = true;
    }

    void onDisconnect(BLEClient* pclient) {
        *this->conectado = false;
        Serial.println("onDisconnect");
    }
};

class AdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
private:
    BLEAdvertisedDevice** dispositivo;
    bool* conetado;
    bool* escanear;
public:
    AdvertisedDeviceCallbacks(BLEAdvertisedDevice** _dispositivo,bool* _conetado,bool* _escanear){
        this->dispositivo = _dispositivo;
        this->conetado = _conetado;
        this->escanear = _escanear;
    }

    void onResult(BLEAdvertisedDevice advertisedDevice) {
        //advertisedDevice.getName()
        Serial.printf("BLE Advertised Dispositivo conectado: [%s]\n",advertisedDevice.getName().c_str());

        // We have found a device, let us now see if it contains the service we are looking for.
        if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
            BLEDevice::getScan()->stop();
            Serial.println("Finalmente achei esta merda....");
            (*this->dispositivo) = new BLEAdvertisedDevice(advertisedDevice);
            *this->conetado = true;
            *this->escanear = true;
        }
    }
};

class DipositivoClienteBLE{
private:
    std::string nome;
    bool conectado;
    bool reconectar;
    bool escanear;
    BLEScan* scan;
    BLERemoteCharacteristic* caracteristicasRemotas;
    BLEAdvertisedDevice* dispositivo;
    BLEClient*  cliente;
    BLERemoteService* servicoRemoto;
    void (*loop)(BLERemoteCharacteristic*);
    void (*err)(void);
    
public:
    bool ConectarAoServidor(){
        Serial.println("Vamos nos conectar a ");
        if(this->dispositivo == nullptr) {
            Serial.println("ESTOU SEM A PORRA DO DISPOSITIVO CONECTADO!\n\n\n\n\n");
            return false;
        }
        Serial.println(this->dispositivo->getAddress().toString().c_str());
        
        free(this->cliente);
        this->cliente = BLEDevice::createClient();

        this->cliente->setClientCallbacks(new ClientCallback(&this->conectado));

        this->cliente->connect(this->dispositivo);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
        Serial.println(" - Connected to server");

        // Obtain a reference to the service we are after in the remote BLE server.
        free(this->servicoRemoto);
        this->servicoRemoto = this->cliente->getService(serviceUUID);
        if (this->servicoRemoto == nullptr) {
            Serial.print("Failed to find our service UUID: ");
            Serial.println(serviceUUID.toString().c_str());
            this->cliente->disconnect();
            return false;
        }
        Serial.println(" - Found our service");


        // Obtain a reference to the characteristic in the service of the remote BLE server.
        free(this->caracteristicasRemotas);
        this->caracteristicasRemotas = this->servicoRemoto->getCharacteristic(charUUID);
        if (this->caracteristicasRemotas == nullptr) {
            Serial.print("Failed to find our characteristic UUID: ");
            Serial.println(charUUID.toString().c_str());
            this->cliente->disconnect();
            return false;
        }
        Serial.println(" - Found our characteristic");

        // Read the value of the characteristic.
        if(this->caracteristicasRemotas->canRead()) {
            std::string value = this->caracteristicasRemotas->readValue();
            Serial.print("The characteristic value was: ");
            Serial.println(value.c_str());
        }

        if(this->caracteristicasRemotas->canNotify())
            this->caracteristicasRemotas->registerForNotify(
                [](BLERemoteCharacteristic* pBLERemoteCharacteristic,
                uint8_t* pData,
                size_t length,
                bool isNotify){
                
                    Serial.print("Notify callback for characteristic ");
                    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
                    Serial.print(" of data length ");
                    Serial.println(length);
                    Serial.print("data: ");
                    Serial.println((char*)pData);
            });

        this->conectado = true;
        return true;
    }
    DipositivoClienteBLE(std::string _nome,bool _reconectar,  
            void (*loop)(BLERemoteCharacteristic*) = [](BLERemoteCharacteristic* brc){},
            void (*err)(void) = [](){}){
        this->nome = _nome;
        this->conectado = false;
        this->reconectar = _reconectar;
        this->dispositivo = nullptr;
        BLEDevice::init(_nome.c_str());

        this->scan = BLEDevice::getScan();
        this->scan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks(&this->dispositivo, &this->conectado, &this->escanear));
        this->scan->setInterval(1349);
        this->scan->setWindow(449);
        this->scan->setActiveScan(true);
        this->scan->start(5, false);
    }

    void run(){
        if (!this->conectado) {
            Serial.println("Desconectado...");
            if (this->ConectarAoServidor()) {
                Serial.println("Consegui reconectar.");
            } else {
                Serial.println("Não consergui reconectar");
            }
        }
        if (this->conectado) {
            Serial.println("Já conectado");
            std::string texto = "Time since boot: ";
            texto.append(String(millis()/1000).c_str());
            Serial.printf("Alterando as caracteristicas para um valor de [%s]", texto.c_str());
            
            this->caracteristicasRemotas->writeValue(texto.c_str(), texto.length());
        } else {
            BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
        }
    }
};