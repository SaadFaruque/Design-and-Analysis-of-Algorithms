#include<iostream>
#include<fstream>
#include<string>
using namespace std;
class scheduling{
	public:
		class node{
			public:
				int jobId;
				int jobTime;
				int dependentCount;
				node* next;
				node(int jobId,int jobTime,int dependentCount){
					this->jobId=jobId;
					this->jobTime=jobTime;
					this->dependentCount=dependentCount;
					this->next=NULL;
				}
				string toString(){
					return to_string(jobId)+" "+to_string(jobTime)+" "+to_string(dependentCount)+"\n";
				}
		};
		class Job{
			public:
				int jobTime;
				int onWhichProc;
				int onOpen;
				int parentCount;
				int dependentCount;
		};
		class Proc{
			public:
				int doWhichJob;
				int timeRemain;
		};
		int numNodes;
		int numProcs;
		int procUsed;
		int currentTime;
		Job* jobAry;
		Proc* procAry;
		node* open;
		int** adjMatrix;
		int* onGraphAry;
		int totalJobTimes;
		int** scheduleTable;
				
		void initialization(ifstream &infile1,ifstream &infile2){
			open=new node(0,0,0);
			infile1>>numNodes;
			cout<<"numNodes "<<numNodes<<endl;
			cout<<"Give the number of processors:";
			cin>>numProcs;
			if(numProcs<=0){
				cout<<"Needs 1 or more processors";
				exit(0) ;
			}else if(numProcs>numNodes)
				numProcs=numNodes;
			cout<<"numProcs "<<numProcs<<endl;
			jobAry=new Job[numNodes+1];
			procAry=new Proc[numProcs+1];
			adjMatrix=new int*[numNodes+1];
			scheduleTable=new int*[numProcs+1];
			for(int i=0;i<=numNodes;i++){
				adjMatrix[i]=new int[numNodes+1];
				for(int j=0;j<=numNodes;j++){
					adjMatrix[i][j]=0; 
				}				
			}
			loadMatrix(infile1);
			computeParentCount();
			computeDependentCount();
			totalJobTimes=constructJobAry(infile2);
			for(int i=0;i<=numProcs;i++){
				scheduleTable[i]=new int[totalJobTimes+1];
				for(int j=0;j<=totalJobTimes;j++){
					scheduleTable[i][j]=0;
				}
			}
			initiateGraphAry();
			procUsed=0;
			currentTime=0;
		}
		void loadMatrix(ifstream &infile1){
			int a,b;
			while(infile1>>a&&infile1>>b)
				adjMatrix[a][b]=1;	
		}
		void loadOpen(){
			int orphanNode=0;
			while(orphanNode!=-1){
				orphanNode=findOrphan();
				if(orphanNode>0){
					int jobId=orphanNode;
					int jobTime=jobAry[jobId].jobTime;
					node* newNode=new node(jobId,jobTime,jobAry[jobId].dependentCount);
					openInsert(newNode);
					jobAry[jobId].onOpen=1;
				}
			}
		}
		void loadProcAry(){
			int availProc=1;
			while(availProc>0&&open->next!=NULL&&procUsed<numProcs){
				availProc=findProcessor();
				if(availProc>0){
					procUsed++;
					node* newJob=open->next;
					open->next=open->next->next;
					int jobId=newJob->jobId;
					int jobTime=newJob->jobTime;
					procAry[availProc].doWhichJob=jobId;
					procAry[availProc].timeRemain=jobTime;
					putJobOnTable(availProc,currentTime,jobId,jobTime);
				}
			}
		}
		int constructJobAry(ifstream &infile2){
			int totalTime=0;
			int nodeID,jobTime;
			infile2>>numNodes;
			int i=0;
			while(i<numNodes){
				i++;
				infile2>>nodeID;
				infile2>>jobTime;
				totalTime+=jobTime;
				jobAry[nodeID].jobTime=jobTime;
				jobAry[nodeID].onWhichProc=-1;
				jobAry[nodeID].onOpen=0;
			}
			return totalTime;
		}
		void computeParentCount(){
			for(int nodeid=1;nodeid<=numNodes;nodeid++){
				jobAry[nodeid].parentCount=0;
				for(int i=1;i<=numNodes;i++)
					if(adjMatrix[i][nodeid]>0)
						jobAry[nodeid].parentCount++;
			}
		}
		void computeDependentCount(){
			for(int nodeid=1;nodeid<=numNodes;nodeid++){
				jobAry[nodeid].dependentCount=0;
				for(int j=1;j<=numNodes;j++)
					if(adjMatrix[nodeid][j]>0)
						jobAry[nodeid].dependentCount++;
			}
		}
		int findOrphan(){
			for(int i=1;i<=numNodes;i++){
				if(jobAry[i].parentCount<=0&&jobAry[i].onOpen==0&&jobAry[i].onWhichProc<=0)
					return i;
			}
			return -1;	
		}
		void openInsert(node* newNode){
			if(open->next==NULL)
				open->next=newNode;
			else{
				node* temp=open;
				bool inserted=false;
				while(temp->next!=NULL&&!inserted){
					if(newNode->dependentCount>=temp->next->dependentCount){
						newNode->next=temp->next;
						temp->next=newNode;
						inserted=true;
					}else
						temp=temp->next;	
				}
				if(!inserted)
					temp->next=newNode;
			}
		}
		void printList(ofstream &outfile2){
			node* temp=open;
			while(temp->next!=NULL){
				outfile2<<temp->next->toString();
				temp=temp->next;
			}
			
		}
		void printScheduleTable(ofstream &outfile1){
			outfile1<<"     ";
			for(int i=0;i<=totalJobTimes;i++)
				if(i<10)
					outfile1<<"-"<<i<<"--";
				else
					outfile1<<i<<"--";
			outfile1<<endl;
			for(int i=1;i<=numProcs;i++){
				outfile1<<"P("<<i<<")";
				for(int j=0;j<=totalJobTimes;j++){
					if(scheduleTable[i][j]<10)
						outfile1<<"| "<<scheduleTable[i][j]<<" ";
					else
						outfile1<<"|"<<scheduleTable[i][j]<<" ";
				}
				outfile1<<"\n     ";
				for(int j=0;j<=totalJobTimes;j++)
					outfile1<<"----";
				outfile1<<endl;
			}	
			outfile1<<endl;	
		}
		int findProcessor(){
			for(int i=1;i<=numProcs;i++){
				if(procAry[i].timeRemain<=0)
					return i;
			}
			return -1;
		}
		void putJobOnTable(int availProc,int currentTime,int jobId,int jobTime){
			int Time=currentTime;
			int endTime=Time+jobTime;
			while(Time<endTime){
				scheduleTable[availProc][Time]=jobId;
				Time++; 
			}
		}
		bool checkCycle(){
			return (open->next==NULL)&&!graphIsEmpty()&&allProcFinished();
		}
		void initiateGraphAry(){
			onGraphAry=new int[numNodes+1];
			for(int i=1;i<=numNodes;i++){
				onGraphAry[i]=1;
			}
		}
		bool graphIsEmpty(){
			for(int i=1;i<=numNodes;i++)
				if(onGraphAry[i]!=0)
					return false;
			return true;
		}
		bool allProcFinished(){
			for(int i=1;i<=numProcs;i++)
				if(procAry[i].timeRemain>0)
					return false;
			return true;
		}
		void updateProcTime(){
			for(int i=1;i<=numProcs;i++)
				if(procAry[i].timeRemain>0)
					procAry[i].timeRemain--;
		}
		int findDoneProc(){
			for(int i=1;i<=numProcs;i++){
				if(procAry[i].timeRemain<=0&&procAry[i].doWhichJob>0){
					int jobID=procAry[i].doWhichJob;
					procAry[i].doWhichJob=-3;
					return jobID;
				}
			}
			return -2;
		}
		void deleteEdge(int jobId){
			for(int i=1;i<=numNodes;i++){
				if(adjMatrix[jobId][i]>0){
					jobAry[i].parentCount--;
					adjMatrix[jobId][i]=0;
				}
			}	
		}						
};
int main(int argc,char* argv[]){
	ifstream infile1,infile2;
	ofstream outfile1,outfile2;
	infile1.open(argv[1]);
	infile2.open(argv[2]);
	outfile1.open(argv[3]);
	outfile2.open(argv[4]);
	scheduling* schedule=new scheduling();
	schedule->initialization(infile1,infile2);
	while(!schedule->graphIsEmpty()){
		schedule->loadOpen();
		schedule->printList(outfile2);
		schedule->loadProcAry();
		if(schedule->checkCycle()){
			cout<<"There is a cycle in the graph"<<endl;
			return 0;
		}
		schedule->printScheduleTable(outfile1);
		schedule->currentTime++;
		schedule->updateProcTime();
		outfile2<<endl;
		int jobId=1;
		while(jobId>0){
			jobId=schedule->findDoneProc();
			if(jobId>0){
				schedule->procUsed--;
				schedule->onGraphAry[jobId]=0;
				schedule->deleteEdge(jobId);
			}
		}
	}
	schedule->printScheduleTable(outfile1);
	infile1.close();
	infile2.close();
	outfile1.close();
	outfile2.close();
	return 0;
}
