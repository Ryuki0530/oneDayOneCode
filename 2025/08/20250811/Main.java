import java.util.Scanner;

class Calculator {
    public Calculator() {
    }

    public double calculateByString(String input) {
        String[] parts = input.split(" ");
        if (parts.length != 3) {
            throw new IllegalArgumentException("入力は '数値 演算子 数値' の形式でなければなりません");
        }
        int a = Integer.parseInt(parts[0]);
        int b = Integer.parseInt(parts[2]);
        String operator = parts[1];
        return singleCalculate(a, b, operator);
    }

    private double singleCalculate(int a, int b, String operator) {
        switch (operator) {
            case "+":
                return add(a, b);
            case "-":
                return subtract(a, b);
            case "*":
                return multiply(a, b);
            case "/":
                return divide(a, b);
            default:
                throw new IllegalArgumentException("無効な演算子です: " + operator);
        }
    }

    private int add(int a, int b) {
        return a + b;
    }

    private int subtract(int a, int b) {
        return a - b;
    }

    private int multiply(int a, int b) {
        return a * b;
    }

    private double divide(int a, int b) {
        if (b == 0) {
            throw new IllegalArgumentException("除数は0であってはいけません");
        }
        return (double) a / b;
    }
}

public class Main {
    public static void main(String[] args) {
        Calculator calculator = new Calculator();
        Scanner sc = new Scanner(System.in);
        for (;;) {
            System.out.print("計算式を入力してください（例: 3 + 4）: ");
            try {
                String input = sc.nextLine();
                if (input.equals("exit")) {
                    System.out.println("終了します。");
                    break;
                }
                double result = calculator.calculateByString(input);
                System.out.println("計算結果: " + result);
            } catch (IllegalArgumentException e) {
                System.out.println("エラー: " + e.getMessage());
            }
        }
    }
}
