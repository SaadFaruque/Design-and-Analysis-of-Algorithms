#include<fstream>
#include<iostream>
#include<string>
#include<cmath>
#include <sstream> 
using namespace std;

struct listNode{
    string data;
    listNode* next;

    listNode(string x){
        data = x;
        next = NULL;
    }

    string toString(){
        string ans = "";
        if(next == NULL)
            ans = ans + "( " + data + ", NULL )";
        else ans = ans + "( " + data + ", " + next->data + " )";
        return ans;
    }
};

class linkedListStack{

    private:
    listNode* top;

    public:
    linkedListStack(){
        top = NULL;
    }

    void push(listNode* newNode){
        newNode->next = top;
        top = newNode;
    }

    listNode* pop(){
        if(top == NULL) return NULL;
        listNode* node = top;
        top = top->next;
        node->next = NULL;
        return node;
    }

    bool isEmpty(){
        return (top == NULL);
    }

    void printStack(ostream& output){
        output << "*** Below is the contructed stack ***" << endl;
        listNode *temp = top;
        output << "TOP";
        while(temp!=NULL){
            output << " -> " << temp->data; // -> toString()
            temp = temp->next;
        }
        output << " -> NULL\n";
    }
};

class linkedListQueue{
    private:
    listNode* head, *tail;

    public:
    linkedListQueue(){
        head = tail = NULL;
    }

    void addTail(listNode* newNode){
        if(tail==NULL) {
            tail = newNode;
            head = tail;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    listNode* deleteFront(){
        listNode* first = head;
        if(head == tail) head = tail = NULL;
        else head = head->next;
        first->next = NULL;
        return first;
    }

    bool isEmpty(){
        return head == NULL;
    }

    void printQueue(ostream& output){
        output << "Front";
        listNode* temp = head;
        while(temp!=NULL){
            output << " -> " << temp->data; //-> toString()
            temp = temp->next;
        }
        output << " -> NULL" << endl;
        temp = tail;
        output << "Tail";
      /*  while(temp!=NULL){
            output << " -> " << temp->data;
            temp = temp->next;
        }
*/
        output << " -> NULL" << endl;
    }
};

class radixSort{
    private:
    int tableSize, currentTable, previousTable, maxDigit, currentDigit;
    linkedListStack stack;
    linkedListQueue** hashTable;

    public:
    radixSort(){
        tableSize = 256;
        currentTable = 0;
        previousTable = 1;
        maxDigit = 0;
        currentDigit = 0;
        linkedListStack stack = linkedListStack();
        hashTable = new linkedListQueue*[2];
        for(int i =0; i<2; i++) hashTable[i] = new linkedListQueue[tableSize];
        for(int i =0; i<2; i++)
            for(int j = 0; j <tableSize; j++)
                hashTable[i][j] = linkedListQueue();
    }

    ~radixSort(){
        for(int i =0; i<2; i++) delete []hashTable[i];
        delete []hashTable;
    }
	
	string padString(string data){
	int propperLength = maxDigit - data.length();
	string tempString;
	if(data.length() <= maxDigit){
		for(int i = 0; i< propperLength; i++){
		tempString.insert(i, " ");
	}
	tempString.insert(0, data);
	}
	return tempString;
	}
	
    void firstReading(ifstream &input){
        int max = 0;
		string data;
        while(!input.eof()){
            input >> data;
			int stringLength = data.length();
            if(stringLength > max) max = stringLength;
			//For reading strings, we dont need min
        }
        maxDigit = max;
      
    }

    void loadStack(ifstream &input, ofstream &output){
		string data;
        while(input >> data){
			string paddedData = padString(data);
            listNode *newNode = new listNode(paddedData);
            stack.push(newNode);
        }
        stack.printStack(output);
    }

    void RadixSort(ofstream &out1, ofstream &out2){
        dumpStack(out2);
        currentDigit++;
        while(currentDigit<=maxDigit){
            int temp = currentTable;
            currentTable = previousTable;
            previousTable = temp;
            for(int i =0; i<tableSize; i++){
                while(!hashTable[previousTable][i].isEmpty()){
                    listNode *temp = hashTable[previousTable][i].deleteFront();
                    string str = temp->data;
					char digitChar = str[str.length()-currentDigit];
                    int digit = int(digitChar);
                    if(str.length() < currentDigit) digit = 0;
                    hashTable[currentTable][digit].addTail(temp);
                }
            }
            printTable(out2);
            currentDigit++;
        }

        getSortedData(out1);
    }

    void dumpStack(ofstream &out){
        while(!stack.isEmpty()){
            listNode* temp = stack.pop();
			string tempString = temp->data;
			char digit = tempString.at(currentDigit);
            hashTable[currentTable][int(digit)].addTail(temp);
        }
        printTable(out);
    }

    void printTable(ofstream &out){
        out << "*** Below is the HASHTABLE ***\n";
        for(int i = 0; i< tableSize; i++){
            if(!hashTable[currentTable][i].isEmpty()){
                out << "Table [" << to_string(currentTable) << "][" << to_string(i) <<"]: ";
                hashTable[currentTable][i].printQueue(out);
            }
        }
    }

    void getSortedData(ofstream &out){
        for(int i = 0; i<tableSize; i++){
            while(!hashTable[currentTable][i].isEmpty()){
                listNode *temp = hashTable[currentTable][i].deleteFront();
                out << temp->data<< " ";
                delete temp;
            }
        }
    }
};

int main(int argc, char** argv){
	ifstream input;
    ofstream output1, output2;

    input.open(argv[1]);
    output1.open(argv[2]);
    output2.open(argv[3]);

    radixSort sort = radixSort();

    sort.firstReading(input); 
    input.close();

    input.open(argv[1]);
    sort.loadStack(input, output2);

    sort.RadixSort(output1, output2);

    input.close();
    output1.close();
    output2.close();
	
}