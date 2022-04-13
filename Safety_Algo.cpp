/*The following is the code for the Classic Banker's Safety algorithm
 
Since this is a deadlock avoidance algorithm, it requires extra info regarding the no. of processes and the Max no. of resources of each type that could be requested
The no. of processes is taken as n
The no. of types of resources is taken as m   eg: A,B,C,D : m=4,    A,B,C,D,E : m=5 etc.
 
The data structures used are :
(i) three 2D arrays (vector of vectors) of size m*n for : Max_Need, Allocation and Current_Need(alias Need in this code)
(ii)two arrays Work and Finish. the Work array stores the available resources at any instant of time, the Finish array tells which Processes have been
executed and which have not.
 
Time Complexity : O(n*n*m)
if a deadlock doesn't occur, the algo runs in the complexity as stated above.
If a deadlock occurs, the algorithm may return Unsafe state in lesser time but the worst case still remains O(n*n*m)
*/
#pragma GCC optimize("O2")
#include <bits/stdc++.h>
using namespace std;
 
// This Function initializes the MAX_Need Matrix , Allocation Matrix, Available array and also finds the Need natrix
void Intialize_Matrices(int RR, int CC, vector<vector<int>> &Allocation, vector<vector<int>> &Max_Need, vector<vector<int>> &Need, vector<int> &Available);
 
// Return the index corresponding the process that can be executed, if no process can be executed, returns -1    //O(m*n)
int Process_no_executed(vector<int> &, vector<vector<int>> &, vector<int>);
 
// Compares 2 vectors A(Available) and B(Present_Need) each of size n. If ∀ i, A[i] >= B[i] returns true
bool Compare_Vectors(vector<int>, vector<int>);
 
// Check if all processes have finished. If yes return true
bool isFinished(vector<int>);
 
// This is the main function to check the safety condition
vector<int> Safety_Check(vector<vector<int>> &Allocation, vector<vector<int>> &Max_Need, vector<vector<int>> &Need, vector<int> Work, vector<int> Finish);
 
// Prints the Matrix
void Display2D(vector<vector<int>> &v)
{
    for (auto i : v)
    {
        for (auto it : i)
        {
            cout << it << " ";
        }
        cout << endl;
    }
}
// Prints the array
void Display1D(vector<int> &v)
{
    for (auto i : v)
    {
        cout << i << " ";
    }
    cout << endl;
}
 
int main()
{
    int n, m, temp = 0;
    cout << "Enter number of processes and number of resource instances: " << endl;
    cin >> n >> m;
    int &RR = n; // RR : no of rows
    int &CC = m; // CC : no of columns
 
    // Declaration of Allocation, Max_Need and Need Matrices, each of size m*n
    vector<vector<int>> Allocation(RR, vector<int>(CC)), Max_Need(RR, vector<int>(CC)), Need(RR, vector<int>(CC));
 
    // Declare the Available Array
    vector<int> Available(m);
 
    // Call Initialize function
    Intialize_Matrices(RR, CC, Allocation, Max_Need, Need, Available);
 
    // Define Work and Finish arrays
 
    vector<int> Work = Available;
    vector<int> Finish(n, 0);
 
    // Answer array stores the safe sequence for n processes. If no safe sequence exists, its size may be less than n and the last index stores -1
    vector<int> Answer = Safety_Check(Allocation, Max_Need, Need, Work, Finish);
 
    // If last index ==-1, no safe sequence exists
    if (Answer[Answer.size() - 1] == -1)
    {
        cout << "\n\nSystem is Unsafe\n";
    }
    // print safe sequence
    else
    {
        cout << "\n\nSystem is safe and the safe sequence is: \n";
        Display1D(Answer);
    }
    return 0;
}
 
