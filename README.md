# BitArray

### Описание
Реализация класса динамического битового массива с переопределением операторов для работы с объектами класса и выполнения битовых операций (OR, AND, XOR).

---

## Сборка

1. **Клонирование репозитория:**
   ```bash
   git clone https://github.com/LevP21/BitArray.git
   ```

2. **Сборка с использованием CMake:**
   - **По умолчанию (с тестами):**
     ```bash
     cmake -S ./ -B ./build
     cmake --build build
     ```
     Создает статическую библиотеку и исполняемый файл для тестирования методов класса.

   - **Без тестов:**
     ```bash
     cmake -S ./ -B ./build -DENABLE_TESTS=OFF
     cmake --build build
     ```
     Создает только статическую библиотеку.

3. **Запуск тестов:**
   ```bash
   ./build/tests/bitarray_tests
   ```
   *(Для Windows: `bitarray_tests.exe`)*
