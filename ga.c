#include <stdio.h>

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
