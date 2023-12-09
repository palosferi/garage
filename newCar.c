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

int addNewCar(CustomerList *customerList, CarList *carList);

int carDetails(Car *newCar, CarList *carList);

bool existingCarHandling(const Car *car, CarList *carList);

bool correctNumberPlate(Car *car);

char **stringArrayFromCarList(CarList *carList);

CarList *readCarsFromFile(FILE *fp, CarList *carList, const CustomerList *customerList);

unsigned int calculateRepairPrice (repairType type);

unsigned int scanYear(unsigned int min, unsigned int max);

unsigned int scanMonth(unsigned int min, unsigned int max);

unsigned int scanDay(Date date, unsigned int min, unsigned int max);
/*a main hivja meg, parameterul kapja az ugyfelek es autok listat, visszaadja, hogy a program futhat-e tovabb, vagy ki kell leptetni.
Uj autot hoz letre egy adott ugyfelhez.*/
bool newCar(CustomerList *customerList, CarList *carList) {
    if(customerList -> head == NULL) {
        printf("Nincs ugyfel az adatbazisban.\n");
        exitOnly();
        return true;
    }
    int addNewCarSuccess = addNewCar(customerList, carList);
    while(addNewCarSuccess != 2) {
        if(addNewCarSuccess == -1)
            return false;
        else if(addNewCarSuccess == 0)
            return true;
        addNewCarSuccess = addNewCar(customerList, carList);
    }
    return true;
}

int addNewCar(CustomerList *customerList, CarList *carList) {
    int customerSearchSuccess = customerSearch(customerList);
    while (customerSearchSuccess < 1) {
        if(customerSearchSuccess == -1)
            return -1;
        else if(customerSearchSuccess == 0)
            return 0;
        printf("\n\n");
        customerSearchSuccess = customerSearch(customerList);
    }
    int tempID = customerSearchSuccess;
    Car *newCar = (Car*) malloc(sizeof(Car));
    Customer* temp;
    for(temp = customerList -> head; temp != NULL; temp = temp -> next) {
        if (tempID == temp -> ID) {
            newCar -> owner = temp;
            break;
        }
    }
    int carDetailsSuccess = carDetails(newCar, carList);
    while(carDetailsSuccess != 2) {
        if(carDetailsSuccess == -1) {
            free(newCar);
            return -1;
        }
        else if(carDetailsSuccess == 0) {
            free(newCar);
            return 0;
        }
        carDetailsSuccess = carDetails(newCar, carList);
    }
    if(!writeToCarFile(carList)) {
        printf("Nem sikerult megnyitni a fajlt vagy nem sikerult memoriat foglalni.\n");
        free(newCar -> numberPlate);
        free(newCar -> brand);
        free(newCar);
        return -1;
    }
    newCar -> owner -> carArray = (Car**) realloc(newCar -> owner -> carArray, sizeof(Car*)*(numberOfCarsPerCustomer(newCar -> owner) + 2));
    if(newCar -> owner -> carArray == NULL) {
        free(newCar -> numberPlate);
        free(newCar -> brand);
        free(newCar);
        memoryAllocError();
        return -1;
    }
    int numberOfCarsPerCustomerD = numberOfCarsPerCustomer(newCar -> owner);
    newCar -> owner -> carArray[numberOfCarsPerCustomerD] = newCar;
    newCar -> owner -> carArray[numberOfCarsPerCustomerD + 1] = NULL;
    if(!writeToCustomerFile(customerList)) {
        printf("Nem sikerult megnyitni a fajlt vagy nem sikerult memoriat foglalni.\n");
        free(newCar -> numberPlate);
        free(newCar -> brand);
        free(newCar);
        return -1;
    }
    printf("\nSikeresen felvett uj auto.\n");
    exitOnly();
    return 2;
}

