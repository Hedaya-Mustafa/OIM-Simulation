#include "local.h"

// Function to end the process safely by Closing and deleteing shared memory and counters lock
int end(int);

// POSIX Shared Memory to share cross points queues, hall queue and parameters
int shm_fd;
shared_memory *shm_ptr;

// Semaphore Locks
sem_t *counters_lock, *male_lock, *female_lock, *reunion_lock, *travel_lock, *id_lock, *birth_lock;

int num_of_palestinians = 0; 



int main(int argc, char **argv)
{

    // Creating shared memory
    if ((shm_fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666)) < 0)
    {
        perror("Error Allocating Shared Memory Object");
        exit(1);
    }

    // Setting space of shared memory
    if (ftruncate(shm_fd, 4096) < 0)
    {
        perror("Error Mapping Shared Memory");
        end(1);
    }

    // Map shared memory into process space
    shm_ptr = (shared_memory *)malloc(sizeof(shared_memory));
    shm_ptr = mmap(0, 4096, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Open the input file
    char input_file_name[20];
    sprintf(input_file_name, "./%s", argv[1]);
    FILE *input_file;

    // Reading simulation parameters from input file
    if (argc != 2)
    {
        printf("Error: Invalid number of arguments\n");
        end(1);
    }
    else
    {
        // Open input file
        if ((input_file = fopen(input_file_name, "r")) == NULL)
        {
            perror("Error opening input file");
            end(1);
        }
        else
        {
            // Read lines of input file and extract paramters values
            char buffer[60], garbage_buffer[50];
            int value;
            int i = 0;
            while (fgets(buffer, 60, input_file))
            {
                // Extract parameter value
                sscanf(buffer, "%s %d", garbage_buffer, &value);
                shm_ptr->parameters[i] = value;
                i++;
            }
        }
    }

    // Creating counters lock
    if ((counters_lock = sem_open(COUNTERS_LOCK_NAME, O_CREAT, 0666, 1)) < 0)
    {
        perror("Counters Lock Error");
        exit(1);
    }

    // Initializing counters
    shm_ptr->number_unserved = 0;
    shm_ptr->number_unhappy = 0;
    shm_ptr->number_satisfied = 0;

    // Initialize time
    shm_ptr->creation = time(NULL);

    // Creating tellers processes
    int teller_id;
    int tellers[4];

    // tellers parameters
    char type[2], index[2], processing_time[4];
    int rand_processing_time;

    // generate the seed for rand() function
    srand(time(0));
    // type: Birth
    sprintf(type, "B");
    // processing time
    rand_processing_time = (int)shm_ptr->parameters[TELLER_B_PERIOD_U] - (int)shm_ptr->parameters[TELLER_B_PERIOD_L];
    sprintf(processing_time, "%d", rand_processing_time);

    // Create the first teller process 
    teller_id = fork();
    if (teller_id == 0)
    {
        execlp("./teller", "teller", type, processing_time, (void *)NULL);
    }
    tellers[0] = teller_id;

    // type: ID
    sprintf(type, "I");

    // processing time
    rand_processing_time = (int)shm_ptr->parameters[TELLER_I_PERIOD_U] - (int)shm_ptr->parameters[TELLER_I_PERIOD_L];
    sprintf(processing_time, "%d", rand_processing_time);

    // Create the second teller process
    teller_id = fork();
    if (teller_id == 0)
    {
        execlp("./teller", "teller", type, processing_time, (void *)NULL);
    }
    tellers[1] = teller_id;

    // type: Travel
    sprintf(type, "T");

    // processing time
    rand_processing_time = (int)shm_ptr->parameters[TELLER_T_PERIOD_U] - (int)shm_ptr->parameters[TELLER_T_PERIOD_L];
    sprintf(processing_time, "%d", rand_processing_time);

    // Create the third teller process
    teller_id = fork();
    if (teller_id == 0)
    {
        execlp("./teller", "teller", type, processing_time, (void *)NULL);
    }
    tellers[2] = teller_id;

    // type: Reunion
    sprintf(type, "R");

    // processing time
    rand_processing_time = (int)shm_ptr->parameters[TELLER_R_PERIOD_U] - (int)shm_ptr->parameters[TELLER_R_PERIOD_L];
    sprintf(processing_time, "%d", rand_processing_time);

    // Create the fourth teller process
    teller_id = fork();
    if (teller_id == 0)
    {
        execlp("./teller", "teller", type, processing_time, (void *)NULL);
        
    }
    tellers[3] = teller_id;

    pid_t officer_id = fork();
    if(officer_id == 0)
    {
        execl("./officer", "officer", (void*) NULL);
        
    }
    officer_id = fork();
    if(officer_id == 0)
    {
        execl("./officer", "officer", (void*) NULL);
        
    }
    pid_t interface_id = fork();
    if(interface_id == 0)
    {
    	execl("./graphics", "graphics", (void*) NULL);
    }
    
    // Creating Palestinians
    while (1)
    {
	if(num_of_palestinians < 200)
	{
		// Palestinian parameters
		int Palestinian_id, rand_patience;
		char gender[2], type[2], paitence[4], complete_request[2];

		// Randomizing gender
		srand(time(NULL));
		int random_number = rand() % 100 + 1;
		if (random_number <= shm_ptr->parameters[MALES])
		{
		    sprintf(gender, "M");
		}
		else
		{
		    sprintf(gender, "F");
		}

		// Randomizing documents: Birth, ID, Travel, Reunion

		random_number = rand() % 100 + 1;
		if (random_number < 25)
		{
		    sprintf(type, "R");
		}
		else if (random_number < 50)
		{
		    sprintf(type, "I");
		}
		else if (random_number < 75)
		{
		    sprintf(type, "B");
		}
		else
		{
		    sprintf(type, "T");
		}

		// Randomizing patience time
		rand_patience = shm_ptr->parameters[PALESTINIANS_PATIENCE_PERIOD_U] - shm_ptr->parameters[PALESTINIANS_PATIENCE_PERIOD_L];
		rand_patience = rand() % rand_patience + shm_ptr->parameters[PALESTINIANS_PATIENCE_PERIOD_L];
		sprintf(paitence, "%d", rand_patience);

		// Randomizing having documents, the majority have.
		random_number = rand() % 100 + 1;
		if (random_number <= 70)
		{
		    sprintf(complete_request, "1");
		}
		else
		{
		    sprintf(complete_request, "0");
		}

		Palestinian_id = fork();
	
		if (Palestinian_id == 0)
		{
		    execlp("./palestinian", "palestinian", gender, type, paitence, complete_request, (void *)NULL);
		    
		}
		else if(Palestinian_id == -1)
		{
			printf("failed to create palestinian\n");
		}
		num_of_palestinians++;// counting the number of palestinian
	    }

        sleep(1);
        sem_wait(counters_lock);
        fflush(stdout);
        printf("\n********************************************\n");
        printf("Number of Unserved Palestinians: %d\nNumber of Un Happy Palestinian: %d\nNumber of Satiesfied:%d",
               shm_ptr->number_unserved,
               shm_ptr->number_unhappy,
               shm_ptr->number_satisfied);
        printf("\n********************************************\n");
        fflush(stdout);
        if (shm_ptr->number_unserved >= shm_ptr->parameters[END_MINIMUM_UNSERVED])
        {
            sem_post(counters_lock);
            break;
        }
        if (shm_ptr->number_unhappy >= shm_ptr->parameters[END_MINIMUM_UN_HAPPY])
        {
            sem_post(counters_lock);
            break;
        }
        if (shm_ptr->number_satisfied >= shm_ptr->parameters[END_MINIMUM_SATISFIED])
        {
            sem_post(counters_lock);
            break;
        }
        sem_post(counters_lock);
    }

    // Send signal to tellers to end execution
    for (int i = 0; i < 4; i++)
    {
        kill(tellers[i], SIGINT);
    }

    // A special shell script which cleans the kernel from idle processes that were created and not killed
    // Also it cleans the shared memory and message queues that wasn't closed properly
    system("./clean.sh");

    sleep(2);
    fflush(stdout);
    printf("\nSimulation Finished\n");
    fflush(stdout);
    end(0);
}

int end(int status)
{
    // Closing and deleteing shared memory
    close(shm_fd);
    shm_unlink(SHARED_MEMORY_NAME);

    // Closing and deleting counters lock
    sem_close(counters_lock);
    sem_unlink(COUNTERS_LOCK_NAME);
    // exit
    exit(0);
}
