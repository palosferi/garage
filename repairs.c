#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "debugmalloc.h"
#include "menu.h"
#include "newCustomer.h"
#include "newCar.h"
#include "searchByPlate.h"
#include "repairs.h"

int getCarDetails(CarList *carList, RepairList *repairList, Car **car);

int repairDetails(CarList *carList, RepairList *repairList, Car *car);

int newRepair(CarList *carList, RepairList *repairList, Car *car, const char **repairTypeNames);

int markAsDone(RepairList *repairList, Car *car, const char **repairTypeNames);

bool writeToRepairFile(RepairList *repairList);

char **stringArrayFromRepairList(RepairList *repairList);

RepairList *readRepairsFromFile(FILE *fp, RepairList *repairList, const CarList *carList);

unsigned int scanPrice(unsigned int min, unsigned int max);

unsigned int calculateRepairPrice(repairType type);

unsigned int scanRepairNum(unsigned int min, unsigned int max);
/*a main hivja meg, parameterul kapja az autok es javitasok listat, visszaadja, hogy a program futhat-e tovabb, vagy ki kell leptetni.
Uj javitast hoz letre, vagy meglevot fejez be egy adott auto eseten.*/
bool repairs (CarList *carList, RepairList *repairList) {
    if(carList -> head == NULL) {
        printf("Nincs auto az adatbazisban.\n");
        exitOnly();
        return true;
    }
    Car *car = NULL;
    int carSearchSuccess = carSearch(carList, &car);
    while(carSearchSuccess != 2) {
        if(carSearchSuccess == -1)
            return false;
        else if(carSearchSuccess == 0)
            return true;
        carSearchSuccess = carSearch(carList, &car);
    }
    int repairDetailsSuccess = repairDetails(carList, repairList, car);
    while(repairDetailsSuccess != 2) {
        if(repairDetailsSuccess == -1)
            return false;
        else if(repairDetailsSuccess == 0)
            return true;
        carSearchSuccess = carSearch(carList, &car);
        while(carSearchSuccess != 2) {
            if(carSearchSuccess == -1)
                return false;
            else if(carSearchSuccess == 0)
                return true;
            carSearchSuccess = carSearch(carList, &car);
        }
        repairDetailsSuccess = repairDetails(carList, repairList, car);
    }
    return true;
}

int repairDetails(CarList *carList, RepairList *repairList, Car *car) {
    const char *repairTypeNames[] = {"olajcsere", "kerekcsere", "fekcsere", "akkucsere", "gyujtogyertyacsere", "legzsakcsere", "fenyszorocsere",\
                                     "szenzorcsere", "ablaktorlocsere", "fenyezes"};
    printf("1.\tUj javitas\n");
    printf("2.\tMeglevo befejezese\n");
    printf("Valasztott menupont sorszama: ");
    int menuNumber = scanDecimal(1, 2);
    if (menuNumber == 1) {
        int newRepairSuccess = newRepair(carList, repairList, car, repairTypeNames);
        if(newRepairSuccess == -1)
            return -1;
    }
    else {
        int markAsDoneSuccess = markAsDone(repairList, car, repairTypeNames);
        if(markAsDoneSuccess == -1)
            return -1;
        else if(markAsDoneSuccess == 0)
            return 0;
    }
    return 2;
}

