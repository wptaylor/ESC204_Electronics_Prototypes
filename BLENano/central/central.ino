#include <ArduinoBLE.h>

bool can_read = true;


void BLE_initialize() {
  BLE.begin();
  Serial.println("BLE String Central Activated");
  BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
}

bool BLE_peripheral_namematche(BLEDevice peripheral) {
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();
    return peripheral.localName().equals("Antenna Cat");
}

bool BLE_connection_attempt(BLEDevice peripheral) {

    Serial.println("Connecting ...");
    if (peripheral.connect()) {
      Serial.println("Connected");
    } else {
      Serial.println("Failed to connect!");
      return false;
    }


    Serial.println("Discovering attributes ...");
    if (peripheral.discoverAttributes()) {
      Serial.println("Attributes discovered");
    } else {
      Serial.println("Attribute discovery failed!");
      peripheral.disconnect();
      return false;
    }


    BLECharacteristic letter = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");
    if (letter) {
      Serial.println("Letter characteristic found");
      return true;
    }
    Serial.println("Peripheral does not have letter characteristic!");
    peripheral.disconnect();
    return false;
}

void BLE_connected(BLECharacteristic letter) {
  if (can_read) {
    byte cur_letter = 0;
    letter.readValue(cur_letter);
    if (cur_letter == 0) {
      Serial.print("\n");
      Serial.println("Message fully recieved!");
    }
    else {
      Serial.print((char)cur_letter);
    }
    can_read = false;
    letter.readValue(cur_letter);
  }
}

void letter_written(BLEDevice peripheral, BLECharacteristic letter) {
  can_read = true;
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  BLE_initiaSlize();
}

void loop() {
  BLEDevice peripheral = BLE.available();
  if (peripheral) {
    if (!BLE_peripheral_namematche(peripheral)) {
      Serial.println("Not Antenna Cat");
      return;
    }

    BLE.stopScan();
    if (!BLE_connection_attempt(peripheral)) return;
    BLECharacteristic letter = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");
    if (!letter.canSubscribe()) {
      Serial.println("Can't subscribe");
      return;
    }
    letter.subscribe();
    letter.setEventHandler(BLEWritten, letter_written);
    while (peripheral.connected()) {
      BLE_connected(letter);
    }
    Serial.println("Peripheral disconnected");
    BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
  }
}
