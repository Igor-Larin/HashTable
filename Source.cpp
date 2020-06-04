#include <stdio.h>
#include <iostream>
#include <fstream>
#include <locale.h>

using namespace std;
/*
 * CMD_FIND - команда поиска элемента по шифру
 * CMD_PRINT - команда вывода таблицы на экран
 * CMD_SORT - команда сортировки по количеству изделий
 * CMD_EXIT - команда выхода из программы
*/
enum Comands { CMD_FIND = 1, CMD_PRINT, CMD_SORT, CMD_EXIT };

const size_t capacity = 13; // Размер таблицы должен быть простым числом

struct hashTable
{
   size_t value, price; // Количество изделия 
   string name, key; // Символьное имя и 8-ми символьный шифр
   hashTable(string k = "", size_t p = 0, size_t v = 0, string n = "") : key(k), price(p), value(v), name(n){}
};

/**
 * Функция преобразования символьного шифра в целочисленный индекс
 * @param key - 8-ми символьная строка
 * Возвращаемое значение - готовый индекс
*/
size_t getIndex(string key)
{
   size_t idx = 0;
   size_t strLen = key.length();
   // Суммируем коды символов строки
   for (size_t i = 0; i < strLen; i++) 
      idx += key[i] > 0 ? key[i] : - key[i];
   return idx;
}

/**
 * Функция вставки элемента в хеш-таблицу с ценами
 * Отсутствуют параметры количества и имени изделия
 * @param hashTable - указатель на хеш-таблицу
 * @param key - 8-ми символьная строка
 * @param price - целое число, цена изделия
 */
void insert(hashTable *t[], string key, size_t price)
{
   if (key.length() == 8)
   {
      bool isNotInserted = true; // Флаг незавершенности операции вставки
      // Независимые хеш-функции, поэтому вероятность коллизий уменьшается
      size_t x = getIndex(key) % capacity, // Первоначальный индекс ячейки - результат хеш-функции
         y = 1 + getIndex(key) % (capacity - 2); // Значение для рехеширования x, 0 < y < capacity
     // Поиск ячейки для вставки, исследуются индексы x
      for (size_t i = 1; isNotInserted && i < capacity; i++)
      {
         // Проверка, что ячейка занята
         if (!t[x])
         {
            // Если ключи совпадают, то суммируем значени
            t[x] = new hashTable(key, price);
            isNotInserted = false;
         }
         x = (x + i * y) % capacity; // Если вставка не удалась - рехешируем x
      }
      if (isNotInserted)
         cout << "Увеличьте размер таблицы" << endl << key << " не поместился в таблицу" << endl;
   }
   else
      cout << key << " - некорректный шифр, его длина должна быть 8 символов" << endl;
}

/**
 * Перегруженный метод для вставки элемента в основную таблицу
 * @param hashTable - указатель на хеш-таблицу
 * @param key - 8-ми символьная строка
 * @param price - цена изделия
 * @param value - количество изделия
 * @param name - символьная строка
 */
void insert(hashTable *t[], string key, size_t price, size_t value, string name)
{
   if (key.length() == 8)
   {
      bool isNotInserted = true; // Флаг незавершенности операции вставки
      // Независимые хеш-функции, поэтому вероятность коллизий уменьшается
      size_t x = getIndex(key) % capacity, // Первоначальный индекс ячейки - результат хеш-функции
         y = 1 + getIndex(key) % (capacity - 2); // Значение для рехеширования x, 0 < y < capacity
     // Поиск ячейки для вставки, исследуются индексы x
      for (size_t i = 1; isNotInserted && i < capacity; i++)
      {
         // Проверка, что ячейка занята
         if (t[x])
         {
            // Если ключи совпадают, то суммируем значения
            if (t[x]->key == key)
            {
               t[x]->value += value;
               isNotInserted = false;
            }
         }
         else // Помещаем элемент в ячейку
         {
            t[x] = new hashTable(key, price, value, name);
            isNotInserted = false;
         }
         x = (x + i * y) % capacity; // Если вставка не удалась - рехешируем x
      }
      if (isNotInserted)
         cout << "Увеличьте размер таблицы" << endl << key << " " << value << " " << name << " не поместился в таблицу" << endl;
   }
   else
      cout << key << " - некорректный шифр, его длина должна быть 8 символов" << endl;
}

/**
 * Функция поиска элемента в хеш-таблице,
 * по условию задачи поиск происходит последовательно
 * @param hashTable - указатель на хеш-таблицу
 * @param key - 8-ми символьная строка
 * Возвращаемое значение - указатель на найденную ячейку
 */
hashTable *find(hashTable *t[], string key)
{
   // Флаг нахождения нужного элемента
   bool isNotFound = true;
   // Переменная позиции найденного элемента
   size_t x = 0;
   for (size_t i = 0; isNotFound && i < capacity; i++)
      // Не пуста ли ячейка, тогда сравниваем ключи
      if (t[i] && key == t[i]->key)
      {
         isNotFound = false;
         x = i;
      }
   if (isNotFound)
      return NULL;
   else
      return t[x];
}

/**
 * Функция вывода элементов в консоль
 * @param hashTable - указатель на хеш-таблицу
 */
void print(hashTable *t[])
{
   for (size_t i = 0; i < capacity; i++)
   {
      if (t[i])
         cout << t[i]->key << " " << t[i]->value << " " << t[i]->name << " " << t[i]->price << endl;
      else
         cout << "NULL" << endl;
   }
}

