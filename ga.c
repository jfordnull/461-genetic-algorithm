#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define POPULATION_SIZE 500
#define NUM_GENERATIONS 100
#define MUTATION_RATE 0.1
#define NUM_ACTIVITIES 11
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
    Gene chromosome[NUM_ACTIVITIES];
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

Activity activities[] = {
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
    int facilitator_load[NUM_FACILITATORS] = {0};
    for(int i = 0; i < NUM_ACTIVITIES; i++){
        Gene gene = individual->chromosome[i];
        Activity activity = activities[i];
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

        // Conflict checks
        for(int j = i + 1; j < NUM_ACTIVITIES; j++){
            Gene other_gene = individual->chromosome[j];
            if(gene.room_id == other_gene.room_id && gene.time_slot == other_gene.time_slot) fitness -= 0.5;
            if(gene.facilitator_id == other_gene.facilitator_id && gene.time_slot == other_gene.time_slot) fitness -= 0.2;
            else fitness += 0.2;
        }

        facilitator_load[gene.facilitator_id]++;
    }

    // Facilitator load
    for(int i = 0; i < NUM_FACILITATORS; i++){
        if(facilitator_load[i] > 4) fitness -= 0.5;
        else if(i != 7 && facilitator_load[i] < 3) fitness -= 0.4;
    }

    // Activity-specific adjustments
    int SLA101A_time = individual->chromosome[0].time_slot;
    int SLA101B_time = individual->chromosome[1].time_slot;
    int SLA191A_time = individual->chromosome[2].time_slot;
    int SLA191B_time = individual->chromosome[3].time_slot;
    int SLA101A_room = individual->chromosome[0].room_id;
    int SLA101B_room = individual->chromosome[1].room_id;
    int SLA191A_room = individual->chromosome[2].room_id;
    int SLA191B_room = individual->chromosome[3].room_id;

    // 101 or 191 sections more than 4 hours apart
    if (abs(SLA101A_time - SLA101B_time) > 4) fitness += 0.5;
    else if (SLA101A_time == SLA101B_time) fitness -= 0.5;
    if (abs(SLA191A_time - SLA191B_time) > 4) fitness += 0.5;
    else if (SLA191A_time == SLA191B_time) fitness -= 0.5;

    // Check if 101 and 191 in consecutive time slots
    int is_consecutive = 0;
    if (abs(SLA101A_time - SLA191A_time) == 1 || abs(SLA101A_time - SLA191B_time) == 1 ||
        abs(SLA101B_time - SLA191A_time) == 1 || abs(SLA101B_time - SLA191B_time) == 1){
        fitness += 0.5;
        is_consecutive = 1;
    }

    if (is_consecutive) {
        // Either activity in Roman (1, 3) or Beach (5, 6)
        int SLA101_in_roman_or_beach = (SLA101A_room == 1 || SLA101A_room == 3 || SLA101A_room == 5 || SLA101A_room == 6 ||
                                    SLA101B_room == 1 || SLA101B_room == 3 || SLA101B_room == 5 || SLA101B_room == 6);
        int SLA191_in_roman_or_beach = (SLA191A_room == 1 || SLA191A_room == 3 || SLA191A_room == 5 || SLA191A_room == 6 ||
                                    SLA191B_room == 1 || SLA191B_room == 3 || SLA191B_room == 5 || SLA191B_room == 6);

        // Apply penalty if one in Roman/Beach and other is not
        if ((SLA101_in_roman_or_beach && !SLA191_in_roman_or_beach) ||
            (!SLA101_in_roman_or_beach && SLA191_in_roman_or_beach)) {
            fitness -= 0.4;
        }
    }

    // 101 and 191 separated by exactly one hour
    if (abs(SLA101A_time - SLA191A_time) == 2 || abs(SLA101A_time - SLA191B_time) == 2 ||
        abs(SLA101B_time - SLA191A_time) == 2 || abs(SLA101B_time - SLA191B_time) == 2) {
        fitness += 0.25;
    }

    // 101 and 191 in same time slot
    if (SLA101A_time == SLA191A_time || SLA101A_time == SLA191B_time ||
        SLA101B_time == SLA191A_time || SLA101B_time == SLA191B_time) {
        fitness -= 0.25;
    }

    return fitness;
}

void initialize_individual(Individual *individual){
    for(int i = 0; i < NUM_ACTIVITIES; i++){
        individual->chromosome[i].activity_id = i;
        individual->chromosome[i].room_id = rand() % NUM_ROOMS;
        individual->chromosome[i].time_slot = rand() % NUM_TIMES;
        individual->chromosome[i].facilitator_id = rand() % NUM_FACILITATORS;
    }
    individual->fitness = calculate_fitness(individual);
}

