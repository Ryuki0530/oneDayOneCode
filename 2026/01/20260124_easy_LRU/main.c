#include <stdio.h>
#include <string.h>

#define MAXK 2000
#define MAXKEY 200001

typedef struct {
    int key, value;
    int prev, next;
} Node;

Node nodes[MAXK + 2];  
int key_to_node[MAXKEY];
int node_count;
int head, tail;
int capacity;

void init(int K) {
    capacity = K;
    node_count = 0;
    memset(key_to_node, -1, sizeof(key_to_node));
    
    head = node_count++;
    tail = node_count++;
    nodes[head].prev = -1;
    nodes[head].next = tail;
    nodes[tail].prev = head;
    nodes[tail].next = -1;
}

void remove_node(int idx) {
    int p = nodes[idx].prev;
    int n = nodes[idx].next;
    nodes[p].next = n;
    nodes[n].prev = p;
}

void insert_after_head(int idx) {
    int next_node = nodes[head].next;
    nodes[idx].prev = head;
    nodes[idx].next = next_node;
    nodes[head].next = idx;
    nodes[next_node].prev = idx;
}

void put(int key, int value) {
    if (key_to_node[key] != -1) {
        int idx = key_to_node[key];
        nodes[idx].value = value;
        remove_node(idx);
        insert_after_head(idx);
    } else {
        if (node_count - 2 >= capacity) {
            int lru_idx = nodes[tail].prev;
            remove_node(lru_idx);
            key_to_node[nodes[lru_idx].key] = -1;
        }
        
        int idx = node_count++;
        nodes[idx].key = key;
        nodes[idx].value = value;
        key_to_node[key] = idx;
        insert_after_head(idx);
    }
}

int get(int key) {
    if (key_to_node[key] == -1) {
        return -1;
    }
    
    int idx = key_to_node[key];
    int value = nodes[idx].value;
    remove_node(idx);
    insert_after_head(idx);
    return value;
}

int main() {
    int K, Q;
    scanf("%d %d", &K, &Q);
    
    init(K);
    
    for (int i = 0; i < Q; i++) {
        char cmd[10];
        scanf("%s", cmd);
        
        if (cmd[0] == 'p') {
            int key, value;
            scanf("%d %d", &key, &value);
            put(key, value);
        } else {
            int key;
            scanf("%d", &key);
            printf("%d\n", get(key));
        }
    }
    
    return 0;
}