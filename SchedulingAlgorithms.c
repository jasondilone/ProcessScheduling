#include <stdio.h>
#include <stdlib.h>
#define TIME 13
#define SIZE 4

typedef struct Process {
    char id;
    int duration, arrival, priority;
    int completedTime;
} Process;

typedef struct Table {
    Process running;
    Process waiting[SIZE];
    Process completed[SIZE];
    int waitIndex, completeIndex;
} Table;

// Function Prototypes
int getOriginalDuration(Process, Process[]);
void getAWT(int*, float*, Table, Process[]);
void initProcesses(Process[]);
void initTable(Table*);
void printList(Process[], int);



// Algorithms
void fcfs(Process[], int*, float*);
void roundRobin(Process[], int*, float*);
void srtf(Process[], int*, float*);
void priority(Process[], int*, float*);

// --------v-MAIN FUNCTION-v--------
int main() {

    // Variables
    Process process[SIZE];
    int tt, tt2, tt3, tt4;
    tt = tt2 = tt3 = tt4 = 0;
    float awt, awt2, awt3, awt4;
    awt = awt2 = awt3 = awt4 = 0.0f;
    initProcesses(process);

    // Algorithms
    fcfs(process, &tt, &awt);
    roundRobin(process, &tt2, &awt2);
    srtf(process, &tt3, &awt3);
    priority(process, &tt4, &awt4);

    // Printing final results
    printf("\n                  FCFS | Round Robin | SRTF | Priority\n");
    printf("Total Turnaround: %-6d %-13d %-6d %d\n", tt, tt2, tt3, tt4);
    printf("Avg. Wait Time:   %-6.2f %-13.2f %-6.2f %.2f\n\n", awt, awt2, awt3, awt4);
    return 0;
}
// --------^-MAIN FUNCTION-^--------

// Get original duration value to get 'awt'
int getOriginalDuration(Process process, Process processes[]) {
    switch (process.id) {
    case 'A':
        return processes[0].duration;
        break;
    case 'B':
        return processes[1].duration;
        break;
    case 'C':
        return processes[2].duration;
        break;
    case 'D':
        return processes[3].duration;
        break;
    default:
        return 0;
        break;
    }
}
void getAWT(int *tt, float *awt, Table table, Process process[]) {
    // Get Average waiting time
    for(int i = 0; i < SIZE; i++) {
        int turnaround = 0;
        turnaround = table.completed[i].completedTime - table.completed[i].arrival;
        *awt += turnaround - getOriginalDuration(table.completed[i], process); // process[] is used here
    }
    *awt /= 4;
}
void initProcesses(Process processes[]) {
    Process process[SIZE] = {{'A', 4, 1, 1}, {'B', 2, 2, 2},
                             {'C', 2, 4, 4}, {'D', 3, 5, 3}};
    for (int i = 0; i < SIZE; i++) {
        processes[i] = process[i];
    }                         
}
void initTable(Table *table) {
    table->running.id = ' ';
    table->waitIndex = 0;
    table->completeIndex = 0;
    for(int i = 0; i < SIZE; i++) {
        table->waiting[i].id = ' ';
        table->waiting[i].arrival = 0;
        table->completed[i].id = ' ';
    }
}
void printList(Process list[], int length) {
    for(int i = 0; i < length; i++) {
        printf("%c", list[i].id);
    }
}

// Runs FCFS algorithm
void fcfs(Process process[], int *tt, float *awt) {
    Table table;
    initTable(&table);
    printf("----------------\nFCFS\n----------------\n");
    printf("TIME | RUNNING | WAITING | COMPLETED\n");
    for (int time = 1; time < TIME; time++) {

        // Check for arrivals and add to waiting
        for (int i = 0; i < SIZE; i++) {
            if (process[i].arrival == time) {
                table.waiting[table.waitIndex] = process[i];
                table.waitIndex++;
            }
        }

        // Check if a process is running and send next in waiting to run
        if (table.running.id == ' ' && time < 12) {
            table.running = table.waiting[0];
            for (int i = 0; i < SIZE; i++) {
                table.waiting[i] = table.waiting[i + 1];
            }
            table.waitIndex--;
        }

        // Print results
        printf("%-6d %-10c", time, table.running.id);
        printList(table.waiting, table.waitIndex);
        printf("           ");
        printList(table.completed, table.completeIndex);
        printf("\n");

        // Run process and send to completed if finished
        table.running.duration--;
        if (table.running.duration == 0) {
            table.completed[table.completeIndex] = table.running;
            table.completed[table.completeIndex].completedTime = time + 1;      
            table.completeIndex++;
            table.running.id = ' ';
        }
    }

    // Get total turnaround
    for(int i = 0; i < SIZE; i++) {
        *tt += table.completed[i].completedTime - table.completed[i].arrival;
    }

    getAWT(tt, awt, table, process);
    printf("\n");
}

