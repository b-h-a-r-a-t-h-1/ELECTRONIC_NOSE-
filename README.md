                                     **Gas Sensor-Based Alert Management System Using Heap**__
1. Objective
This Arduino program monitors air quality using two gas sensors — MQ-2 and MQ-135 — to detect flammable and toxic gases. It classifies alerts by severity and manages them using a max-heap data structure to always prioritize the most critical notifications.

2. Sensors Used
🧪 MQ-2 Gas Sensor
Detects flammable gases such as LPG, smoke, methane, and hydrogen.

Analog output corresponds to gas concentration.

Connected to analog pin A2 (GPIO 34).

🧪 MQ-135 Gas Sensor
Detects toxic gases such as CO2, NH3, benzene, and alcohol.

Provides analog readings relative to air quality.

Connected to analog pin A3 (GPIO 35).

3. Key Functional Features
✅ Real-time Gas Monitoring:
The loop() function periodically reads analog values from both gas sensors.

Based on thresholds (>1500), it classifies the gas level condition into four categories:

Priority 4: Both gases critical — flammable & toxic gases detected.

Priority 3: Only flammable gases are high.

Priority 2: Only toxic gases are high.

Priority 1: Normal condition.

✅ Alert Management via Max-Heap:
Alerts are encapsulated in a struct Alert containing:

int priority — a value from 1 to 4.

char message[100] — a human-readable description.

Alerts are inserted into a max-heap, ensuring the most severe alert is always at the top.

The heap supports:

insertAlert() — inserts an alert while maintaining heap order.

extractMax() — retrieves and removes the highest-priority alert.

✅ Controlled Output:
After inserting an alert, the highest-priority alert is immediately retrieved and printed via Serial output to represent a "send" action.

✅ Error Handling:
If the heap reaches its maximum size (10), new alerts are ignored and a warning is printed.

4. Data Structure: Max-Heap
The max-heap is implemented as a fixed-size array (heap[MAX_HEAP_SIZE]) with the following properties:

Maintains ordering such that the parent node always has a greater or equal priority than its children.

Supports heapify-up and heapify-down operations to maintain max-heap structure after insertion and extraction.

Efficiently prioritizes urgent alerts in O(log n) time.

5. Technical Specifications
Board: ESP32 or Arduino-compatible (supports analog pins 34 & 35).

Language: C++ using Arduino framework.

Serial Baud Rate: 115200 bps.

Polling Delay: 3000 ms between sensor reads.

6. Applications
Smart home safety systems (gas leakage detection).

Industrial gas monitoring.

Real-time hazard alerting with prioritization logic.

