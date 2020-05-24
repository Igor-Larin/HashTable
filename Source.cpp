#include <stdio.h>
#include <iostream>
#include <fstream>
#include <locale.h>

using namespace std;
/*
 * CMD_FIND - ������� ������ �������� �� �����
 * CMD_POP - ������� �������� �������� �� �����
 * CMD_PRINT - ������� ������ ������� �� �����
 * CMD_EXIT - ������� ������ �� ���������
*/
enum Comands { CMD_FIND = 1, CMD_POP, CMD_PRINT, CMD_EXIT };
const unsigned int capacity = 13; // ������ ������� ������ ���� ������� ������

struct hashTable
{
   unsigned int value; // ���������� ������� 
   string name, key; // ���������� ��� � 8-�� ���������� ����
   hashTable(unsigned int v, string k, string n) : value(v), key(k), name(n){}
};

/**
 * ������� �������������� ����������� ����� � ������������� ������
 * @param key - 8-�� ���������� ������
 * ������������ �������� - ������� ������
*/
unsigned int getIndex(string key)
{
   unsigned int idx = 0;
   // ��������� ���� �������� ������
   for (size_t i = 0; i < key.length(); i++) {
      idx += key[i] > 0 ? key[i] : -1 * key[i];
   }
   return idx;
}

/**
 * ������� ������� �������� � ���-�������
 * @param hashTable - ��������� �� ���-�������
 * @param key - 8-�� ���������� ������
 * @param name - ���������� ������
 */
void insert(hashTable *t[], string key, unsigned int value, string name)
{
   if (key.length() == 8)
   {
      bool isNotInserted = true; // ���� ��������������� �������� �������
      // ����������� ���-�������, ������� ����������� �������� �����������
      unsigned int x = getIndex(key) % capacity, // �������������� ������ ������ - ��������� ���-�������
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
            t[x] = new hashTable(value, key, name);
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
 * ������� �������� �������� � ���-�������,
 * ��������������� �������, ������� ���� ��� �������
 * @param hashTable - ��������� �� ���-�������
 * @param key - 8-�� ���������� ������
 */
void remove(hashTable *t[], string key)
{
   bool isNotRemoved = true; // ���� ��������������� �������� ��������
   int x = getIndex(key) % capacity, // �������������� ������ ������ - ��������� ���-�������
       y = 1 + getIndex(key) % (capacity - 2); // �������� ��� ������������� x, 0 < y < capacity
   // ����� ���������� �� �������� ��������� �������� �����
   for (size_t i = 1; isNotRemoved && i < capacity; i++)
   {
      // ��������, ��� ������ �� �����
      if (t[x])
         if (t[x]->key == key) 
         {
            hashTable *dl_ptr = t[x];
            t[x] = NULL;
            delete dl_ptr;
            cout << "������� ������" << endl;
            isNotRemoved = false;
         }
      x = (x + i * y) % capacity; // ��������� ��������� ��������� �������
   }
   // ���� ��� ����� ���������� �������� �� ����� �������
   if(isNotRemoved)
      cout << "������� �����������" << endl;
}

/**
 * ������� ������ �������� � ���-�������,
 * ��������������� �������, ������� ���� ��� �������
 * @param hashTable - ��������� �� ���-�������
 * @param key - 8-�� ���������� ������
  * ������������ �������� - ��������� �� ��������� ������
 */
hashTable *find(hashTable *t[], string key)
{
   int x = getIndex(key) % capacity, // ������ ������
      y = 1 + getIndex(key) % (capacity - 2); // �������� ��� ������������� x, 0 < y < capacity
   // ����� ���������� �� �������� ��������� �������� �����
   for (size_t i = 1; i < capacity; i++)
   {
      // ��������, ��� ������ �� �����
      // ������ ����� ������������ ����� ������� ���������� NULL
      if (t[x] && t[x]->key == key)
            return t[x];
      x = (x + i * y) % capacity;
   }
   // ���� ��� ����� ���������� �������� �� ����� �������
   return NULL;
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
         cout << t[i]->key << " " << t[i]->value << " " << t[i]->name << endl;
      else
         cout << "NULL" << endl;
   }
}

int main()
{
   setlocale(LC_ALL, "");
   // fileVal - ���������� �������
   // n�omand - ����� ������� ��������
   unsigned int fileVal, n�omand = 0;
   // fileName - ��� �������
   // fileKey - ���� �������
   // fnd_key - ���������� ���������� �����
   string fileName, fileKey, fnd_key;
   bool exitFlag = false, repeatFlag = false;
   hashTable *hTable[capacity];
   for (int i = 0; i < capacity; i++) 
      hTable[i] = NULL;
   ifstream in("work.txt");
   // ���� �� ������� ���� ����
   while (in >> fileKey >> fileVal >> fileName)
      insert(hTable, fileKey, fileVal, fileName);
   // ���������������� ����
   do
   {
      cout << "<1> - ����� ������� �� �����" << endl;
      cout << "<2> - ������� ������� �� �����" << endl;
      cout << "<3> - ������� �������� �������" << endl;
      cout << "<4> - �����" << endl;
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
            case CMD_POP: // �������� �������� 
               cout << "������� ���� �������: ";
               if (cin >> fnd_key)
                  remove(hTable, fnd_key);
               else
               {
                  cout << "������ �����" << endl;
                  return 0;
               }
               break;
            case CMD_PRINT: // ����� ��������� 
               print(hTable);
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