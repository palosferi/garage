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
#include "repairs.h"

unsigned int scanID(const CustomerList *customerList);
/*a main hivja meg, parameterul kapja az ugyfelek, autok es javitasok listat, visszaadja, hogy a program futhat-e tovabb, vagy ki kell leptetni.
Egy adott ugyfelet keres meg, ha megtalalta, kiirja a hozza tartozo autokat, azok javitasait.*/
bool searchByCustomer(CustomerList *customerList, CarList *carList, RepairList *repairList) {
    if(customerList -> head == NULL) {
        printf("Nincs ugyfel az adatbazisban.\n");
        exitOnly();
        return true;
    }
    int customerSearchSuccess = customerSearch(customerList);
    while (customerSearchSuccess < 1) {
        if(customerSearchSuccess == -1)
            return false;
        else if(customerSearchSuccess == 0)
            return true;
        printf("\n\n");
        customerSearchSuccess = customerSearch(customerList);
    }
    int tempID = customerSearchSuccess;
    Customer *searchedCustomer;
    for(searchedCustomer = customerList -> head; searchedCustomer != NULL; searchedCustomer = searchedCustomer -> next) {
        if (tempID == searchedCustomer -> ID) {
            break;
        }
    }
    printf("%s %s %lu\n", searchedCustomer -> surName, searchedCustomer -> firstName, searchedCustomer -> phoneNumber);
    if(numberOfCarsPerCustomer(searchedCustomer) > 0 ) {
        for(int i = 0; i < numberOfCarsPerCustomer(searchedCustomer); ++i) {
            if(searchedCustomer -> carArray[i] -> active == 1) {
                printf("Ugyfelhez tartozo auto(k):\n");
                break;
            }
        }
    }
    for(int i = 0; i <numberOfCarsPerCustomer(searchedCustomer); ++i) {
        if (searchedCustomer -> carArray[i] -> active == 1)
            printf("%s\n", searchedCustomer -> carArray[i] -> numberPlate);
        else
            printf("Torolt auto: %s\n", searchedCustomer -> carArray[i] -> numberPlate);
        if(numberOfRepairsPerCar(searchedCustomer -> carArray[i]) > 0)
            printf("Autohoz tartozo javitas(ok):\n");
        else
            printf("Nincs az autohoz tartozo javitas.\n");
        for (int j = 0; j < numberOfRepairsPerCar(searchedCustomer -> carArray[i]); ++j) {
            printf("%u %u %u %d/%d/%d\n", searchedCustomer -> carArray[i] -> repairArray[j] -> name, searchedCustomer -> carArray[i] -> repairArray[j] -> price,
                   searchedCustomer -> carArray[i] -> repairArray[j] -> done, searchedCustomer -> carArray[i] -> repairArray[j] -> readyDate.year,
                   searchedCustomer -> carArray[i] -> repairArray[j] -> readyDate.month, searchedCustomer -> carArray[i] -> repairArray[j] -> readyDate.day);
        }
    }
    exitOnly();
    return true;
}

int customerSearch(CustomerList *customerList) {
    int success = 1;
    clearInputBuffer();
    printf("Ugyfel neve: ");
    char* ownerNameSubString = readInString(&success);
    if (!success) {
        if (errorHandling() == 1)
            return -2;
        return 0;
    }
    if (ownerNameSubString == NULL) {
        memoryAllocError();
        return -1;
    }
    int existCheck = 0;
    int *possibleIDs = (int*) malloc(sizeof(int) * (existCheck+1));
    if(possibleIDs == NULL) {
        free(ownerNameSubString);
        memoryAllocError();
        return -1;
    }
    Customer *temp;
    for (temp = customerList -> head; temp != NULL; temp = temp -> next) {
        char* haystack = (char*) malloc(sizeof(char)*(strlen(temp -> surName) + 1 + strlen(temp -> firstName) + 1));
        if (haystack != NULL) {
            sprintf(haystack, "%s %s", temp -> surName, temp -> firstName);
            haystack[(strlen(temp -> surName) + 1 + strlen(temp -> firstName))] = '\0';
            if(strstr(haystack, ownerNameSubString) != NULL) {
                printf("%u %s %s\n", temp -> ID, temp -> surName, temp -> firstName);
                ++existCheck;
                if (existCheck != 1) {
                    possibleIDs = realloc(possibleIDs, sizeof(int) * (existCheck));
                    if(possibleIDs == NULL) {
                        free(haystack);
                        free(possibleIDs);
                        free(ownerNameSubString);
                        memoryAllocError();
                        return -1;
                    }
                }
                possibleIDs[existCheck-1] = temp -> ID;
            }
            free(haystack);
        }
        else {
            free(possibleIDs);
            free(ownerNameSubString);
            memoryAllocError();
            return -1;
        }
    }
    free(ownerNameSubString);
    if (existCheck == 0) {
        free(possibleIDs);
        printf("Nem letezik ilyen ugyfel.\n\n");
        if (errorHandling() == 1)
            return -2;
        return 0;
    }
    while(1) {
        printf("A keresett ugyfel sorszama: ");
        unsigned int tempID = scanID(customerList);
        for(int i = 0; i < existCheck; ++i) {
            if(tempID == possibleIDs[i]) {
                free(possibleIDs);
                return tempID;
            }
        }
        printf("Hibas sorszam.\n");
        if (errorHandling() == 0) {
            free(possibleIDs);
            return 0;
        }
    }
}

unsigned int scanID(const CustomerList *customerList) {
    while (true) {
        unsigned int read;
        int successful = scanf("%u", &read);
        if (successful != 1) {
            printf("Amit megadott, nem pozitiv szam volt!\n");
            clearInputBuffer();
            printf("A keresett ugyfel sorszama: ");
        }
        else if (read > customerList -> tail -> ID || read == 0) {
            printf("Nem letezo ugyfel, probalja ujra.\n");
            printf("A keresett ugyfel sorszama: ");
        }
        else {
            return read;
        }
    }
}
