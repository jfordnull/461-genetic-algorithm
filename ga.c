#include <stdio.h>
#include <stdlib.h>

#define POPULATION_SIZE 500
#define NUM_GENERATIONS 100
#define MUTATION_RATE 0.1
#define NUM_ACTIVITES 11
#define NUM_ROOMS 9
#define NUM_TIMES 6
#define NUM_FACILITATORS 10

typedef struct {
    char name[8];
    int expected_enrollment;
    int preferred_facilitators[NUM_FACILITATORS];
    int other_facilitators[NUM_FACILITATORS];
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
    Gene chromosome[NUM_ACTIVITES];
    float fitness;
} Individual;

char facilitators[][9] = {
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

Activity activites[] = {
    {"SLA100A", 50, 
        {1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 1, 0, 0, 0, 0, 1, 0}
    },
    {"SLA100B", 50,
        {1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 1, 0, 0, 0, 0, 1, 0}
    },
    {"SLA191A", 50,
        {1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 1, 0, 0, 0, 0, 1, 0}
    },
    {"SLA191B", 50,
        {1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 1, 0, 0, 0, 0, 1, 0}
    },
    {"SLA201", 50,
        {0, 1, 1, 0, 1, 0, 0, 0, 0, 1},
        {0, 0, 0, 1, 0, 1, 0, 0, 1, 0}
    },
    {"SLA291", 50,
        {1, 0, 1, 0, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 1, 1, 0, 0, 1, 1, 0}
    },
    {"SLA303", 60,
        {0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 1, 1, 0, 0, 1, 0}
    },
    {"SLA304", 25,
        {0, 1, 1, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 0, 1, 1}
    },
    {"SLA394", 20,
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 1}
    },
    {"SLA449", 60,
        {0, 0, 0, 0, 1, 1, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 1}
    },
    {"SLA451", 100,
        {0, 0, 0, 0, 1, 1, 0, 1, 0, 0},
        {0, 0, 1, 1, 0, 0, 1, 0, 0, 1}
    }
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

char times[][6] = {
    "10 AM",
    "11 AM",
    "12 PM",
    "1 PM",
    "2 PM",
    "3 PM"
};

float calculate_fitness(Individual *individual){
    float fitness = 0.0;
    for(int i = 0; i < NUM_ACTIVITES; i++){
        Gene gene = individual->chromosome[i];
        Activity activity = activites[gene.activity_id];
        Room room = rooms[gene.room_id];

        // Capacity
        if(room.capacity < activity.expected_enrollment) fitness -= 0.5;
        else if(room.capacity > activity.expected_enrollment * 6) fitness -= 0.4;
        else if(room.capacity > activity.expected_enrollment * 3) fitness -= 0.2;
        else fitness += 0.3;

        // Preferred facilitator
        if(activity.preferred_facilitators[gene.facilitator_id]) fitness += 0.5;
        else if(activity.other_facilitators[gene.facilitator_id]) fitness += 0.2;
        else fitness -= 0.1;
    }
}

void initialize_individual(Individual* individual){
    for(int i = 0; i < NUM_ACTIVITES; i++){
        individual->chromosome[i].activity_id = i;
        individual->chromosome[i].room_id = rand() % NUM_ROOMS;
        individual->chromosome[i].time_slot = rand() % NUM_TIMES;
        individual->chromosome[i].facilitator_id = rand() % NUM_FACILITATORS;
    }
    individual->fitness = calculate_fitness(individual);
}

void mutate(Individual *individual);
void crossover(Individual *parent1, Individual *parent2, Individual *child);