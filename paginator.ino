#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <esp32notifications.h>

#define LED_PIN 23
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
BLENotifications notifications;
char targetType[] = "com.apple.MobileSMS";  // Replace with type of your choosing

// This callback will be called when a Bluetooth LE connection is made or broken.
// You can update the ESP 32's UI or take other action here.
void onBLEStateChanged(BLENotifications::State state) {
  switch (state) {
    case BLENotifications::StateConnected:
      Serial.println("StateConnected - connected to a phone or tablet");
      break;
    case BLENotifications::StateDisconnected:
      Serial.println("StateDisconnected - disconnected from a phone or tablet");
      notifications.startAdvertising();
      break;
  }
}

// Setup a callback for when a notification arrives
void onNotificationArrived(const ArduinoNotification* notification, const Notification* rawNotificationData) {
  Serial.println("=== Got notification ===");
  Serial.println(notification->title);                                                       // The title, ie name of who sent the message
  Serial.println(notification->message);                                                     // The detail, ie "be home for dinner at 7".
  Serial.println(notification->type);                                                        // Which app sent it
  Serial.println(notifications.getNotificationCategoryDescription(notification->category));  // ie "social media"
  Serial.println(notification->categoryCount);                                               // How may other notifications are there from this app (ie badge number)
  Serial.println(F("---"));

  if (notification->type.equals(targetType)) {
    // Set message to display
    display.stopscroll();
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(notification->message);
    display.display();
    display.startscrolldiagright(0x0F, 0x0F);

    // Blink LED SOS
    blinkSOS();
    blinkSOS();
    blinkSOS();

    delay(10000);

    // Clear display
    display.stopscroll();
    display.clearDisplay();
    display.display();
  }
}

void blinkSOS() {
  blinkDot();
  blinkDot();
  blinkDot();
  delay(600);
  blinkDash();
  blinkDash();
  blinkDash();
  delay(600);
  blinkDot();
  blinkDot();
  blinkDot();
  delay(600);
}

void blinkDot() {
  digitalWrite(LED_PIN, HIGH);
  delay(200);
  digitalWrite(LED_PIN, LOW);
  delay(200);
}

void blinkDash() {
  digitalWrite(LED_PIN, HIGH);
  delay(600);
  digitalWrite(LED_PIN, LOW);
  delay(600);
}

void setup() {
  Serial.begin(115200);

  Serial.println(F("Starting up..."));

  // Set up OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;) {}
  }

  // Set up LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  // Set up display
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.display();

  delay(2000);

  // Initialize
  digitalWrite(LED_PIN, LOW);
  display.clearDisplay();
  display.display();

  Serial.println(F("Setting up notifications..."));

  // Start looking for a device connection
  notifications.begin("Paginator");

  // Register the callback to be informed when a notification arrives
  notifications.setConnectionStateChangedCallback(onBLEStateChanged);
  notifications.setNotificationCallback(onNotificationArrived);
}

void loop() {
}
