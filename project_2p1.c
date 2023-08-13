#include "queue.c"
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

int simulationTime = 120;    // simulation time
int seed = 10;               // seed for randomness
int emergencyFrequency = 30; // frequency of emergency gift requests from New Zealand
int n = 30;                  // time to display the waiting tasks in the queues

void* ElfA(void *arg); // the one that can paint
void* ElfB(void *arg); // the one that can assemble
void* Santa(void *arg); 
void* ControlThread(void *arg); // handles printing and queues (up to you)


Queue *queues[10];

int completed[5000];

struct timeval start_time;
struct timeval timee;
    
pthread_mutex_t file_mutex;

pthread_mutex_t taskmutex[5000];
pthread_mutex_t packaging_mutex;
pthread_mutex_t painting_mutex;
pthread_mutex_t assembly_mutex;
pthread_mutex_t delivery_mutex;
pthread_mutex_t qa_mutex;
pthread_mutex_t NZ_packaging_mutex;
pthread_mutex_t NZ_painting_mutex;
pthread_mutex_t NZ_assembly_mutex;
pthread_mutex_t NZ_delivery_mutex;
pthread_mutex_t NZ_qa_mutex;
int count;
int task_count;
// pthread sleeper function
int pthread_sleep (int seconds)
{
    pthread_mutex_t mutex;
    pthread_cond_t conditionvar;
    struct timespec timetoexpire;
    if(pthread_mutex_init(&mutex,NULL))
    {
        return -1;
    }
    if(pthread_cond_init(&conditionvar,NULL))
    {
        return -1;
    }
    struct timeval tp;
    //When to expire is an absolute time, so get the current time and add it to our delay time
    gettimeofday(&tp, NULL);
    timetoexpire.tv_sec = tp.tv_sec + seconds; timetoexpire.tv_nsec = tp.tv_usec * 1000;
    
    pthread_mutex_lock(&mutex);
    int res =  pthread_cond_timedwait(&conditionvar, &mutex, &timetoexpire);
    pthread_mutex_unlock(&mutex);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&conditionvar);
    
    //Upon successful completion, a value of zero shall be returned
    return res;
}



