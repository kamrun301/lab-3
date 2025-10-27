// ==== ESP32 UART Stress Test - Receiver ====
#define TXD1 19
#define RXD1 21
HardwareSerial mySerial(2);

unsigned long startTime;
unsigned long testDuration = 30000; // 30 seconds
long received = 0;
long valid = 0;
int lastCounter = -1;

void setup() {
  Serial.begin(115200);
  delay(1000);

  int baud = 115200;  // must match sender
  mySerial.begin(baud, SERIAL_8N1, RXD1, TXD1);

  Serial.println("ESP32 UART Receiver - Stress Test");
  startTime = millis();
}

void loop() {
  if (millis() - startTime >= testDuration) {
    float duration = testDuration / 1000.0;
    
    float throughput = (float)(valid * 100) / duration; // approximate payload size
    float msgRate = valid / duration;
    float errorRate = 100.0 * (received - valid) / received;

    Serial.println("\n--- Test Summary ---");
    Serial.printf("Received: %ld | Valid: %ld\n", received, valid);
    Serial.printf("Throughput: %.2f B/s | Msg Rate: %.2f msg/s | Error: %.2f%%\n",
                  throughput, msgRate, errorRate);

    while (true); // stop
  }

  if (mySerial.available()) {
    String msg = mySerial.readStringUntil('\n');
    received++;
    
    // check validity (simple numeric check)
    if (msg.toInt() == lastCounter + 1 || lastCounter == -1) {
      valid++;
      lastCounter = msg.toInt();
    } else {
      // missing or corrupt message
      Serial.println("Invalid or skipped msg: " + msg);
    }
  }
}

