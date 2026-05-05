#ifndef LINKED_LIST_H
#define LINKED_LIST_H

// Wlasna lista jednokierunkowa bez uzycia std::list
template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;
    int size;

public:
    LinkedList();
    ~LinkedList();

    void add(const T& value);
    T& get(int index);

    int getSize() const;
    bool isEmpty() const;
    void clear();
};

template <typename T>
LinkedList<T>::LinkedList() : head(nullptr), size(0) {}

template <typename T>
LinkedList<T>::~LinkedList() {
    clear();
}

// Dodanie elementu na koniec listy
template <typename T>
void LinkedList<T>::add(const T& value) {
    Node* newNode = new Node(value);

    if (!head) {
        head = newNode;
    } else {
        Node* current = head;

        while (current->next) {
            current = current->next;
        }

        current->next = newNode;
    }

    size++;
}

// Pobranie elementu przez przejscie po liscie
template <typename T>
T& LinkedList<T>::get(int index) {
    Node* current = head;

    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    return current->data;
}

template <typename T>
int LinkedList<T>::getSize() const {
    return size;
}

template <typename T>
bool LinkedList<T>::isEmpty() const {
    return size == 0;
}

// Usuniecie wszystkich elementow i zwolnienie pamieci
template <typename T>
void LinkedList<T>::clear() {
    Node* current = head;

    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }

    head = nullptr;
    size = 0;
}

#endif