int carDetails(Car *newCar, CarList *carList) {
    int readInSuccess = 1;
    clearInputBuffer();
    char *numberPlate = readNumberPlate(&readInSuccess);
    if (!readInSuccess) {
        if (errorHandling() == 1)
            return 1;
        return 0;
    }
    if (numberPlate == NULL) {
        memoryAllocError();
        return -1;
    }
    newCar -> numberPlate = numberPlate;
    if (!correctNumberPlate(newCar)) {
        free(newCar -> numberPlate);
        if (errorHandling() == 1)
            return 1;
        return 0;
    }
    if (!existingCarHandling(newCar, carList)) {
        free(newCar -> numberPlate);
        if (errorHandling() == 1)
            return 1;
        return 0;
    }
    printf("Auto markaja: ");
    char *brand = readInString(&readInSuccess);
    if (!readInSuccess) {
        free(newCar -> numberPlate);
        if (errorHandling() == 1) {
            return 1;
        }
        return 0;
    }
    if (brand == NULL) {
        free(newCar -> numberPlate);
        memoryAllocError();
        return -1;
    }
    newCar -> brand = brand;
    printf("Vizsga ervenyessege ev: ");
    newCar -> testDate.year = scanYear(1900, 3000);
    printf("Vizsga ervenyessege honap (szam): ");
    newCar -> testDate.month = scanMonth(1, 12);
    printf("Vizsga ervenyessege nap: ");
    newCar -> testDate.day = 0;
    newCar -> testDate.day = scanDay(newCar -> testDate, 1, 31);
    newCar -> active = 1;
    newCar -> repairArray = malloc(sizeof(Repair*));
    newCar -> repairArray[0] = NULL;
    carList = insertCarAtEnd(carList, newCar);
    return 2;
}

char *readNumberPlate(int *readInSuccess) {
    int success = 1;
    printf("Auto rendszama (nagybetuk es szamok egyben irva): ");
    char *numberPlate = readInString(&success);
    if (!success) {
        *readInSuccess = 0;
        return NULL;
    }
    if (numberPlate == NULL)
        return NULL;
    return numberPlate;
}

bool existingCarHandling(const Car *car, CarList *carList) {
    bool existingCar = false;
    Car *temp;
    for (temp = carList -> head; temp != NULL; temp = temp -> next) {
        if (strcmp(car -> numberPlate, temp -> numberPlate) == 0) {
            existingCar = true;
            break;
        }
    }
    if (existingCar) {
        printf("Ilyen auto mar letezik.\n");
        return false;
    }
    return true;
}

bool writeToCarFile(CarList *carList) {
    FILE* fp;
    fp = fopen("autok.txt", "w");
    if (fp == NULL) {
        return false;
    }
    unsigned int numberOfCarsUI = numberOfCars(carList);
    char **stringArray = stringArrayFromCarList(carList);
    if (stringArray == NULL) {
        return false;
    }
    for(int i = 0; i < numberOfCarsUI; ++i) {
        fprintf(fp, "%s\n", stringArray[i]);
    }
    for(int i = 0; i < numberOfCarsUI; ++i) {
        free(stringArray[i]);
    }
    free(stringArray);
    fclose(fp);
    return true;
}

char **stringArrayFromCarList(CarList *carList) {
    unsigned int numberOfCarsUI = numberOfCars(carList);
    char **stringArray = (char**) malloc(sizeof(char*)*numberOfCarsUI);
    if(stringArray == NULL) {
        return NULL;
    }
    Car *temp = carList -> head;
    for (int i = 0; i < numberOfCarsUI && temp != NULL; ++i, temp = temp -> next) {
        int IDDigits = 1;
        unsigned int tempID = temp -> owner -> ID;
        while(tempID /= 10)
            ++IDDigits;
        int repairIDsLength = 0;
        int numberOfRepairsPerCarD = numberOfRepairsPerCar(temp);
        for(int j = 0; j < numberOfRepairsPerCarD; ++j) {
            int repairIDDigits = 1;
            unsigned int temprepairID = temp -> repairArray[j] -> ID;
            while(temprepairID /= 10)
                ++repairIDDigits;
            repairIDsLength += repairIDDigits;
        }
        stringArray[i] = (char*) malloc(sizeof(char)*(strlen(temp -> numberPlate) + 1 + IDDigits + 1 + strlen(temp -> owner -> surName)) + 1 +
                                        strlen(temp -> owner -> firstName) + 1 + 1 + 1 + strlen(temp -> brand) + 1 + 10 + 1 +
                                        repairIDsLength + numberOfRepairsPerCarD + 1);
        if(stringArray[i] == NULL) {
            return NULL;
        }
        sprintf(stringArray[i], "%u;%s;%s;%s;%u;%s;%d/%d/%d", temp -> owner -> ID, temp -> owner -> surName, temp -> owner -> firstName,
                temp -> numberPlate, temp -> active, temp -> brand, temp -> testDate.year, temp -> testDate.month, temp -> testDate.day);
        for(int j = 0; j < numberOfRepairsPerCarD; ++j) {
            char buf[20];
            sprintf(buf, ";%u", temp -> repairArray[j] -> ID);
            strcat(stringArray[i], buf);
        }
    }
    return stringArray;
}

