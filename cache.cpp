#include "cache.h"
#include <sstream>
#include <iostream>

Cache::Cache() : head(nullptr), tail(nullptr), count(0) {
    table = new Node*[CACHE_SIZE]();
}

Cache::~Cache() {
    clear();  // 캐시 비우기
    delete[] table;  // 동적 할당된 메모리 해제
}

void Cache::add(std::string key, int value) {
    int index = hash(key);
    if (count >= CACHE_SIZE) {
        removeLast();
    }
    int* storedValue = new int(value);
    Node* newNode = new Node(key, static_cast<void*>(storedValue), NodeType::IntType, nullptr, nullptr);
    
    // 이중 연결 리스트에 추가
    newNode->next = head;
    if (head != nullptr) {
        head->prev = newNode;
    }
    head = newNode;
    if (tail == nullptr) {
        tail = newNode;
    }

    // 해시 테이블 업데이트
    newNode->nextInHash = table[index];
    table[index] = newNode;

    count++;
}

void Cache::add(std::string key, double value) {
    int index = hash(key);
    if (count >= CACHE_SIZE) {
        removeLast();
    }
    double* storedValue = new double(value);
    Node* newNode = new Node(key, static_cast<void*>(storedValue), NodeType::DoubleType, nullptr, nullptr);

    // 이중 연결 리스트에 추가
    newNode->next = head;
    if (head != nullptr) {
        head->prev = newNode;
    }
    head = newNode;
    if (tail == nullptr) {
        tail = newNode;
    }

    // 해시 테이블 업데이트
    newNode->nextInHash = table[index];
    table[index] = newNode;

    count++;
}

bool Cache::get(std::string key, int &value) {
    int index = hash(key);
    Node* current = table[index];
    while (current != nullptr) {
        if (current->key == key && current->type == NodeType::IntType) {
            value = *(static_cast<int*>(current->value));

            // 맨 앞으로 이동
            if (current != head) {
                if (current->prev) {
                    current->prev->next = current->next;
                }
                if (current->next) {
                    current->next->prev = current->prev;
                }
                if (current == tail) {
                    tail = current->prev;
                }

                current->next = head;
                current->prev = nullptr;
                head->prev = current;
                head = current;
            }

            return true;
        }
        current = current->nextInHash;
    }
    return false;
}

bool Cache::get(std::string key, double &value) {
    int index = hash(key);
    Node* current = table[index];
    while (current != nullptr) {
        if (current->key == key && current->type == NodeType::DoubleType) {
            value = *(static_cast<double*>(current->value));

            // 맨 앞으로 이동
            if (current != head) {
                if (current->prev) {
                    current->prev->next = current->next;
                }
                if (current->next) {
                    current->next->prev = current->prev;
                }
                if (current == tail) {
                    tail = current->prev;
                }

                current->next = head;
                current->prev = nullptr;
                head->prev = current;
                head = current;
            }

            return true;
        }
        current = current->nextInHash;
    }
    return false;
}

void Cache::removeLast() {
    if (tail == nullptr) return;

    int index = hash(tail->key);
    Node* current = table[index];
    Node* prev = nullptr;

    while (current != tail) {
        prev = current;
        current = current->nextInHash;
    }

    if (prev) {
        prev->nextInHash = current->nextInHash;
    } else {
        table[index] = current->nextInHash;
    }

    if (tail->prev) {
        tail->prev->next = nullptr;
    }
    delete tail;
    tail = tail->prev;
    if (tail == nullptr) {
        head = nullptr;
    }

    count--;
}

void Cache::clear() {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
    tail = nullptr;

    for (int i = 0; i < CACHE_SIZE; i++) {
        table[i] = nullptr;
    }
    count = 0;
}

std::string Cache::toString() {
    std::stringstream ss;
    Node* current = head;
    bool first = true;
    while (current != nullptr) {
        if (!first) {
            ss << " -> ";
        }
        if (current->type == NodeType::IntType) {
            ss << "[" << current->key << ": " << *(static_cast<int*>(current->value)) << "]";
        } else {
            ss << "[" << current->key << ": " << *(static_cast<double*>(current->value)) << "]";
        }
        first = false;
        current = current->next;
    }
    ss << std::endl;
    return ss.str();
}
