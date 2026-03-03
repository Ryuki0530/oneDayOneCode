import java.io.*;
import java.util.*;

public class Main {
    static class Event {
        int time;
        String type;
        String user;
        Event(int time, String type, String user) {
            this.time = time;
            this.type = type;
            this.user = user;
        }
    }
    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        String[] firstLine = br.readLine().split(" ");
        int Q = Integer.parseInt(firstLine[0]);
        int W = Integer.parseInt(firstLine[1]);
        Deque<Event> window = new LinkedList<>();
        Map<String, Integer> typeCount = new HashMap<>();
        Map<String, Integer> userCount = new HashMap<>();
        Map<Integer, Integer> freqCount = new HashMap<>();
        int currentMaxFreq = 0;
        
        for (int i = 0; i < Q; i++) {
            String[] query = br.readLine().split(" ");
            String command = query[0];
            
            if (command.equals("ADD")) {
                int t = Integer.parseInt(query[1]);
                String type = query[2];
                String user = query[3];
                
                window.addLast(new Event(t, type, user));
                
                int oldTypeFreq = typeCount.getOrDefault(type, 0);
                typeCount.put(type, oldTypeFreq + 1);
                
                if (oldTypeFreq > 0) {
                    freqCount.put(oldTypeFreq, freqCount.get(oldTypeFreq) - 1);
                    if (freqCount.get(oldTypeFreq) == 0) {
                        freqCount.remove(oldTypeFreq);
                    }
                }
                freqCount.put(oldTypeFreq + 1, freqCount.getOrDefault(oldTypeFreq + 1, 0) + 1);
                currentMaxFreq = Math.max(currentMaxFreq, oldTypeFreq + 1);
                userCount.put(user, userCount.getOrDefault(user, 0) + 1);
                
            } else {
                int t = Integer.parseInt(query[1]);
                
                while (!window.isEmpty() && window.peekFirst().time <= t - W) {
                    Event removed = window.removeFirst();
                    
                    int oldTypeFreq = typeCount.get(removed.type);
                    freqCount.put(oldTypeFreq, freqCount.get(oldTypeFreq) - 1);
                    if (freqCount.get(oldTypeFreq) == 0) {
                        freqCount.remove(oldTypeFreq);
                    }
                    if (oldTypeFreq > 1) {
                        freqCount.put(oldTypeFreq - 1, freqCount.getOrDefault(oldTypeFreq - 1, 0) + 1);
                    }
                    typeCount.put(removed.type, oldTypeFreq - 1);
                    
                    int userCnt = userCount.get(removed.user) - 1;
                    if (userCnt == 0) {
                        userCount.remove(removed.user);
                    } else {
                        userCount.put(removed.user, userCnt);
                    }
                }
                while (currentMaxFreq > 0 && !freqCount.containsKey(currentMaxFreq)) {
                    currentMaxFreq--;
                }
                
                if (command.equals("COUNT")) {
                    System.out.println(window.size());
                } else if (command.equals("USERS")) {
                    System.out.println(userCount.size());
                } else if (command.equals("MODE")) {
                    System.out.println(currentMaxFreq);
                } else if (command.equals("TYPE")) {
                    String type = query[2];
                    System.out.println(typeCount.getOrDefault(type, 0));
                }
            }
        }
    }
}