int main(int argc,char **argv){
    // -t (int) => simulation time in seconds
    // -s (int) => change the random seed
    // -n (int) => time to display the waiting tasks in the queue
    
    FILE *logfile;
    logfile = fopen("events1.log", "w");   // create the log file
    pthread_mutex_init(&file_mutex,NULL); // initialize the file mutex lock
    fprintf(logfile, "TaskID GiftID GiftType TaskType\t RequestTime TaskArrival  TT\tResponsible\n");
    fprintf(logfile, "----------------------------------------------------------------------------\n");

    fclose(logfile);

    for(int i=1; i<argc; i++){
        if(!strcmp(argv[i], "-t")) {simulationTime = atoi(argv[++i]);}
        else if(!strcmp(argv[i], "-s"))  {seed = atoi(argv[++i]);}
        else if(!strcmp(argv[i], "-n"))  {n = atoi(argv[++i]);}
    }
    
    srand(seed); // feed the seed
    
    /* Queue usage example
        Queue *myQ = ConstructQueue(1000);
        Task t;
        t.ID = myID;
        t.type = 2;
        Enqueue(myQ, t);
        Task ret = Dequeue(myQ);
        DestructQueue(myQ);
    */

    // your code goes here
    // you can simulate gift request creation in here, 
    // but make sure to launch the threads first

    queues[0] = ConstructQueue(simulationTime+2);    //packaging
    queues[1] = ConstructQueue(simulationTime+2);    //painting
    queues[2] = ConstructQueue(simulationTime+2);    //assembly
    queues[3] = ConstructQueue(simulationTime+2);    //delivery
    queues[4] = ConstructQueue(simulationTime+2);    //qa
    queues[5] = ConstructQueue(simulationTime+2);    //NZ packaging  
    queues[6] = ConstructQueue(simulationTime+2);    //NZ painting  
    queues[7] = ConstructQueue(simulationTime+2);    //NZ assembly 
    queues[8] = ConstructQueue(simulationTime+2);    //NZ delivery
    queues[9] = ConstructQueue(simulationTime+2);    //NZ qa

    

    pthread_mutex_init(&packaging_mutex,NULL);
    pthread_mutex_init(&painting_mutex,NULL);
    pthread_mutex_init(&assembly_mutex,NULL);
    pthread_mutex_init(&delivery_mutex,NULL);
    pthread_mutex_init(&qa_mutex,NULL);
    pthread_mutex_init(&NZ_packaging_mutex,NULL);
    pthread_mutex_init(&NZ_painting_mutex,NULL);
    pthread_mutex_init(&NZ_assembly_mutex,NULL);
    pthread_mutex_init(&NZ_delivery_mutex,NULL);
    pthread_mutex_init(&NZ_qa_mutex,NULL);




    
    fflush(stdout);
    pthread_t tid;
    count = 0;
    int p;
    pthread_create(&tid, NULL, &ControlThread, NULL);
   
    gettimeofday(&start_time, NULL);
   
    
    for (int i = 0; i < simulationTime; i++){
        
        p=rand()%1000 + 1;
        
        if (p> 900) {}  // no gift creation
        else {
            count ++;
            gettimeofday(&timee, NULL);
            Task t;
            t.ID = count;
            t.arrival_time = timee.tv_sec - start_time.tv_sec;
 
            if (p<400){
                
                pthread_mutex_init(&taskmutex[count],NULL);
                
                t.type=1;

                if(i%emergencyFrequency ==0){
                    gettimeofday(&timee, NULL);
                    t.request_time = timee.tv_sec - start_time.tv_sec;
                    task_count++;
                    t.task_ID = task_count;
                    pthread_mutex_lock(&NZ_packaging_mutex);
                    Enqueue(queues[5], t);
                    pthread_mutex_unlock(&NZ_packaging_mutex);
                }
                else{
                    gettimeofday(&timee, NULL);
                    t.request_time = timee.tv_sec - start_time.tv_sec;
                    task_count++;
                    t.task_ID = task_count;
                    pthread_mutex_lock(&packaging_mutex);
                    Enqueue(queues[0], t);
                    pthread_mutex_unlock(&packaging_mutex);
                }
            } 
            else if (p<600){

                pthread_mutex_init(&taskmutex[count],NULL);
                
                t.type=2;
                if(i%emergencyFrequency ==0){
                    gettimeofday(&timee, NULL);
                    t.request_time = timee.tv_sec - start_time.tv_sec;
                    task_count++;
                    t.task_ID = task_count;
                    pthread_mutex_lock(&NZ_painting_mutex);
                    Enqueue(queues[6], t);
                    pthread_mutex_unlock(&NZ_painting_mutex);
                }
                else{
                    gettimeofday(&timee, NULL);
                    t.request_time = timee.tv_sec - start_time.tv_sec;
                    task_count++;
                    t.task_ID = task_count;
                    pthread_mutex_lock(&painting_mutex);
                    Enqueue(queues[1], t);
                    pthread_mutex_unlock(&painting_mutex);
                }
             
            }
            else if (p<800){
                pthread_mutex_init(&taskmutex[count],NULL);
               
                t.type=3;
                if(i%emergencyFrequency ==0){
                    gettimeofday(&timee, NULL);
                    t.request_time = timee.tv_sec - start_time.tv_sec;
                    task_count++;
                    t.task_ID = task_count;
                    pthread_mutex_lock(&NZ_assembly_mutex);
                    Enqueue(queues[7], t);
                    pthread_mutex_unlock(&NZ_assembly_mutex);
                }
                else{
                    gettimeofday(&timee, NULL);
                    t.request_time = timee.tv_sec - start_time.tv_sec;
                    task_count++;
                    t.task_ID = task_count;
                    pthread_mutex_lock(&assembly_mutex);
                    Enqueue(queues[2], t);
                    pthread_mutex_unlock(&assembly_mutex);
                }

            } 
             else if (p<850){
                pthread_mutex_init(&taskmutex[count],NULL);
                
                t.type=4;

                if(i%emergencyFrequency ==0){
                    gettimeofday(&timee, NULL);
                    t.request_time = timee.tv_sec - start_time.tv_sec;
                    task_count++;
                    t.task_ID = task_count;
                    pthread_mutex_lock(&NZ_painting_mutex);
                    Enqueue(queues[6], t);
                    pthread_mutex_unlock(&NZ_painting_mutex);

                    gettimeofday(&timee, NULL);
                    t.request_time = timee.tv_sec - start_time.tv_sec;
                    task_count++;
                    t.task_ID = task_count;
                    pthread_mutex_lock(&NZ_qa_mutex);
                    Enqueue(queues[9], t);
                    pthread_mutex_unlock(&NZ_qa_mutex);
                }

                else{
                    gettimeofday(&timee, NULL);
                    t.request_time = timee.tv_sec - start_time.tv_sec;
                    task_count++;
                    t.task_ID = task_count;
                    pthread_mutex_lock(&painting_mutex);
                    Enqueue(queues[1], t);
                    pthread_mutex_unlock(&painting_mutex);

                    gettimeofday(&timee, NULL);
                    t.request_time = timee.tv_sec - start_time.tv_sec;
                    task_count++;
                    t.task_ID = task_count;
                    pthread_mutex_lock(&qa_mutex);
                    Enqueue(queues[4], t);
                    pthread_mutex_unlock(&qa_mutex);
                }
                
            } 

            else if (p<=900){
                
                pthread_mutex_init(&taskmutex[count],NULL);
                t.type=5;
                if(i%emergencyFrequency ==0){
                    gettimeofday(&timee, NULL);
                    t.request_time = timee.tv_sec - start_time.tv_sec;
                    task_count++;
                    t.task_ID = task_count;
                    pthread_mutex_lock(&NZ_assembly_mutex);
                    Enqueue(queues[7], t);
                    pthread_mutex_unlock(&NZ_assembly_mutex);

                    gettimeofday(&timee, NULL);
                    t.request_time = timee.tv_sec - start_time.tv_sec;
                    task_count++;
                    t.task_ID = task_count;
                    pthread_mutex_lock(&NZ_qa_mutex);
                    Enqueue(queues[9], t);
                    pthread_mutex_unlock(&NZ_qa_mutex);

                }
                else{
                    gettimeofday(&timee, NULL);
                    t.request_time = timee.tv_sec - start_time.tv_sec;
                    task_count++;
                    t.task_ID = task_count;
                    pthread_mutex_lock(&assembly_mutex);
                    Enqueue(queues[2], t);
                    pthread_mutex_unlock(&assembly_mutex);

                    gettimeofday(&timee, NULL);
                    t.request_time = timee.tv_sec - start_time.tv_sec;
                    task_count++;
                    t.task_ID = task_count;
                    pthread_mutex_lock(&qa_mutex);
                    Enqueue(queues[4], t);
                    pthread_mutex_unlock(&qa_mutex);
                }
                
            } 
        }
            
        gettimeofday(&timee, NULL);
      
        pthread_sleep(1);

    }
    for (int i=0; i < 10; i++){
        DestructQueue(queues[i]);
    }
    return 0;
}

