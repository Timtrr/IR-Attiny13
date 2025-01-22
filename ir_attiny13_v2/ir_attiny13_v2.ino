#define IR_PIN PB4    // Пин ИК-приемника
#define LED_PIN PB3   // Пин управления ночником
#define DEBUG_PIN PB0 // Пин кнопки для записи команды

bool ledState = false;
uint32_t savedCommand = 0; // Переменная для сохранённой ИК-команды

// Функция для чтения ИК команды в формате NEC
uint32_t readNEC() {
  uint32_t command = 0;
  uint8_t bits = 0;

  // Ждём LOW на IR_PIN, пока сигнал не начнёт срабатывать
  while (digitalRead(IR_PIN) == HIGH);  // Ждем, пока сигнал не станет низким
  delayMicroseconds(9000);               // Ждем 9 мс HIGH

  // Переход на 4.5 мс LOW
  if (digitalRead(IR_PIN) == LOW) {
    delayMicroseconds(4500); // Ждем 4.5 мс LOW
  }

  // Чтение 32 бит команды
  for (bits = 0; bits < 32; bits++) {
    while (digitalRead(IR_PIN) == LOW);  // Ждем HIGH на IR_PIN
    delayMicroseconds(560);              // Начало бита

    command <<= 1;                       // Сдвигаем влево, чтобы освободить место для следующего бита

    // Если сигнал HIGH, то это логическая "1", иначе "0"
    if (digitalRead(IR_PIN) == HIGH) {
      command |= 1;                      // Записываем 1 в команду
      delayMicroseconds(1690);           // Задержка для логической "1"
    } else {
      delayMicroseconds(560);            // Задержка для логической "0"
    }
  }

  return command;
}


// Функция для мигания светодиодом несколько раз
void blinkLED(uint8_t times) {
  for (uint8_t i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH); // Включаем светодиод
    delay(500);
    digitalWrite(LED_PIN, LOW); // Выключаем светодиод
    delay(500);
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);   // Настройка LED_PIN как выход
  pinMode(IR_PIN, INPUT);     // Настройка IR_PIN как вход
  pinMode(DEBUG_PIN, INPUT_PULLUP); // Настройка DEBUG_PIN как вход с подтяжкой
  digitalWrite(LED_PIN, LOW); // Выключаем светодиод по умолчанию
}

void loop() {
  // Проверка состояния кнопки
  if (digitalRead(DEBUG_PIN) == LOW) { // Проверяем состояние кнопки (кнопка замкнута на землю)
    blinkLED(3); // Мигаем светодиодом, чтобы показать, что входим в режим записи
    // Ожидаем ИК сигнал и сохраняем его
    uint32_t command = readNEC();
    if (command != 0) {
      savedCommand = command; // Сохраняем команду
      blinkLED(6); // Мигаем светодиодом 3 раза, чтобы показать, что команда сохранена
    } else {
      blinkLED(2); // Мигаем светодиодом 3 раза, чтобы показать, что команда сохранена
    }

    // Защита от повторных срабатываний (задержка после записи команды)
    delay(2000); 
  } else {
    // Не в режиме записи, светодиод может переключаться в зависимости от команды
    uint32_t command = readNEC();
    if (command == savedCommand) {
      ledState = !ledState; // Переключаем состояние светодиода
      if (ledState) {
        digitalWrite(LED_PIN, HIGH);  // Включить
      } else {
        digitalWrite(LED_PIN, LOW);   // Выключить
      }
      delay(500); // Защита от повторных срабатываний
    }
  }
  
}
