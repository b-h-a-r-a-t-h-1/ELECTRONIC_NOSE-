#include <Arduino.h>

#define MQ2_PIN 34       // MQ-2 sensor pin
#define MQ135_PIN 35     // MQ-135 sensor pin

#define MAX_HEAP_SIZE 10

// Alert structure
struct Alert {
  int priority;
  char message[100];
};

// Heap and size
Alert heap[MAX_HEAP_SIZE];
int heapSize = 0;

// Swap utility
void swap(Alert* a, Alert* b) {
  Alert temp = *a;
  *a = *b;
  *b = temp;
}

// Heapify Up
void heapifyUp(int index) {
  int parent = (index - 1) / 2;
  while (index > 0 && heap[index].priority > heap[parent].priority) {
    swap(&heap[index], &heap[parent]);
    index = parent;
  }
}

// Heapify Down
void heapifyDown(int index) {
  int largest = index;
  int left = 2 * index + 1;
  int right = 2 * index + 2;

  if (left < heapSize && heap[left].priority > heap[largest].priority)
    largest = left;
  if (right < heapSize && heap[right].priority > heap[largest].priority)
    largest = right;

  if (largest != index) {
    swap(&heap[index], &heap[largest]);
    heapifyDown(largest);
  }
}

// Insert alert into heap
void insertAlert(int priority, const char* message) {
  if (heapSize >= MAX_HEAP_SIZE) {
    Serial.println("Heap full. Cannot insert new alert.");
    return;
  }

  Alert newAlert;
  newAlert.priority = priority;
  strncpy(newAlert.message, message, sizeof(newAlert.message) - 1);
  newAlert.message[sizeof(newAlert.message) - 1] = '\0';

  heap[heapSize] = newAlert;
  heapifyUp(heapSize);
  heapSize++;
}

// Extract top-priority alert
Alert extractMax() {
  Alert topAlert;
  if (heapSize == 0) {
    strcpy(topAlert.message, "No alerts");
    topAlert.priority = 0;
    return topAlert;
  }

  topAlert = heap[0];
  heap[0] = heap[heapSize - 1];
  heapSize--;
  heapifyDown(0);

  return topAlert;
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  int mq2_val = analogRead(MQ2_PIN);
  int mq135_val = analogRead(MQ135_PIN);

  Serial.print("MQ2: "); Serial.print(mq2_val);
  Serial.print(" | MQ135: "); Serial.println(mq135_val);

  // Insert alert based on gas levels
  if (mq2_val > 1500 && mq135_val > 1500) {
    insertAlert(4, "CRITICAL: Flammable & toxic gas leak detected!");
  } else if (mq2_val > 1500) {
    insertAlert(3, "ALERT: Flammable gas (LPG/Smoke) detected!");
  } else if (mq135_val > 1500) {
    insertAlert(2, "WARNING: Toxic gas (CO2/Ammonia) detected!");
  } else {
    insertAlert(1, "Status: Gas levels normal.");
  }

  // Send highest-priority alert
  if (heapSize > 0) {
    Alert alertToSend = extractMax();
    Serial.print("Sending Alert (Priority ");
    Serial.print(alertToSend.priority);
    Serial.print("): ");
    Serial.println(alertToSend.message);
  } else {
    Serial.println("No alert to send.");
  }

  delay(3000); // Wait 3 seconds before next read
}
