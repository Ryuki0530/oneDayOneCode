#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUF_SIZE 1024
#define INPUT_FILE "students.txt"
#define OUTPUT_FILE "sorted_students.txt"



typedef struct Node {
    int id;
    char name[100];
    int score;
    struct Node* next;
} Node;

Node *quick_sort_by_score(Node *head);

int main() {
    FILE *input_file = fopen(INPUT_FILE, "r");
    if(input_file == NULL){
        printf("ERROR: Could not open %s\n", INPUT_FILE);
        perror("fopen");
        return 1;
    }
    
    // 線形リストの初期化
    Node *students = NULL, *students_tail = NULL;
    
    char buf[BUF_SIZE];
    while(fgets(buf, BUF_SIZE, input_file) != NULL) {
        // 新しいノードを作成。メモリを確保
        Node *new_node = (Node *)malloc(sizeof(Node));
        if(new_node == NULL) {
            printf("ERROR: Could not allocate memory for new node\n");
            perror("malloc");
            return 1;
        }
        // bufの内容を新しいノードへ格納
        sscanf(buf, "%d %99s %d", &new_node->id, new_node->name, &new_node->score);
        new_node->next = NULL;
        if(students == NULL) {
            students = new_node;
            students_tail = new_node;
        } else {
            students_tail->next = new_node;
            students_tail = new_node;
        }
    }
    fclose(input_file);

    // クイックソート実行
    students = quick_sort_by_score(students);

    

    FILE *output_file = fopen(OUTPUT_FILE, "w");
    if(output_file == NULL){
        printf("ERROR: Could not open %s\n", OUTPUT_FILE);
        perror("fopen");
        return 1;
    }
    int rank = 1;
    Node *current = students;
    while(current != NULL){
        fprintf(output_file, "%d %d %s %d\n",rank,current->id,current->name,current->score);
        current = current->next;
        rank++;
    }
    fclose(output_file);

    // // Debug
    // *current = students;
    // while(current != NULL) {
    //     printf("ID: %d, Name: %s, Score: %d\n", current->id, current->name, current->score);
    //     Node *temp = current;
    //     current = current->next;
    //     free(temp);
    // }

    return 0;
}

Node *quick_sort_by_score(Node *head) {
    if (head == NULL || head->next == NULL) {
        return head; 
    }

    Node *pivot = head;
    Node *greater_head = NULL, *greater_tail = NULL;
    Node *less_head = NULL, *less_tail = NULL;

    Node *current = head->next;
    while (current != NULL) {
        if (current->score > pivot->score) {
            if (greater_head == NULL) {
                greater_head = current;
                greater_tail = current;
            } else {
                greater_tail->next = current;
                greater_tail = current;
            }
        } else {
            if (less_head == NULL) {
                less_head = current;
                less_tail = current;
            } else {
                less_tail->next = current;
                less_tail = current;
            }
        }
        current = current->next;
    }

    if (greater_tail != NULL) greater_tail->next = NULL;
    if (less_tail != NULL) less_tail->next = NULL; 

    greater_head = quick_sort_by_score(greater_head);
    less_head = quick_sort_by_score(less_head);

    if (greater_head == NULL) {
        pivot->next = less_head; 
        return pivot; 
    } else {
        Node *new_head = greater_head;
        greater_tail->next = pivot;
        pivot->next = less_head;
        return new_head;
    }
}
