interface DiscountStrategy {
	int apply(int price);
}

class NoDiscount implements DiscountStrategy {
	@Override
	public int apply(int price) {
		return price;
	}
}

class MemberDiscount implements DiscountStrategy {
	@Override
	public int apply(int price) {
		return price * 90 / 100;
	}
}

class FixedDiscount implements DiscountStrategy {
	private final int amount;

	FixedDiscount(int amount) {
		this.amount = amount;
	}

	@Override
	public int apply(int price) {
		return Math.max(0, price - amount);
	}
}

class Checkout {
	private DiscountStrategy strategy;

	Checkout(DiscountStrategy strategy) {
		this.strategy = strategy;
	}

	void setStrategy(DiscountStrategy strategy) {
		this.strategy = strategy;
	}

	int calculate(int price) {
		return strategy.apply(price);
	}
}

public class Main {
	public static void main(String[] args) {
		int price = 1200;
		Checkout checkout = new Checkout(new NoDiscount());

		System.out.println("Normal: " + checkout.calculate(price));

		checkout.setStrategy(new MemberDiscount());
		System.out.println("Member: " + checkout.calculate(price));

		checkout.setStrategy(new FixedDiscount(300));
		System.out.println("Fixed300: " + checkout.calculate(price));

		checkout.setStrategy(new FixedDiscount(1500));
		System.out.println("Fixed1500: " + checkout.calculate(price));
	}
}
