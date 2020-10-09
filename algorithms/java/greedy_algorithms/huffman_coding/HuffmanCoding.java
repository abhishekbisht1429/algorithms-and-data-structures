import java.util.PriorityQueue;
import java.util.Scanner;
import java.util.Comparator;
import java.util.Map;
import java.util.HashMap;
class HuffmanNode {
  char c;
  int freq;
  HuffmanNode left,right;

  HuffmanNode(char c,int freq,HuffmanNode left,HuffmanNode right) {
    this.c = c;
    this.freq = freq;
    this.left = left;
    this.right = right;
  }
  void setChar(char c) {
    this.c=c;
  }
  void setFreqency(int freq) {
    this.freq = freq;
  }

  char getChar() {
    return c;
  }

  int getFrequency() {
    return freq;
  }

  void setLeft(HuffmanNode left) {
    this.left = left;
  }
  void setRight(HuffmanNode right) {
    this.right = right;
  }
  HuffmanNode getLeft() {
    return left;
  }
  HuffmanNode getRight() {
    return right;
  }
}

class MyComparator implements Comparator<HuffmanNode> {
  @Override
  public int compare(HuffmanNode n1,HuffmanNode n2) {
    return n1.getFrequency() - n2.getFrequency();
  }
}

public class HuffmanCoding {

  static HuffmanNode huffmanCoding(PriorityQueue<HuffmanNode> pq) {
    int size = pq.size();
    while(pq.size()>1) {
      HuffmanNode n1 = pq.poll();
      HuffmanNode n2 = pq.poll();
      HuffmanNode newNode = new HuffmanNode('-',n1.getFrequency()+n2.getFrequency(),
                              n1,n2);
      pq.add(newNode);
    }
    //return the root
    return pq.poll();
  }

  static void findCodes(HuffmanNode root,String s,Map<Character,String> map) {
    if(root.getLeft()==null && root.getRight()==null) {
      map.put(root.getChar(),s);
      return;
    }

    findCodes(root.getLeft(),s+"0",map);
    findCodes(root.getRight(),s+"1",map);
  }
  public static void main(String[] args) {
    Scanner s =new Scanner(System.in);
    System.out.println("Enter the number of characters and frequencies(char and freq seperated by space and next characters"+
                        "seperated by space)");
    int n = s.nextInt();
    PriorityQueue<HuffmanNode> pq = new PriorityQueue<>(n,new MyComparator());
    for(int i=0;i<n;i++) {
      char c;
      int freq;
      c = s.next().charAt(0);
      freq = s.nextInt();
      HuffmanNode node = new HuffmanNode(c,freq,null,null);
      pq.add(node);
    }
    
    HuffmanNode  root = huffmanCoding(pq);
    Map<Character,String> codeMap = new HashMap<>();
    findCodes(root,"",codeMap);

    System.out.println("\nHuffman Codes for the Characters");
    for(Character c:codeMap.keySet()) {
      System.out.printf("%c %s\n",c,codeMap.get(c));
    }
  }
}
