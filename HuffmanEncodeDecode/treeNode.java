import java.io.FileNotFoundException;
import java.io.PrintStream;
public class treeNode {
	String chStr;
	int prob;
	treeNode next;
	treeNode left;
	treeNode right;
	String code;
	treeNode(String chStr,int prob){
		this.chStr=chStr;
		this.prob=prob;
		next=null;
		left=null;
		right=null;
		code="";
	}
	boolean isLeaf() {
		return this!=null&&this.left==null&&this.right==null;
	}
	void printNode(PrintStream outFile) throws FileNotFoundException{
		System.setOut(outFile);
		if(next==null&&left==null&&right==null)
			System.out.println(chStr+" "+prob+" "+code+" NULL NULL NULL");
		else if(next==null)
			System.out.println(chStr+" "+prob+" "+code+" NULL "+left.chStr+" "+right.chStr);
		else if(left==null&&right==null)
			System.out.println(chStr+" "+prob+" "+code+" "+next.chStr+" NULL NULL");
		else
			System.out.println(chStr+" "+prob+" "+code+" "+next.chStr+" "+left.chStr+" "+right.chStr);
		/*System.setOut(outFile);
		System.out.println(chStr+" "+prob+" "+code+" "+next.chStr+" "+left.chStr+" "+right.chStr);*/
	}
}
