#include <avr/io.h>
#include <util/delay.h>

#define IR_PIN PB4    // Пин ИК-приемника
#define LED_PIN PB3   // Пин управления ночником

bool ledState = false;

uint32_t readNEC() {
  uint32_t command = 0;
  uint8_t bits = 0;

  // Ждём LOW
  while (bit_is_set(PINB, IR_PIN));
  _delay_us(9000);  // Ждём 9 мс HIGH
  if (bit_is_clear(PINB, IR_PIN)) return 0;  // Если нет HIGH, ошибка
  _delay_us(4500);  // Ждём 4.5 мс LOW

  // Чтение 32 бит
  for (bits = 0; bits < 32; bits++) {
    while (bit_is_clear(PINB, IR_PIN));  // Ждём HIGH
    _delay_us(560);  // Начало бита
    command <<= 1;
    if (bit_is_set(PINB, IR_PIN)) {
      command |= 1;
      _delay_us(1690);  // Длительность "1"
    } else {
      _delay_us(560);  // Длительность "0"
    }
  }
  return command;
}

void setup() {
  DDRB |= (1 << LED_PIN);  // Настройка LED_PIN как выход
  PORTB &= ~(1 << LED_PIN); // Выключить светодиоды по умолчанию
  DDRB &= ~(1 << IR_PIN);   // Настройка IR_PIN как вход
  PORTB |= (1 << IR_PIN);   // Включить подтяжку
}

void loop() {
  uint32_t command = readNEC();
  if (command == 0xFF50AF) {  // Проверьте правильность кода
    ledState = !ledState;
    if (ledState) {
      PORTB |= (1 << LED_PIN);  // Включить
    } else {
      PORTB &= ~(1 << LED_PIN); // Выключить
    }
  }
}
