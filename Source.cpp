#include <stdio.h>
#include <iostream>
#include <fstream>
#include <locale.h>

using namespace std;
/*
 * CMD_FIND - ������� ������ �������� �� �����
 * CMD_PRINT - ������� ������ ������� �� �����
 * CMD_SORT - ������� ���������� �� ���������� �������
 * CMD_EXIT - ������� ������ �� ���������
*/
enum Comands { CMD_FIND = 1, CMD_PRINT, CMD_SORT, CMD_EXIT };

const size_t capacity = 13; // ������ ������� ������ ���� ������� ������

struct hashTable
{
   size_t value, price; // ���������� ������� 
   string name, key; // ���������� ��� � 8-�� ���������� ����
   hashTable(string k = "", size_t p = 0, size_t v = 0, string n = "") : key(k), price(p), value(v), name(n){}
};

/**
 * ������� �������������� ����������� ����� � ������������� ������
 * @param key - 8-�� ���������� ������
 * ������������ �������� - ������� ������
*/
size_t getIndex(string key)
{
   size_t idx = 0;
   size_t strLen = key.length();
   // ��������� ���� �������� ������
   for (size_t i = 0; i < strLen; i++) 
      idx += key[i] > 0 ? key[i] : - key[i];
   return idx;
}

/**
 * ������� ������� �������� � ���-������� � ������
 * ����������� ��������� ���������� � ����� �������
 * @param hashTable - ��������� �� ���-�������
 * @param key - 8-�� ���������� ������
 * @param price - ����� �����, ���� �������
 */
void insert(hashTable *t[], string key, size_t price)
{
   if (key.length() == 8)
   {
      bool isNotInserted = true; // ���� ��������������� �������� �������
      // ����������� ���-�������, ������� ����������� �������� �����������
      size_t x = getIndex(key) % capacity, // �������������� ������ ������ - ��������� ���-�������
         y = 1 + getIndex(key) % (capacity - 2); // �������� ��� ������������� x, 0 < y < capacity
     // ����� ������ ��� �������, ����������� ������� x
      for (size_t i = 1; isNotInserted && i < capacity; i++)
      {
         // ��������, ��� ������ ������
         if (!t[x])
         {
            // ���� ����� ���������, �� ��������� �������
            t[x] = new hashTable(key, price);
            isNotInserted = false;
         }
         x = (x + i * y) % capacity; // ���� ������� �� ������� - ���������� x
      }
      if (isNotInserted)
         cout << "��������� ������ �������" << endl << key << " �� ���������� � �������" << endl;
   }
   else
      cout << key << " - ������������ ����, ��� ����� ������ ���� 8 ��������" << endl;
}

/**
 * ������������� ����� ��� ������� �������� � �������� �������
 * @param hashTable - ��������� �� ���-�������
 * @param key - 8-�� ���������� ������
 * @param price - ���� �������
 * @param value - ���������� �������
 * @param name - ���������� ������
 */
void insert(hashTable *t[], string key, size_t price, size_t value, string name)
{
   if (key.length() == 8)
   {
      bool isNotInserted = true; // ���� ��������������� �������� �������
      // ����������� ���-�������, ������� ����������� �������� �����������
      size_t x = getIndex(key) % capacity, // �������������� ������ ������ - ��������� ���-�������
         y = 1 + getIndex(key) % (capacity - 2); // �������� ��� ������������� x, 0 < y < capacity
     // ����� ������ ��� �������, ����������� ������� x
      for (size_t i = 1; isNotInserted && i < capacity; i++)
      {
         // ��������, ��� ������ ������
         if (t[x])
         {
            // ���� ����� ���������, �� ��������� ��������
            if (t[x]->key == key)
            {
               t[x]->value += value;
               isNotInserted = false;
            }
         }
         else // �������� ������� � ������
         {
            t[x] = new hashTable(key, price, value, name);
            isNotInserted = false;
         }
         x = (x + i * y) % capacity; // ���� ������� �� ������� - ���������� x
      }
      if (isNotInserted)
         cout << "��������� ������ �������" << endl << key << " " << value << " " << name << " �� ���������� � �������" << endl;
   }
   else
      cout << key << " - ������������ ����, ��� ����� ������ ���� 8 ��������" << endl;
}

/**
 * ������� ������ �������� � ���-�������,
 * �� ������� ������ ����� ���������� ���������������
 * @param hashTable - ��������� �� ���-�������
 * @param key - 8-�� ���������� ������
 * ������������ �������� - ��������� �� ��������� ������
 */