int newRepair(CarList *carList, RepairList *repairList, Car *car, const char **repairTypeNames) {
    clearInputBuffer();
    for(int i = 0; i < 10; ++i) {
        printf("%d %s\n", i + 1, (char*)repairTypeNames[i]);
    }
    printf("Javitas sorszama: ");
    int repairTypeD = scanDecimal(1, 10);
    Repair *repair = (Repair*) malloc(sizeof(Repair));
    if (repair == NULL) {
        memoryAllocError();
        return -1;
    }
    int nextID;
    if(repairList -> tail != NULL)
        nextID = (repairList -> tail -> ID + 1);
    else
        nextID = 1;
    repair -> ID = nextID;
    Car *tempCar;
    for(tempCar = carList -> head; tempCar != NULL; tempCar = tempCar -> next) {
        if(strcmp(tempCar -> numberPlate, car -> numberPlate) == 0) {
            repair -> car = tempCar;
            break;
        }
    }
    repair -> name = (repairType)repairTypeD;
    repair -> done = 0;
    repair -> price = calculateRepairPrice(repair -> name);
    repair -> readyDate.year = 0;
    repair -> readyDate.month = 0;
    repair -> readyDate.day = 0;
    repairList = insertRepairAtEnd(repairList, repair);
    if(!writeToRepairFile(repairList)) {
        free(repair);
        printf("Nem sikerult megnyitni a fajlt vagy nem sikerult memoriat foglalni.\n");
        return -1;
    }
    repair -> car -> repairArray = (Repair**) realloc(repair -> car -> repairArray, sizeof(Repair*) * (numberOfRepairsPerCar(repair -> car) + 2));
    if(repair -> car -> repairArray == NULL) {
        free(repair);
        memoryAllocError();
        return -1;
    }
    int numberOfRepairsPerCarD = numberOfRepairsPerCar(repair -> car);
    repair -> car -> repairArray[numberOfRepairsPerCarD] = repair;
    repair -> car -> repairArray[numberOfRepairsPerCarD + 1] = NULL;
    if(!writeToCarFile(carList)) {
        printf("Nem sikerult megnyitni a fajlt vagy nem sikerult memoriat foglalni.\n");
        free(repair);
        return -1;
    }
    printf("Sikeresen felvett uj javitas.\n");
    exitOnly();
    return 2;
}

int markAsDone(RepairList *repairList, Car *car, const char **repairTypeNames) {
    if (repairList -> head == NULL) {
        printf("\nNincs jegyzett javitas.\n");
        exitOnly();
        return 0;
    }
    if(car ->repairArray == NULL || car -> repairArray[0] == NULL) {
        printf("Nincs az autohoz tartozo javitas.\n");
        exitOnly();
        return 0;
    }
    int existCheck = 0;
    for(int i = 0; i < numberOfRepairsPerCar(car); ++i) {
        if(car -> repairArray[i] -> done == 0) {
            printf("%u %s\n", car -> repairArray[i] -> ID, (char*)repairTypeNames[car -> repairArray[i] -> name - 1]);
            ++existCheck;
        }
    }
    if (!existCheck) {
        printf("\nNincs az autohoz tartozo fuggoben levo javitas.\n");
        exitOnly();
        return 0;
    }
    unsigned repairID = 0;
    bool successfulIDInput;
    do {
        successfulIDInput = true;
        printf("Az elkeszult javitas sorszama: ");
        repairID = scanRepairNum(1, numberOfRepairs(repairList));
        bool IDCheck = false;
        for(int i = 0; i < numberOfRepairsPerCar(car); ++i) {
            if(repairID == car -> repairArray[i] -> ID && car -> repairArray[i] -> done == 0)
                IDCheck = true;
        }
        if(!IDCheck) {
            printf("\n\nNem megfelelo sorszam.\n");
            if(errorHandling() == 1)
                successfulIDInput = false;
            else
                return 0;
        }
    } while(!successfulIDInput);
    Repair *tempRepair;
    for (tempRepair = repairList -> head; tempRepair != NULL; tempRepair = tempRepair -> next) {
        if (tempRepair -> ID == repairID) {
            tempRepair -> done = 1;
            tempRepair -> readyDate.year = getCurrentDate().year;
            tempRepair -> readyDate.month = getCurrentDate().month;
            tempRepair -> readyDate.day = getCurrentDate().day;
            break;
        }
    }
    if(!writeToRepairFile(repairList)) {
        printf("Nem sikerult megnyitni a fajlt vagy nem sikerult memoriat foglalni.\n");
        return -1;
    }
    printf("Sikeresen modositott javitas.\n");
    exitOnly();
    return 2;
}

