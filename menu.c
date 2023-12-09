#include <stdio.h>
#include <stdlib.h>
#include "debugmalloc.h"
#include "menu.h"
#include "logOut.h"
#include "newCustomer.h"
#include "newCar.h"
#include "searchByCustomer.h"
#include "searchByPlate.h"
#include "repairs.h"
#include "deleteCar.h"
#include "soonExpires.h"
#include "repairHistory.h"

int printMenu();

bool functionSelector(const int menuNumber, CustomerList *customerList, CarList *carList, RepairList *repairList);
/*A harom lancolt listat kapja parameterul, kiiratja a menut és meghivja a funkciovalasztot. Visszateresi erteke az utobbi erteketol fugg,
igaz ha a program futhat tovabb, hamis ha ki kell lepni.*/
bool menu(CustomerList *customerList, CarList *carList, RepairList *repairList) {
    int menuNumber = printMenu();
    printf("\n\n");
    bool run = functionSelector(menuNumber, customerList, carList, repairList);
    return run;
}
/*Kiiratja a menut, bekeri a valasztott menupont sorszamat. Visszateresi erteke az utobbi egesz szam.*/
int printMenu() {
    printf("\n\tUdvozoljuk!\n\n");
    printf("1.\tUj ugyfel felvetele\n");
    printf("2.\tUj auto felvetele\n");
    printf("3.\tUgyfel szerinti kereses\n");
    printf("4.\tRendszam szerinti kereses\n");
    printf("5.\tJavitas felvetele vagy modositasa\n");
    printf("6.\tAuto torlese\n");
    printf("7.\tHamarosan lejaro vizsgaju autok listaja\n");
    printf("8.\tSzerviztortenet megjelenitese\n");
    printf("0.\tKilepes\n");
    printf("\n\tValasztott menupont sorszama: ");
    return scanDecimal(0, 8);
}
/*A harom lancolt listat es a valasztott funkcio szamat kapja parameterul, visszateresi erteke az adott funkciotol fuggoen igaz vagy hamis.
A kapott szam alapjan hiv meg egy funkciot.*/
bool functionSelector(const int menuNumber, CustomerList *customerList, CarList *carList, RepairList *repairList) {
    bool run = true;
    switch (menuNumber) {
        case 0:
            run = logOut(); break;
        case 1:
            run = newCustomer(customerList); break;
        case 2:
            run = newCar(customerList, carList); break;
        case 3:
            run = searchByCustomer(customerList, carList, repairList); break;
        case 4:
            run = searchByPlate(carList, repairList); break;
        case 5:
            run = repairs(carList, repairList); break;
        case 6:
            run = deleteCar(carList); break;
        case 7:
            run = soonExpires(carList);  break;
        case 8:
            run = repairHistory(repairList); break;
        default: break;
    }
    return run;
}
/*Beolvas egy egesz szamot a parameterul kapott minimum es maximum kozott. Ezt a szamot visszaadja, addig fut, amig a beolvasas nem sikeres.*/
int scanDecimal(const int min, const int max) {
    while (true) {
        int read;
        int successful = scanf("%d", &read);
        if (successful != 1) {
            printf("Amit megadott, nem szam volt!\n");
            clearInputBuffer();
            printf("Valasztott menupont sorszama: ");
        } else if (read > max || read < min) {
            printf("Nem letezo menupont, probalja ujra. (min: %d, max: %d)\n", min, max);
            printf("Valasztott menupont sorszama: ");
        } else {
            return read;
        }
    }
}

int errorHandling() {
    printf("\n1.\tProbalja ujra\n");
    printf("0.\tKilepes\n");
    printf("Valasztott menupont sorszama: ");
    int menuNumber = scanDecimal(0, 1);
    return menuNumber;
}

void memoryAllocError() {
    printf("\nNem sikerult memoriat foglalni.\n");
}

void fileOpenError() {
    printf("\nNem sikerult megnyitni a fajlt.\n");
}

void exitOnly() {
    printf("\n0.\tKilepes\n");
    printf("Valasztott menupont sorszama: ");
    int menuNumber = scanDecimal(0, 0);
    if (!menuNumber)
        printf("\n\n");
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void printASCIIART() {
    printf("\t         __________         \t          __________         \n");
    printf("\t      __|----------|__      \t       __|----------|__      \n");
    printf("\t     | |            | |     \t      | |            | |     \n");
    printf("\t     | |            | |     \t      | |            | |     \n");
    printf("\t    _|_|____________|_|_    \t     _|_|____________|_|_    \n");
    printf("\t <->|________  ________|<-> \t  <->|_________ ________|<-> \n");
    printf("\t[ _ ]        00        [ _ ]\t [ _ ]         0        [ _ ]\n");
    printf("\t|(_)|        !!        |(_)|\t |(_)|         !        |(_)|\n");
    printf("\t|___|________!!________|___|\t |___|_________!________|___|\n");
    printf("\t[_________|GARAGE|_________]\t [_________|SZERVIZ|________]\n");
    printf("\t ||||     ~~~~~~~~     |||| \t  ||||     ~~~~~~~~~    |||| \n");
    printf("\t '--'                  '--' \t  '--'                  '--' \n");
    printf("\n\n\t\t     ----------- AUTOSZERVIZ -----------\n");
}
