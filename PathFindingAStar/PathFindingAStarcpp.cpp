#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <functional>
#include <iostream>

using namespace std;

int FindPath(const int nStartX, const int nStartY, const int nTargetX, const int nTargetY,
	const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
	int* pOutBuffer, const int nOutBufferSize);
int getIndexFromPoint(int x, int y, int mapWidth);

/* Fun Fact: using pointers to previous nodes has a bug with a STL as the pointer becomes set to 
the first node in the STL and will then always point to itself on the pop in an infinite loop since the pointers
are never updated on push and pop functions. Would need to create my own priority queue or I could just use a hashtable to 
hold the pair of an index and it's predecessor*/
struct node {
	node() {
		index = 0;
		nodeX = 0;
		nodeY = 0;
		predecessorIndex = 0;
		distance = 0;
		priority = 0;
	}

	int index;
	int nodeX;
	int nodeY;
	int predecessorIndex;
	int distance;
	int priority;
};

bool operator >(const node& a, const node& b) {
	return (a.priority > b.priority);
}

int FindPath(const int nStartX, const int nStartY, const int nTargetX, const int nTargetY,
	const unsigned char* pMap, const int nMapWidth, const int nMapHeight, 
	int* pOutBuffer, const int nOutBufferSize) {

	int startIndex = getIndexFromPoint(nStartX, nStartY, nMapWidth);
	int targetIndex = getIndexFromPoint(nTargetX, nTargetY, nMapWidth);
	int numberOfIndices = nMapWidth * nMapHeight;

	unordered_set<int> visitedIndex;
	unordered_map<int, int> predecessors;

	priority_queue<node, vector<node>, greater<node>> priorityQueue;

	node startingNode;
	startingNode.index = startIndex;
	startingNode.nodeX = nStartX;
	startingNode.nodeY = nStartY;
	startingNode.predecessorIndex = startIndex;
	startingNode.distance = 0;
	startingNode.priority = abs(nStartX - nTargetX) + abs(nStartY - nTargetY);

	priorityQueue.push(startingNode);

	while (!priorityQueue.empty()) {
		node currentNode = priorityQueue.top();
		priorityQueue.pop();
		int currentIndex = currentNode.index;

		if (visitedIndex.find(currentIndex) != visitedIndex.end()) {
			continue;
		}

		predecessors.insert(std::make_pair(currentIndex, currentNode.predecessorIndex));

		if (currentIndex == targetIndex) {
			if (currentNode.distance <= nOutBufferSize) {
				int predecessor = currentIndex;
				for (int i = currentNode.distance - 1; i >= 0; i--) {
					pOutBuffer[i] = predecessor;
					unordered_map<int, int>::const_iterator iterator = predecessors.find(predecessor);
					predecessor = iterator->second;
				}
			}
			return currentNode.distance;
		}

		int currentX = currentNode.nodeX;
		int currentY = currentNode.nodeY;
		int neighborDistance = currentNode.distance + 1;

		int indexUp = currentIndex - nMapWidth;
		int upX = currentX;
		int upY = currentY - 1;
		if ((visitedIndex.find(indexUp) == visitedIndex.end()) && (indexUp >= 0)  && (pMap[indexUp] != 0)) {
			node newNode;
			newNode.index = indexUp;
			newNode.nodeX = upX;
			newNode.nodeY = upY;
			newNode.predecessorIndex = currentIndex;
			newNode.distance = neighborDistance;
			newNode.priority = neighborDistance + abs(upX - nTargetX) + abs(upY - nTargetY);
			priorityQueue.push(newNode);
		}

		int indexLeft = currentIndex - 1;
		int leftX = currentX - 1;
		int leftY = currentY;
		if ((visitedIndex.find(indexLeft) == visitedIndex.end()) && (leftX >= 0) && (pMap[indexLeft] != 0)) {
			node newNode;
			newNode.index = indexLeft;
			newNode.nodeX = leftX;
			newNode.nodeY = leftY;
			newNode.predecessorIndex = currentIndex;
			newNode.distance = neighborDistance;
			newNode.priority = neighborDistance + abs(leftX - nTargetX) + abs(leftY - nTargetY);
			priorityQueue.push(newNode);
		}

		int indexDown = currentIndex + nMapWidth;
		int downX = currentX;
		int downY = currentY + 1;
		if ((visitedIndex.find(indexDown) == visitedIndex.end()) && (indexDown < numberOfIndices) && (pMap[indexDown] != 0)) {
			node newNode;
			newNode.index = indexDown;
			newNode.nodeX = downX;
			newNode.nodeY = downY;
			newNode.predecessorIndex = currentIndex;
			newNode.distance = neighborDistance;
			newNode.priority = neighborDistance + abs(downX - nTargetX) + abs(downY - nTargetY);
			priorityQueue.push(newNode);
		}

		int indexRight = currentIndex + 1;
		int rightX = currentX + 1;
		int rightY = currentY;
		if ((visitedIndex.find(indexRight) == visitedIndex.end()) && (rightX < nMapWidth) && (pMap[indexRight] != 0)) {
			node newNode;
			newNode.index = indexRight;
			newNode.nodeX = rightX;
			newNode.nodeY = rightY;
			newNode.predecessorIndex = currentIndex;
			newNode.distance = neighborDistance;
			newNode.priority = neighborDistance + abs(rightX - nTargetX) + abs(rightY - nTargetY);
			priorityQueue.push(newNode);
		}
		visitedIndex.insert(currentIndex);
	}

	return -1;
}

int getIndexFromPoint(int x, int y, int mapWidth) {
	return ((y*mapWidth) + x);
}

/* int main() {
	// My solution to https://paradox.kattis.com/problems/paradoxpath
	cout << "Testing pathfinding \n";
	unsigned char pMap[] = { 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1 };
	int pOutBuffer[12];
	int dist1 = FindPath(0, 0, 1, 2, pMap, 4, 3, pOutBuffer, 12);

	cout << "Shortest Distance: " << dist1 << endl;
	cout << "Path Found " << endl;
	for (int i = 0; i < dist1; i++) {
		cout << pOutBuffer[i] << ", ";
	}
	cout << endl;

	unsigned char pMap2[] = { 0, 0, 1, 0, 1, 1, 1, 0, 1 };
	int pOutBuffer2[7];
	int dist2 = FindPath(2, 0, 0, 2, pMap2, 3, 3, pOutBuffer2, 7);

	cout << "Shortest Distance: " << dist2 << endl;
	cout << "Path Found " << endl;
	for (int i = 0; i < dist2; i++) {
		cout << pOutBuffer2[i] << ", ";
	}
	cout << endl;
	cin.get();
} */