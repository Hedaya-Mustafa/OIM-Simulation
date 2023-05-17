#include "local.h"

void end(int);

// Signal Handlers
void body_check();
void accepted();
void denied();

// Signal Flags
// go to body check!
volatile int enter = 1;
// everything is fine
volatile int accept = 1;
// incompelte request
volatile int unhappy = 1;

// gender of this palestinan
char GENDER;

// POSIX Shared Memory to share queues and parameters
int shm_fd;
shared_memory *shm_ptr;

// POSIX Message Queue
mqd_t mq;

// Semaphore Locks
sem_t *counters_lock;


// Args gender, type, patiente , complete_request, arrival
int main(int argc, char **argv)
{

    // signal telling the palestinian to go to the queue
    if (signal(SIGUSR1, accepted) == SIG_ERR)
    {
        perror("Error Attaching SIGUSR1");
        exit(1);
    }

    // signal telling the palestinian that his request is denied
    if (signal(SIGUSR2, denied) == SIG_ERR)
    {
        perror("Error Attaching SIGUSR2");
        exit(1);
    }

    // signal telling the palestinian to go to the body check
    if (signal(SIGINT, body_check) == SIG_ERR)
    {
        perror("Error Attaching SIGINT");
        exit(1);
    }

    // opening counters lock
    if ((counters_lock = sem_open(COUNTERS_LOCK_NAME, O_CREAT)) < 0)
    {
        perror("Counters Lock Error");
        exit(1);
    }

    // Open shared memory object
    if ((shm_fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666)) < 0)
    {
        perror("Error Allocating Shared Memory Object");
        end(1);
    }

    // Pointer to shared memory object
    shm_ptr = (shared_memory *)malloc(sizeof(shared_memory));
    shm_ptr = mmap(0, 4096, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // set the GENDER
    GENDER = argv[1][0];

    // palestinian parameters=> gender, type of documents, patience time, complete request, arrival time and current time
    char gender = argv[1][0];
    char type = argv[2][0];
    int patience = atoi(argv[3]);
    int complete_request = atoi(argv[4]);
    time_t arrival = time(NULL);
    time_t current_time;
    
    // if the time is still not 8 am, wait outside the gate
    while ((shm_ptr->creation + 3) > time(NULL))
    {
        sleep(1);
    }

    // check patience
    time(&current_time);
    if ((current_time - arrival) > patience)
    {
        // going back
        sem_wait(counters_lock);
        shm_ptr->number_unserved += 1;
        sem_post(counters_lock);
        fflush(stdout);
        printf("\nPalestinain %d returned home because they waited too long.\n", getpid());
        fflush(stdout);
        end(0);
    }

    // Looking at the queue to see if it's below threshold
    char mq_name[50], gender_string[7];
    struct mq_attr attr;
   // attr.mq_msgsize = sizeof(Palestinian);
    int queue_size;

    // check the queue according to gender
    if (gender == 'F')
    {
        sprintf(mq_name, FEMALE_QUEUE_NAME);
        sprintf(gender_string, "Female");
    }
    else
    {
        sprintf(mq_name, MALE_QUEUE_NAME);
        sprintf(gender_string, "Male");
    }
    mq = mq_open(mq_name, O_RDWR | O_CREAT, 0666, NULL);
    mq_getattr(mq, &attr);
    // Get the queue size
    queue_size = attr.mq_curmsgs;

    // check if queue has more than the max threshold
    if (queue_size >= (int)shm_ptr->parameters[THRESHOLD_QUEUE_U])
    {
        // block until the queue reaches the min threshold
        fflush(stdout);
        printf("\n%s Queue stopped reciving palestinian beacuse it has reached its max threshold, now it has %d palestinains\n", gender_string, queue_size);
        fflush(stdout);

        // update the size
        mq_getattr(mq, &attr);
        queue_size = attr.mq_curmsgs;
        while (queue_size >= (int)shm_ptr->parameters[THRESHOLD_QUEUE_L])
        {
            sleep(1);
        }
    }

    // check patience after waiting for the queue
    time(&current_time);
    if ((current_time - arrival) > patience)
    {
        // going back
        sem_wait(counters_lock);
        shm_ptr->number_unserved += 1;
        sem_post(counters_lock);
        fflush(stdout);
        printf("\nPalestinain %d returned home because they waited too long.\n", getpid());
        fflush(stdout);
        end(0);
    }

   


    // send palestinian to the queue
    Palestinian p = {getpid(), gender, complete_request, type, arrival, patience};
    mq_getattr(mq, &attr);
    // result
    int res = mq_send(mq, (const char *)&p, sizeof(p), 0);
    //printf("res %d\n",res);
    fflush(stdout);
    //print values for palestinian whos waiting in the queue to their turn for body check 
    printf("\nPalestinan %d is waiting in their queue with following paramters: Gender: %c, Patience: %d, Type: %c, Complete Papers/Request: %d and result = %d\n",
           getpid(),
           gender,
           patience,
           type,
           complete_request, res);
    fflush(stdout);

  

    // wait for the occupation officer to take action
    while (enter)
    {
        // check patientce while waiting in the queue
        time(&current_time);
        if (current_time - arrival >= patience)
        {
            // going back
            sem_wait(counters_lock);
            shm_ptr->number_unserved += 1;
            sem_post(counters_lock);
            fflush(stdout);
            printf("\nPalestinain %d returned home because they waited too long.\n", getpid());
            fflush(stdout);
            end(0);
        }
        sleep(1);
    }

    // done the body check, send to the queue for tellers (inner grouping area)
    if (enter == 0)
    {
        // check patience after waiting for the queue
        time(&current_time);
        if ((current_time - p.arrival) > p.paitence)
        {
            // going back
            sem_wait(counters_lock);
            shm_ptr->number_unserved += 1;
            sem_post(counters_lock);
            fflush(stdout);
            printf("\nPalestinain %d returned home because they waited too long.\n", getpid());
            fflush(stdout);
            end(0);
        }



	char mq_name[50];
        struct mq_attr attr;
        // open the queue regarding the palestinian type
	switch(p.type)
	{
	    case 'B':
                sprintf(mq_name, B_QUEUE);
                break;
            case 'T':
                sprintf(mq_name, T_QUEUE);
                break;
            case 'I':
                sprintf(mq_name, I_QUEUE);
                break;
            default:
                sprintf(mq_name, R_QUEUE);
                break;
	}
	mq = mq_open(mq_name, O_RDWR | O_CREAT, 0666, NULL);
	mq_getattr(mq, &attr);
	int m_size = attr.mq_curmsgs;
	mq_close(mq);
	char empty_mq[50];	
	// if the queue is not empty means that teller is busy, try to choose another empty teller
	if(m_size > 0)
	{
		mq_close(mq);
		mq = mq_open(B_QUEUE, O_RDWR | O_CREAT, 0666, NULL);
		mq_getattr(mq, &attr);
		if (attr.mq_curmsgs == 0)
		{
		    sprintf(empty_mq, B_QUEUE);
		}
		mq_close(mq);
		if (empty_mq[0] == '\0')
		{
		    mq = mq_open(R_QUEUE, O_RDWR | O_CREAT, 0666, NULL);
		    mq_getattr(mq, &attr);
		    if (attr.mq_curmsgs == 0)
		    {
		        sprintf(empty_mq, R_QUEUE);
		    }
		}
		mq_close(mq);
		if (empty_mq[0] == '\0')
		{
		    mq = mq_open(I_QUEUE, O_RDWR | O_CREAT, 0666, NULL);
		    mq_getattr(mq, &attr);
		    if (attr.mq_curmsgs == 0)
		    {
		        sprintf(empty_mq, I_QUEUE);
		    }
		}
		mq_close(mq);
		if (empty_mq[0] == '\0')
		{
		    mq = mq_open(T_QUEUE, O_RDWR | O_CREAT, 0666, NULL);
		    mq_getattr(mq, &attr);
		    if (attr.mq_curmsgs == 0)
		    {
		        sprintf(empty_mq, T_QUEUE);
		    }
		}
		// if there is an empty teller, put his name in the queue to open
		if(empty_mq[0] != '\0')
			strcpy(mq_name, empty_mq);
	}	
	mq = mq_open(mq_name, O_RDWR | O_CREAT, 0666, NULL);
        // send the palestinian to the message queue for its teller
        res = mq_send(mq, (const char *)&p, sizeof(Palestinian), 0);
	fflush(stdout);
        printf("palestinian %d sent his documents to teller %s with result %d\n", getpid(), mq_name, res);
	fflush(stdout);        
	mq_close(mq);
    }


    while(unhappy && accept)
    {
    	// check patientce while waiting in the queue
        time(&current_time);
        if (current_time - arrival >= patience)
        {
            // going back
            sem_wait(counters_lock);
            shm_ptr->number_unserved += 1;
            sem_post(counters_lock);
            fflush(stdout);
            printf("\nPalestinain %d returned home because they waited too long.\n", getpid());
            fflush(stdout);
            end(0);
        }
        sleep(1);
    }
    
    // if the palestinian has an incomplete request, send them home.
    if (unhappy == 0)
    {
        // Return home
        // Increase number of unhappy palestinains
        sem_wait(counters_lock);
        shm_ptr->number_unhappy += 1;
        sem_post(counters_lock);
        fflush(stdout);
        printf("\nPalestinan %d returned home because their request is incomplete.\n", getpid());
        fflush(stdout);
        end(0);
    }
	
    
    // done the request and have to return
    if (accept == 0)
    {
        // Increase number of palestinian  who accepeted
        sem_wait(counters_lock);

        shm_ptr->number_satisfied += 1;

        sem_post(counters_lock);

        fflush(stdout);
        printf("\nPalestian %d is done.\n", getpid());
        fflush(stdout);
    }

    end(0);
}

void end(int status)
{

    // Closing the message queue
    mq_close(mq);


    sem_close(counters_lock);


    // Closing shared memory
    close(shm_fd);

    exit(status);
}

void accepted()
{
    accept = 0;
}

void denied()
{
    unhappy = 0;
}
void body_check()
{
    enter = 0;
}
