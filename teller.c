#include "local.h"

// function for Closing shared memory descriptors and the open semaphores. also, Closing and deleteing message queue of the teller
void stop_running();

// POSIX Shared Memory to share queues and parameters
int shm_fd;
shared_memory *shm_ptr;

// POSIX Message Queue
mqd_t mq;

// Semaphore Locks
sem_t *counters_lock;

// Semaphore for the gate checker/ body checker
sem_t *body_check_lock;

// name of the queue for the teller
char mq_name[30];

// the teller is still running
volatile int run_flag = 1;

// finish and clean
void end();

// Arguments: Type, Processing time
int main(int argc, char **argv)
{
    if (signal(SIGINT, end) == SIG_ERR)
    {
        perror("Error attaching SIGING");
        exit(0);
    }

    // Signal tell teller to stop execution
    if (signal(SIGINT, stop_running) == SIG_ERR)
    {
        perror("Error Attaching SIGUSR1");
        exit(1);
    }

    // Open shared memory object
    if ((shm_fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666)) < 0)
    {
        perror("Error Allocating Shared Memory Object");
        exit(1);
    }

    // opening counters lock
    if ((counters_lock = sem_open(COUNTERS_LOCK_NAME, O_CREAT)) < 0)
    {
        perror("Counters Lock Error");
        exit(1);
    }
    // opening body check lock
    if ((body_check_lock = sem_open(BODY_CHECK_LOCK_NAME, O_CREAT)) < 0)
    {
        perror("Body Check Lock Error");
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
    // Teller parameters
    char teller_type = argv[1][0];
    int processing_time = atoi(argv[2]);
    printf("teller type %c\n", teller_type);
    // Teller message queue parameters

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_msgsize = sizeof(Palestinian);
    attr.mq_curmsgs = 0;

    switch (teller_type)
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
    case 'R':
        sprintf(mq_name, R_QUEUE);
        break;
    }

    
    

    // Keep receiving tellers until the office closes
    while (run_flag)
    {
	mq = mq_open(mq_name, O_RDWR | O_CREAT, 0666, &attr);
        // Get message queue status
        struct mq_attr attr_updated;
        mq_getattr(mq, &attr_updated);
        char *buffer = calloc(attr_updated.mq_msgsize, 1);

        // Receive the Palestinian
        if(mq_receive(mq, buffer, attr_updated.mq_msgsize, 0) != -1)
{
        Palestinian *p = (Palestinian *)buffer;

        fflush(stdout);
        printf("\nPalestinian %d reached the teller (%c) and waiting for him to do his request.\n",
               p->pid,
               teller_type);
        fflush(stdout);

        // teller processing time
        sleep(processing_time);

        // check for compelete request
        if (p->complete_req == 0)
        {
            // Send him signal so that he can return home
	    fflush(stdout);
	    printf("Palestinian %d is denied because of the lack of the documents\n", p->pid);
            fflush(stdout);
	    kill(p->pid, SIGUSR2);
        }
        else
        {
            // accepted!
	    fflush(stdout);
	    printf("Palestinian %d is accepted\n", p->pid);
            fflush(stdout);
	    kill(p->pid, SIGUSR1);
        }
}
	mq_close(mq);
	sleep(1);
    }
    
}

void stop_running()
{
    run_flag = 0;
    // Closing shared memory descriptors
    close(shm_fd);

    // Closing and deleteing message queue of the teller
    mq_close(mq);
    mq_unlink(mq_name);

    // Closing open semaphores
    sem_close(counters_lock);
    sem_close(body_check_lock);
    exit(0);
}
