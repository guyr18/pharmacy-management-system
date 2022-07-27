#include <queue>
#include <vector>
#include <iostream>
using namespace std;

int main()
{

priority_queue<int> pq;
vector<int> testData = {9, 5, 6, 2, 15, 13, 14};

for(const int num : testData)
{

    pq.push(num);

}

for(int right = testData.size() - 1; right >= 0; right--)
{

    testData[right] = pq.top();
    pq.pop();

}

for(int k = 0; k < testData.size(); k++)
{

    cout << testData[k] << endl;

}

return 0;

}
