#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "router.h"

// Sets all weights to default MAX_WEIGHT. Then randomly generates weights from router to neighbor
void init_weights(router_t *r)
{
    // Preset costs all to 9999
    for (int i = 0; i < N_NEIGHBORS; i++)
    {
        for (int j = 0; j < N_NEIGHBORS; j++)
        {
            r->cost[i][j] = MAX_WEIGHT;
        }
    }

    // Set random weights for router;
    for (int neighbor = 0; neighbor < N_NEIGHBORS; neighbor++)
    {
        if (neighbor == r->id)
        {
            r->cost[r->id][r->id] = 0;
        }
        else
        {
            r->cost[r->id][neighbor] = rand() / 100000000 + 1;
        }
    }
}

void table_divider()
{
    printf("+---------------+");
    for (int i = 0; i < N_NEIGHBORS; i++)
    {
        printf("------+");
    }
    printf("\n");
}
// Print out the distance vector table
void display_router(router_t *r)
{
    // print header
    table_divider();
    printf("| Router (id %d) |", r->id);
    for (int i = 0; i < N_NEIGHBORS; i++)
    {
        printf(" %d    |", i);
    }
    printf("\n");
    table_divider();

    // Print rows
    for (int row = 0; row < N_NEIGHBORS; row++)
    {
        printf("|   %-4d        |", row);

        // Print columns
        for (int col = 0; col < N_NEIGHBORS; col++)
        {
            printf(" %-4d |", r->cost[row][col]);
        }
        printf("\n");
    }
    table_divider();
}

void print_weights(router_t * r) {
    for (int neighbor = 0; neighbor < N_NEIGHBORS; neighbor++)
    {
        printf("Weight %d->%d: %-4d\n", r->id, neighbor, r->cost[r->id][neighbor]);
    }
}

// This function closes all the file descriptors associated with this router
void close_router(router_t *r)
{
    // Close the read end from other routers
    close(r->r_readfd);

    // Close all the write descriptors to other routers
    for (int i = 0; i < N_NEIGHBORS; i++)
    {
        close(r->r_writefds[i]);
    }

    // Close file descriptors involving talking to main
    close(r->shell_readfd);
    close(r->shell_writefd);
}