import java.io.*;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
public class Project5{
	public static void main(String[] args)throws FileNotFoundException{
		//Scanner file=new Scanner(new File(args[0]));
		//PrintStream freqTable=new PrintStream(new File(args[1]));
		//createFrequencyTable(file,freqTable);
		//file.close();
		//freqTable.close();
		Scanner inFile=new Scanner(new File(args[0]));
		HuffmanBinaryTree hbt=new HuffmanBinaryTree();
		PrintStream debug=new PrintStream(new File(args[1]));
		PrintStream preOrder=new PrintStream(new File(args[2]));
		PrintStream inOrder=new PrintStream(new File(args[3]));
		PrintStream postOrder=new PrintStream(new File(args[4]));
		hbt.constructHuffmanLList(inFile,debug);
		hbt.constructCode(hbt.Root,"");
		hbt.preOrderTraversal(hbt.Root, preOrder);
		hbt.inOrderTraversal(hbt.Root, inOrder);
		hbt.postOrderTraversal(hbt.Root, postOrder);
		hbt.userInterface();
		inFile.close();
		debug.close();
		preOrder.close();
		inOrder.close();
		postOrder.close();
	}
}
/*	public static void createFrequencyTable(Scanner file,PrintStream freqTable) {
		String nextline,out="";
		int count=0;
		Map<Character,Integer> map=new HashMap<>();
		if(file.hasNext()) {
			nextline=file.nextLine();
			count+=nextline.length();
			for(int i=0;i<nextline.length();i++) {
				if(!map.containsKey(nextline.charAt(i))) 
					map.put(nextline.charAt(i), 1);
				else
					map.put(nextline.charAt(i), map.get(nextline.charAt(i))+1);
			}
		}
		System.setOut(freqTable);
		for(char c:map.keySet()) {
			System.out.println(c+" "+100*map.get(c)/count);
		}
	}
}*/



