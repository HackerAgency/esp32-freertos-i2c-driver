#include <Arduino.h>
#include <Wire.h>

using namespace std;

// C++ Драйвер I2C для датчика MPU6050
class I2CSensor {
private:
    uint8_t m_address;

public:
    explicit I2CSensor(uint8_t address) : m_address(address) {}

    void init() { 
        Wire.begin(); 
    }

    uint8_t readRegister(uint8_t reg) {
        Wire.beginTransmission(m_address);
        Wire.write(reg);
        Wire.endTransmission();
        Wire.requestFrom(m_address, (uint8_t)1);
        return Wire.read();
    }

    int16_t readTemperatureRaw() {
        uint8_t highByte = readRegister(0x41);
        uint8_t lowByte = readRegister(0x42);
        return (highByte << 8) | lowByte; // Побитовая склейка 16-бит
    }

    float tempC() {
        return (readTemperatureRaw() / 340.0) + 36.53;
    }
};

static I2CSensor mpu6050(0x68);
static QueueHandle_t tempQueue;

// ПОТОК 1: Опрос датчика
void sensorTask(void* pvParameters) {
    while (true) {
        float currentTemp = mpu6050.tempC();
        xQueueSend(tempQueue, &currentTemp, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// ПОТОК 2: Обработка и вывод данных из очереди
void displayTask(void* pvParameters) {
    float receivedTemp = 0.0;
    while (true) {
        if (xQueueReceive(tempQueue, &receivedTemp, portMAX_DELAY)) {
            Serial.print("[FreeRTOS Queue] Temp: ");
            Serial.print(receivedTemp);
            Serial.println(" °C");
        }
    }
}

void setup() {
    Serial.begin(115200);
    mpu6050.init();

    // Очередь на 5 элементов типа float
    tempQueue = xQueueCreate(5, sizeof(float));

    // Регистрация потоков в FreeRTOS
    xTaskCreate(sensorTask,  "SensorTask",  2048, NULL, 1, NULL);
    xTaskCreate(displayTask, "DisplayTask", 2048, NULL, 1, NULL);
}

void loop() {
    // Вся логика выполняется параллельно в задачах FreeRTOS
}
