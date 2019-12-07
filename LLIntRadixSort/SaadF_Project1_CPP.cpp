#include<iostream>
using namespace std;


//Constructor for node class
struct node
  {
    int data;
    node *nextNode;
  };	

//Creating singley linked list
class list{
	public:
	node *head, *tail;
	//pointers for head and tail
	
	public:
	list(){
		head=NULL;
		tail=NULL;
	
	}
	void listInsert(int item){
		//keeping track of the node through pointer
		node *tempNode = new node;
		tempNode -> data = item;
		tempNode -> nextNode = NULL;
		if(head == NULL){
			head = tempNode;
			tail = tempNode;
			tempNode = NULL;
		}
		else{
			tail -> nextNode = tempNode;
			tail = tempNode;
		}
	}
		

		void queueDelete(){
			node *tempNode=new node;
			tempNode=head;
			head=head->nextNode;
			delete tempNode;
		}
		
		void stackDelete(){
		if (head == NULL){
			cout << "List is empty! \n";
			return;
		}
		if (head == tail){
			cout << "List is now empty! \n";
			head = NULL;
			tail = NULL;
			return;
		}
			
			node *currentNode = NULL;
			node *previousNode = NULL;
			currentNode = head;
			while(currentNode -> nextNode != NULL){
				previousNode=currentNode;
				currentNode=currentNode->nextNode;
				
		}
		tail = previousNode;
		previousNode -> nextNode = NULL;
		delete currentNode;
	}

		void displayList(){
			node *tempNode = new node;
			tempNode = head;
			cout << " [BEGIN] ";
			while(tempNode != NULL)
			{
				cout <<tempNode -> data<<"  ---> ";
				tempNode = tempNode -> nextNode;
			}
			cout << " [END] ";
			cout << " \n";
		}
		
		void askInsert(int value, bool stacky){
			cout << "Please give the interger you want to insert. \n";
			cin >> value;
			if(stacky == 1){
			//obj.createNode(temp);
				listInsert(value);
				displayList();
		}
		}
			
			
};

int main(int argc, char *argv[]){
	
	list obj;
	bool ifStack;
	string stackQueue;
	string methodChoice;
	int temp;
	bool conti;
	bool inserty;
	bool starty;
	bool repeat;
	repeat = true;
	starty = true;
	inserty = true;
	conti = true;
	string yn;
	
//Step 1
	cout << "Do you want to use a stack or a queue? \n";
	
	while(starty){
		cout << "Input either an S for stack, or Q for queue. \n";
		cin >> stackQueue;
		if (stackQueue.compare("S") == 0 || stackQueue.compare("Q") == 0){
			starty = false;
		}
	}//end while
	if (stackQueue == "S"){
		ifStack = true;
	}
	else{
		ifStack = false;
	}

//Step 2
	cout << "Saad Faruque, Class Section 323-35 \n";	
//continuing
	while(conti){

//Step 3
	while (inserty){
		cout << "Input either an I for insert, or D for delete. \n";
		cin >> methodChoice;
		if (methodChoice.compare("I") == 0 || methodChoice.compare("D") == 0 ){
		inserty = false;
	}
	}
//begin insert for stack
	if (methodChoice == "I"){
//Step 4
		cout << "You chose insert. \n";
//Step 5.1
		cout << "Please give the interger you want to insert. \n";
		cin >> temp;
//Step 5.2
		cout << "You inputed: ";
		cout << temp;
		cout << " \n ";
//Step 6.1
//Step 6.2
		obj.listInsert(temp);
//Step 7		
		obj.displayList();
		
}//end insert for stack
//begin delete for stack

	if (methodChoice == "D"){		
//Step Four		
		cout << "You chose delete. \n"; 
		if( ifStack == true){
			cout << "Stack Delete \n";
			obj.stackDelete();
			}
		else{
			if( ifStack == false){
				cout << " Queue Delete \n";
				obj.queueDelete();
			}
		}
		obj.displayList();
		
	}
	inserty = true;

	while(repeat){
//Step 8
	cout << "Would you like to continue? Input Y for yes, or N for no \n";
	cin >> yn ;
	if ( yn.compare("Y") == 0 || yn.compare("N") == 0 ){
		repeat = false;
	}
	}// end repeat
	
	if (yn == "N"){
//Step 9	
		cout << "You chose no. GoodBye. \n";
//Step 10		
		conti = false;
	}
	else{
//Step 9		
		cout << "You chose yes. Now repeating. \n";
		repeat = true;
	}

} //while loop for continue
}//end main





	
	
	