#include<iostream>
#include<fstream>
#include<cstdlib>
#include<string>
using namespace std;

class DijkstraSSS{
	public:
	int numNodes;
	int sourceNode;
	int minNode;
	int currentNode;
	int newCost;
	int** costMatrix;
	int* fatherAry;
	int* markedAry;
	int* bestCostAry;
		
	void loadCostMatrix(ifstream &inputfile){
		inputfile>>numNodes;
		costMatrix=new int*[numNodes+1];
		for(int i=0;i<numNodes+1;i++){
			costMatrix[i]=new int[numNodes+1];
			for(int j=0;j<numNodes+1;j++){
				if(i==j)
					costMatrix[i][j]=0;
				else
					costMatrix[i][j]=99999;
			}
		}
		int arr[3];
		int i=0;
		while(inputfile>>arr[i++]){
			if(i==3){
				i=0;
				costMatrix[arr[0]][arr[1]]=arr[2];
			}
		}
	} 
	void setBestCostAry(int sourceNode){
		bestCostAry=new int[numNodes+1];
		for(int i=0;i<numNodes+1;i++)
			bestCostAry[i]=costMatrix[sourceNode][i];
	}  
	void setFatherAry(){
		fatherAry=new int[numNodes+1];
		for(int i=0;i<numNodes+1;i++)
			fatherAry[i]=i;
	} 
	void setMarkedAry(){
		markedAry=new int[numNodes+1];
		for(int i=0;i<numNodes+1;i++)
			markedAry[i]=0;
	}
	int findMinNode(){
		int minCost=99999;
		minNode=0;
		int index=1;
		while(index<=numNodes){
			if(markedAry[index]==0&&bestCostAry[index]<minCost){
				minCost=bestCostAry[index];
				minNode=index;
			}
			index++;
		} 
		return minNode;
	}
	int computeCost(int minNode,int currentNode){
		return bestCostAry[minNode]+costMatrix[minNode][currentNode];
	}
	void debugPrint(ofstream &outfile2){
		outfile2<<"\n*****************************\n";
		outfile2<<" sourceNode is: "<<sourceNode;
		outfile2<<"\nfatherAry is:\n";
		for(int i=1;i<numNodes+1;i++)	
			outfile2<<fatherAry[i]<<" ";
		outfile2<<"\nbestCostAry is:\n";	
		for(int i=1;i<numNodes+1;i++)	
			outfile2<<bestCostAry[i]<<" ";
		outfile2<<"\nmarkedAry is:\n";
		for(int i=1;i<numNodes+1;i++)	
			outfile2<<markedAry[i]<<" ";
	}
	void printShortestPath(int currentNode,int sourceNode,ofstream &outfile1){
		outfile1<<"The path from "<<sourceNode<<" to "<<currentNode<<":"<<currentNode;
		int temp=currentNode;
		while(fatherAry[temp]!=temp&&fatherAry[temp]!=sourceNode){
			outfile1<<"->"<<fatherAry[temp];
			temp=fatherAry[temp]; 
		}
		outfile1<<"->"<<fatherAry[temp]<<", cost="<<bestCostAry[currentNode]<<endl; 
		
	}
	bool allMarked(){
		for(int i=1;i<numNodes+1;i++)
			if(markedAry[i]==0)
				return false;
		return true;
	}
};

int main(int argc, char *argv[]){
	ifstream infile;
	ofstream outfile1,outfile2;
	infile.open(argv[1]);
	outfile1.open(argv[2]);
	outfile2.open(argv[3]);
	DijkstraSSS* path=new DijkstraSSS();
	path->loadCostMatrix(infile);
	path->sourceNode=1;
	while(path->sourceNode<=path->numNodes){
		path->setBestCostAry(path->sourceNode);
		path->setFatherAry();
		path->setMarkedAry();
		while(!path->allMarked()){
			path->minNode=path->findMinNode();
			path->markedAry[path->minNode]=1;
			path->debugPrint(outfile2); 
			path->currentNode=1;
			while(path->currentNode<=path->numNodes){
				if(path->markedAry[path->currentNode]==0){
					int newCost=path->computeCost(path->minNode,path->currentNode);
					if(newCost<=path->bestCostAry[path->currentNode]){
						path->bestCostAry[path->currentNode]=newCost;
						path->fatherAry[path->currentNode]=path->minNode;
						path->debugPrint(outfile2);
					}
				}
				path->currentNode++;
			}
		}
		path->currentNode=1;
		outfile1<<"the sourceNode is "<<path->sourceNode<<endl;
		while(path->currentNode<=path->numNodes){
			path->printShortestPath(path->currentNode,path->sourceNode,outfile1);
			path->currentNode++;
		}
		outfile1<<"*************"<<endl;
		path->sourceNode++;
	}
	infile.close();
	outfile1.close();
	outfile2.close();
	return 0;
}





