import java.io.BufferedReader;
import java.io.File;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;

public class Main {
    public static void main(String[] args) throws Exception {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in, StandardCharsets.UTF_8));

        String dirPath = br.readLine();
        if (dirPath == null) return;
        dirPath = dirPath.trim();

        String extInput = br.readLine();
        if (extInput == null) return;
        String targetExt = normalizeExt(extInput);

        File dir = new File(dirPath);
        if (!dir.isDirectory()) return;

        File[] files = dir.listFiles(f -> f.isFile() && matchesExt(f.getName(), targetExt));
        if (files == null) return;

        for (File f : files) {
            System.out.println(f.getName());
        }
    }

    private static String normalizeExt(String ext) {
        ext = ext.trim();
        while (ext.startsWith(".")) {
            ext = ext.substring(1);
        }
        return ext.toLowerCase();
    }

    private static boolean matchesExt(String fileName, String targetExt) {
        int idx = fileName.lastIndexOf('.');
        if (idx < 0 || idx == fileName.length() - 1) return false;
        String fileExt = fileName.substring(idx + 1);
        return fileExt.equalsIgnoreCase(targetExt);
    }
}