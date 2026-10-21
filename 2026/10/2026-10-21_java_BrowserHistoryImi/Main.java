import java.util.ArrayList;
import java.util.List;

class BrowserHistory {
	private final List<String> history = new ArrayList<>();
	private int currentIndex;

	public BrowserHistory() {
		history.add("home");
		currentIndex = 0;
	}

	public void visit(String page) {
		if (page == null || page.isEmpty()) {
			throw new IllegalArgumentException("page must not be empty");
		}

		while (history.size() > currentIndex + 1) {
			history.remove(history.size() - 1);
		}
		history.add(page);
		currentIndex++;
	}

	public void back() {
		if (currentIndex > 0) {
			currentIndex--;
		}
	}

	public void forward() {
		if (currentIndex < history.size() - 1) {
			currentIndex++;
		}
	}

	public String current() {
		return history.get(currentIndex);
	}
}

// test
public class Main {
	public static void main(String[] args) {
		BrowserHistory browser = new BrowserHistory();
		System.out.println(browser.current());

		browser.visit("news");
		System.out.println(browser.current());

		browser.visit("article");
		System.out.println(browser.current());

		browser.back();
		System.out.println(browser.current());

		browser.visit("weather");
		System.out.println(browser.current());

		browser.forward();
		System.out.println(browser.current());

		browser.back();
		System.out.println(browser.current());

		browser.back();
		System.out.println(browser.current());

		browser.back();
		System.out.println(browser.current());

		browser.forward();
		System.out.println(browser.current());
	}
}
