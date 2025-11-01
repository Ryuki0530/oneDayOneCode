import java.util.*;
import java.io.*;

abstract class Shape {
    abstract double getArea();
    abstract String getLabel();
}

class Circle extends Shape {
    private final double r;
    Circle(double r) { this.r = r; }
    @Override double getArea() { return Math.PI * r * r; }
    @Override String getLabel() { return "CIRCLE"; }
}

class Rectangle extends Shape {
    private final double w, h;
    Rectangle(double w, double h) { this.w = w; this.h = h; }
    @Override double getArea() { return w * h; }
    @Override String getLabel() { return "RECT"; }
}

class Triangle extends Shape {
    private final double b, h;
    Triangle(double b, double h) { this.b = b; this.h = h; }
    @Override double getArea() { return 0.5 * b * h; }
    @Override String getLabel() { return "TRI"; }
}

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        while (sc.hasNext()) {
            if (!sc.hasNextInt()) break;
            int n = sc.nextInt();
            List<Shape> shapes = new ArrayList<>(n);
            for (int i = 0; i < n; i++) {
                if (!sc.hasNext()) return;
                String type = sc.next();
                double a = sc.hasNextDouble() ? sc.nextDouble() : 0.0;
                double b = sc.hasNextDouble() ? sc.nextDouble() : 0.0;

                switch (type) {
                    case "CIRCLE":
                        shapes.add(new Circle(a));
                        break;
                    case "RECT":
                        shapes.add(new Rectangle(a, b));
                        break;
                    case "TRI":
                        shapes.add(new Triangle(a, b));
                        break;
                    default:
                        // Unknown type: skip (but keep input aligned)
                        break;
                }
            }
            for (Shape s : shapes) {
                System.out.printf("%s: %.2f%n", s.getLabel(), s.getArea());
            }
        }
    }
}