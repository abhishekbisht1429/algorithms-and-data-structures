class LCSubsequence { //LC - Longest Common

  int lcsubsequenceLength(String str1,String str2) {
    if(str1.length()==0 || str2.length()==0)
      return 0;

    if(str1.charAt(0) == str2.charAt(0)) {
      return 1+lcsubsequenceLength(str1.substring(1),str2.substring(1));
    } else {
      int count1 = lcsubsequenceLength(str1,str2.substring(1));
      int count2 = lcsubsequenceLength(str1.substring(1),str2);
      return count1>count2?count1:count2;
    }
  }

  public static void main(String[] args) {
    String str1 = args[0];
    String str2 = args[1];
    System.out.println(new LCSubsequence().lcsubsequenceLength(str1,str2));
  }
}