void Intialize_Matrices(int RR, int CC, vector<vector<int>> &Allocation, vector<vector<int>> &Max_Need, vector<vector<int>> &Need, vector<int> &Available)
{
    // Obtain the Allocation Matrix
    cout << "\nFill the Allocation Matrix:\n";
    for (int i = 0; i < RR; i++)
    {
        for (int j = 0; j < CC; j++)
        {
            cin >> Allocation[i][j];
        }
    }
 
    // Obtain the Max Need Matrix
    cout << "\nFill the Max Need Matrix:\n";
    for (int i = 0; i < RR; i++)
    {
        for (int j = 0; j < CC; j++)
        {
            cin >> Max_Need[i][j];
        }
    }
 
    // Calculate Current Need a.k.a Need,   Need[i][j] = Max_Need[i][j] - Allocation[i][j]
    for (int i = 0; i < RR; i++)
    {
        for (int j = 0; j < CC; j++)
        {
            Need[i][j] = Max_Need[i][j] - Allocation[i][j];
        }
    }
 
    // Obtain the Available Resources array
    cout << "\nFill the Available Resources array:\n";
    for (int i = 0; i < CC; i++)
    {
        cin >> Available[i];
    }
    cout << "\nThe Need Matrix is:\n"
         << endl;
 
    // Print Need Matrix
    Display2D(Need);
}
 
// Compares 2 vectors A(Available) and B(Current Need) each of size n.  If ∀ i, A[i] >= B[i] returns true
bool Compare_Vectors(vector<int> A, vector<int> B)
{
    int n = A.size();
    for (int i = 0; i < n; i++)
    {
        if (A[i] < B[i])
            return false;
    }
    return true;
}
 
// Check if all processes have finished. If yes return true
bool isFinished(vector<int> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] == 0)
            return false;
    }
 
    return true;
}
 
// Returns process id for the process that can be executed. If no process can be executed returns -1
int Process_no_executed(vector<int> &Available, vector<vector<int>> &Need, vector<int> Finish)
{
 
    int n = Need.size();
 
    // the outer loop runs for max n times
    for (int i = 0; i < n; i++)
    {
        // only check for processes Pi that are waiting to be executed i.e. Finish[i]==0
        if (Finish[i] == 0)
        {
            // Check if the resource requirements of Pi can be fulfilled by the Available vector (a.k.a  Work)  ....O(m)
            if (Compare_Vectors(Available, Need[i]) == true)
            {
                // If Pi can be executed, return i and stop, there may be more such process still remaining but no need to check for them
                return i;
            }
        }
        // If Pi has been already executed , skip
        else
            continue;
    }
    // if no process can be executed return an invalid Pid @ -1
    return -1;
}
 
// This is the main function to check the safety condition
vector<int> Safety_Check(vector<vector<int>> &Allocation, vector<vector<int>> &Max_Need, vector<vector<int>> &Need, vector<int> Work, vector<int> Finish)
{
    // define m,n
    int m = Work.size();
    int n = Need.size();
 
    // This array stores the safe sequence order (in terms of Pids). If safe sequence exists, no element of this array can be -1 i.e all Pids should be valid
    vector<int> Process_Order;
 
    // Run the loop for as long as all processes have not been finished executing. It breaks if an unsafe state occurs
    while (isFinished(Finish) == false)
    {
        // Get the Pid for the first process that can be executed
        int j = Process_no_executed(Work, Need, Finish);
 
        // if invalid Pid @-1, unsafe state occurs, hence break the loop
        if (j == -1)
        {
            // This array will contain invalid pid as its last index location
            Process_Order.push_back(-1);
            break;
        }
 
        // If Pid is valid
        else
        {
            // assign Finish[j] as true since Pi has been executed
            Finish[j] = 1;
            // insert the Pid in the Process_Order array
            Process_Order.push_back(j);
 
            // Relieve all resources held by Pi and add them to the Work array (list of Available resources)
            for (int i = 0; i < m; i++)
                Work[i] += Allocation[j][i];
        }
    }
 
    // Return safe sequence, if its exits, all Pids will be valid (!= -1), if not the last element will be -1 as the loop breaks after detecting unsafe state
    return Process_Order;
}
