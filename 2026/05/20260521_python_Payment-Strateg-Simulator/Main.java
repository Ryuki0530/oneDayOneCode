import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.StringTokenizer;

abstract class PaymentStrategy {
    public abstract long calculate(long amount);
}

class CashPayment extends PaymentStrategy {
    @Override
    public long calculate(long amount) {
        return amount;
    }
}

class CreditPayment extends PaymentStrategy {
    @Override
    public long calculate(long amount) {
        return (amount * 105) / 100;
    }
}

class PointPayment extends PaymentStrategy {
    @Override
    public long calculate(long amount) {
        return (amount * 90) / 100;
    }
}

class PaymentContext {
    private PaymentStrategy strategy;

    public void setStrategy(PaymentStrategy strategy) {
        this.strategy = strategy;
    }

    public long execute(long amount) {
        return strategy.calculate(amount);
    }
}

public class Main {
    public static void main(String[] args) throws Exception {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        int n = Integer.parseInt(br.readLine().trim());

        StringTokenizer st = new StringTokenizer(br.readLine());
        long total = 0;
        for (int i = 0; i < n; i++) {
            total += Long.parseLong(st.nextToken());
        }

        String method = br.readLine().trim();

        PaymentContext context = new PaymentContext();
        switch (method) {
            case "CASH":
                context.setStrategy(new CashPayment());
                break;
            case "CREDIT":
                context.setStrategy(new CreditPayment());
                break;
            case "POINT":
                context.setStrategy(new PointPayment());
                break;
            default:
                throw new IllegalArgumentException("Unknown payment method: " + method);
        }

        System.out.println(context.execute(total));
    }
}