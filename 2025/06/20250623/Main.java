import java.nio.file.*;

public class Main {
    public static void main(String[] args) {
        int alphabetsCount = 0;
        int digitsCount = 0;
        int spaceCount = 0;
        int othersCount = 0;
        try{
            String input = new String(
                Files.readAllBytes(
                    Paths.get("input.txt")
                ),
                "UTF-8"
            );

            int inputSize = input.length();
            for(int i =0; i<inputSize; i++) {
                char c = input.charAt(i);
                if (Character.isAlphabetic(c)) {
                    alphabetsCount++;
                } else if (Character.isDigit(c)) {
                    digitsCount++;
                } else if (c == ' ') {
                    spaceCount++;
                } else {
                    othersCount++;
                }
            }

            // Prepare output content
            StringBuilder sb = new StringBuilder();
            sb.append("Alphabets: ").append(alphabetsCount).append(System.lineSeparator());
            sb.append("Digits: ").append(digitsCount).append(System.lineSeparator());
            sb.append("Spaces: ").append(spaceCount).append(System.lineSeparator());
            sb.append("Others: ").append(othersCount).append(System.lineSeparator());

            // Write to output.txt
            Files.write(Paths.get("output.txt"), sb.toString().getBytes("UTF-8"));
            
            // //Ddebugging output
            // System.out.println(input);
            // System.out.println(input.length());
            // System.out.println("Alphabets: " + alphabetsCount);
            // System.out.println("Digits: " + digitsCount);
            // System.out.println("Spaces: " + spaceCount);
            // System.out.println("Others: " + othersCount);

        } catch (Exception e) {
            System.err.println("Error reading input file: " + e.getMessage());
            System.out.println("OMG!!!");
        }
        
    } 
}