void* ElfA(void *arg){
    FILE *logfile;

    while (1) {
        if (!isEmpty(queues[5])){    
            pthread_mutex_lock(&NZ_packaging_mutex);
            Task ret = Dequeue(queues[5]);
            pthread_mutex_unlock(&NZ_packaging_mutex);
            pthread_sleep(1);
            gettimeofday(&timee, NULL);
            
            logfile = fopen("events1.log", "a");   
            pthread_mutex_lock(&file_mutex); // acquire the file mutex lock

            fprintf(logfile, "%d\t %d\t %d\t %c\t    %d\t\t %d\t  %ld\t %c\t\t--NZ gift\n", 
                                ret.task_ID, ret.ID, ret.type, 'C', ret.request_time, ret.arrival_time, timee.tv_sec - start_time.tv_sec - ret.arrival_time, 'A');
            pthread_mutex_unlock(&file_mutex); // release the file mutex lock
            
            fclose(logfile);

            gettimeofday(&timee, NULL);
            ret.request_time = timee.tv_sec - start_time.tv_sec;
             
            pthread_mutex_lock(&NZ_delivery_mutex);
            task_count++;
            ret.task_ID = task_count;
            Enqueue(queues[8],ret);
            pthread_mutex_unlock(&NZ_delivery_mutex);
        
            }
        
        else if (!isEmpty(queues[6])){ 
            

            pthread_mutex_lock(&NZ_painting_mutex);
            Task ret = Dequeue(queues[6]);
            gettimeofday(&timee, NULL);
            pthread_mutex_unlock(&NZ_painting_mutex);
            pthread_sleep(3);
            gettimeofday(&timee, NULL);
  
            logfile = fopen("events1.log", "a");   
            pthread_mutex_lock(&file_mutex); // acquire the file mutex lock
            fprintf(logfile, "%d\t %d\t %d\t %c\t    %d\t\t %d\t  %ld\t %c\t\t--NZ gift\n", 
                                ret.task_ID, ret.ID, ret.type, 'P', ret.request_time, ret.arrival_time, timee.tv_sec - start_time.tv_sec - ret.arrival_time, 'A');
            pthread_mutex_unlock(&file_mutex); // release the file mutex lock
            
            fclose(logfile);

            if (ret.type==4){

                pthread_mutex_lock(&taskmutex[ret.ID]);
                completed[ret.ID]+=1;
                pthread_mutex_unlock(&taskmutex[ret.ID]);
                if (completed[ret.ID]==2){
                    gettimeofday(&timee, NULL);
                    ret.request_time = timee.tv_sec - start_time.tv_sec;
                    pthread_mutex_lock(&NZ_packaging_mutex);
                    task_count++;
                    ret.task_ID = task_count;
                    Enqueue(queues[5],ret);
                    pthread_mutex_unlock(&NZ_packaging_mutex);
                    gettimeofday(&timee, NULL);
 
                }

            }
            else{    //type 2 NZ gift
                gettimeofday(&timee, NULL);
                ret.request_time = timee.tv_sec - start_time.tv_sec;
                pthread_mutex_lock(&NZ_packaging_mutex);
                task_count++;
                ret.task_ID = task_count;
                Enqueue(queues[5],ret);
                pthread_mutex_unlock(&NZ_packaging_mutex);
                gettimeofday(&timee, NULL);
 
            }

        }
        //NZ GIFT SECTION END
        else if (!isEmpty(queues[0])){    
            pthread_mutex_lock(&packaging_mutex);
            Task ret = Dequeue(queues[0]);
            pthread_mutex_unlock(&packaging_mutex);
            pthread_sleep(1);
            gettimeofday(&timee, NULL);
 
            logfile = fopen("events1.log", "a");   
            pthread_mutex_lock(&file_mutex); // acquire the file mutex lock

            fprintf(logfile, "%d\t %d\t %d\t %c\t    %d\t\t %d\t  %ld\t %c\n", 
                                ret.task_ID, ret.ID, ret.type, 'C', ret.request_time, ret.arrival_time, timee.tv_sec - start_time.tv_sec - ret.arrival_time, 'A');
            pthread_mutex_unlock(&file_mutex); // release the file mutex lock
            
            fclose(logfile);

            gettimeofday(&timee, NULL);
            ret.request_time = timee.tv_sec - start_time.tv_sec;
            pthread_mutex_lock(&delivery_mutex);
            task_count++;
            ret.task_ID = task_count;
            Enqueue(queues[3],ret);
            pthread_mutex_unlock(&delivery_mutex);
        
                }
        else{
            if (!isEmpty(queues[1])){ 
                
                pthread_mutex_lock(&painting_mutex);
                Task ret = Dequeue(queues[1]);
                gettimeofday(&timee, NULL);
                pthread_mutex_unlock(&painting_mutex);
                pthread_sleep(3);
                gettimeofday(&timee, NULL);

                logfile = fopen("events1.log", "a");   
                pthread_mutex_lock(&file_mutex); // acquire the file mutex lock
                fprintf(logfile, "%d\t %d\t %d\t %c\t    %d\t\t %d\t  %ld\t %c\n", 
                                ret.task_ID, ret.ID, ret.type, 'P', ret.request_time, ret.arrival_time, timee.tv_sec - start_time.tv_sec - ret.arrival_time, 'A');
                pthread_mutex_unlock(&file_mutex); // release the file mutex lock
            
                fclose(logfile);


                if (ret.type==4){

                    pthread_mutex_lock(&taskmutex[ret.ID]);
                    completed[ret.ID]+=1;
                    pthread_mutex_unlock(&taskmutex[ret.ID]);
                    if (completed[ret.ID]==2){
                        gettimeofday(&timee, NULL);
                        ret.request_time = timee.tv_sec - start_time.tv_sec;
                        pthread_mutex_lock(&packaging_mutex);
                        task_count++;
                        ret.task_ID = task_count;
                        Enqueue(queues[0],ret);
                        pthread_mutex_unlock(&packaging_mutex);
                        gettimeofday(&timee, NULL);
 
                    }

                }
                else{    //type 2 gift
                    gettimeofday(&timee, NULL);
                    ret.request_time = timee.tv_sec - start_time.tv_sec;
                    pthread_mutex_lock(&packaging_mutex);
                    task_count++;
                    ret.task_ID = task_count;
                    Enqueue(queues[0],ret);
                    pthread_mutex_unlock(&packaging_mutex);
                    gettimeofday(&timee, NULL);
 
                }

            }

        }

    }

}

