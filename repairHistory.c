#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "debugmalloc.h"
#include "menu.h"
#include "newCustomer.h"
#include "newCar.h"
#include "repairs.h"
#include "repairHistory.h"
#include "soonExpires.h"

RepairList *sortRepairList(RepairList *repairList);

RepairList *insertRepairAtEndWithClone(RepairList *repairList, Repair *newRepair);

RepairList *swap(Repair *a, Repair *b, RepairList *repairList);

void bubbleSort(RepairList *repairList);

void printList(RepairList *repairList);
/*a main hivja meg, parameterul kapja az autok es javitasok listat, visszaadja, hogy a program futhat-e tovabb, vagy ki kell leptetni.
Kiirja a kesz javitasokat idorendi (csokkeno) sorrendben.*/
bool repairHistory(RepairList *repairList) {
    RepairList *sortedRepairList = sortRepairList(repairList);
    if (sortedRepairList == NULL) {
        memoryAllocError();
        return false;
    }
    if (sortedRepairList -> head == NULL)
        printf("Nincs szerviztortenet.\n");
    else {
        printf("Szerviztortenet:\n\n");
        printList(sortedRepairList);
    }
    freeRepairList(sortedRepairList);
    exitOnly();
    return true;
}

RepairList *sortRepairList(RepairList *repairList) {
    RepairList *sortedRepairList = (RepairList*) malloc(sizeof(RepairList));
    if (sortedRepairList == NULL) {
        return NULL;
    }
    sortedRepairList -> head = NULL;
    sortedRepairList -> tail = NULL;
    Repair *tempRepair;
    for (tempRepair = repairList -> head; tempRepair != NULL; tempRepair = tempRepair -> next) {
        if (tempRepair -> done == 1) {
            sortedRepairList = insertRepairAtEndWithClone(sortedRepairList, tempRepair);
            if (sortedRepairList == NULL) {
                return NULL;
            }
        }
    }
    bubbleSort(sortedRepairList);
    return sortedRepairList;
}

RepairList *insertRepairAtEndWithClone(RepairList *repairList, Repair *newRepair) {
    Repair *cloneRepair = (Repair*) malloc(sizeof(Repair));
    if(cloneRepair == NULL) {
        freeRepairList(repairList);
        return NULL;
    }
    cloneRepair -> car = newRepair -> car;
    cloneRepair -> ID = newRepair -> ID;
    cloneRepair -> name = newRepair -> name;
    cloneRepair -> done = newRepair -> done;
    cloneRepair -> price = newRepair -> price;
    cloneRepair -> readyDate.year = newRepair -> readyDate.year;
    cloneRepair -> readyDate.month = newRepair -> readyDate.month;
    cloneRepair -> readyDate.day = newRepair -> readyDate.day;
    if (repairList -> head == NULL) {
        repairList -> head = cloneRepair;
        repairList -> tail = cloneRepair;
    }
    else {
        cloneRepair -> prev = repairList -> tail;
        repairList -> tail -> next = cloneRepair;
        repairList -> tail = cloneRepair;
        cloneRepair -> next = NULL;
    }
    repairList -> head -> prev = NULL;
    repairList -> tail -> next = NULL;
    return repairList;
}

void bubbleSort(RepairList *repairList) {
    int swapped;
    Repair* ptr1;
    if (repairList -> head == NULL)
        return;
    do {
        ptr1 = repairList -> head;
        swapped = 0;
        while (ptr1 != repairList -> tail) {
            if (daysDifference(ptr1 -> readyDate, ptr1 -> next -> readyDate) < 0) {
                repairList = swap(ptr1, ptr1 -> next, repairList);
                swapped = 1;
            }
            if (ptr1 -> next != NULL)
                ptr1 = ptr1 -> next;
            else
                break;
        }
    } while (swapped);
}

RepairList *swap(Repair *a, Repair *b, RepairList *repairList) {
    if(a -> prev != NULL)
        a -> prev -> next = b;
    else
        repairList -> head = b;
    if(b -> next != NULL)
        b -> next -> prev = a;
    else
        repairList -> tail = a;
    a -> next = b -> next;
    b -> next = a;
    Repair *temp = a -> prev;
    a -> prev = b;
    b -> prev = temp;
    return repairList;
}

void printList(RepairList *repairList) {
    Repair *tempRepair;
    for (tempRepair = repairList -> head; tempRepair != NULL; tempRepair = tempRepair -> next)
        printf("%s %u %u %d/%d/%d\n", tempRepair -> car -> numberPlate, tempRepair -> name, tempRepair -> price, tempRepair -> readyDate.year,
                   tempRepair -> readyDate.month, tempRepair -> readyDate.day);
}
