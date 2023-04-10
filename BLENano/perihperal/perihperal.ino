#include <ArduinoBLE.h>

BLEService letterService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLECharCharacteristic letter("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEIndicate);

char* message = "This is a test message of arbitrary length.";
int message_idx = 0;
bool can_send = true;
bool message_sent = false;


void BLE_initialize() {
  
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy failed!");
  }

  BLE.setLocalName("Antenna Cat");

  BLE.setAdvertisedService(letterService);
  
  letterService.addCharacteristic(letter);
  BLE.addService(letterService);
  letter.setEventHandler(BLERead, letter_read);
  Serial.println("BLE String Peripheral, waiting for connections....");
  BLE.advertise();
}

void BLE_connected() {
  if (!message_sent && can_send) {
    char tosend = message[message_idx++];
    letter.writeValue(tosend);
    //Serial.print("Sent: ");
    //Serial.println(tosend);
    if (tosend == 0) {
      Serial.println("Message fully sent!");
      message_sent = true;
    }
    can_send = false;
  }
}

void letter_read(BLEDevice central, BLECharacteristic letter) {
  can_send = true;
}


void setup() {
  Serial.begin(115200);
  BLE_initialize();
}


void loop() {
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    while (central.connected()) {
      BLE_connected();
    }
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
    Serial.println("BLE String Peripheral, waiting for connections....");
    BLE.advertise();
  }
}