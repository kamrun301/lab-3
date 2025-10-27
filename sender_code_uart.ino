// ==== ESP32 UART Stress Test - Sender ====
#define TXD1 19
#define RXD1 21
HardwareSerial mySerial(1);

unsigned long msgInterval = 0;     // in milliseconds (0, 10, 100)
unsigned long lastSend = 0;
unsigned long testDuration = 30000; // 30 seconds test
unsigned long startTime;
int msgSize = 10;                  // in bytes (10, 50, 100)
long counter = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Change these manually for each test run
  int baud = 115200;  // (9600, 38400, 115200)
  msgSize = 100;     // (10, 50, 100)
  msgInterval = 100;  // (0, 10, 100)

  mySerial.begin(baud, SERIAL_8N1, RXD1, TXD1);

  Serial.println("ESP32 UART Sender - Stress Test");
  startTime = millis();
}

void loop() {
  if (millis() - startTime >= testDuration) {
    Serial.println("Test complete. Messages sent: " + String(counter));
    while (true); // stop
  }

  unsigned long now = millis();
  if (now - lastSend >= msgInterval) {
    lastSend = now;

    // Build a message of msgSize bytes
    String message = String(counter);
    while (message.length() < msgSize - 1) message += 'X';
    message += '\n';

    mySerial.print(message);
    counter++;
  }
}