void population_heapify(Individual *population, int size, int index){
    int smallest = index;
    int left = 2 * index + 1; // Left child
    int right = 2 * index + 2; // Right child

    if(left < size && population[left].fitness < population[smallest].fitness) smallest = left;
    if(right < size && population[right].fitness < population[smallest].fitness) smallest = right;
    if(smallest != index){
        Individual temp = population[index];
        population[index] = population[smallest];
        population[smallest] = temp;
        population_heapify(population, size, smallest);
    }
}

void crossover(Individual *parent1, Individual *parent2, Individual *child){
    for(int i = 0; i < NUM_ACTIVITIES; i++){
        if(rand() % 2) child->chromosome[i] = parent1->chromosome[i];
        else child->chromosome[i] = parent2->chromosome[i];
    }
    child->fitness = calculate_fitness(child);
}

void mutate(Individual *individual){
    for(int i = 0; i < NUM_ACTIVITIES; i++){
        if((float)rand() / RAND_MAX < MUTATION_RATE){
            individual->chromosome[i].room_id = rand() % NUM_ROOMS;
            individual->chromosome[i].time_slot = rand() % NUM_TIMES;
            individual->chromosome[i].facilitator_id = rand() % NUM_FACILITATORS;
        }
    }
    individual->fitness = calculate_fitness(individual);
}

// Replace root of heap with last individual and heapify (sift down)
void cull(Individual *population, int *size){
    population[0] = population[--(*size)];
    population_heapify(population, *size, 0);
}

// Insert individual at end of tree and sift up
void insert_individual(Individual *population, int *size, Individual individual){
    (*size)++;
    int i = *size - 1;
    // (i - 1)/2 : Parent
    while(i > 0 && population[(i - 1)/2].fitness > individual.fitness){
        population[i] = population[(i - 1)/2];
        i = (i - 1)/2;
    }
    population[i] = individual;
}

void initialize_population(Individual *population){
    for(int i = 0; i < POPULATION_SIZE; i++) initialize_individual(&population[i]);
    // Construct heap (POPSIZE / 2 - 1 starts at middle of heap, above leaf nodes, and moves toward root heapifying)
    for(int i = POPULATION_SIZE / 2 - 1; i >= 0; i--) population_heapify(population, POPULATION_SIZE, i);
}

void display_individual(Individual *individual) {
    printf("Schedule (Fitness = %.2f):\n", individual->fitness);
    printf("--------------------------------------------------------\n");
    printf("| %-10s | %-10s | %-8s | %-10s |\n", "Activity", "Room", "Time", "Facilitator");
    printf("--------------------------------------------------------\n");
    for (int i = 0; i < NUM_ACTIVITIES; i++) {
        int activity_id = individual->chromosome[i].activity_id;
        int room_id = individual->chromosome[i].room_id;
        int time_slot = individual->chromosome[i].time_slot;
        int facilitator_id = individual->chromosome[i].facilitator_id;
        printf("| %-10s | %-10s | %-8s | %-10s |\n",
               activities[activity_id].name,
               rooms[room_id].name,
               times[time_slot],
               facilitators[facilitator_id]);
    }
    printf("--------------------------------------------------------\n");
}


void genetic_algorithm(){
    Individual population[POPULATION_SIZE];
    initialize_population(population);
    int headcount = POPULATION_SIZE;

    for(int generation = 0; generation < NUM_GENERATIONS; generation++){
        for(int i = 0; i < POPULATION_SIZE; i++){
            cull(population, &headcount);
            Individual parent1 = population[rand() % headcount];
            Individual parent2 = population[rand() % headcount];
            Individual child;
            crossover(&parent1, &parent2, &child);
            mutate(&child);
            insert_individual(population, &headcount, child);
        }
        float best_fitness = population[0].fitness;
        for(int i = 1; i < POPULATION_SIZE; i++){
            if(population[i].fitness > best_fitness) best_fitness = population[i].fitness;
        }
        printf("Generation %d: Best Fitness = %.2f\n", generation, best_fitness);
    }
    
    Individual *fittest_individual = &population[0];
    for (int i = 1; i < POPULATION_SIZE; i++) {
        if (population[i].fitness > fittest_individual->fitness) {
            fittest_individual = &population[i];
        }
    }
    printf("\nFinal Generation - Fittest Individual\n");
    display_individual(fittest_individual);
}

int main(){
    srand(time(NULL));
    genetic_algorithm();
    return 0;
}
