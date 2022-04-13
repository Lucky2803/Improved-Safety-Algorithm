# Improved-Safety-Algorithm
There are three methods for handling deadlocks in resource allocation systems: deadlock prevention, deadlock avoidance and deadlock detection combined with recovery. Of these three methods, deadlock avoidance is preferable in many cases but seldom used on account of its high cost that lies not only in the time and space complexity of the algorithm, but also in its limits, the concurrency of the system. 

This projects presents the reviews of three innovative techniques for safe state detection in a system.
The first approach is based on the maximum resource requirements of processes and the minimum resources available. Through our test cases, we compare our approach with some other methods including the classical banker’s algorithm. The results show that the proposed method provides less time complexity and less space complexity than the other methods.
The second approach is based on  dynamic programming. In this technique, apart from the' resource needs of the processes and  the availability, the burst time of the processes is also considered to find the optimal safe sequence of processes that leads to a minimum average waiting time.
The Project is in building phase, second approach would be implemeted soon.

