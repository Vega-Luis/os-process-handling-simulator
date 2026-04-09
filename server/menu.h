#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include "scheduler_type.h"

SchedulerType select_scheduler() {
    int option;

    printf("Seleccione el algoritmo de scheduling:\n");
    printf("1. FIFO\n");
    printf("2. SJF (Shortest Job First)\n");
    printf("3. HPF (Highest Priority First)\n");
    printf("4. Round Robin (RR)\n");
    printf("Opcion: ");

    scanf("%d", &option);

    switch (option) {
        case 1: return FIFO;
        case 2: return SJF;
        case 3: return HPF;
        case 4: return RR;
        default:
            printf("Opcion invalida, usando FIFO por defecto.\n");
            return FIFO;
    }
}

#endif // MENU_H