void* ElfB(void *arg){
    FILE *logfile;
    while (1) {

        if (!isEmpty(queues[5])){    
            pthread_mutex_lock(&NZ_packaging_mutex);
            Task ret = Dequeue(queues[5]);
            pthread_mutex_unlock(&NZ_packaging_mutex);
            pthread_sleep(1);
 
            logfile = fopen("events1.log", "a");   
            pthread_mutex_lock(&file_mutex); // acquire the file mutex lock
            fprintf(logfile, "%d\t %d\t %d\t %c\t    %d\t\t %d\t  %ld\t %c\t\t--NZ gift\n", 
                                ret.task_ID, ret.ID, ret.type, 'C', ret.request_time, ret.arrival_time, timee.tv_sec - start_time.tv_sec - ret.arrival_time, 'B');
            pthread_mutex_unlock(&file_mutex); // release the file mutex lock
            
            fclose(logfile);

            gettimeofday(&timee, NULL);
            ret.request_time = timee.tv_sec - start_time.tv_sec;
            pthread_mutex_lock(&NZ_delivery_mutex);
            task_count++;
            ret.task_ID = task_count;
            Enqueue(queues[8],ret);
            pthread_mutex_unlock(&NZ_delivery_mutex);
        
            }
        
         else if (!isEmpty(queues[7])){ 
                pthread_mutex_lock(&NZ_assembly_mutex);
                Task ret = Dequeue(queues[7]);
                pthread_mutex_unlock(&NZ_assembly_mutex);
                pthread_sleep(2);
 
                logfile = fopen("events1.log", "a");   
                pthread_mutex_lock(&file_mutex); // acquire the file mutex lock

                fprintf(logfile, "%d\t %d\t %d\t %c\t    %d\t\t %d\t  %ld\t %c\t\t--NZ gift\n", 
                                    ret.task_ID, ret.ID, ret.type, 'A', ret.request_time, ret.arrival_time, timee.tv_sec - start_time.tv_sec - ret.arrival_time, 'B');
                pthread_mutex_unlock(&file_mutex); // release the file mutex lock
                
                fclose(logfile);

                if (ret.type==5){

                    pthread_mutex_lock(&taskmutex[ret.ID]);
                    completed[ret.ID]+=1;
                    pthread_mutex_unlock(&taskmutex[ret.ID]);
                    if (completed[ret.ID]==2){
                        gettimeofday(&timee, NULL);
                        ret.request_time = timee.tv_sec - start_time.tv_sec;
                        pthread_mutex_lock(&NZ_packaging_mutex);
                        task_count++;
                        ret.task_ID = task_count;
                        Enqueue(queues[5],ret);

                        pthread_mutex_unlock(&NZ_packaging_mutex);

                    }

                }
                else{    //type 3 gift
                        gettimeofday(&timee, NULL);
                        ret.request_time = timee.tv_sec - start_time.tv_sec;
                        pthread_mutex_lock(&NZ_packaging_mutex);
                        task_count++;
                        ret.task_ID = task_count;
                        Enqueue(queues[5],ret);
                        pthread_mutex_unlock(&NZ_packaging_mutex);

                }

            }


//NZ SECTION END

        else if (!isEmpty(queues[0])){    
            pthread_mutex_lock(&packaging_mutex);
            Task ret = Dequeue(queues[0]);
            pthread_mutex_unlock(&packaging_mutex);
            pthread_sleep(1);
 
            logfile = fopen("events1.log", "a");   
            pthread_mutex_lock(&file_mutex); // acquire the file mutex lock

            fprintf(logfile, "%d\t %d\t %d\t %c\t    %d\t\t %d\t  %ld\t %c\n", 
                                ret.task_ID, ret.ID, ret.type, 'C', ret.request_time, ret.arrival_time, timee.tv_sec - start_time.tv_sec - ret.arrival_time, 'B');
            pthread_mutex_unlock(&file_mutex); // release the file mutex lock
            
            fclose(logfile);

            gettimeofday(&timee, NULL);
            ret.request_time = timee.tv_sec - start_time.tv_sec;
            pthread_mutex_lock(&delivery_mutex);
            task_count++;
            ret.task_ID = task_count;
            Enqueue(queues[3],ret);
            pthread_mutex_unlock(&delivery_mutex);
        
                }
        else{
            if (!isEmpty(queues[2])){ 
                pthread_mutex_lock(&assembly_mutex);
                Task ret = Dequeue(queues[2]);
                pthread_mutex_unlock(&assembly_mutex);
                pthread_sleep(2);
 
                logfile = fopen("events1.log", "a");   
                pthread_mutex_lock(&file_mutex); // acquire the file mutex lock

                fprintf(logfile, "%d\t %d\t %d\t %c\t    %d\t\t %d\t  %ld\t %c\n", 
                                    ret.task_ID, ret.ID, ret.type, 'A', ret.request_time, ret.arrival_time, timee.tv_sec - start_time.tv_sec - ret.arrival_time, 'B');
                pthread_mutex_unlock(&file_mutex); // release the file mutex lock
                
                fclose(logfile);

                if (ret.type==5){

                    pthread_mutex_lock(&taskmutex[ret.ID]);
                    completed[ret.ID]+=1;
                    pthread_mutex_unlock(&taskmutex[ret.ID]);
                    if (completed[ret.ID]==2){
                        gettimeofday(&timee, NULL);
                        ret.request_time = timee.tv_sec - start_time.tv_sec;
                        pthread_mutex_lock(&packaging_mutex);
                        task_count++;
                        ret.task_ID = task_count;
                        Enqueue(queues[0],ret);

                        pthread_mutex_unlock(&packaging_mutex);

                    }

                }
                else{    //type 3 gift
                        gettimeofday(&timee, NULL);
                        ret.request_time = timee.tv_sec - start_time.tv_sec;
                        pthread_mutex_lock(&packaging_mutex);
                        task_count++;
                        ret.task_ID = task_count;
                        Enqueue(queues[0],ret);
                        pthread_mutex_unlock(&packaging_mutex);

                }

            }

        }



    }


}

