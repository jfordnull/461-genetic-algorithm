#include <stdio.h>

#define POPULATION_SIZE 500
#define NUM_GENERATIONS 100
#define MUTATION_RATE 0.1
#define NUM_ACTIVITES 10
#define NUM_ROOMS 9
#define NUM_TIMES 6
#define NUM_FACILITATORS 10

typedef struct {
    int expected_enrollment;
    //Preferred Facilitators
    //Other Facilitators
} Activity;

typedef struct {
    char name[11];
    int capacity;
} Room;

typedef struct {
    int activity_id;
    int room_id;
    int time_slot;
    int facilitator_id;
} Gene;

typedef struct {
    //Array of assignments
    float fitness;
} Individual;

char facilitators[10][9] = {
    "Lock",
    "Glen",
    "Banks",
    "Richards",
    "Shaw",
    "Singer",
    "Uther",
    "Tyler",
    "Numen",
    "Zeldin"
};

Room rooms[] = {
    {"Slater 003", 45},
    {"Roman 216", 30},
    {"Loft 206", 75},
    {"Roman 201", 50},
    {"Loft 310", 108},
    {"Beach 201", 60},
    {"Beach 301", 75},
    {"Logos 325", 450},
    {"Frank 119", 60}
};



