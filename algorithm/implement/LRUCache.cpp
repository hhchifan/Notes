//least recently used 最近最少使用
#include <iostream>
#include <map>
using namespace std;

struct Node {
    int key , val;
    Node* pre;
    Node* nxt;
    Node() : key(0) , val(0) , pre(nullptr) , nxt(nullptr){}
    Node(int _key, int _val) : key(_key) , val(_val) , pre(nullptr) , nxt(nullptr){}
};

class LruCache {
    public:
    Node* head;
    Node* tail;
    int cap;
    int size;
    std::map<int, Node*> map;

    LruCache(int capacity): cap(capacity) , size(0) {
        head = new Node();
        tail = new Node();
        head->nxt = tail;
        tail->pre = head;
    }

    int get(int key){
        if(!map.count(key))
            return -1;
        Node* node = map[key];
        moveToHead(node);
        return node->val;
    }
    
    void put(int key, int value){
        if(map.count(key)){
            Node* node = map[key];
            node->val = value;
            moveToHead(node);
        }else {
            Node* node = new Node(key,value);
            map[key] = node;
            insertToHead(node);
            size++;
            if(size > cap){
                deleteOneNode();
                size--;
            }
        }
    }

    void moveToHead(Node* node) {
        node->pre->nxt = node->nxt;
        node->nxt->pre = node->pre;
        insertToHead(node);
    }

    void insertToHead(Node* node) {
        node->nxt = head->nxt;
        node->pre = head;
        head->nxt = node;
        node->nxt->pre = node;
    }

    void deleteOneNode() {
        Node* del = tail->pre;
        del->pre->nxt = tail;
        tail->pre = del->pre;
        map.erase(del->key);
        delete del;
    }
};

int main() {
    LruCache cache(3);
    cache.put(1,1);
    cache.put(2,2);
    cache.put(3,3);
    printf("get(3) = %d \n",cache.get(3));
    cache.put(4,4);
    printf("get(1) = %d \n",cache.get(1));
    printf("get(2) = %d \n",cache.get(2));
    cache.put(5,5);
    printf("get(3) = %d \n",cache.get(3));
    cache.put(5,6);
    printf("get(5) = %d \n",cache.get(5));
}