// manages Santa's tasks
void* Santa(void *arg){
    FILE *logfile;
    while(1){

    if (!isEmpty(queues[8])){    
            pthread_mutex_lock(&NZ_delivery_mutex);
            Task ret = Dequeue(queues[8]);
            pthread_mutex_unlock(&NZ_delivery_mutex);
            pthread_sleep(1);
            gettimeofday(&timee, NULL);
          
            logfile = fopen("events1.log", "a");   
            pthread_mutex_lock(&file_mutex); // acquire the file mutex lock
            fprintf(logfile, "%d\t %d\t %d\t %c\t    %d\t\t %d\t  %ld\t %c\t\t--NZ gift\n", 
                                ret.task_ID, ret.ID, ret.type, 'D', ret.request_time, ret.arrival_time, timee.tv_sec - start_time.tv_sec - ret.arrival_time, 'S');
            pthread_mutex_unlock(&file_mutex); // release the file mutex lock
            
            fclose(logfile);

           
    }
    
    else if (!isEmpty(queues[9])){  
            pthread_mutex_lock(&NZ_qa_mutex);
            Task ret = Dequeue(queues[9]);
            gettimeofday(&timee, NULL);
            pthread_mutex_unlock(&NZ_qa_mutex);
            pthread_sleep(1);
            gettimeofday(&timee, NULL);
            
            logfile = fopen("events1.log", "a");   
            pthread_mutex_lock(&file_mutex); // acquire the file mutex lock

            fprintf(logfile, "%d\t %d\t %d\t %c\t    %d\t\t %d\t  %ld\t %c\t\t--NZ gift\n", 
                                ret.task_ID, ret.ID, ret.type, 'Q', ret.request_time, ret.arrival_time, timee.tv_sec - start_time.tv_sec - ret.arrival_time, 'S');
            pthread_mutex_unlock(&file_mutex); // release the file mutex lock
            
            fclose(logfile);
            pthread_mutex_lock(&taskmutex[ret.ID]);
            completed[ret.ID]+=1;
 
            pthread_mutex_unlock(&taskmutex[ret.ID]);

            if (completed[ret.ID]==2){
                gettimeofday(&timee, NULL);
                ret.request_time = timee.tv_sec - start_time.tv_sec;
                pthread_mutex_lock(&NZ_packaging_mutex);
                task_count++;
                ret.task_ID = task_count;
                Enqueue(queues[5],ret);
                pthread_mutex_unlock(&NZ_packaging_mutex);
                gettimeofday(&timee, NULL);
 
                    }



        }
    //NZ SECTION END
    else if (!isEmpty(queues[3])){    
            pthread_mutex_lock(&delivery_mutex);
            Task ret = Dequeue(queues[3]);
            pthread_mutex_unlock(&delivery_mutex);
            pthread_sleep(1);
            gettimeofday(&timee, NULL);
            
            logfile = fopen("events1.log", "a");   
            pthread_mutex_lock(&file_mutex); // acquire the file mutex lock

            fprintf(logfile, "%d\t %d\t %d\t %c\t    %d\t\t %d\t  %ld\t %c\n", 
                                ret.task_ID, ret.ID, ret.type, 'D', ret.request_time, ret.arrival_time, timee.tv_sec - start_time.tv_sec - ret.arrival_time, 'S');
            pthread_mutex_unlock(&file_mutex); // release the file mutex lock
            
            fclose(logfile);
           
    }
    else{
        if (!isEmpty(queues[4])){ 
 
            pthread_mutex_lock(&qa_mutex);
            Task ret = Dequeue(queues[4]);
            gettimeofday(&timee, NULL);
            pthread_mutex_unlock(&qa_mutex);
            pthread_sleep(1);
            gettimeofday(&timee, NULL);

            logfile = fopen("events1.log", "a");   
            pthread_mutex_lock(&file_mutex); // acquire the file mutex lock

            fprintf(logfile, "%d\t %d\t %d\t %c\t    %d\t\t %d\t  %ld\t %c\n", 
                                ret.task_ID, ret.ID, ret.type, 'Q', ret.request_time, ret.arrival_time, timee.tv_sec - start_time.tv_sec - ret.arrival_time, 'S');
            pthread_mutex_unlock(&file_mutex); // release the file mutex lock
            
            fclose(logfile);

            pthread_mutex_lock(&taskmutex[ret.ID]);
            completed[ret.ID]+=1;
 
            pthread_mutex_unlock(&taskmutex[ret.ID]);

            if (completed[ret.ID]==2){
                gettimeofday(&timee, NULL);
                ret.request_time = timee.tv_sec - start_time.tv_sec;
                pthread_mutex_lock(&packaging_mutex);
                task_count++;
                ret.task_ID = task_count;
                Enqueue(queues[0],ret);
                pthread_mutex_unlock(&packaging_mutex);
                gettimeofday(&timee, NULL);
 
            }
        }
    }
}


}

