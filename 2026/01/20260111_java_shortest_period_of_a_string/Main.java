import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;

public class Main {
    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        String s = br.readLine().trim();
        int n = s.length();
        
        int[] lps = buildLps(s);
        
        int periodLength = n - lps[n - 1];
        
        if (n % periodLength == 0) {
            System.out.println(periodLength);
        } else {
            System.out.println(n);
        }
    }
    
    private static int[] buildLps(String s) {
        int n = s.length();
        int[] lps = new int[n];
        int j = 0;
        
        for (int i = 1; i < n; i++) {
            while (j > 0 && s.charAt(i) != s.charAt(j)) {
                j = lps[j - 1];
            }
            if (s.charAt(i) == s.charAt(j)) {
                j++;
            }
            lps[i] = j;
        }
        
        return lps;
    }
}