CarList *insertCarAtEnd(CarList *carList, Car *car) {
    if (carList -> head == NULL) {
        carList -> head = car;
        carList -> tail = car;
        carList -> head -> prev = NULL;
        carList -> tail -> next = NULL;
    }
    else {
        car -> prev = carList -> tail;
        carList -> tail -> next = car;
        carList -> tail = car;
        carList -> head -> prev = NULL;
        carList -> tail -> next = NULL;
    }
    return carList;
}

unsigned int numberOfCars(const CarList *carList) {
    int numberOfCars = 0;
    Car *temp;
    if (carList -> head == NULL)
        return 0;
    for(temp = carList -> head; temp != NULL; temp = temp -> next) {
        ++numberOfCars;
    }
    return numberOfCars;
}

bool correctNumberPlate(Car *car) {
    int correctcarNumberPlate = 0;
    for (int i = 0; i < strlen(car -> numberPlate); ++i) {
        if ((car -> numberPlate[i] >= 'A' && car -> numberPlate[i] <= 'Z') || (car -> numberPlate[i] >= '0' && car -> numberPlate[i] <= '9'))
            ++correctcarNumberPlate;
    }
    if(correctcarNumberPlate != strlen(car -> numberPlate)) {
        printf("\n\nHelytelen adatbevitel. Hibas rendszam.\n");
        return false;
    }
    return true;
}

int numberOfCarsPerCustomer(Customer *customer) {
    if(customer -> carArray == NULL)
        return 0;
    int i = 0;
    while(customer -> carArray[i] != NULL) {
        i++;
    }
    return i;
}

unsigned int scanYear(const unsigned int min, const unsigned int max) {
    while (true) {
        unsigned int read;
        int successful = scanf("%u", &read);
        if (successful != 1) {
            printf("Amit megadott, nem pozitiv szam volt!\n");
            clearInputBuffer();
            printf("\nVizsga ervenyessege ev: ");
        }
        else if (read > max || read < min) {
            printf("Nem megfelelo ev, probalja ujra. (min: %d, max: %d)\n", min, max);
            printf("\nVizsga ervenyessege ev: ");
        }
        else
            return read;
    }
}

unsigned int scanMonth(const unsigned int min, const unsigned int max) {
    while (true) {
        unsigned int read;
        int successful = scanf("%u", &read);
        if (successful != 1) {
            printf("Amit megadott, nem pozitiv szam volt!\n");
            clearInputBuffer();
            printf("\nVizsga ervenyessege honap (szam): ");
        }
        else if (read > max || read < min) {
            printf("Nem megfelelo honap, probalja ujra. (min: %d, max: %d)\n", min, max);
            printf("\nVizsga ervenyessege honap (szam): ");
        }
        else
            return read;
    }
}

unsigned int scanDay(Date date, unsigned int min, unsigned int max) {
    switch (date.month) {
        case 2: max = 28; break;
        case 4: max = 30; break;
        case 6: max = 30; break;
        case 9: max = 30; break;
        case 11: max = 30; break;
        default: break;
    }
    if(date.year%4 == 1 && date.year%100 == 0 && date.year%400 == 1 && date.month == 2)
        max = 29;
    while (true) {
        unsigned int read;
        int successful = scanf("%u", &read);
        if (successful != 1) {
            printf("Amit megadott, nem pozitiv szam volt!\n");
            clearInputBuffer();
            printf("\nVizsga ervenyessege nap: ");
        }
        else if (read > max || read < min) {
            printf("Nem megfelelo nap, probalja ujra. (min: %d, max: %d)\n", min, max);
            printf("\nVizsga ervenyessege nap: ");
        }
        else
            return read;
    }
}

