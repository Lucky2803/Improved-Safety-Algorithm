/*The following is the code for the Improved Banker's Safety algorithm
 
Since this is a deadlock avoidance algorithm, it requires extra info regarding the no. of processes and the Max no. of resources of each type that could be requested
The no. of processes is taken as n
The no. of types of resources is taken as m   eg: A,B,C,D : m=4,    A,B,C,D,E : m=5 etc.
In addition to this, the algo maintains 2 matrices: Sorted_Current_Need and Sorted_Allocation.
The current_Need is sorted in the increasing order of its maximum value of resource instance.
The Sorted_Allocation is obtained by matching the Pids with the Sorted_Current_Need
Also the sorted order is maintained in array: Order
 
The data structures used are :
(i) three 2D arrays (vector of vectors) of size m*n for : Max_Need, Allocation and Current_Need(alias Need in this code)
(ii) two 2D arrays of size m*n : Sorted_Current_Need and Sorted_Allocation
(iii)two arrays Work and Order. the Work array stores the available resources at any instant of time. The Order arrays contains the sorted order of PIDs
Time Complexity : O(nlogn + nm)
Assuming logn ~ m so the time complexity can be written as O(mn) (n times faster than Classic Safety Algo)
If the number of processes is very large compared to the number of resource instance types then logn is the limiting factor and the
time complexity becomes O(nlogn) but still better than O(n*n*m)
 
if a deadlock doesn't occur, the algo runs in the complexity as stated above.
If a deadlock occurs, the algorithm may return Unsafe state in lesser time but the worst case still remains O(n*(logn + m))
*/
 
#pragma GCC optimize("O2")
#include <bits/stdc++.h>
using namespace std;
 
// This Function initializes the MAX_Need Matrix , Allocation Matrix, Available array and also finds the Need natrix
void Intialize_Matrices(int RR, int CC, vector<vector<int>> &Allocation, vector<vector<int>> &Max_Need, vector<vector<int>> &Need, vector<int> &Available);
 
// Check if all processes have completed. If yes return 1
bool isCompleted(vector<bool> &v);
 
// Return max allocation need array
vector<pair<int, int>> Max_alloc_array(vector<vector<int>> &Current_Need);
 
// Sort Current_Need matrix and Allocation matrix in increasing order of max. number of resource instances. Return an array storing the
// sorted order of Pids.
vector<int> Sorted_ids(vector<vector<int>> &Current_Need, vector<vector<int>> &Allocation);
 
// Update Work It updates the Work array by adding the free resources of a completed process
void Update_Available(vector<int> &work, vector<int> &alloc);
 
// This is the main function to check the safety condition
void Safety_Check(vector<vector<int>> &Allocation, vector<int> Work, vector<vector<int>> &Sorted_Current_Need, vector<int> order);
 
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
    int n = 0, m = 0;
    cout << "\nEnter number of processes and number of resource instances: " << endl;
    cin >> n >> m;
    int &RR = n; // RR : no of rows
    int &CC = m; // CC : no of columns
 
    // Declaration of Allocation, Max_Need and Current_Need Matrices, each of size m*n
    vector<vector<int>> Allocation(RR, vector<int>(CC)), Max_Need(RR, vector<int>(CC)), Current_Need(RR, vector<int>(CC));
 
    // Declare the Available Array
    vector<int> Available(m);
 
    // Call Initialize function
    Intialize_Matrices(RR, CC, Allocation, Max_Need, Current_Need, Available);
 
    // return sorted order of Pids in Order and also sort Current_Need and Allocation Matrices
    vector<int> Order = Sorted_ids(Current_Need, Allocation);
 
    // Run the safety check
    Safety_Check(Allocation, Available, Current_Need, Order);
 
    return 0;
}
 
