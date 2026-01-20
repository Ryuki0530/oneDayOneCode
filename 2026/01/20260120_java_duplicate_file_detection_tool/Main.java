import java.io.*;
import java.nio.file.*;
import java.security.MessageDigest;
import java.util.*;

public class Main {
    private static final int BUFFER_SIZE = 8192;
    private static final String SHA_256 = "SHA-256";
    
    static class FileInfo {
        String path;
        long size;
        
        FileInfo(String path, long size) {
            this.path = path;
            this.size = size;
        }
    }
    
    public static void main(String[] args) throws Exception {
        if (args.length == 0) {
            System.err.println("Usage: java Main <directory>");
            System.exit(1);
        }
        
        Path rootDir = Paths.get(args[0]);
        if (!Files.isDirectory(rootDir)) {
            System.err.println("Error: Not a directory");
            System.exit(1);
        }
        
        // サイズ -> (パス, ハッシュ) のリスト
        Map<Long, List<FileHashPair>> sizeGroups = new TreeMap<>();
        int skipped = 0;
        
        // ファイル走査
        try (var stream = Files.walk(rootDir)) {
            for (Path path : (Iterable<Path>) stream
                    .filter(Files::isRegularFile)::iterator) {
                try {
                    long size = Files.size(path);
                    String hash = computeSHA256(path);
                    String pathStr = path.toAbsolutePath().toString();
                    
                    sizeGroups.computeIfAbsent(size, k -> new ArrayList<>())
                            .add(new FileHashPair(pathStr, hash));
                } catch (Exception e) {
                    skipped++;
                }
            }
        }
        
        // ダブり検出
        List<DuplicateGroup> groups = new ArrayList<>();
        
        for (Map.Entry<Long, List<FileHashPair>> entry : sizeGroups.entrySet()) {
            long size = entry.getKey();
            Map<String, List<String>> hashGroups = new TreeMap<>();
            
            for (FileHashPair pair : entry.getValue()) {
                hashGroups.computeIfAbsent(pair.hash, k -> new ArrayList<>())
                        .add(pair.path);
            }
            
            for (Map.Entry<String, List<String>> hashEntry : hashGroups.entrySet()) {
                if (hashEntry.getValue().size() >= 2) {
                    List<String> paths = new ArrayList<>(hashEntry.getValue());
                    Collections.sort(paths);
                    groups.add(new DuplicateGroup(size, hashEntry.getKey(), paths));
                }
            }
        }
        
        // 総サイズでソート（降順）
        groups.sort((a, b) -> Long.compare(b.getTotalSize(), a.getTotalSize()));
        
        // 出力
        for (int i = 0; i < groups.size(); i++) {
            DuplicateGroup group = groups.get(i);
            System.out.printf("[Group %d] size=%d bytes, count=%d, sha256=%s%n",
                    i + 1, group.size, group.paths.size(), group.hash);
            for (String path : group.paths) {
                System.out.println(path);
            }
        }
        
        if (skipped > 0) {
            System.out.printf("Skipped: %d%n", skipped);
        }
    }
    
    private static String computeSHA256(Path path) throws Exception {
        MessageDigest digest = MessageDigest.getInstance(SHA_256);
        byte[] buffer = new byte[BUFFER_SIZE];
        
        try (InputStream is = Files.newInputStream(path)) {
            int bytesRead;
            while ((bytesRead = is.read(buffer)) != -1) {
                digest.update(buffer, 0, bytesRead);
            }
        }
        
        return bytesToHex(digest.digest());
    }
    
    private static String bytesToHex(byte[] bytes) {
        StringBuilder sb = new StringBuilder();
        for (byte b : bytes) {
            sb.append(String.format("%02x", b));
        }
        return sb.toString();
    }
    
    static class FileHashPair {
        String path;
        String hash;
        
        FileHashPair(String path, String hash) {
            this.path = path;
            this.hash = hash;
        }
    }
    
    static class DuplicateGroup {
        long size;
        String hash;
        List<String> paths;
        
        DuplicateGroup(long size, String hash, List<String> paths) {
            this.size = size;
            this.hash = hash;
            this.paths = paths;
        }
        
        long getTotalSize() {
            return size * paths.size();
        }
    }
}