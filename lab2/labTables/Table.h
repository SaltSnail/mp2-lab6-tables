#pragma once
#include <exception>
#include <iostream>
#include <algorithm>
#include <list>
#include <string>
#include <vector>
#include <stack>

using namespace std;

template<class Key, class Data>
class Record {
public:
    Key key;
    Data data;

    bool operator ==(const Key&);
    bool operator !=(const Key&);
    friend ostream& operator<<(ostream& os, const Record<Key, Data>& d);

    Record(Key _key, Data _data) : key(_key), data(_data) {}
};

template<class Key, class Data>
inline bool Record<Key, Data>::operator!=(const Key& _key)
{
    return key != _key;
}

template<class Key, class Data>
inline bool Record<Key, Data>::operator==(const Key& _key)
{
    return key == _key;
}

template<class Key, class Data>
ostream& operator<<(ostream& os, const Record<Key, Data>& d)
{
    os << "key: " << d.key << " data: " << d.data <<endl;
    return os;
}

template<class Key, class Data>
class Table {
protected:
    int size;
    int count;
    Record<Key, Data>* dataArray;

public:
    Table(int _size = 10) : size(_size), count(0)
    {
        dataArray = new Record<Key, Data>[size];
    }

    void Add(Key key, Data data) {
        dataArray[count].key = key;
        dataArray[count].data = data;
        ++count;
    }

    Data Find(Key key) {
        for (int i = 0; i < count; ++i)
            if (dataArray[i].key == key)
                return dataArray[i].data;
        throw exception("Not found");
    }
    
    int getSize() const { return size; }
    int getCount() const { return count; }

    Record<Key, Data> getRecord(int i) const
    {
        return dataArray[i];
    }
};


template<class Key, class Data>
class SortTable : public Table<Key, Data >
{
public:
    SortTable(size_t _size);
    SortTable(const SortTable<Key, Data>& table);
    SortTable(const Table<Key, Data>& table); //done
    void Add(Key key, Data data);
    Data Find(Key key);
    void Delete(const Key& key);
    Data& operator[](const Key& key);
};

template<class Key, class Data>
SortTable<Key, Data>::SortTable(size_t _size) : Table<Key, Data>::Table(_size) { };

template<class Key, class Data>
SortTable<Key, Data>::SortTable(const SortTable<Key, Data>& table) {
    this->size = table.size;
    this->count = table.count;
    this->dataArray = new Record<Key, Data>[this->size];
    for (int i = 0; i < this->count; ++i)
    {
        this->dataArray[i] = table.dataArray[i];
    }
}

template<class Key, class Data>
SortTable<Key, Data>::SortTable(const Table<Key, Data>& table)
{
    this->size = table.getSize();
    this->count = table.getCount();
    this->dataArray = new Record<Key, Data>[this->size];
    for (int i = 0; i < this->count; i++)
        this->dataArray[i] = table.getRecord(i);

    for (int i = 0; i < this->count; i++)
        for (int j = 0; j < this->count; j++)
            if (this->dataArray[i].data > this->dataArray[j].data)
                swap(this->dataArray[i], this->dataArray[j]);
}

template<class Key, class Data>
void SortTable<Key, Data>::Add(Key key, Data data) {
    int i = 0;
    while (i < this->count)
    {
        if (this->dataArray[i].key <= key) continue;
        break;
    }
    for (int j = this->count; j > i; j--)
    {
        this->dataArray[j].key = this->dataArray[j - 1].key;
        this->dataArray[j].data = this->dataArray[j - 1].data;
    }
    this->dataArray[i].key = key;
    this->dataArray[i].data = data;
    ++this->count;
}

template<class Key, class Data>
Data SortTable<Key, Data>::Find(Key key) {
    int l = 0, r = this->count - 1;
    while (l + 1 < r)
    {
        int mid = (l + r) / 2;
        if (this->dataArray[mid].key > key)
            r = mid;
        else l = mid;
    }
    if (this->dataArray[l].key == key) return this->dataArray[l].data;
    if (this->dataArray[r].key == key) return this->dataArray[r].data;
    throw runtime_error("Not Found");
}

template<class Key, class Data>
Data& SortTable<Key, Data>::operator[](const Key& key) {
    int l = 0, r = this->count - 1;
    while (l + 1 < r)
    {
        int mid = (l + r) / 2;
        if (this->dataArray[mid].key > key)
            r = mid;
        else l = mid;
    }
    if (this->dataArray[l].key == key) return this->dataArray[l].data;
    if (this->dataArray[r].key == key) return this->dataArray[r].data;
    throw runtime_error("Not Found");
}

template<class Key, class Data>
void SortTable<Key, Data>::Delete(const Key& key)
{
    int i = 0;
    while (i < this->count)
    {
        if (this->dataArray[i++].key < key) continue;
        break;
    }
    for (i; i < this->count; i++)
    {
        this->dataArray[i].key = this->dataArray[i + 1].key;
        this->dataArray[i].data = this->dataArray[i + 1].data;
    }
}

//------------------------hashTable

template <class Key>
int HashFunc(Key key, int size)
{
    return key % size;
}

int HashFunc(string key, int size)
{
    int res = 0;
    for (int i = 0; i < key.size(); i++)
        res += key[i];
    return res % size;
}

template<class Key, class Data>
class HashTable
{
protected:
    vector<list<Record<Key, Data> > > dataVector;
    int size;
    int (*hashFncPointer)(Key, int);
    Data* lastElem = nullptr; 


public:
    HashTable(int Size = 10, int (*_hashFncPointer)(Key, int) = HashFunc<Key>) : size(Size)
    {
        hashFncPointer = _hashFncPointer;
        dataVector.resize(size);
    }

    void Add(Key key, Data data)
    {
        int index = hashFncPointer(key, size);
        dataVector[index].push_back(Record<Key, Data>(key, data));
    }

    Data* Find(Key key)
    {
        int i = 0;
        int index = hashFncPointer(key, size);
        typename list<Record<Key, Data> >::iterator it = dataVector[index].begin();

        while (i < dataVector[index].size() ) //поиск нужного ключа
        {
            if (it->key == key)
                return &it->data; //возврот указателя на данные
            i++;
        }
        return nullptr; //возврат нулевого указателя
    }

    void Delete(Key key)
    {
        int index = hashFncPointer(key, size);
        typename list<Record<Key, Data> >::iterator it = dataVector[index].begin();

        while (it->key != key)
            it++;
        dataVector[index].erase(it); // удаление элемента из массива
    }

    inline Data* Print()
    {
        /*for (int i = 0; i < dataVector.size(); i++)
        {
            Record<Key, Data>* tmp = this->dataVector + i;
            cout << i << "table:" << endl;
            
            cout << endl;
        }*/
        return lastElem;
    }

    int getSize() const { return size; }
};