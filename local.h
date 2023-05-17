#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <signal.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <mqueue.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/resource.h>

// Enum used to access parameters
enum {
FEMALES,
MALES,
PALESTINIANS_PATIENCE_PERIOD_L,
PALESTINIANS_PATIENCE_PERIOD_U,
TELLER_B_PERIOD_L,
TELLER_B_PERIOD_U,
TELLER_I_PERIOD_L,
TELLER_I_PERIOD_U,
TELLER_T_PERIOD_L,
TELLER_T_PERIOD_U,
TELLER_R_PERIOD_L,
TELLER_R_PERIOD_U,
THRESHOLD_QUEUE_L,
THRESHOLD_QUEUE_U,
END_MINIMUM_UN_HAPPY,
END_MINIMUM_SATISFIED,
END_MINIMUM_UNSERVED
};

// Define a struct to store shared memory items
typedef struct shared_memory{
    // To store the program parameters
    int parameters[50];

    // Counters to store # of palestinian who unserved, unhappy, satisfied 
    int number_unserved;
    int number_unhappy;
    int number_satisfied;
    
    // To count the time
    time_t creation;
} shared_memory;

 // Struct for representing palestinian
typedef struct Palestinian{
    int pid;
    char gender;
    int complete_req;
    char type;
    time_t arrival;
    int paitence;
} Palestinian;

// Definitions
#define SHARED_MEMORY_NAME "/rtp_shared_memory"
#define COUNTERS_LOCK_NAME "/rtp_counters_lock"
#define BODY_CHECK_LOCK_NAME "/rtp_body_CHECK_lock"
#define MALE_QUEUE_NAME "/rtp_male_queue"
#define FEMALE_QUEUE_NAME "/rtp_female_queue"
#define R_QUEUE "/rtp_R_queue"
#define I_QUEUE "/rtp_I_queue"
#define T_QUEUE "/rtp_T_queue"
#define B_QUEUE "/rtp_B_queue"
