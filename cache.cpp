#include "cache.h"
#include <sstream>
#include <iostream>

Cache::Cache() : count(0) {
    table = new Node*[CACHE_SIZE]();  // 캐시 크기만큼의 배열을 동적 할당
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
    Node* newNode = new Node(key, static_cast<void*>(storedValue), NodeType::IntType, nullptr);
    // 맨 앞에 노드를 추가하는 로직 수정
    if (table[index] != nullptr) {
        newNode->next = table[index];
    }
    table[index] = newNode;
    count++;
}

void Cache::add(std::string key, double value) {
    int index = hash(key);
    if (count >= CACHE_SIZE) {
        removeLast();
    }
    double* storedValue = new double(value);
    Node* newNode = new Node(key, static_cast<void*>(storedValue), NodeType::DoubleType, nullptr);
    // 맨 앞에 노드를 추가하는 로직 수정
    if (table[index] != nullptr) {
        newNode->next = table[index];
    }
    table[index] = newNode;
    count++;
}

// 항목을 찾았을 때 맨 앞으로 이동
bool Cache::get(std::string key, int &value) {
    int index = hash(key);
    Node* current = table[index];
    Node* prev = nullptr;
    while (current != nullptr) {
        if (current->key == key && current->type == NodeType::IntType) {
            value = *(static_cast<int*>(current->value));
            if (prev) {  // 노드가 맨 앞이 아닐 경우
                prev->next = current->next;  // 현재 노드를 리스트에서 분리
                current->next = table[index];  // 맨 앞에 현재 노드를 추가
                table[index] = current;
            }
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}
bool Cache::get(std::string key, double &value) {
    int index = hash(key);
    Node* current = table[index];
    Node* prev = nullptr;
    while (current != nullptr) {
        if (current->key == key && current->type == NodeType::DoubleType) {
            value = *(static_cast<double*>(current->value));
            if (prev) {  // 노드가 맨 앞이 아닐 경우
                prev->next = current->next;  // 현재 노드를 리스트에서 분리
                current->next = table[index];  // 맨 앞에 현재 노드를 추가
                table[index] = current;
            }
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}
void Cache::removeLast() {
    if (count == 0) return;
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (table[i] && !table[i]->next) {  // 하나의 노드만 있는 경우
            delete table[i];
            table[i] = nullptr;
            count--;
            return;
        }

        Node* current = table[i];
        Node* prev = nullptr;
        while (current && current->next) {
            prev = current;
            current = current->next;
        }
        if (current && prev) {
            prev->next = nullptr;
            delete current;
            count--;
            return;
        }
    }
}


void Cache::clear() {
    for (int i = 0; i < CACHE_SIZE; i++) {
        Node* current = table[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        table[i] = nullptr;
    }
    count = 0;
}

std::string Cache::toString() {
    std::stringstream ss;
    bool first = true;
    for (int i = 0; i < CACHE_SIZE; i++) {
        Node* current = table[i];
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
    }
    ss << std::endl;
    return ss.str();
}