// the function that controls queues and output
void* ControlThread(void *arg){
    struct timeval time_now;
    char queue_names[5][10] = {"Packaging", "Painting", "Assembly", "Delivery", "QA"};
    
    pthread_t tid[3];
    pthread_create(&tid[0], NULL, &ElfA, (void *)1);
    pthread_create(&tid[1], NULL, &ElfB, (void *)2);
    pthread_create(&tid[2], NULL, &Santa, (void *)3);

    while(1){
        gettimeofday(&time_now, NULL);
        if (time_now.tv_sec - start_time.tv_sec >= n){
            printf("------------Waiting tasks at %ld secs------------\n", time_now.tv_sec - start_time.tv_sec);
            int i = 5; // start from New Zealand gifts since they have the priority
            while (i < 10){
                int normal_queue_index = i%5; 
                printf("%s :",queue_names[normal_queue_index]);

                Queue *pQueue;
                pQueue = queues[i];
                NODE *item;
                Task ret;
                int taskID;

                item = pQueue->head;
                while(item != NULL){
                    ret = item->data;
                    taskID = ret.task_ID;
                    printf("%d(NZ) ",taskID);
                    item = item->prev;
                }
                // iterate the normal gifts waiting for the same task
                
                pQueue = queues[normal_queue_index];
                item = pQueue->head;
                while(item != NULL){
                    ret = item->data;
                    taskID = ret.task_ID;
                    printf("%d ",taskID);
                    item = item->prev;
                }
                printf("\n");
                i++;
            }
        } 
        pthread_sleep(1);
    }
}