hashTable *find(hashTable *t[], string key)
{
   // ���� ���������� ������� ��������
   bool isNotFound = true;
   // ���������� ������� ���������� ��������
   size_t x = 0;
   for (size_t i = 0; isNotFound && i < capacity; i++)
      // �� ����� �� ������, ����� ���������� �����
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
 * ������� ������ ��������� � �������
 * @param hashTable - ��������� �� ���-�������
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
 * ������� ��������� ������������� ����� ������� ������
 * ���������� �� ��������, �� ����� � ������� ������������
 * @param keyF - ������ ������������ ����
 * @param keyL - ������ ������������ ����
 */
bool needToSort(string keyF, string keyL)
{
   // ���� ������������� ����������
   // ���������� ������� - �� ����������� ����� ��������
   bool exitFlag = false, needToS = false;
   /*��������� ������ ���������� �����������
     ���� - 8-�� ���������� ������
     ���� ������ ������� ������������� ������� ����������,
     �� ��� ������ �������� ��������� ������� 
    */
   if(keyF[0] >= keyL[0])
      for (size_t i = 0; !exitFlag && !needToS && i < 8; i++)
      {
         if (keyF[i] > keyL[i])
            needToS = true;
         // ���� ����� ������, ��� ������� ����������� ������� ����������
         else if (keyF[i] < keyL[i])
            exitFlag = true;
      }
   return needToS;
}

/**
 * ������� ���������� ������� �����
 * ������������ �� ������ ��������� �������,
 * � ����� � ��������� ����� step
 * @param hashTable - ��������� �� ���-�������
 * @param priceFlag - ���� �� ������ ����� �����������
 */
void shellsort(hashTable *t[], bool cipherSort)
{
   int step, i, j;
   // ���������� ��� ������������ �����
   hashTable *tmp;
   // ���������� �� �����
   if (cipherSort)
   {
      // ����� ����
      for (step = capacity / 2; step > 0; step /= 2)
         // ��������, ������� ����������� �� ����������� ����
         for (i = step; i < capacity; i++)
            // ������������ ��������� ������ ���������
            for (j = i - step; j >= 0; j -= step)
               // ���� j-� ������ ����� - ������������ ������
               if (!t[j] && t[j + step])
               {
                  t[j] = t[j + step];
                  t[j + step] = NULL;
               }
               // ��������� ������������� ������������
               else if (t[j + step] && needToSort(t[j]->key, t[j + step]->key))
               {
                  tmp = t[j];
                  t[j] = t[j + step];
                  t[j + step] = tmp;
               }
   }
   // ���������� �� ����������
   else
   {
      // ����� ����
      for (step = capacity / 2; step > 0; step /= 2)
         // ��������, ������� ����������� �� ����������� ����
         for (i = step; i < capacity; i++)
            // ������������ ��������� ������ ���������
            for (j = i - step; j >= 0; j -= step)
               // ���� j-� ������ ����� - ������������ ������
               if (!t[j] && t[j + step])
               {
                  t[j] = t[j + step];
                  t[j + step] = NULL;
               }
               // ��������� ������������� ������������
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
   // fileVal - ���������� �������
   // n�omand - ����� ������� ��������
   size_t fileVal, filePrice, n�omand = 0;
   // fileName - ��� �������
   // fileKey - ���� �������
   // fnd_key - ���������� ���������� �����
   string fileName, fileKey, fnd_key;
   bool exitFlag = false, repeatFlag = false;
   // �������� �������
   hashTable *hTable[capacity];
   // ������� �������
   hashTable *pTable[capacity];
   // ��� ���������� �������� �� ������� �����
   for (size_t i = 0; i < capacity; i++)
   {
      hTable[i] = NULL;
      pTable[i] = NULL;
   }
   // ���� ������� �������
   ifstream priceIn("pricelist.txt");
   // ���� �������� �������
   ifstream in("work.txt");
   // ��������� ������� � ������
   while (priceIn >> fileKey >> filePrice)
      insert(pTable, fileKey, filePrice);
   // ��������� ������� � ������ �� �����
   shellsort(pTable, true);
   // ��������� �������� �������
   // ���� � ������� ������� 
   // �� ��������� ������� �������� - �������� 0
   while (in >> fileKey >> fileVal >> fileName)
      insert(hTable, fileKey, find(pTable, fileKey) ? find(pTable, fileKey)->price : 0, fileVal,  fileName);
   // ���������������� ����
   do
   {
      cout << "<1> - ����� ������� �� �����" << endl
         << "<2> - ������� �������� �������" << endl
         << "<3> - ������������� ������� �� ����������" << endl
         << "<4> - �����" << endl;
      repeatFlag = false;
      do
      {
         cout << "������� ����� �������: ";
         if ((cin >> n�omand))
         {
            switch (n�omand)
            {
            case CMD_FIND: // ����� ��������
               cout << "������� ���� �������: ";
               if (cin >> fnd_key)
               {
                  cout << (find(hTable, fnd_key) ? "������� ������������" : "������� �����������") << endl;
               }
               else
               {
                  cout << "������ �����" << endl;
                  return 0;
               }
               break;
            case CMD_PRINT: // ����� ��������� 
               print(hTable);
               break;
              case CMD_SORT: // ���������� �� ����������
               shellsort(hTable, false);
               break;
            case CMD_EXIT: // ����� �� ���������
               exitFlag = true;
               break;
            default:
               cout << "������������ ����� ��������" << endl;
               repeatFlag = true;
               break;
            }
         }
         else
         {
            cout << "������ �����" << endl;
            return 0;
         }
      } while (!repeatFlag && !exitFlag); // ���� �� ��������� ������ ����� � ����� �� ���������
   } while (!exitFlag);
   return 0;
}