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

struct node
{
    node(int nodeIndex, int posX, int posY, int nodePredecessor, int nodeDistance, int nodePriority)
    {
        index = nodeIndex;
        nodeX = posX;
        nodeY = posY;
        predecessorIndex = nodePredecessor;
        distance = nodeDistance;
        priority = nodePriority;
    }

    node()
    {
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

    bool operator >(const node& n) const;
};

bool node::operator >(const node& n) const
{
    return (priority > n.priority);
}

int FindPath(const int nStartX, const int nStartY, const int nTargetX, const int nTargetY,
             const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
             int* pOutBuffer, const int nOutBufferSize)
{
    int obstacleValue = 0;
    int startIndex = getIndexFromPoint(nStartX, nStartY, nMapWidth);
    int targetIndex = getIndexFromPoint(nTargetX, nTargetY, nMapWidth);
    int numberOfIndices = nMapWidth * nMapHeight;

    //Guard clauses
    if(startIndex == targetIndex)
    {
        return 0;
    }
    if(targetIndex > numberOfIndices || targetIndex < 0)
    {
        return -1;
    }

    priority_queue<node, vector<node>, greater<node>> priorityQueue;
    int startingPriority = abs(nStartX - nTargetX) + abs(nStartY - nTargetY);
    node startingNode(startIndex, nStartX, nStartY, startIndex, 0, startingPriority);
    priorityQueue.push(startingNode);

    unordered_set<int> visitedIndex;
    unordered_map<int, int> predecessors;
    while(!priorityQueue.empty())
    {
        node currentNode = priorityQueue.top();
        priorityQueue.pop();
        int currentIndex = currentNode.index;

        if(visitedIndex.find(currentIndex) != visitedIndex.end())
        {
            continue;
        }

        predecessors.insert(std::make_pair(currentIndex, currentNode.predecessorIndex));

        if(currentIndex == targetIndex)
        {
            if(currentNode.distance <= nOutBufferSize)
            {
                int predecessor = currentIndex;
                for(int i = currentNode.distance - 1; i >= 0; i--)
                {
                    pOutBuffer[i] = predecessor;
                    unordered_map<int, int>::const_iterator iterator = predecessors.find(predecessor);
                    predecessor = iterator->second;
                }
            }
            return currentNode.distance;
        }

        int currentX = currentNode.nodeX;
        int currentY = currentNode.nodeY;

        int indexUp = currentIndex - nMapWidth;
        int upX = currentX;
        int upY = currentY - 1;
        if((visitedIndex.find(indexUp) == visitedIndex.end()) && (indexUp >= 0) && (pMap[indexUp] != obstacleValue))
        {
            int neighborDistance = currentNode.distance + pMap[indexUp];
            int priority = neighborDistance + abs(upX - nTargetX) + abs(upY - nTargetY);
            node newNode(indexUp, upX, upY, currentIndex, neighborDistance, priority);
            priorityQueue.push(newNode);
        }

        int indexLeft = currentIndex - 1;
        int leftX = currentX - 1;
        int leftY = currentY;
        if((visitedIndex.find(indexLeft) == visitedIndex.end()) && (leftX >= 0) && (pMap[indexLeft] != obstacleValue))
        {
            int neighborDistance = currentNode.distance + pMap[indexLeft];
            int priority = neighborDistance + abs(leftX - nTargetX) + abs(leftY - nTargetY);
            node newNode(indexLeft, leftX, leftY, currentIndex, neighborDistance, priority);
            priorityQueue.push(newNode);
        }

        int indexDown = currentIndex + nMapWidth;
        int downX = currentX;
        int downY = currentY + 1;
        if((visitedIndex.find(indexDown) == visitedIndex.end()) && (indexDown < numberOfIndices) && (pMap[indexDown] != obstacleValue))
        {
            int neighborDistance = currentNode.distance + pMap[indexDown];
            int priority = neighborDistance + abs(downX - nTargetX) + abs(downY - nTargetY);
            node newNode(indexDown, downX, downY, currentIndex, neighborDistance, priority);
            priorityQueue.push(newNode);
        }

        int indexRight = currentIndex + 1;
        int rightX = currentX + 1;
        int rightY = currentY;
        if((visitedIndex.find(indexRight) == visitedIndex.end()) && (rightX < nMapWidth) && (pMap[indexRight] != obstacleValue))
        {
            int neighborDistance = currentNode.distance + pMap[indexRight];
            int priority = neighborDistance + abs(rightX - nTargetX) + abs(rightY - nTargetY);
            node newNode(indexRight, rightX, rightY, currentIndex, neighborDistance, priority);
            priorityQueue.push(newNode);
        }
        visitedIndex.insert(currentIndex);
    }

    return -1;
}

int getIndexFromPoint(int x, int y, int mapWidth)
{
    return ((y*mapWidth) + x);
}

int main()
{
    // My solution to https://paradox.kattis.com/problems/paradoxpath
    cout << "Starting pathfinding tests\n\n";

    // Test case 1
    {
        cout << "Testing no vaild path:";
        unsigned char pMap[] = {1, 1, 1, 1,
                                0, 1, 0, 0,
                                0, 1, 0, 1};
        int pOutBuffer[12];
        int distance = FindPath(0, 0, 3, 2, pMap, 4, 3, pOutBuffer, 12);
        if(distance == -1)
        {
            cout << " - Pass.\n";
        }
        else
        {
            cout << " - Fail.\n";
        }
    }

    // Test case 2
    {
        cout << "Testing unreachable target:";
        unsigned char pMap[] = {1, 1, 1, 1,
                                0, 1, 0, 1,
                                0, 1, 1, 1};
        int pOutBuffer[12];
        int distance = FindPath(0, 0, 2, 1, pMap, 4, 3, pOutBuffer, 12);
        if(distance == -1)
        {
            cout << " - Pass.\n";
        }
        else
        {
            cout << " - Fail.\n";
        }
    }

    // Test case 3
    {
        cout << "Testing start equals target:";
        unsigned char pMap[] = {1, 1, 1, 1,
                                0, 1, 0, 1,
                                0, 1, 1, 1};
        int pOutBuffer[12];
        int distance = FindPath(1, 1, 1, 1, pMap, 4, 3, pOutBuffer, 12);
        if(distance == 0)
        {
            cout << " - Pass.\n";
        }
        else
        {
            cout << " - Fail.\n";
        }
    }

    // Test case 4
    {
        cout << "Testing out-of-bounds target:";
        unsigned char pMap[] = {1, 1, 1, 1,
                                0, 1, 0, 1,
                                0, 1, 1, 1};
        int pOutBuffer[12];
        int distance = FindPath(1, 1, 4, 3, pMap, 4, 3, pOutBuffer, 12);
        if(distance == -1)
        {
            cout << " - Pass.\n";
        }
        else
        {
            cout << " - Fail.\n";
        }
    }

    // Test case 5
    {
        cout << "Testing shortest path:";
        unsigned char pMap[] = {1, 1, 1, 1,
                                0, 1, 0, 1,
                                0, 1, 1, 1};
        int pOutBuffer[3] = {0};
        int distance = FindPath(1, 1, 2, 0, pMap, 4, 3, pOutBuffer, 3);
        if(distance == 2 && pOutBuffer[0] == 1 && pOutBuffer[1] == 2 && pOutBuffer[2] == 0)
        {
            cout << " - Pass.\n";
        }
        else
        {
            cout << " - Fail.\n";
        }
    }

    // Test case 6
    {
        cout << "Testing complex path:";
        unsigned char pMap[] = {1, 1, 1, 1, 1, 1, 1,
                                0, 1, 0, 0, 0, 0, 1,
                                1, 1, 0, 0, 0, 0, 1,
                                1, 0, 1, 1, 1, 0, 1,
                                1, 1, 1, 0, 1, 1, 1};
        int pOutBuffer[11] = {0};
        int distance = FindPath(0, 0, 4, 3, pMap, 7, 5, pOutBuffer, 11);
        if(distance == 11 && pOutBuffer[0] == 1 && pOutBuffer[1] == 8)
        {
            cout << " - Pass.\n";
        }
        else
        {
            cout << " - Fail.\n";
            cout << "Path found was: " << endl;
            for(int i = 0; i < sizeof(pOutBuffer) / sizeof(int); i++)
            {
                cout << pOutBuffer[i] << ", ";
            }
            cout << endl;
        }
    }

    // Test case 7
    {
        cout << "Testing complex path again:";
        unsigned char pMap[] = {1, 1, 1, 1, 1, 1, 1,
                                0, 1, 0, 0, 0, 0, 1,
                                1, 1, 0, 0, 0, 0, 1,
                                1, 0, 1, 1, 1, 0, 1,
                                1, 1, 1, 0, 1, 1, 1};
        int pOutBuffer[11] = {0};
        int distance = FindPath(0, 0, 5, 4, pMap, 7, 5, pOutBuffer, 11);
        if(distance == 11 && pOutBuffer[0] == 1 && pOutBuffer[1] == 2)
        {
            cout << " - Pass.\n";
        }
        else
        {
            cout << " - Fail.\n";
            cout << "Path found was: " << endl;
            for(int i = 0; i < sizeof(pOutBuffer) / sizeof(int); i++)
            {
                cout << pOutBuffer[i] << ", ";
            }
            cout << endl;
        }
    }

    // Test case 7
    {
        // Paradox instructions say that tests won't look at buffer if path is longer than buffer size
        // so our program can skip writting to the buffer to save time
        cout << "Testing too small output buffer:";
        unsigned char pMap[] = {1, 1, 1, 1, 1, 1, 1,
                                0, 1, 0, 0, 0, 0, 1,
                                1, 1, 0, 0, 0, 0, 1,
                                1, 0, 1, 1, 1, 0, 1,
                                1, 1, 1, 0, 1, 1, 1};
        int pOutBuffer[11] = {0};
        int distance = FindPath(0, 0, 5, 4, pMap, 7, 5, pOutBuffer, 10);
        if(distance == 11 && pOutBuffer[0] == 0 && pOutBuffer[1] == 0)
        {
            cout << " - Pass.\n";
        }
        else
        {
            cout << " - Fail.\n";
            cout << "Path found was: " << endl;
            for(int i = 0; i < sizeof(pOutBuffer) / sizeof(int); i++)
            {
                cout << pOutBuffer[i] << ", ";
            }
            cout << endl;
        }
    }

    // Pause output window to view results
    cout << "\nTesting Complete - Press any key to exit.";
    cin.get();
}
