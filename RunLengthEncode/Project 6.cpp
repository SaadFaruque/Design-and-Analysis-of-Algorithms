#include<iostream>
#include<cstdlib>
#include<string> 
#include<fstream>
#include <sstream> 
using namespace std;
class runLength{
public:
	int firstGrayScale;
	int numRows=1;
	int numCols=1;
	int minVal;
	int maxVal;
	int method;
	int totalRuns=0;
	string nameEncodeFile;
	string nameDecodeFile;
	void getHeader(ifstream &image,string inputfile){
		image.open(inputfile);
		int arr[5];
		int i=0;
		while(i<5&&image>>arr[i++]);
		numRows=arr[0];
		numCols=arr[1];
		minVal=arr[2];
		maxVal=arr[3];
		firstGrayScale=arr[4];	
		image.close();
	}
	runLengthEncode(ifstream &image,string input,ofstream &encode,int Method,ofstream &record){
		method=Method;
		switch(Method){
			case 1 :
				encodeMethod1(image,input,encode,record);
				break;
			case 2 :
				encodeMethod2(image,input,encode,record);
				break;
			case 3 :
				encodeMethod3(image,input,encode,record);
				break;
			case 4 :
				encodeMethod4(image,input,encode,record);
				break;
			default:
				break;
		}
	}
	void recordFile(ofstream &record,int method,int runs){
		record<<"Method "<<method<<" was uselected\n"
		<<runs<<" runs produced in run-Length encoding\n"
		<<runs*16<<" bytes used\n";
	}
	void encodeMethod1(ifstream &image,string input,ofstream &encodefile,ofstream &record){
		image.open(input);
		int x;
		int i=0;
		while(i++<4&&image>>x);
		int r=0;
		while(r<numRows){
			int c=1;
			int start=0;
			int count=1;
			image>>x;
			int curScale=x;
			while(c<numCols){
				image>>x;
				if(x==curScale){
					count++;
					c++;
				}else{
					encodefile<<r<<" "<<start<<" "<<curScale<<" "<<count<<endl;
					totalRuns++;
					count=1;
					curScale=x;
					start=c;
					c++;
				}
			}
			encodefile<<r<<" "<<start<<" "<<curScale<<" "<<count<<endl;
			totalRuns++;
			r++;
		}
		recordFile(record,method,totalRuns);
		image.close();
		encodefile.close();
	}
	void encodeMethod2(ifstream &image,string input,ofstream &encodefile,ofstream &record){
		image.open(input);
		int x;
		int i=0;
		while(i++<4&&image>>x);
		int r=0;
		while(r<numRows){
			int c=1;
			int start=0;
			int count=1;
			image>>x;
			int curScale=x;
			while(c<numCols){
				image>>x;
				if(x==curScale){
					count++;
					c++;
				}else{
					if(curScale!=0){
						encodefile<<r<<" "<<start<<" "<<curScale<<" "<<count<<endl;
						totalRuns++;
					}
					count=1;
					curScale=x;
					start=c;
					c++;
				}
			}
			if(curScale!=0){
				encodefile<<r<<" "<<start<<" "<<curScale<<" "<<count<<endl;
				totalRuns++;
			}
			r++;
		}
		recordFile(record,method,totalRuns);
		image.close();
		encodefile.close();
	}
	void encodeMethod3(ifstream &image,string input,ofstream &encodefile,ofstream &record){
		image.open(input);
		int i=0,x;
		while(i++<4&&image>>x);
		int r=0,c=1,count=1,start=0,curRow=0;
		image>>x;
		int curScale=x;
		while(r<20){	
			while(c<20){
				image>>x;
				if(x==curScale){
					count++;
					c++;
				}else{
					encodefile<<curRow<<" "<<start<<" "<<curScale<<" "<<count<<endl;
					totalRuns++;
					curRow=r;
					count=1;
					curScale=x;
					start=c;
					c++;
				}
			}
			r++;
			c=0;
		}
		encodefile<<curRow<<" "<<start<<" "<<curScale<<" "<<count<<endl;
		totalRuns++;
		recordFile(record,method,totalRuns);
		image.close();
		encodefile.close();
	}
	void encodeMethod4(ifstream &image,string input,ofstream &encodefile,ofstream &record){
		image.open(input);
		int i=0,x;
		while(i++<4&&image>>x);
		int r=0,c=1,count=1,start=0,curRow=0;
		image>>x;
		int curScale=x;
		while(r<20){	
			while(c<20){
				image>>x;
				if(x==curScale){
					count++;
					c++;
				}else{
					if(curScale!=0){
						encodefile<<curRow<<" "<<start<<" "<<curScale<<" "<<count<<endl;
						totalRuns++;
					}
					curRow=r;
					count=1;
					curScale=x;
					start=c;
					c++;
				}
			}
			r++;
			c=0;
		}
		if(curScale!=0){
			encodefile<<curRow<<" "<<start<<" "<<curScale<<" "<<count<<endl;
			totalRuns++;
		}
		recordFile(record,method,totalRuns);
		image.close();
		encodefile.close();
	}
	void runLengthDecode(ifstream &encode,string encoded,ofstream &decode,string decoded){	
		encode.open(encoded);
		decode.open(decoded);
		int i=0;
		int x;
		while(i++<4){
			encode>>x;
			decode<<x<<" ";
		}
		encode>>x;
		decode<<endl;
		int approach=x;
		switch(approach){
			case 1 :
				decodeMethod1(encode,decode);
				break;
			case 2 :
				decodeMethod2(encode,decode);
				break;
			case 3 :
				decodeMethod3(encode,decode);
				break;
			case 4 :
				decodeMethod4(encode,decode);
				break;
			default:
				break;
		}
	}
	void decodeMethod1(ifstream &encode,ofstream &decode){
		int r=0,c=0;
		while(r<numRows){
			int i=0;
			int arr[4];
			while(i<4)
			encode>>arr[i++];
			for(int j=0;j<arr[3];j++){
				decode<<arr[2]<<" ";
				c++;
			}
			if(c==numCols){
				decode<<endl;
				c=0;
				r++;
			}
		}
		encode.close();
		decode.close();
	}
	void decodeMethod2(ifstream &encode,ofstream &decode){
		int r=0,c=0;
		while(r<numRows){
			int i=0;
			int arr[4];
			while(i<4&&totalRuns>0)
				encode>>arr[i++];
			totalRuns--;
			if(totalRuns==0){
				while(r<numRows){
					for(int j=c;j<numCols;j++)
						decode<<0<<" ";
					decode<<endl;
					c=0;
					r++;
				}
				break;
			}
			while(r<arr[0]){
				for(int j=c;j<numCols;j++)
					decode<<0<<" ";
				decode<<endl;
				r++;
				c=0;
			}
			while(c<arr[1]){
				decode<<0<<" ";
				c++;
				}
			while(c<arr[1]+arr[3]){
				decode<<arr[2]<<" ";
				c++;
			}
			if(c>=numCols){
				r++;
				c=0;
				decode<<endl;
			}
		}
		encode.close();
		decode.close();
	}
	void decodeMethod3(ifstream &encode,ofstream &decode){
		int r=0,c=0,i=0;
		int arr[4];
		while(r<numRows){
			while(i<4)
				encode>>arr[i++];
			i=0;
			for(int j=0;j<arr[3];j++){
				decode<<arr[2]<<" ";
				c++;
				if(c>=numCols){
					decode<<endl;
					r++;
					c=0;
				}
			}		
		}
		encode.close();
		decode.close();
	}
	void decodeMethod4(ifstream &encode,ofstream &decode){
		int r=0,c=0,i=0;
		int arr[4];
		while(r<numRows){
			while(i<4&&totalRuns>0)
				encode>>arr[i++];
			i=0;
			totalRuns--;
			if(totalRuns==0){
				while(r++<numRows){
					while(c++<numCols)
						decode<<0<<" ";
					decode<<endl;
					c=0;
				}
				break;
			}
			while(r<arr[0]){
				for(int j=c;j<numCols;j++)
					decode<<0<<" ";
				decode<<endl;
				r++;
				c=0;
			}
			while(c<arr[1]){
				decode<<0<<" ";
				c++;
			}
				while(arr[3]>0){
				decode<<arr[2]<<" ";
				arr[3]--;
				c++;
				if(c>=numCols){
					decode<<endl;
					c=0;
					r++;
				}
			}
		}
	}
};
int main(int argc, char *argv[]){
	ifstream imageFile;
	ofstream outfile;
	outfile.open(argv[2]);
	cout<<"Method 1 is with zero and not wrap around.\nMethod 2 is no zero and no wrap around.\nMethod 3 is with zero and wrap around.\nMethod 4 is no zero and wrap around.\n\n\n";
 	while(true){
		cout<<"Do you want to compress an image file, Y for yes, N for no:"<<endl;
		char yesNo;
		cin>>yesNo;
		int method;
		if(yesNo=='Y'){
			while(true){
				cout<<"Enter a number between 1 and 4 to choose your method.\n";
				cin>>method;
				if(method>=1&&method<=4)
					break;
			}
			runLength* rl=new runLength();
			rl->method=method; 
			stringstream conca;
			conca<<argv[1]<<"_EncodeMethod"<<method<<".txt";
			rl->nameEncodeFile=conca.str();
			ofstream encodeFile;
			encodeFile.open(rl->nameEncodeFile);
			rl->getHeader(imageFile,argv[1]);
			encodeFile<<rl->numRows<<" "<<rl->numCols<<" "<<rl->minVal<<" "<<rl->maxVal
			<<"\n"<<method<<endl;
			rl->runLengthEncode(imageFile,argv[1],encodeFile,method,outfile);
			stringstream ssconca;
			ssconca<<argv[1]<<"EncodeMethod"<<method<<"_Decoded.txt";
			rl->nameDecodeFile=ssconca.str();	
			ifstream encodedfile;
			ofstream decodefile;
			rl->runLengthDecode(encodedfile,rl->nameEncodeFile,decodefile,rl->nameDecodeFile);
		}else{
			outfile.close();
			return 0;
		}
	}
	
}