CarList *readCarListFromFile(const CustomerList *customerList) {
    FILE *fp;
    fp = fopen("autok.txt", "r");
    CarList *carList = (CarList*) malloc(sizeof(CarList)); //memoriafoglalas az autolista strazsait tartalmazo strukturanak
    if (carList == NULL)
        return NULL;
    carList -> head = NULL;
    carList -> tail = NULL;
    if (fp == NULL)
        return carList;
    carList = readCarsFromFile(fp, carList, customerList); //a lista feltoltese fajlbol
    if(carList == NULL) {
        return NULL;
    }
    fclose(fp);
    return carList;
}

CarList *readCarsFromFile(FILE *fp, CarList *carList, const CustomerList *customerList) {
    while(1) {
        Car *car = (Car*) malloc(sizeof(Car)); //az egyes autok memoriafoglalasa
        if (car == NULL) {
            freeCarList(carList);
            return NULL;
        }
        unsigned int tempID;
        if(fscanf(fp, "%u;", &tempID) <= 0 || tempID <= 0) { //Az ID beolvasasaval ellenorizzuk, hogy a fajl vegen vagyunk e
            free(car);
            return carList;
        }
        char tempChar;
        while (fscanf(fp, "%c", &tempChar) == 1 && tempChar != ';') //A vezeteknevre nincs szukseg
            ;
        while (fscanf(fp, "%c", &tempChar) == 1 && tempChar != ';') //A keresztnevre nincs szukseg
            ;
        char *numberPlate = readInStringFromFile(fp); //rendszam beolvasasa
        if (numberPlate == NULL) {
            free(car);
            freeCarList(carList);
            return NULL;
        }
        car -> numberPlate = numberPlate;
        fscanf(fp, "%u;", &(car -> active));
        char* brand = readInStringFromFile(fp); //markanev beolvasasa
        if (brand == NULL) {
            free(car -> numberPlate);
            free(car);
            freeCarList(carList);
            return NULL;
        }
        car -> brand = brand;
        fscanf(fp, "%d/%d/%d", &(car -> testDate.year), &(car -> testDate.month), &(car -> testDate.day));
        Customer *tempCustomer;
        for(tempCustomer = customerList -> head; tempCustomer != NULL; tempCustomer = tempCustomer -> next) {
            if (tempID == tempCustomer -> ID) {
                car -> owner = tempCustomer;
                int numberOfCarsPerCustomerD = numberOfCarsPerCustomer(tempCustomer);
                tempCustomer -> carArray = (Car**) realloc(tempCustomer -> carArray, sizeof(Car*) * (numberOfCarsPerCustomerD + 2));
                tempCustomer -> carArray[numberOfCarsPerCustomerD] = car;
                tempCustomer -> carArray[numberOfCarsPerCustomerD + 1] = NULL;
                break;
            }
        }
        char input;
        while(fscanf(fp, "%c", &input) == 1 && input != '\n')
            ;
        car -> repairArray = (Repair**) malloc(sizeof(Repair*));
        car -> repairArray[0] = NULL;
        carList = insertCarAtEnd(carList, car);
    }
    return carList;
}

char *readInPlateFromFile(FILE *fp) {
    char inputChar;
    int newLength = 0;
    char *newString = NULL;
    while(fscanf(fp, "%c", &inputChar) == 1 && inputChar != ';' && inputChar != '\n') {
        ++newLength;
        newString = realloc(newString, newLength + 1);
        if (newString == NULL) {
            return NULL;
        }
        newString[newLength - 1] = inputChar;
        newString[newLength] = '\0';
        }
    return newString;
}

void freeCarList(CarList *carList) {
    Car *iter = carList -> head;
    while(iter != NULL) {
        Car *temp = iter -> next;
        free(iter -> numberPlate);
        free(iter -> brand);
        free(iter -> repairArray);
        free(iter);
        iter = temp;
    }
    carList -> head = NULL;
    carList -> tail = NULL;
    free(carList);
}
