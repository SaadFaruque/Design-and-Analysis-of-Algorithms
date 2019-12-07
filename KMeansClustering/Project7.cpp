#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <cmath>
using namespace std;

class xyCoord;

class KMean {
public:
    class Point {
    public:
        int xCoord, yCoord, label;
        double distance;
    };

    class xyCoord {
    public:
        double xCoord, yCoord, label;
    };

    int K, numPts = 0, numRows, numCols, minVal, maxVal, change = 0;
    Point *pointSet;
    int **imgAry;
    xyCoord *KCentroids;

    KMean(int nRows, int nCols, int minV, int maxV, int K) {
        imgAry = new int*[nRows];
        for (int i = 0; i < nRows; i++) imgAry[i] = new int[nCols];

        KCentroids = new xyCoord[K + 1];
        numRows = nRows;
        numCols = nCols;
        minVal = minV;
        maxVal = maxV;
        this->K = K;
    }

    int extractPts(ifstream &inFile, ofstream &outFile) {
        int point;

        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                inFile >> point;
                if (point > 0) {
                    numPts++;
                    outFile << i << ' ' << j << endl;
                }
            }
        }
        pointSet = new Point[numPts];
        return numPts;
    }

    void loadPointSet(ifstream &outFile) {
        for (int i = 0; i < numPts; i++) {
            outFile >> pointSet[i].xCoord >> pointSet[i].yCoord;
            pointSet[i].distance = 9999.0;
        }
    }

    void assignLabel(Point pointSet[], int K) {
        int KCount = 1;
        for (int i = 0; i < numPts; i++) {
            pointSet[i].label = KCount;
            KCount++;
            if (KCount > K) KCount = 1;
        }
    }

    void point2Image(Point pointSet[], int **imgAry) {
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                imgAry[i][j] = 0;
            }
        }
        for (int i = 0; i < numPts; i++) {
            int j = pointSet[i].xCoord;
            int k = pointSet[i].yCoord;
            imgAry[j][k] = pointSet[i].label;
        }
    }

    void printImage(ofstream &outFile, int iteration) {
        outFile << "*** Result of iteration " << iteration << " ****\n\n";
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                if (imgAry[i][j] > 0) outFile << imgAry[i][j] << ' ';
                else { outFile << ' '; }
            }
            outFile << endl;
        }
        outFile << "\n\n";
    }

    void computeCentroids() {
        double *sumX = new double[K + 1];
        double *sumY = new double[K + 1];
        double *totalPt = new double[K + 1];
        int index = 0, label;

        for (int i = 0; i < K; i++) {
            sumX[i] = 0;
            sumY[i] = 0;
            totalPt[i] = 0;
        }

        for (int index = 0; index < numPts; index++) {
            label = pointSet[index].label;
            sumX[label] += pointSet[index].xCoord;
            sumY[label] += pointSet[index].yCoord;
            totalPt[label]++;
        }

        label = 1;

        for (label = 1; label <= K; label++) {
            KCentroids[label].xCoord = (sumX[label]/ totalPt[label]);
            KCentroids[label].yCoord = (sumX[label]/ totalPt[label]);
        }

    }

    double computeDist(Point p1, xyCoord p2) {
        int xSquared = (p2.xCoord - p1.xCoord) * (p2.xCoord - p1.xCoord);
        int ySquared = (p2.yCoord - p1.yCoord) * (p2.yCoord - p1.yCoord);
        double distance = sqrt(xSquared + ySquared);
        return distance;
    }

    int DistanceMinLabel(Point &p, xyCoord KCent[], int &change) {
        double minDist = 9999;
        int minLabel = 0, label = 1;

        while (label <= K) {
            double dist = computeDist(p, KCent[label]);
            if (dist < minDist) {
                minLabel = label;
                minDist = dist;
            }
            label++;
        }

        if (minDist < p.distance) {
            p.distance = minDist;
            p.label = minLabel;
            change++;
        }
        return change;
    }

    void kMeansClustering(ofstream &outFile, Point pointSet[], int K) {
        int iteration = 1, change, index = 0;
        assignLabel(pointSet, K);

        do {
            point2Image(pointSet, imgAry);
            printImage(outFile, iteration);
            change = 0;
            computeCentroids();
            while (index < numPts) {
                change = DistanceMinLabel(pointSet[index], KCentroids, change);
                index++;
            }
            index = 0;
            iteration++;
        } while(change != 0);

    }

    void writePtSet(ofstream &outFile) {
        outFile << numPts << " data points\n";
        outFile << numRows << " rows and " << numCols << " columns\n";

        for (int i = 0; i < numPts; i++) {
            outFile << pointSet[i].xCoord << ' ' << pointSet[i].yCoord << ' ' << pointSet[i].label << endl;
        }
    }
};

int main(int argc, char** argv) {
    ifstream inFile;
    ofstream outFile1, outFile2, outFile3;
    int numRows, numCols, minVal, maxVal, K;


    inFile.open(argv[1]);

    if (inFile.good()) {
        string header;
        getline(inFile, header);
        istringstream hCopy(header);
        hCopy >> skipws >> numRows >> numCols >> minVal >> maxVal;
    }

    cout << "Entere a value K:";
    cin >> K;
    KMean test = KMean(numRows, numCols, minVal, maxVal, K);

    outFile1.open(argv[2]);
    outFile2.open(argv[3]);
    outFile3.open(argv[4]);

    test.numPts = test.extractPts(inFile, outFile1);

    outFile1.close();
    ifstream outFile_1;
    outFile_1.open(argv[2]);
    test.loadPointSet(outFile_1);
    test.kMeansClustering(outFile2, test.pointSet, test.K);
    test.writePtSet(outFile3);

    for (int i = 0; i < test.numRows; i++) delete[] test.imgAry[i];
    delete[] test.imgAry;
    delete[] test.pointSet;
    delete[] test.KCentroids;

    inFile.close();
    outFile_1.close();
    outFile2.close();
    outFile3.close();
    return 0;
}
