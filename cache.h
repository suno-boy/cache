#ifndef CACHE_H
#define CACHE_H
#include <string>

#define CACHE_SIZE 10  // 캐시의 최대 용량을 정의

// 노드가 저장할 수 있는 데이터 타입을 나타내는 열거형
enum class NodeType { IntType, DoubleType };

class Cache {
private:
    struct Node {
        std::string key;
        void* value;
        NodeType type;  // 노드의 타입을 저장
        Node* next;  // 다음 노드를 가리키는 포인터
        Node* prev;  // 이전 노드를 가리키는 포인터
        Node* nextInHash;  // 해시 테이블에서 다음 노드를 가리키는 포인터

        // 노드 생성자 업데이트: 이중 연결 리스트 지원
        Node(std::string k, void* v, NodeType t, Node* n = nullptr, Node* p = nullptr, Node* nh = nullptr)
            : key(k), value(v), type(t), next(n), prev(p), nextInHash(nh) {}
    };

    Node** table;  // Node 포인터의 배열로 구성된 해시 테이블 선언
    Node* head;  // 이중 연결 리스트의 헤드
    Node* tail;  // 이중 연결 리스트의 테일
    int count;  // 현재 저장된 노드의 수

    void removeLast();  // 리스트에서 가장 오래된 노드를 제거하는 함수
    void clear();  // 전체 리스트를 비우는 함수
    int hash(std::string key) {  // 해시 함수
        int sum = 0;
        for (char ch : key) {
            sum += ch;
        }
        return sum % 10;
    }

public:
    Cache();  // 생성자
    ~Cache();  // 소멸자

    void add(std::string key, int value);  // 캐시에 int 값을 추가하는 함수
    void add(std::string key, double value);  // 캐시에 double 값을 추가하는 함수
    bool get(std::string key, int &value);  // key에 해당하는 int 값의 존재 유무와 값을 반환하는 함수
    bool get(std::string key, double &value);  // key에 해당하는 double 값의 존재 유무와 값을 반환하는 함수

    std::string toString();  // 캐시의 내용을 문자열로 반환하는 함수 (디버깅 목적)
};

#endif
