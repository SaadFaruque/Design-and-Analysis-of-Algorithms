import java.io.FileNotFoundException;
import java.io.PrintStream;
public class LinkedList {
	treeNode listHead;
	LinkedList(){
		listHead=new treeNode("dummy",0);
	}
	treeNode findSpot(treeNode newNode) {
		treeNode temp=listHead;
		while(temp.next!=null) {
			if(temp.prob<newNode.prob&&newNode.prob<=temp.next.prob)
				return temp;
			temp=temp.next;
		}
		return temp;
	}
	void insertOneSpot(treeNode spot,treeNode newNode) {
		newNode.next=spot.next;
		spot.next=newNode;
	}
	void printList(PrintStream outFile)throws FileNotFoundException {
		String output="";
		treeNode temp=listHead;
		while(temp.next!=null) {
			output+="-->("+temp.chStr+","+temp.prob+","+temp.next.chStr+")";
			temp=temp.next;
		}
		output+="-->("+temp.chStr+","+temp.prob+","+"NULL)-->NULL\n";
		System.setOut(outFile);
		System.out.println(output);
	}
}
