import java.util.ArrayList;
class Subsequence {
  int subsequence(String input,ArrayList<String> output) {
    if(input.length() == 0) {
      output.add("");
      return 1;
    }

    int size = subsequence(input.substring(1),output);
    for(int i=0;i<size;i++) {
      output.add(size+i,input.substring(0,1)+output.get(i));
    }

    return 2*size;
  }

  public static void main(String[] args) {
    String input = args[0];
    ArrayList<String> output = new ArrayList<String>();
    int size = new Subsequence().subsequence(input,output);
    for(int i=0;i<size;i++) {
      System.out.println(output.get(i));
    }
  }
}