// Runs Round Robin algorithm
void roundRobin(Process process[], int *tt2, float *awt2) {
    Table table;
    initTable(&table);
    printf("----------------\nRound Robin\n----------------\n");
    printf("TIME | RUNNING | WAITING | COMPLETED\n");
    for(int time = 1; time < TIME; time++) {

        // make space for previous ran process in 'waiting' if new process arrived
        for(int i = table.waitIndex; i >= 0; i--) {
            if(table.waiting[i].arrival == time) {
                table.waiting[table.waitIndex + 1] = table.waiting[table.waitIndex];
            } 
        }

        // Check arrivals
        for(int i = 0; i < SIZE; i++) {
            if(process[i].arrival == time) {
                table.waiting[table.waitIndex] = process[i];
                table.waitIndex++;
            }
        }

        // Send to run
        table.running = table.waiting[0];
        for(int i = 0; i < SIZE; i++) {
            table.waiting[i] = table.waiting[i + 1];
        }
        table.waitIndex--;
        table.running.duration--;
        
        // Print results
        printf("%-6d %-10c", time, table.running.id);
        printList(table.waiting, table.waitIndex);
        printf("           ");
        printList(table.completed, table.completeIndex);
        printf("\n");

        // Send to complete if finished; else send back to waiting
        if(table.running.duration == 0) {
            table.completed[table.completeIndex] = table.running;
            table.completed[table.completeIndex].completedTime = time + 1;
            table.completeIndex++;
            if (table.completeIndex == SIZE) {
                printf("%-6d %-10c", time+1, ' ');
                printList(table.waiting, table.waitIndex);
                printf("           ");
                printList(table.completed, table.completeIndex);
                printf("\n\n");

                // Get total turnaround
                for(int i = 0; i < SIZE; i++) {
                    *tt2 += table.completed[i].completedTime - table.completed[i].arrival;
                }
                getAWT(tt2, awt2, table, process);
                break;
            }
            table.running.id = ' ';
        } else {
            table.waiting[table.waitIndex] = table.running;
            table.waitIndex++;
        }
    }
}

// Runs SRTF algorithm
void srtf(Process process[], int *tt3, float *awt3) {
    Table table;
    initTable(&table);
    Process shortest;
    shortest.duration = 10;
    int shortestIndex;
    printf("----------------\nSRTF\n----------------\n");
    printf("TIME | RUNNING | WAITING | COMPLETED\n");
    for(int time = 1; time < TIME; time++) {

        // Check for arrivals
        for(int i = 0; i < SIZE; i++) {
            if(process[i].arrival == time) {
                table.waiting[table.waitIndex] = process[i];
                table.waitIndex++;
            }
        }

        // Check which process in wait has shortest time left
        for(int i = 0; i < table.waitIndex; i++) {
            if(table.waiting[i].duration <= shortest.duration) {
                shortest = table.waiting[i];
                shortestIndex = i;
            }
        }
        table.running = shortest;

        // Move processes in wait down from where shortest was
        for(int i = shortestIndex; i < table.waitIndex; i++) {
            table.waiting[i] = table.waiting[i + 1];
        }
        table.waitIndex--;
        table.running.duration--;

        // Print results
        printf("%-6d %-10c", time, table.running.id);
        printList(table.waiting, table.waitIndex);
        printf("           ");
        printList(table.completed, table.completeIndex);
        printf("\n");

        // Send to completed if finished else add back to wait
        if(table.running.duration == 0) {
            table.completed[table.completeIndex] = table.running;
            table.completed[table.completeIndex].completedTime = time + 1;
            table.completeIndex++;

            // Print last row and break loop
            if (table.completeIndex == SIZE) {
                printf("%-6d %-10c", time+1, ' ');
                printList(table.waiting, table.waitIndex);
                printf("           ");
                printList(table.completed, table.completeIndex);
                printf("\n\n");

                // Get total turnaround
                for(int i = 0; i < SIZE; i++) {
                    *tt3 += table.completed[i].completedTime - table.completed[i].arrival;
                }
                getAWT(tt3, awt3, table, process);
                break;
            }

            // Reset 'shortest'
            //shortest.duration = 10;
        } else {
            table.waiting[table.waitIndex] = table.running;
            table.waitIndex++;
            shortest = table.running;
        }
        shortest.duration = 10;
    }
}

// Runs Priority algorithm
void priority(Process process[], int *tt4, float *awt4) {
    Table table;
    initTable(&table);
    Process highestPriority;
    highestPriority.priority = 0;
    int hpIndex;
    printf("----------------\nPriority\n----------------\n");
    printf("TIME | RUNNING | WAITING | COMPLETED\n");
    for(int time = 1; time < TIME; time++) {

        // Check for arrivals
        for(int i = 0; i < SIZE; i++) {
            if(process[i].arrival == time) {
                table.waiting[table.waitIndex] = process[i];
                table.waitIndex++;
            }
        }

        // Select highest priority to run from waiting
        for(int i = 0; i < table.waitIndex; i++) {
            if(table.waiting[i].priority > highestPriority.priority) {
                highestPriority = table.waiting[i];
                hpIndex = i;
            }
        }
        table.running = highestPriority;

        // Move processes in waiting down one step from where highest priority was
        for(int i = hpIndex; i < table.waitIndex; i++) {
            table.waiting[i] = table.waiting[i + 1];
        }
        table.waitIndex--;
        table.running.duration--;
        
        // Print results
        printf("%-6d %-10c", time, table.running.id);
        printList(table.waiting, table.waitIndex);
        printf("           ");
        printList(table.completed, table.completeIndex);
        printf("\n");

        if(table.running.duration == 0) {
            table.completed[table.completeIndex] = table.running;
            table.completed[table.completeIndex].completedTime = time + 1;
            table.completeIndex++;
            if (table.completeIndex == SIZE) {
                printf("%-6d %-10c", time+1, ' ');
                printList(table.waiting, table.waitIndex);
                printf("           ");
                printList(table.completed, table.completeIndex);
                printf("\n\n");

                // Get total turnaround
                for(int i = 0; i < SIZE; i++) {
                    *tt4 += table.completed[i].completedTime - table.completed[i].arrival;
                }
                getAWT(tt4, awt4, table, process);
                break;
            }
            highestPriority.priority = 0;
        } else {
            table.waiting[table.waitIndex] = table.running;
            table.waitIndex++;
            highestPriority = table.running;
        }
    }
}