RepairList *insertRepairAtEnd(RepairList *repairList, Repair *newRepair) {
    if (repairList -> head == NULL) {
        repairList -> head = newRepair;
        repairList -> tail = newRepair;
    }
    else {
        newRepair -> prev = repairList -> tail;
        repairList -> tail -> next = newRepair;
        repairList -> tail = newRepair;
        newRepair -> next = NULL;
    }
    repairList -> head -> prev = NULL;
    repairList -> tail -> next = NULL;
    return repairList;
}

unsigned int numberOfRepairs(const RepairList *repairList) {
    int numberOfRepairs = 0;
    Repair *temp;
    if (repairList -> head == NULL)
        return 0;
    for(temp = repairList -> head; temp != NULL; temp = temp -> next) {
        ++numberOfRepairs;
    }
    return numberOfRepairs;
}

int numberOfRepairsPerCar(Car *car) {
    if(car->repairArray == NULL) return 0;
    int i = 0;
    while(car -> repairArray[i] != NULL) {
        ++i;
    }
    return i;
}

unsigned int scanRepairNum(unsigned int min, unsigned int max) {
    while (true) {
        unsigned int read;
        int successful = scanf("%u", &read);
        if (successful != 1) {
            printf("Amit megadott, nem pozitiv szam volt!\n");
            printf("\nAz elkeszult javitas sorszama: ");
            clearInputBuffer();
        }
        else if (read > max || read < min) {
            printf("Nem megfelelo ID, probalja ujra. (min: %u, max: %u)\n", min, max);
            printf("\nAz elkeszult javitas sorszama: ");
        }
        else
            return read;
    }
}

bool writeToRepairFile(RepairList *repairList) {
    FILE* fp;
    fp = fopen("javitasok.txt", "w");
    if (fp == NULL)
        return false;
    unsigned int numberOfRepairsUI = numberOfRepairs(repairList);
    char **stringArray = stringArrayFromRepairList(repairList);
    if (stringArray == NULL) {
        return false;
    }
    for(int i = 0; i < numberOfRepairsUI; ++i) {
        fprintf(fp, "%s\n", stringArray[i]);
    }
    for(int i = 0; i < numberOfRepairsUI; ++i) {
        free(stringArray[i]);
    }
    free(stringArray);
    fclose(fp);
    return true;
}

char **stringArrayFromRepairList(RepairList *repairList) {
    unsigned int numberOfRepairsUI = numberOfRepairs(repairList);
    char **stringArray = (char**) malloc(sizeof(char*)*numberOfRepairsUI);
    if(stringArray == NULL) {
        return NULL;
    }
    Repair *temp = repairList -> head;
    for (int i = 0; i < numberOfRepairsUI && temp != NULL; ++i, temp = temp -> next) {
        unsigned int priceDigits = 1;
        unsigned int tempPrice = temp -> price;
        while(tempPrice /= 10)
            ++priceDigits;
        unsigned int IDDigits = 1;
        unsigned int tempID = temp -> ID;
        while(tempID /= 10)
            ++IDDigits;
        unsigned int nameDigits = 1;
        unsigned int tempName = (unsigned int)temp -> name;
        while(tempName /= 10)
            ++nameDigits;
        stringArray[i] = (char*) malloc(sizeof(char)*(IDDigits + 1 + strlen(temp -> car -> numberPlate) + 1 + nameDigits\
                                                      + 1 + priceDigits + 1 + 1 + 1 + 10 + 1));
        if(stringArray[i] == NULL) {
            for(int j = 0; j < i; ++j) {
                free(stringArray[j]);
            }
            free(stringArray);
            return NULL;
        }
        sprintf(stringArray[i], "%u;%s;%u;%u;%u;%u/%u/%u", temp -> ID, temp -> car -> numberPlate, (unsigned int)temp -> name, temp -> price, temp -> done, temp -> readyDate.year, temp -> readyDate.month, temp -> readyDate.day);
        }
    return stringArray;
}

