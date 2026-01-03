#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXQ 200005
#define HASH_SIZE 524287  // ハッシュテーブル用の大きな素数

// タスク情報構造体
typedef struct {
    int id;       // タスクID
    int t;        // 実行時刻
    int p;        // 優先度
    int version;  // バージョン（更新回数）
} Task;

// ヒープに格納するノード
typedef struct {
    int id;       // タスクID
    int t;        // 実行時刻
    int p;        // 優先度
    int version;  // バージョン
} HeapNode;

// タスクの状態を管理する辞書エントリ
typedef struct {
    int id;       // タスクID
    int t;        // 実行時刻
    int p;        // 優先度
    int version;  // バージョン
    int alive;    // 0: 無効, 1: 有効
} DictEntry;

HeapNode heap[MAXQ]; // ヒープ配列
int heap_size = 0;   // ヒープのサイズ

// id→DictEntryのハッシュテーブル（オープンアドレッシング）
DictEntry dict[HASH_SIZE];

// ハッシュ関数
unsigned int hash(int id) {
    return ((unsigned int)id) % HASH_SIZE;
}

// idに対応する辞書エントリを取得（なければcreate=1で新規作成）
DictEntry* dict_get(int id, int create) {
    unsigned int h = hash(id);
    while (1) {
        // 空きスロットかつidが一致しない場合
        if (dict[h].alive == 0 && dict[h].id != id) {
            if (create) {
                dict[h].id = id;
                dict[h].version = 0;
                dict[h].alive = 0;
                dict[h].t = 0;
                dict[h].p = 0;
                return &dict[h];
            } else {
                return NULL;
            }
        }
        // idが一致した場合
        if (dict[h].id == id) {
            return &dict[h];
        }
        // 衝突時は次のスロットへ
        h = (h + 1) % HASH_SIZE;
    }
}

// ヒープの比較関数（優先度降順、時刻昇順、ID昇順）
int heap_cmp(HeapNode *a, HeapNode *b) {
    if (a->p != b->p) return b->p - a->p;
    if (a->t != b->t) return a->t - b->t;
    return a->id - b->id;
}

// ヒープの要素を交換
void heap_swap(int i, int j) {
    HeapNode tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

// ヒープにノードを追加
void heap_push(HeapNode node) {
    int i = heap_size++;
    heap[i] = node;
    // 上に向かってヒープ条件を満たすまで交換
    while (i > 0) {
        int p = (i - 1) / 2;
        if (heap_cmp(&heap[p], &heap[i]) <= 0) break;
        heap_swap(p, i);
        i = p;
    }
}

// ヒープの先頭（最小値）を削除
void heap_pop() {
    heap[0] = heap[--heap_size];
    int i = 0;
    // 下に向かってヒープ条件を満たすまで交換
    while (1) {
        int l = i * 2 + 1, r = i * 2 + 2, smallest = i;
        if (l < heap_size && heap_cmp(&heap[l], &heap[smallest]) < 0) smallest = l;
        if (r < heap_size && heap_cmp(&heap[r], &heap[smallest]) < 0) smallest = r;
        if (smallest == i) break;
        heap_swap(i, smallest);
        i = smallest;
    }
}

// ヒープの先頭要素を取得
HeapNode heap_top() {
    return heap[0];
}

// ヒープが空か判定
int heap_empty() {
    return heap_size == 0;
}

int main() {
    int Q;
    scanf("%d", &Q);

    char cmd[16];
    for (int q = 0; q < Q; ++q) {
        scanf("%s", cmd);
        if (strcmp(cmd, "ADD") == 0) {
            // タスク追加
            int id, t, p;
            scanf("%d%d%d", &id, &t, &p);
            DictEntry *entry = dict_get(id, 1); // 辞書に登録
            entry->t = t;
            entry->p = p;
            entry->version += 1; // バージョン更新
            entry->alive = 1;    // 有効化
            HeapNode node = {id, t, p, entry->version};
            heap_push(node);     // ヒープに追加

        } else if (strcmp(cmd, "CANCEL") == 0) {
            // タスクキャンセル
            int id;
            scanf("%d", &id);
            DictEntry *entry = dict_get(id, 0);
            if (entry && entry->alive) {
                entry->alive = 0; // 無効化
            }
            
        } else if (strcmp(cmd, "RUN") == 0) {
            // タスク実行
            int now;
            scanf("%d", &now);
            int found = 0;
            while (!heap_empty()) {
                HeapNode node = heap_top();
                DictEntry *entry = dict_get(node.id, 0);
                // 最新かつ有効なタスクか確認
                if (!entry || entry->version != node.version || !entry->alive || entry->t != node.t || entry->p != node.p) {
                    heap_pop(); // 無効なノードは削除
                    continue;
                }
                if (node.t > now) break; // 実行時刻未到達
                // 実行可能なタスク
                printf("%d\n", node.id);
                entry->alive = 0; // 実行済みにする
                heap_pop();
                found = 1;
                break;
            }
            if (!found) {
                printf("-1\n"); // 実行可能なタスクなし
            }
        }
    }
    return 0;
}