/**
 * Функция проверяет необходимость смены позиций ключей
 * Сортировка по алфавиту, но цифры и символы приоритетнее
 * @param keyF - первый сравниваемый ключ
 * @param keyL - второй сравниваемый ключ
 */
bool needToSort(string keyF, string keyL)
{
   // Флаг необходимости сортировки
   // Правильная позиция - по возрастанию кодов символов
   bool exitFlag = false, needToS = false;
   /*Сравнение ключей происходит посимвольно
     Ключ - 8-ми символьная строка
     Если первые символы удовлетворяют условию сортировки,
     то нет смысла смотреть остальные символы 
    */
   if(keyF[0] >= keyL[0])
      for (size_t i = 0; !exitFlag && !needToS && i < 8; i++)
      {
         if (keyF[i] > keyL[i])
            needToS = true;
         // Если нашли символ, при котором выполняется условие сортировки
         else if (keyF[i] < keyL[i])
            exitFlag = true;
      }
   return needToS;
}

/**
 * Функция сортировки методом Шелла
 * Сравниваются не только ближайшие позиции,
 * а также с некоторым шагом step
 * @param hashTable - указатель на хеш-таблицу
 * @param priceFlag - флаг по какому ключу сортировать
 */
void shellsort(hashTable *t[], bool cipherSort)
{
   int step, i, j;
   // Переменная для перестановки ячеек
   hashTable *tmp;
   // Сортировка по шифру
   if (cipherSort)
   {
      // Выбор шага
      for (step = capacity / 2; step > 0; step /= 2)
         // Элементы, которые сортируются на определённом шаге
         for (i = step; i < capacity; i++)
            // Перестановка элементов внутри подсписка
            for (j = i - step; j >= 0; j -= step)
               // Если j-я ячейка пуста - переставляем ячейки
               if (!t[j] && t[j + step])
               {
                  t[j] = t[j + step];
                  t[j + step] = NULL;
               }
               // Проверяем необходимость перестановки
               else if (t[j + step] && needToSort(t[j]->key, t[j + step]->key))
               {
                  tmp = t[j];
                  t[j] = t[j + step];
                  t[j + step] = tmp;
               }
   }
   // Сортировка по количеству
   else
   {
      // Выбор шага
      for (step = capacity / 2; step > 0; step /= 2)
         // Элементы, которые сортируются на определённом шаге
         for (i = step; i < capacity; i++)
            // Перестановка элементов внутри подсписка
            for (j = i - step; j >= 0; j -= step)
               // Если j-я ячейка пуста - переставляем ячейки
               if (!t[j] && t[j + step])
               {
                  t[j] = t[j + step];
                  t[j + step] = NULL;
               }
               // Проверяем необходимость перестановки
               else if (t[j + step] && t[j]->value > t[j + step]->value)
               {
                  tmp = t[j];
                  t[j] = t[j + step];
                  t[j + step] = tmp;
               }
   }
}

int main()
{
   setlocale(LC_ALL, "");
   // fileVal - Количество изделия
   // nСomand - Номер команды операции
   size_t fileVal, filePrice, nСomand = 0;
   // fileName - Имя изделия
   // fileKey - Шифр изделия
   // fnd_key - Переменная поискового ключа
   string fileName, fileKey, fnd_key;
   bool exitFlag = false, repeatFlag = false;
   // Основная таблица
   hashTable *hTable[capacity];
   // Ценовая таблица
   hashTable *pTable[capacity];
   // Для дальнейшей проверки на пустоту ячеек
   for (size_t i = 0; i < capacity; i++)
   {
      hTable[i] = NULL;
      pTable[i] = NULL;
   }
   // Файл ценовой таблицы
   ifstream priceIn("pricelist.txt");
   // Файл основной таблицы
   ifstream in("work.txt");
   // Заполняем таблицу с ценами
   while (priceIn >> fileKey >> filePrice)
      insert(pTable, fileKey, filePrice);
   // Сортируем таблицу с ценами по шифру
   shellsort(pTable, true);
   // Заполняем основную таблицу
   // Если в ценовой таблице 
   // не оказалось нужного элемента - записать 0
   while (in >> fileKey >> fileVal >> fileName)
      insert(hTable, fileKey, find(pTable, fileKey) ? find(pTable, fileKey)->price : 0, fileVal,  fileName);
   // Пользовательское меню
   do
   {
      cout << "<1> - Найти элемент по шифру" << endl
         << "<2> - Вывести элементы таблицы" << endl
         << "<3> - Отсортировать таблицу по количеству" << endl
         << "<4> - Выход" << endl;
      repeatFlag = false;
      do
      {
         cout << "Введите номер команды: ";
         if ((cin >> nСomand))
         {
            switch (nСomand)
            {
            case CMD_FIND: // Поиск элемента
               cout << "Введите шифр изделия: ";
               if (cin >> fnd_key)
               {
                  cout << (find(hTable, fnd_key) ? "Элемент присутствует" : "Элемент отсутствует") << endl;
               }
               else
               {
                  cout << "Ошибка ввода" << endl;
                  return 0;
               }
               break;
            case CMD_PRINT: // Вывод элементов 
               print(hTable);
               break;
              case CMD_SORT: // Сортировка по количеству
               shellsort(hTable, false);
               break;
            case CMD_EXIT: // Выход из программы
               exitFlag = true;
               break;
            default:
               cout << "Неправильный номер комманды" << endl;
               repeatFlag = true;
               break;
            }
         }
         else
         {
            cout << "Ошибка ввода" << endl;
            return 0;
         }
      } while (!repeatFlag && !exitFlag); // Пока не произошла ошибка ввода и выход из программы
   } while (!exitFlag);
   return 0;
}