import java.io.File;
import java.io.FileDescriptor;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;
import java.util.Scanner;

public class HuffmanBinaryTree {
	String[] charCode=new String[256];
	treeNode Root;
	HuffmanBinaryTree(){
		Root=new treeNode("",0);
	}
	void constructHuffmanLList(Scanner inFile,PrintStream outFile)throws FileNotFoundException {
		int count=0;
		treeNode newNode,spot;
		String chr="",prob,word;
		LinkedList ll=new LinkedList();
		while(inFile.hasNext()) {
			word=inFile.next();
			count++;
			if(count%2==1) {
				chr=word;
			}
			else {
				prob=word;
					
				
					newNode=new treeNode(chr+"",Integer.parseInt(prob));
				spot=ll.findSpot(newNode);
				ll.insertOneSpot(spot, newNode);
				ll.printList(outFile);
			}
		}
		constructHuffmanBinTree(ll,outFile);
	}
	void constructHuffmanBinTree(LinkedList ll,PrintStream outFile)throws FileNotFoundException {
		treeNode removed1,removed2,newNode,spot;
		//Root=ll.listHead.next;
		while(ll.listHead.next.next!=null) {
			removed1=ll.listHead.next;
			removed2=ll.listHead.next.next;
			ll.listHead.next=ll.listHead.next.next.next;
			newNode=new treeNode(removed1.chStr+removed2.chStr,removed1.prob+removed2.prob);
			newNode.left=removed1;
			newNode.right=removed2;
			spot=ll.findSpot(newNode);
			ll.insertOneSpot(spot, newNode);
			ll.printList(outFile);
		}
		if(ll.listHead.next==null)
			return ;
		if(ll.listHead.next.next==null) {
			Root=ll.listHead.next;
			//constructCode(Root,"");
			ll.printList(outFile);
		}
		//constructCode(Root,"");
	}
	void constructCode(treeNode T,String code) {
		if(T.isLeaf()) {
			charCode[T.chStr.charAt(0)]=code;
			T.code=code;
		}
		else {
		constructCode(T.left,code+"0");
		constructCode(T.right,code+"1");
		}
	}
	void preOrderTraversal(treeNode Root,PrintStream outFile)throws FileNotFoundException {
		if(Root!=null) {
			Root.printNode(outFile);
			preOrderTraversal(Root.left,outFile);
			preOrderTraversal(Root.right,outFile);
		}
	}
	void inOrderTraversal(treeNode Root,PrintStream outFile)throws FileNotFoundException{
		if(Root!=null) {
			inOrderTraversal(Root.left,outFile);
			Root.printNode(outFile);
			inOrderTraversal(Root.right,outFile);
		}
	}
	void postOrderTraversal(treeNode Root,PrintStream outFile)throws FileNotFoundException {
		if(Root!=null) {
			postOrderTraversal(Root.left,outFile);
			postOrderTraversal(Root.right,outFile);
			Root.printNode(outFile);
		}
		
	}
	void Encode(Scanner inFile,PrintStream outFile) {
		String nextline,out="";
		while(inFile.hasNext()) {
			nextline=inFile.nextLine();
			for(int i=0;i<nextline.length();i++) {
				if(nextline.charAt(i)==(char)32)
					out+=charCode[64];
				else
					out+=charCode[nextline.charAt(i)];
			}
			out+=charCode[35];
		}
		System.setOut(outFile);
		System.out.print(out);
	}
	void Decode(Scanner inFile,PrintStream outFile) {
		String nextline,out="";
		treeNode spot=Root;
		char oneBit;
		while(inFile.hasNext()) {
			nextline=inFile.nextLine();
			for(int i=0;i<nextline.length();i++) {
				if(spot.isLeaf()) {
					if(spot.chStr.charAt(0)==(char)64)
						out+=(char)32;
					else if(spot.chStr.charAt(0)==(char)35)
						out+="\n";
					else
						out+=spot.chStr;
					spot=Root;
				}
				oneBit=nextline.charAt(i);
				if(oneBit=='0')
					spot=spot.left;
				else if(oneBit=='1')
					spot=spot.right;
				else {
					out="Error! The compress file contains invalid character!";
					System.setOut(outFile);
					System.out.print(out);
					outFile.close();
				}	
			}
			out+="\n";
		}
		if(!spot.isLeaf()) {
			out="Error:  The compress file is corrupted!";
		}
		System.setOut(outFile);
		System.out.print(out);
		outFile.close();
	}
	void userInterface()throws FileNotFoundException {
		System.setOut(new PrintStream(
                new FileOutputStream(FileDescriptor.out)));
		for(int i=0;i<256;i++) {
		if(charCode[i]!=null&&charCode[i].length()!=0)
			System.out.println((char)i+" "+charCode[i]);
		}
		String nameOrg="";
		String nameCompress;
		String nameDepress;
		
		char yesNo='Y';
		Scanner input=new Scanner(System.in);
		
		while(true) {
			System.out.println("Do you want to compress a file, Y for yes N for No:");
			yesNo=input.nextLine().charAt(0);
			if(yesNo=='N') {
				break;
			}
			else if(yesNo=='Y'){
				System.out.println("please enter the file name you want to compress");
				nameOrg=input.nextLine();
				nameCompress=nameOrg+"_Compressed.txt";
				nameDepress=nameOrg+"_Decompressed.txt";
				Scanner infile=new Scanner(new File(nameOrg));
				PrintStream compress=new PrintStream(new File(nameCompress));
				PrintStream decompress=new PrintStream(new File(nameDepress));
				Encode(infile,compress);
				infile.close();
				compress.close();
				Scanner compressed=new Scanner(new File(nameCompress));
				Decode(compressed,decompress);
				compressed.close();
				decompress.close();
				System.setOut(new PrintStream(
						new FileOutputStream(FileDescriptor.out)));
			}
			else {
				System.out.println("Cannot recognize");
			}
		}
	}
}
