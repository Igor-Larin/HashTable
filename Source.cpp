#include <stdio.h>
#include <iostream>
#include <fstream>
#include <locale.h>

using namespace std;
/*
 * CMD_FIND - команда поиска элемента по шифру
 * CMD_POP - команда удаления элемента по шифру
 * CMD_PRINT - команда вывода таблицы на экран
 * CMD_EXIT - команда выхода из программы
*/
enum Comands { CMD_FIND = 1, CMD_POP, CMD_PRINT, CMD_EXIT };
const unsigned int capacity = 13; // Размер таблицы должен быть простым числом

struct hashTable
{
   unsigned int value; // Количество изделия 
   string name, key; // Символьное имя и 8-ми символьный шифр
   hashTable(unsigned int v, string k, string n) : value(v), key(k), name(n){}
};

/**
 * Функция преобразования символьного шифра в целочисленный индекс
 * @param key - 8-ми символьная строка
 * Возвращаемое значение - готовый индекс
*/
unsigned int getIndex(string key)
{
   unsigned int idx = 0;
   // Суммируем коды символов строки
   for (size_t i = 0; i < key.length(); i++) {
      idx += key[i] > 0 ? key[i] : -1 * key[i];
   }
   return idx;
}

/**
 * Функция вставки элемента в хеш-таблицу
 * @param hashTable - указатель на хеш-таблицу
 * @param key - 8-ми символьная строка
 * @param name - символьная строка
 */
void insert(hashTable *t[], string key, unsigned int value, string name)
{
   if (key.length() == 8)
   {
      bool isNotInserted = true; // Флаг незавершенности операции вставки
      // Независимые хеш-функции, поэтому вероятность коллизий уменьшается
      unsigned int x = getIndex(key) % capacity, // Первоначальный индекс ячейки - результат хеш-функции
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
            t[x] = new hashTable(value, key, name);
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
 * Функция удаления элемента в хеш-таблицу,
 * рассматриваются индексы, которые были при вставке
 * @param hashTable - указатель на хеш-таблицу
 * @param key - 8-ми символьная строка
 */
void remove(hashTable *t[], string key)
{
   bool isNotRemoved = true; // Флаг незавершенности операции удаления
   int x = getIndex(key) % capacity, // Первоначальный индекс ячейки - результат хеш-функции
       y = 1 + getIndex(key) % (capacity - 2); // Значение для рехеширования x, 0 < y < capacity
   // Поиск происходит по наиболее возможным индексам ячеек
   for (size_t i = 1; isNotRemoved && i < capacity; i++)
   {
      // Проверка, что ячейка не пуста
      if (t[x])
         if (t[x]->key == key) 
         {
            hashTable *dl_ptr = t[x];
            t[x] = NULL;
            delete dl_ptr;
            cout << "Элемент удален" << endl;
            isNotRemoved = false;
         }
      x = (x + i * y) % capacity; // Исследуем следующие возможные позиции
   }
   // Если под всеми возможными ячейками не нашли элемент
   if(isNotRemoved)
      cout << "Элемент отсутствует" << endl;
}

/**
 * Функция поиска элемента в хеш-таблице,
 * рассматриваются индексы, которые были при вставке
 * @param hashTable - указатель на хеш-таблицу
 * @param key - 8-ми символьная строка
  * Возвращаемое значение - указатель на найденную ячейку
 */
hashTable *find(hashTable *t[], string key)
{
   int x = getIndex(key) % capacity, // Индекс ячейки
      y = 1 + getIndex(key) % (capacity - 2); // Значение для рехеширования x, 0 < y < capacity
   // Поиск происходит по наиболее возможным индексам ячеек
   for (size_t i = 1; i < capacity; i++)
   {
      // Проверка, что ячейка не пуста
      // Нельзя после невыполнения этого условия возвращать NULL
      if (t[x] && t[x]->key == key)
            return t[x];
      x = (x + i * y) % capacity;
   }
   // Если под всеми возможными ячейками не нашли элемент
   return NULL;
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
         cout << t[i]->key << " " << t[i]->value << " " << t[i]->name << endl;
      else
         cout << "NULL" << endl;
   }
}

int main()
{
   setlocale(LC_ALL, "");
   // fileVal - Количество изделия
   // nСomand - Номер команды операции
   unsigned int fileVal, nСomand = 0;
   // fileName - Имя изделия
   // fileKey - Шифр изделия
   // fnd_key - Переменная поискового ключа
   string fileName, fileKey, fnd_key;
   bool exitFlag = false, repeatFlag = false;
   hashTable *hTable[capacity];
   for (int i = 0; i < capacity; i++) 
      hTable[i] = NULL;
   ifstream in("work.txt");
   // Пока не прочтем весь файл
   while (in >> fileKey >> fileVal >> fileName)
      insert(hTable, fileKey, fileVal, fileName);
   // Пользовательское меню
   do
   {
      cout << "<1> - Найти элемент по шифру" << endl;
      cout << "<2> - Извлечь элемент по шифру" << endl;
      cout << "<3> - Вывести элементы таблицы" << endl;
      cout << "<4> - Выход" << endl;
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
            case CMD_POP: // Удаление элемента 
               cout << "Введите шифр изделия: ";
               if (cin >> fnd_key)
                  remove(hTable, fnd_key);
               else
               {
                  cout << "Ошибка ввода" << endl;
                  return 0;
               }
               break;
            case CMD_PRINT: // Вывод элементов 
               print(hTable);
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