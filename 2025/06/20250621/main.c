#include<stdio.h>
#include<stdbool.h>

bool checkTriangle(int a, int b, int c);

int main() {
    // Print a message to the console
    int a = 0;
    int b = 0;
    int c = 0;
    bool isTriangle = false;

    scanf("%d %d %d", &a, &b, &c);
    isTriangle = checkTriangle(a, b, c);
    if(!isTriangle){
        printf("Not a triangle\n");
        return 0;
    }
    if( a == b && b == c){
        printf("Equilateral\n");
    }else if(a == b || b == c || a == c){
        printf("Isosceles\n");
    }else{
        printf("Scalene\n");
    }
    
    // //Debug
    // printf("a = %d, b = %d, c = %d\n", a, b, c);
    // printf("isTriangle = %d\n", isTriangle);
    return 0;
}

bool checkTriangle(int a, int b, int c){
    if(a+b>c && a+c>b && c+b>a){
        return true;
    }else{
        return false;
    }
}