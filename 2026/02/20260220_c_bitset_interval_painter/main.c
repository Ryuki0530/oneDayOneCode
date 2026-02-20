#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 200005

typedef struct {
    long long sum;
    int lazy;
} Node;

Node tree[4 * MAX_N];
int N, Q;

void push(int node, int l, int r) {
    if (tree[node].lazy == -1) return;
    
    tree[node].sum = (long long)(r - l);
    if (l + 1 < r) {
        tree[2 * node].lazy = tree[node].lazy;
        tree[2 * node + 1].lazy = tree[node].lazy;
    }
    tree[node].lazy = -1;
}

void update(int node, int l, int r, int ql, int qr, int val) {
    push(node, l, r);
    
    if (qr <= l || r <= ql) return;
    
    if (ql <= l && r <= qr) {
        tree[node].lazy = val;
        push(node, l, r);
        return;
    }
    
    int mid = (l + r) / 2;
    update(2 * node, l, mid, ql, qr, val);
    update(2 * node + 1, mid, r, ql, qr, val);
    
    push(2 * node, l, mid);
    push(2 * node + 1, mid, r);
    tree[node].sum = tree[2 * node].sum + tree[2 * node + 1].sum;
}

long long query(int node, int l, int r, int ql, int qr) {
    push(node, l, r);
    
    if (qr <= l || r <= ql) return 0;
    
    if (ql <= l && r <= qr) {
        return tree[node].sum;
    }
    
    int mid = (l + r) / 2;
    long long left_sum = query(2 * node, l, mid, ql, qr);
    long long right_sum = query(2 * node + 1, mid, r, ql, qr);
    
    return left_sum + right_sum;
}

int main() {
    scanf("%d %d", &N, &Q);
    
    for (int i = 0; i < 4 * MAX_N; i++) {
        tree[i].sum = 0;
        tree[i].lazy = -1;
    }
    
    for (int i = 0; i < Q; i++) {
        char op[10];
        int l, r;
        scanf("%s %d %d", op, &l, &r);
        
        if (op[0] == 'P') {
            update(1, 0, N, l, r, 1);
        } else {
            printf("%lld\n", query(1, 0, N, l, r));
        }
    }
    
    return 0;
}