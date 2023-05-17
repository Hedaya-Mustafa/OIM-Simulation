#include "local.h"

volatile int run_flag = 1;

// POSIX Message Queue
mqd_t mq;



void end();

int main(int argc, char argv[])
{

    if (signal(SIGINT, end) == SIG_ERR)
    {
        perror("Error attaching SIGING");
        exit(0);
    }

    char mq_name[50];
    struct mq_attr attr;

    Palestinian *in_gate;

    fflush(stdout);
    printf("Officer is ready\n");
    fflush(stdout);
    while (run_flag)
    {
        //get first palestinian from the males or females queue randomly
        srand(time(NULL));
        int random = rand() % 100 + 1;
        if (random > 50)
        {
            sprintf(mq_name, FEMALE_QUEUE_NAME);
        }
        else
        {
            sprintf(mq_name, MALE_QUEUE_NAME);
        }

        mq = mq_open(mq_name, O_RDWR | O_CREAT, 0666, NULL);
       

        mq_getattr(mq, &attr);
        char *buffer = calloc(attr.mq_msgsize, 1);

        in_gate = (Palestinian *) buffer;
        
        if (mq_receive(mq, buffer, attr.mq_msgsize, 0) != -1)
        {
	    fflush(stdout);
            printf("palestinian %d has a body check now\n", in_gate->pid);
	    fflush(stdout);
	    sleep(5);
            kill(in_gate->pid, SIGINT);
            
        }
        sleep(1);
        mq_close(mq);
    }
    fflush(stdout);
    printf("Officer is closing\n");
    fflush(stdout);
}

void end()
{
    run_flag = 0;
}
