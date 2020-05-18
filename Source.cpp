#include <stdio.h>
#include <iostream>
#include <fstream>
#include <locale.h>

using namespace std;
enum Comands { CMD_FIND = 1, CMD_POP, CMD_PRINT, CMD_EXIT };
const unsigned char capacity = 13;

struct hashTable
{
   int value, key;
   string name;
   hashTable(int v, int k, string n) : value(v), key(k), name(n){}
};

void insert(hashTable *t[],int key, int value, string name)
{
   int x, y;
   x = key % capacity; // Хеш-функции
   y = 1 + key % (capacity - 2);
   for (int i = 1; i < capacity; i++)
   {
      if (t[x])
      {
         if (t[x]->key == key)
         {
            t[x]->value += value;
            return;
         }
      }
      else
      {
         t[x] = new hashTable(value, key, name);
         return;
      }
      x = (x + i * y) % capacity;
   }
   cout << "Увеличьте размер таблицы" << endl;
   cout << key << " " << value << " " << name << "Не поместился в таблицу" << endl;
}

void remove(hashTable *t[], int key)
{
   int x, y;
   x = key % capacity; //Исследуем возможные позиции
   y = 1 + key % (capacity - 2);
   for (int i = 1; i < capacity; i++)
   {
      if (t[x])
      {
         if (t[x]->key == key)
         {
            hashTable *dl_ptr = t[x];
            t[x] = NULL;
            delete dl_ptr;
            cout << "Элемент удален" << endl;
            return;
         }
      }
      x = (x + i * y) % capacity;
   }
   cout << "Элемент отсутствует" << endl;
}

void find(hashTable *t[], int key)
{
   int x, y;
   x = key % capacity; //Исследуем возможные позиции
   y = 1 + key % (capacity - 2);
   for (int i = 1; i < capacity; i++)
   {
      if (t[x])
      {
         if (t[x]->key == key)
         {
            cout << "Элемент присутствует" << endl;
            return;
         }
      }
      x = (x + i * y) % capacity;
   }
   cout << "Элемент отсутствует" << endl;
}

void print(hashTable *t[])
{
   for (int i = 0; i < capacity; i++)
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
   int a, b, fnd_key, n = 0;
   string s;
   bool exitFlag = false, repeatFlag = false;
   hashTable *hTable[capacity];
   for (int i = 0; i < capacity; i++) //Все ячейки пусты
      hTable[i] = NULL;
   ifstream in("work.txt");
   while (in >> a >> b >> s)
      insert(hTable, a, b, s);
   do
   {
      cout << "<1> - Найти элемент по шифру" << endl;
      cout << "<2> - Извлечь элемент по шифру" << endl;
      cout << "<3> - Вывести элементы таблицы" << endl;
      cout << "<4> - Выход" << endl;
      do
      {
         cout << "Введите номер команды): ";
         if ((cin >> n))
         {
            switch (n)
            {
            case CMD_FIND:
               cout << "Введите шифр изделия: ";
               if (cin >> fnd_key)
                  find(hTable, fnd_key);
               else
               {
                  cout << "Ошибка ввода" << endl;
                  return 0;
               }
               break;
            case CMD_POP:
               cout << "Введите шифр изделия: ";
               if (cin >> fnd_key)
                  remove(hTable, fnd_key);
               else
               {
                  cout << "Ошибка ввода" << endl;
                  return 0;
               }
               break;
            case CMD_PRINT:
               print(hTable);
               break;
            case CMD_EXIT:
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
      } while (!repeatFlag && !exitFlag);
   } while (!exitFlag);
   return 0;
}