Date getCurrentDate() {
    time_t t;
    struct tm *current_time;
    time(&t);
    current_time = localtime(&t);
    Date today = {.year = current_time -> tm_year + 1900, .month = current_time -> tm_mon + 1, .day = current_time -> tm_mday};
    return today;
}

unsigned int calculateRepairPrice(repairType type) {
    switch (type) {
        case (olajcsere): return 20000;
        case (kerekcsere): return 4000;
        case (fekcsere): return 40000;
        case (akkucsere): return 70000;
        case (gyujtogyertyacsere): return 6000;
        case (legzsakcsere): return 180000;
        case (fenyszorocsere): return 5000;
        case (szenzorcsere): return 10000;
        case (ablaktorlocsere): return 20000;
        case (fenyezes): return 15000;
        default: return 0;
    }
}

RepairList *readRepairListFromFile(const CarList *carList) {
    FILE *fp;
    fp = fopen("javitasok.txt", "r");
    RepairList *repairList = (RepairList*) malloc(sizeof(RepairList)); //memoriafoglalas a javitaslista strazsait tartalmazo strukturanak
    if (repairList == NULL)
        return NULL;
    repairList -> head = NULL;
    repairList -> tail = NULL;
    if (fp == NULL)
        return repairList;
    repairList = readRepairsFromFile(fp, repairList, carList); //a lista feltoltese fajlbol
    if(repairList == NULL) {
        return NULL;
    }
    fclose(fp);
    return repairList;
}

RepairList *readRepairsFromFile(FILE *fp, RepairList *repairList, const CarList *carList) {
    while(1) {
        Repair *repair = (Repair*) malloc(sizeof(Repair)); //az egyes javitasok memoriafoglalasa
        if (repair == NULL) {
            freeRepairList(repairList);
            return NULL;
        }
        unsigned int tempID = 0;
        int result = fscanf(fp, "%u;", &tempID); //Az ID beolvasasaval ellenorizzuk, hogy a fajl vegen vagyunk e
        if(result == EOF) {
            free(repair);
            return repairList;
        }
        char *numberPlate = readInStringFromFile(fp); //rendszam beolvasasa
        if (numberPlate == NULL) {
            free(repair);
            freeRepairList(repairList);
            return NULL;
        }
        Car *tempCar;
        for(tempCar = carList -> head; tempCar != NULL; tempCar = tempCar -> next) {
            if(strcmp(tempCar -> numberPlate, numberPlate) == 0) {
                repair -> car = tempCar;
                int numberOfRepairsPerCarD = numberOfRepairsPerCar(tempCar);
                tempCar -> repairArray = (Repair**) realloc(tempCar -> repairArray, sizeof(Repair*)*(numberOfRepairsPerCarD + 2));
                tempCar -> repairArray[numberOfRepairsPerCarD] = repair;
                tempCar -> repairArray[numberOfRepairsPerCarD + 1] = NULL;
                break;
            }
        }
        free(numberPlate);
        repair -> ID = tempID;
        fscanf(fp, "%u;%u;%u;%u/%u/%u", &(repair -> name), &(repair -> price), &(repair -> done),\
               &(repair -> readyDate.year),&(repair -> readyDate.month),&(repair -> readyDate.day));
        repairList = insertRepairAtEnd(repairList, repair);
    }
    return repairList;
}

void freeRepairList(RepairList *repairList) {
    Repair *iter = repairList -> head;
    while(iter != NULL) {
        Repair *temp = iter -> next;
        free(iter);
        iter = temp;
    }
    repairList -> head = NULL;
    repairList -> tail = NULL;
    free(repairList);
}