void Intialize_Matrices(int RR, int CC, vector<vector<int>> &Allocation, vector<vector<int>> &Max_Need, vector<vector<int>> &Current_Need, vector<int> &Available)
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
 
    // Calculate Current Need,   Need[i][j] = Max_Need[i][j] - Allocation[i][j]
    for (int i = 0; i < RR; i++)
    {
        for (int j = 0; j < CC; j++)
        {
            Current_Need[i][j] = Max_Need[i][j] - Allocation[i][j];
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
    Display2D(Current_Need);
}
 
// Returns max allocation need array
// vector of pairs is used to store both the Max. allocates resources instance and the corresponding Pid
vector<pair<int, int>> Max_alloc_array(vector<vector<int>> &Current_Need)
{
    int n = Current_Need.size();
    vector<pair<int, int>> max_alloc;
 
    // Runs for n times
    for (int i = 0; i < n; i++)
    {
        // pair (a,b)  a : Max allocated resources for i = i   b= Pid corresponding to a
        max_alloc.push_back({*max_element(Current_Need[i].begin(), Current_Need[i].end()), i});
    }
 
    // Sort the vector of pairs, the Max allocated resources get sorted in increasing order.
    sort(max_alloc.begin(), max_alloc.end());
 
    return max_alloc;
}
 
/* Sort Current_Need matrix and Allocation matrix in increasing order of max. number of resource instances. Return an array storing the
 sorted order of Pids. */
vector<int> Sorted_ids(vector<vector<int>> &Current_Need, vector<vector<int>> &Allocation)
{
    int n = Current_Need.size();
    // Declare new matrices to store Sorted Current Need and Sorted Allocations
    vector<vector<int>> Sorted_Current_Need;
    vector<vector<int>> Sorted_Allocation;
    // This array list stores the PId order
    vector<int> order;
 
    // initialize max allocation array by passing Current_Need
    vector<pair<int, int>> max_all = Max_alloc_array(Current_Need);
 
    int j = 0;
    for (int i = 0; i < n; i++)
    {
        // j stores the Pid number
        j = max_all[i].second;
 
        // Fill Sorted_Current_Need and Sorted_Allcation matrices.
        Sorted_Current_Need.push_back(Current_Need[j]);
        Sorted_Allocation.push_back(Allocation[j]);
        // this array stores the order of Pids
        order.push_back(j);
    }
    // Assign new matrices to old ones
    Current_Need = Sorted_Current_Need;
    Allocation = Sorted_Allocation;
 
    // Clear redundant matrices
    Sorted_Current_Need.clear();
    Sorted_Allocation.clear();
 
    // return the sorted order of Pids
    return order;
}
 
// Update Work It updates the Work array by adding the free resources of a completed process
void Update_Available(vector<int> &work, vector<int> &alloc)
{
 
    for (int i = 0; i < work.size(); i++)
    {
        // Add allocated resources of completed process ∀ i
        work[i] += alloc[i];
    }
}
 
// Check if all processes have completed. If yes, return 1. It processes a array of boolean values
bool isCompleted(vector<bool> &v)
{
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] == 0)
            return 0;
        else
            continue;
    }
    return 1;
}
 
// Run the safety check
void Safety_Check(vector<vector<int>> &Sorted_Allocation, vector<int> Available, vector<vector<int>> &Sorted_Current_Need, vector<int> order)
{
    // Define Work array (Local to this function) = Available
    vector<int> Work = Available;
    int m = Work.size();
    int n = Sorted_Current_Need.size();
 
    // declare an array that stores completed processes. Initially all processes are yet to start hence assign 0 ∀ i
    vector<bool> flag(n, 0);
 
    // This array list stores the safe sequence
    vector<int> Pids;
 
    // declare local variables: max_need and min_available
    int max_need = 0, min_available = 0;
 
    // runs for at most n times
    for (int i = 0; i < n; i++)
    {
        // Assign max_need to the max element in the Sorted_Current_Need vector for ith index.
        max_need = *max_element(Sorted_Current_Need[i].begin(), Sorted_Current_Need[i].end());
 
        // Assign min_availble to min element in the Work array
        min_available = *min_element(Work.begin(), Work.end());
 
        // If process not completed and max need <= min  available, continue in just one step  ...O(1)
        if (!flag[i] && max_need <= min_available)
        {
            // Process i has been completed hence turn on flag i
            flag[i] = 1;
            // update the available array by adding the allocated resources of Process i
            Update_Available(Work, Sorted_Allocation[i]);
            // Store the pid of the executed process
            Pids.push_back(order[i]);
            continue;
        }
 
        // If max_need > min_available, check if enough resources are present in work array ∀ j , 0 <= j <m   ...O(m)
        else if (!flag[i] && max_need > min_available)
        {
            int j = 0;
            for (j = 0; j < m; j++)
            {
                if (Work[j] >= Sorted_Current_Need[i][j])
                    continue;
                else
                    break;
            }
            // If the work array fulfils the request, do the following:
            if (j == m)
            {
                flag[i] = 1;
                Pids.push_back(order[i]);
                Update_Available(Work, Sorted_Allocation[i]);
                continue;
            }
            // if work array cannot fulfil the request break the loop
            else if (j < m)
            {
                break;
            }
        }
        // skip if process has already completed
        else if (flag[i] == 1)
            continue;
    }
 
    // if all processes have been completed return the safe sequence @ Pids arraylist
    if (isCompleted(flag))
    {
        cout << "\nSystem is safe and the safe sequence is: \n";
        Display1D(Pids);
        return;
    }
 
    // else return unsafe state
    cout << "\nSystem is Unsafe\n";
}
