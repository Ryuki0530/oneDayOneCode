class Compresser{
     String compress(String str){
        
        if (str.isEmpty()) {
            return "";
        }

        StringBuilder sb = new StringBuilder();
        
        int count = 1;
        for(int i = 1;i < str.length(); i++){
            if(str.charAt(i) == str.charAt(i - 1)){
                count++;
            } else {
                sb.append(str.charAt(i - 1));
                if(count > 1) {
                    sb.append(count);
                }

                count = 1;
            }
        }
        sb.append(str.charAt(str.length() - 1));
        if(count > 1) {
            sb.append(count);
        }
        
        return sb.toString();
    }
}

public class Main{
    public static void main(String[] args) {
        Compresser comp = new Compresser();
        String[] testStrings = {"AAABBCCCCDA", "aAAbb", ""};
        for(String test : testStrings){
            String compressed = comp.compress(test);
            System.out.println("Original: " + test + " | Compressed: " + compressed);
        }

    }
}