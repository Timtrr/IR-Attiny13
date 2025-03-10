# при тестирование выявлены баги через раз принимаются команды - НЕ ИСПРАЛЕНО 
# используется microcore https://github.com/MCUdude/MicroCore
## V1: Базовая версия без возможности записи команд.
## V2: Добавлен режим записи команды, но без сохранения в энергонезависимую память.
## V3: Поддерживает сохранение команды в EEPROM, что делает управление удобнее и надёжнее при использовании.

### Описание каждой версии:
***
### V1
**Функционал:**
Управляет ночником (светодиодом) с помощью ИК-пульта.
Приёмник работает по протоколу NEC.
Код проверяет, совпадает ли полученная команда с заданным значением (0xFF50AF). Если совпадает, то переключает состояние светодиода (включение/выключение).
Без режима записи команды.  
**Особенности:**
Поддерживает базовую работу ИК-приёмника.
Простая реализация: только переключение состояния светодиода.
***
### V2
**Функционал:**
Добавлен режим записи ИК-команды.
Для записи команды используется кнопка на отдельном пине (DEBUG_PIN).
После нажатия кнопки:
Светодиод мигает 3 раза, показывая, что устройство перешло в режим записи.
Сохраняет считанную команду в переменную savedCommand.
Подтверждает сохранение команды миганием светодиода 6 раз.
После записи команда используется для управления светодиодом: переключение состояния по совпадению с записанной командой.  
**Особенности:**
Возможность записи новой команды без изменения прошивки.
Используется только оперативная память для хранения команды (теряется при перезапуске).
***
### V3
**Функционал:**
Расширена функциональность записи команды: добавлена поддержка сохранения команды в EEPROM.
Сохранённая команда восстанавливается из EEPROM при перезапуске устройства.
Режим записи команды аналогичен V2, но после записи команда сохраняется в EEPROM.
При запуске устройства:
Если команда найдена в EEPROM, устройство мигает светодиодом 1 раз.
Если команда не найдена, светодиод мигает 8 раз.
Управление светодиодом по записанной команде, как в предыдущих версиях.  
**Особенности:**
Постоянное хранение команды в EEPROM.
Поддержка работы без необходимости повторной записи команды после перезагрузки.
Индикация состояния через мигание светодиода.
