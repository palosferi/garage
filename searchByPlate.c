#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "debugmalloc.h"
#include "menu.h"
#include "newCustomer.h"
#include "newCar.h"
#include "searchByCustomer.h"
#include "searchByPlate.h"
#include "repairs.h"

void printDetails(Car *searchedCar, RepairList *repairList);
/*a main hivja meg, parameterul kapja az autok es javitasok listat, visszaadja, hogy a program futhat-e tovabb, vagy ki kell leptetni.
Egy adott autot keres meg, ha megtalalta, kiirja a javitasokat.*/
bool searchByPlate(CarList *carList, RepairList *repairList) {
    if(carList -> head == NULL) {
        printf("Nincs auto az adatbazisban.\n");
        exitOnly();
        return true;
    }
    Car *searchedCar = NULL;
    int carSearchSuccess = carSearch(carList, &searchedCar);
    while(carSearchSuccess != 2) {
        if(carSearchSuccess == -1)
            return false;
        else if(carSearchSuccess == 0)
            return true;
        carSearchSuccess = carSearch(carList, &searchedCar);
    }
    if(searchedCar -> active == 0)
        printf("Az auto torolve lett.\n");
    else
        printDetails(searchedCar, repairList);
    exitOnly();
    return true;
}

int carSearch(CarList *carList, Car **car) {
    int readInSuccess = 1;
    clearInputBuffer();
    char* numberPlate = readNumberPlate(&readInSuccess);
    if (!readInSuccess) {
        if (errorHandling() == 1)
            return 1;
        return 0;
    }
    if (numberPlate == NULL) {
        memoryAllocError();
        return -1;
    }
    int existCheck = 0;
    Car *searchedCar;
    for (searchedCar = carList -> head; searchedCar != NULL; searchedCar = searchedCar -> next) {
        if (strcmp(searchedCar -> numberPlate, numberPlate) == 0) {
                ++existCheck;
                break;
        }
    }
    free(numberPlate);
    if (existCheck == 0) {
        printf("Nem letezik ilyen rendszam.\n\n");
        if (errorHandling() == 1)
            return 1;
        return 0;
    }
    *car = searchedCar;
    return 2;
}

void printDetails(Car *searchedCar, RepairList *repairList) {
    printf("%s %s %s %d/%d/%d\n", searchedCar -> owner -> surName, searchedCar -> owner -> firstName, searchedCar -> brand,
           searchedCar -> testDate.year, searchedCar -> testDate.month, searchedCar -> testDate.day);
    if(numberOfRepairsPerCar(searchedCar) > 0)
        printf("Autohoz tartozo javitas(ok):\n");
    else
        printf("Nincs az autohoz tartozo javitas.\n");
    for (int i = 0; i < numberOfRepairsPerCar(searchedCar); ++i) {
        printf("%u %u %u %d/%d/%d\n", searchedCar -> repairArray[i] -> done, searchedCar -> repairArray[i] -> name,
               searchedCar -> repairArray[i] -> price, searchedCar -> repairArray[i] -> readyDate.year,
               searchedCar -> repairArray[i] -> readyDate.month,searchedCar -> repairArray[i] -> readyDate.day);
    }
    return;
}
