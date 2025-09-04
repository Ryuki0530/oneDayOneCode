import javax.swing.*;
import java.awt.*;

public class Main {
    public static void main(String[] args) {
        new WordCounterUI(new WordCounter());
    }
}

class WordCounter {
    public static int countWords(String text) {
        if (text == null || text.isEmpty()) {
            return 0;
        }
        String[] words = text.trim().split("\\s+");
        return words.length;
    }
}

class WordCounterUI extends JFrame {
    private JTextArea textArea;
    private JButton countButton;
    private JLabel resultLabel;

    public WordCounterUI(WordCounter wordCounter) {
        setTitle("Word Counter");
        setSize(400, 300);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new java.awt.BorderLayout());

        textArea = new JTextArea();
        countButton = new JButton("カウント");
        
        String resultLabelText = "単語数: ";
        resultLabel = new JLabel(resultLabelText);

        countButton.addActionListener(e -> {
            String text = textArea.getText();
            int wordCount = wordCounter.countWords(text);
            resultLabel.setText(resultLabelText + wordCount);
        });

        add(new JScrollPane(textArea), BorderLayout.CENTER);
        add(countButton, BorderLayout.SOUTH);
        add(resultLabel, BorderLayout.NORTH);
git
        setVisible(true);
    }

}
