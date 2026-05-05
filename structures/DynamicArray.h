#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H


template <typename T>
class DynamicArray {
private:
    T* data;       // tablica z danymi
    int size;      // liczba elementów
    int capacity;  // rozmiar zaalokowanej pamięci

    void resize();

    public:
        DynamicArray();
        ~DynamicArray();

        void add(const T& value);
        T& get(int index);
        const T& get(int index) const;

        int getSize() const;
        bool isEmpty() const;
        void clear();
    };

    template <typename T>
    DynamicArray<T>::DynamicArray() : data(nullptr), size(0), capacity(0) {}

    template <typename T>
    DynamicArray<T>::~DynamicArray() {
        delete[] data;
    }

    // Powieksza tablice, gdy brakuje miejsca
    template <typename T>
    void DynamicArray<T>::resize() {
        int newCapacity = (capacity == 0) ? 2 : capacity * 2;
        T* newData = new T[newCapacity];

        // Kopiowanie starych elementow do nowej tablicy
        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

    // Dodanie elementu na koniec tablicy
    template <typename T>
    void DynamicArray<T>::add(const T& value) {
        if (size == capacity) {
            resize();
        }

        data[size] = value;
        size++;
    }

    template <typename T>
    T& DynamicArray<T>::get(int index) {
        return data[index];
    }

    template <typename T>
    const T& DynamicArray<T>::get(int index) const {
        return data[index];
    }

    template <typename T>
    int DynamicArray<T>::getSize() const {
        return size;
    }

    template <typename T>
    bool DynamicArray<T>::isEmpty() const {
        return size == 0;
    }

    // Zwolnienie pamieci i wyczyszczenie tablicy
    template <typename T>
    void DynamicArray<T>::clear() {
        delete[] data;
        data = nullptr;
        size = 0;
        capacity = 0;
    }

#endif