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
   x = key % capacity; // ���-�������
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
   cout << "��������� ������ �������" << endl;
   cout << key << " " << value << " " << name << "�� ���������� � �������" << endl;
}

void remove(hashTable *t[], int key)
{
   int x, y;
   x = key % capacity; //��������� ��������� �������
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
            cout << "������� ������" << endl;
            return;
         }
      }
      x = (x + i * y) % capacity;
   }
   cout << "������� �����������" << endl;
}

void find(hashTable *t[], int key)
{
   int x, y;
   x = key % capacity; //��������� ��������� �������
   y = 1 + key % (capacity - 2);
   for (int i = 1; i < capacity; i++)
   {
      if (t[x])
      {
         if (t[x]->key == key)
         {
            cout << "������� ������������" << endl;
            return;
         }
      }
      x = (x + i * y) % capacity;
   }
   cout << "������� �����������" << endl;
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
   for (int i = 0; i < capacity; i++) //��� ������ �����
      hTable[i] = NULL;
   ifstream in("work.txt");
   while (in >> a >> b >> s)
      insert(hTable, a, b, s);
   do
   {
      cout << "<1> - ����� ������� �� �����" << endl;
      cout << "<2> - ������� ������� �� �����" << endl;
      cout << "<3> - ������� �������� �������" << endl;
      cout << "<4> - �����" << endl;
      do
      {
         cout << "������� ����� �������): ";
         if ((cin >> n))
         {
            switch (n)
            {
            case CMD_FIND:
               cout << "������� ���� �������: ";
               if (cin >> fnd_key)
                  find(hTable, fnd_key);
               else
               {
                  cout << "������ �����" << endl;
                  return 0;
               }
               break;
            case CMD_POP:
               cout << "������� ���� �������: ";
               if (cin >> fnd_key)
                  remove(hTable, fnd_key);
               else
               {
                  cout << "������ �����" << endl;
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
      } while (!repeatFlag && !exitFlag);
   } while (!exitFlag);
   return 0;
}