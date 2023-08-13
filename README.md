# COMP 304 Project 2
Yiğit Yakar 72269 , Ece Pınar Özer 72047

To run the code for Part 1, compile with:  `$ gcc project_2p1.c -lpthread` <br />
To run the code for Part 2, compile with:  `$ gcc project_2p2.c -lpthread`

*Note: Since Part 3 included the introduction of a new gift type and no other modifications to the main source code, we added the New Zealand gifts that are introduced at Part 3 to our implementation of parts 1 and 2.*

## Part 1
In this part, we created the backbone of the project by implementing Elf A, Elf B, Santa, gift creation, task management and synchronization. 

### Tasks
In our implementation, each gift is stored in a `Task` struct instance. The attributes of `Task` struct are as follows:
* `int ID`              *gift ID*
* `int type`            *gift type*
* `int task_ID`         *task ID*
* `int arrival_time`    *arrival time of the task*
* `int request_time`    *request time of the task*

*Note: Task IDs are assigned upon the creation of a task, so they are incremented before enqueueing the task to the corresponding queue. However, the log file entries are created upon completion of a task, after the task is dequeued from a queue. Therefore, the order of the task IDs in the log file may not be exactly consequtive.*

In Part 1, gift creation and type assignments are determined according to probabilities. We created and properly assigned the types within the main function using `rand()` with command line argument for the `seed`. These gifts, contained within tasks, are moved between queues for operations required for the gift's type. 

### Queues
For each task (painting, packaging, assembly, delivery, qa), we implemented a separate `Queue`. Whenever an elf or Santa becomes available, it dequeues a task from the corresponding queue using `Dequeue()` function. After that task is done, it calls `Enqueue()` method to send the gift to the next task. We also created an array of queues, `Queue *queues[10]` to store them in some common data structure. 

### Threads
We first created `ControlThread` in the main function. Then, we created separate POSIX threads for `Elf A`, `Elf B` and `Santa` under this `ControlThread`. ControlThread is responsible for creating the workers and displaying the console output regarding the waiting tasks. Within the function of each elf and Santa, we checked the queues in the order of their priority and dequeued a task from the highest priority, non-empty queue. By using `pthread_sleep()` function, we ensured that
tasks painting, assembly, packaging, delivery, qa tasks take 3t, 2t, 1t, 1t, 1t time, respectively.

### Mutex Locks
In order to allow concurrent execution of threads and 
avoid race conditions, we created the following mutex locks:

1) mutex locks for each task queue (enqueue/ dequeue operations)
* `packaging_mutex` 
* `painting_mutex` 
* `assembly_mutex`
* `delivery_mutex`
* `qa_mutex`

2) mutex locks for each task object, indexed by ID
* `taskmutex[5000]` 

3) mutex lock for writing to the log file
* `file_mutex` 

### Other variables
* `n` - time to display the waiting tasks in the queues *(also a console input)*
* `completed[5000]` - array of integers for gift types 4 and 5, used to signal whether both operations before packaging are completed.
* `start_time` - timeval structure to denote the starting time of the simulation
* `timee` - timeval structure to be used to get the current time at any point in the simulation (used for time calculations)
* `count` - keeps the number of gifts generated to be used in ID creation
* `task_count` - keeps the number of tasks generated to be used in task ID creation

## Part 2
In this part, we changed the probabilities of gift creation to follow the guidelines. We also added an if clause to Santa's function to check the size of the delivery and qa queues first. If the delivery queue is not empty and the qa queue size is less than 3, then Santa should do delivery first. 

## Part 3
To create gifts from New Zealand according to the `emergency_frequency`, we used `%` operator inside our gift creation loop. To give them the priority, we created separate queues and corresponding mutex locks for New Zealand gifts and made each elf and Santa check the New Zealand queues first. 

## Keeping Logs
The log file `events1.log` and `events2.log` record the completed tasks at their attributes, as well as the turnaround times for parts 1 and 2, respectively. Each thread opens and writes to the log file every time a task is completed. In order to synchronize the writing of the threads, we created a mutex lock for writing permission to the log file.

To output the snapshot of the waiting tasks with their IDs in every second starting from *nth* secs to the terminal, we used the ControlThread to iterate over the queues once the *nth* second is reached and print the task IDs. 


