#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "debugmalloc.h"
#include "menu.h"
#include "newCustomer.h"
#include "newCar.h"

Customer *createEmptyCustomer();

int newCustomerDataInput(Customer *customer, CustomerList *customerList);

int CheckAndAdd(Customer *customer, CustomerList *customerList);

bool correctName(const Customer *customer);

unsigned long scanUL(const unsigned long min, const unsigned long max);

bool existingCustomer(const Customer *customer, const CustomerList *customerList);

char **stringArrayFromCustomerList(CustomerList *customerList);

CustomerList *readCustomersFromFile(FILE *fp, CustomerList *customerList);
/*a main hivja meg, parameterul kapja az ugyfelek listat, visszaadja, hogy a program futhat-e tovabb, vagy ki kell leptetni. Uj ugyfelet hoz letre.*/
bool newCustomer(CustomerList *customerList) {
    Customer *newCustomer = createEmptyCustomer();
    if (newCustomer == NULL) {
        memoryAllocError();
        return false;
    }
    int newCustomerDataInputSuccess = newCustomerDataInput(newCustomer, customerList);
    while(newCustomerDataInputSuccess != 2) {
        if(newCustomerDataInputSuccess == -1)
            return false;
        else if(newCustomerDataInputSuccess == 0)
            return true;
        else
            newCustomerDataInputSuccess = newCustomerDataInput(newCustomer, customerList);
    }
    int CheckAndAddSuccess = CheckAndAdd(newCustomer, customerList);
    while(CheckAndAddSuccess != 2) {
        if(CheckAndAddSuccess == -1)
            return false;
        else if(CheckAndAddSuccess == 0)
            return true;
        newCustomerDataInputSuccess = newCustomerDataInput(newCustomer, customerList);
        while(newCustomerDataInputSuccess != 2) {
            if(newCustomerDataInputSuccess == -1)
                return false;
            else if(newCustomerDataInputSuccess == 0)
                return true;
            else
                newCustomerDataInputSuccess = newCustomerDataInput(newCustomer, customerList);
        }
        CheckAndAddSuccess = CheckAndAdd(newCustomer, customerList);
    }
    return true;
}

Customer *createEmptyCustomer() {
    Customer *newCustomer = (Customer*) malloc(sizeof(Customer));
    if (newCustomer == NULL)
        return NULL;
    newCustomer -> ID = 0;
    newCustomer -> surName = NULL;
    newCustomer -> firstName = NULL;
    newCustomer -> phoneNumber = 0;
    newCustomer -> prev = NULL;
    newCustomer -> next = NULL;
    return newCustomer;
}

int newCustomerDataInput(Customer *customer, CustomerList *customerList) {
    int success = 1;
    clearInputBuffer();
    printf("Uj ugyfel vezetekneve: ");
    char *newSurName = readInString(&success);
    if (!success) {
        free(newSurName);
        if (errorHandling() == 1)
            return 1;
        free(customer);
        return 0;
    }
    if(newSurName == NULL) {
        memoryAllocError();
        return -1;
    }
    customer -> surName = newSurName;
    printf("Uj ugyfel keresztneve: ");
    char* newFirstName = readInString(&success);
    if (!success) {
        free(customer -> surName);
        free(newFirstName);
        if (errorHandling() == 1)
            return 1;
        free(customer);
        return 0;
    }
    if(newFirstName == NULL) {
        memoryAllocError();
        return -1;
    }
    customer -> firstName = newFirstName;
    printf("Uj ugyfel telefonszama (formatum: 301234567): ");
    customer -> phoneNumber = scanUL(100000000, 999999999);
    printf("\n");
    bool nameCheck = correctName(customer);
    if (!nameCheck) {
        printf("Helytelen adatbevitel. Hibas nev.\n");
        free(customer -> surName);
        free(customer -> firstName);
        if (errorHandling() == 1)
            return 1;
        free(customer);
        return 0;
    }
    return 2;
}

int CheckAndAdd(Customer *customer, CustomerList *customerList) {
    if (existingCustomer(customer, customerList)) {
        printf("Ilyen ugyfel mar letezik.\n");
        free(customer -> surName);
        free(customer -> firstName);
        if (errorHandling() == 1)
            return 1;
        free(customer);
        return 0;
    }
    else {
        int nextID;
        if(customerList -> tail != NULL)
            nextID = (customerList -> tail -> ID + 1);
        else
            nextID = 1;
        customer -> ID = nextID;
        customer -> carArray = (Car**) malloc(sizeof(Car*));
        customer -> carArray[0] = NULL;
        customerList = insertCustomerAtEnd(customerList, customer);
        if (writeToCustomerFile(customerList))
            printf("Sikeresen felvett uj ugyfel.\n");
        else {
            printf("Nem sikerult megnyitni a fajlt vagy nem sikerult memoriat foglalni.\n");
            free(customer -> surName);
            free(customer -> firstName);
            free(customer);
            return -1;
        }
        exitOnly();
        return 2;
    }
}

bool writeToCustomerFile(CustomerList *customerList) {
    FILE* fp;
    fp = fopen("ugyfelek.txt", "w");
    if (fp == NULL)
        return false;
    unsigned int numberOfCustomersUI = numberOfCustomers(customerList);
    char **stringArray = stringArrayFromCustomerList(customerList);
    if (stringArray == NULL) {
        return false;
    }
    for(int i = 0; i < numberOfCustomersUI; ++i) {
        fprintf(fp, "%s\n", stringArray[i]);
        free(stringArray[i]);
    }
    free(stringArray);
    fclose(fp);
    return true;
}

char **stringArrayFromCustomerList(CustomerList *customerList) {
    unsigned int numberOfCustomersUI = numberOfCustomers(customerList);
    char **stringArray = (char**) malloc(sizeof(char*)*numberOfCustomersUI);
    if (stringArray == NULL) {
        return NULL;
    }
    Customer *temp = customerList -> head;
    for (int i = 0; i < numberOfCustomersUI && temp != NULL; ++i, temp = temp -> next) {
        int IDDigits = 1;
        int tempID = temp -> ID;
        while(tempID /= 10)
            ++IDDigits;
        int numberPlatesLength = 0;
        int numberOfCarsPerCustomerD = numberOfCarsPerCustomer(temp);
        for(int i = 0; i < numberOfCarsPerCustomerD; ++i) {
            numberPlatesLength += strlen(temp -> carArray[i] -> numberPlate);
        }
        stringArray[i] = (char*) malloc(sizeof(char)*(IDDigits + 1 + strlen(temp -> surName)) + 1 + strlen(temp -> firstName) + 1 + 9 + 1\
                                        + numberPlatesLength + numberOfCarsPerCustomerD - 1 + 1);
        if (stringArray[i] == NULL) {
            return NULL;
        }
        sprintf(stringArray[i], "%u;%s;%s;%lu", temp -> ID, temp -> surName, temp -> firstName, temp -> phoneNumber);
        for(int j = 0; j < numberOfCarsPerCustomerD; ++j) {
            strcat(stringArray[i], ";");
            strcat(stringArray[i], temp -> carArray[j] -> numberPlate);
        }
    }
    return stringArray;
}

CustomerList *insertCustomerAtEnd(CustomerList *customerList, Customer *newCustomer) {
    if (customerList -> head == NULL) {
        customerList -> head = newCustomer;
        customerList -> tail = newCustomer;
    }
    else {
        newCustomer -> prev = customerList -> tail;
        customerList -> tail -> next = newCustomer;
        customerList -> tail = newCustomer;
        newCustomer -> next = NULL;
    }
    customerList -> head -> prev = NULL;
    customerList -> tail -> next = NULL;
    return customerList;
}

unsigned int numberOfCustomers(const CustomerList *customerList) {
    int numberOfCustomers = 0;
    Customer *temp;
    if (customerList -> head == NULL)
        return 0;
    for(temp = customerList -> head; temp != NULL; temp = temp -> next) {
        ++numberOfCustomers;
    }
    return numberOfCustomers;
}

bool correctName(const Customer *customer) {
    if(strcmp(customer -> surName, " ") == 0 || strcmp(customer -> firstName," ") == 0) {
        return false;
    }
    int correctLetter = 0;
    for(int i = 0; i < strlen(customer -> surName); ++i) {
        if((customer -> surName[i] >= 'a' && customer -> surName[i] <= 'z') || (customer -> surName[i] >= 'A' && customer -> surName[i] <= 'Z') || customer -> surName[i] == '-')
            ++correctLetter;
    }
    for(int i = 0; i < strlen(customer -> firstName); ++i) {
        if((customer -> firstName[i] >= 'a' && customer -> firstName[i] <= 'z') || (customer -> firstName[i] >= 'A' && customer -> firstName[i] <= 'Z') || customer -> firstName[i] == '-')
            ++correctLetter;
    }
    if (correctLetter == (strlen(customer -> surName) + strlen(customer -> firstName)))
        return true;
    else
        return false;
}

bool existingCustomer(const Customer *customer, const CustomerList *customerList) {
    if (customer == NULL || customer -> surName == NULL || customer -> firstName == NULL || customerList -> head == NULL)
        return false;
    for (const Customer *temp = customerList -> head; temp != NULL; temp = temp -> next) {
        if (temp -> ID == 0 || temp -> surName == NULL || temp -> firstName == NULL || temp -> phoneNumber == 0)
            break;
        if (strcmp(customer -> surName, temp -> surName) == 0 && strcmp(customer->firstName, temp -> firstName) == 0 && customer -> phoneNumber == temp -> phoneNumber) {
            return true;
        }
        if (temp == customerList -> tail)
            break;
    }
    return false;
}

char *readInString(int *correctNameInput) {
    *correctNameInput = 1;
    char inputChar;
    int newLength = 0;
    char *newString = NULL;
    while(scanf("%c", &inputChar) == 1 && inputChar != '\n') {
        ++newLength;
        newString = realloc(newString, sizeof(char)*(newLength + 1));
        if (newString == NULL) {
            return NULL;
        }
        newString[newLength - 1] = inputChar;
        newString[newLength] = '\0';
    }
    if (newLength == 0) {
        printf("\nHelytelen adatbevitel.\n");
        *correctNameInput = 0;
        return NULL;
    }
    return newString;
}

unsigned long scanUL(const unsigned long min, const unsigned long max) {
    while (true) {
        unsigned long read;
        unsigned long successful = scanf("%lu", &read);
        if (successful != 1) {
            printf("Amit megadott, nem szam volt!\n");
            clearInputBuffer();
            printf("Uj ugyfel telefonszama (301234567): ");
        }
        else if (read > max || read < min) {
            printf("Nem megfelelo telefonszam, probalja ujra. (min: %lu, max: %lu)\n", min, max);
            printf("Uj ugyfel telefonszama (301234567): ");
        }
        else
            return read;
    }
}

CustomerList *readCustomerListFromFile() {
    FILE *fp;
    fp = fopen("ugyfelek.txt", "r");
    CustomerList *customerList = (CustomerList*) malloc(sizeof(CustomerList)); //memoriafoglalas az ugyfellista strazsait tartalmazo strukturanak
    if (customerList == NULL) {
        return NULL;
    }
    customerList -> head = NULL;
    customerList -> tail = NULL;
    if (fp == NULL) {
        return customerList;
    }
    customerList = readCustomersFromFile(fp, customerList); //a lista feltoltese fajlbol
    if(customerList == NULL) {
        return NULL;
    }
    fclose(fp);
    return customerList;
}

CustomerList *readCustomersFromFile(FILE *fp, CustomerList *customerList) {
    while(1) {
        Customer *customer = (Customer*) malloc(sizeof(Customer));  //az egyes ugyfelek memoriafoglalasa
        if (customer == NULL) {
            freeCustomerList(customerList);
            return NULL;
        }
        unsigned int tempID;
        if (fscanf(fp, "%u;", &(tempID)) <= 0 || tempID <= 0) {
            free(customer);
            return customerList;
        }
        customer -> ID = tempID;
        char *newSurName = readInStringFromFile(fp); //vezeteknev beolvasasa
        if (newSurName == NULL) {
            free(customer);
            freeCustomerList(customerList);
            return NULL;
        }
        customer -> surName = newSurName;
        char* newFirstName = readInStringFromFile(fp); //keresztnev beolvasasa
        if (newFirstName == NULL) {
            free(customer -> surName);
            free(customer);
            freeCustomerList(customerList);
            return NULL;
        }
        customer -> firstName = newFirstName;
        fscanf(fp, "%lu", &(customer -> phoneNumber));
        char input;
        while(fscanf(fp, "%c", &input) == 1 && input != '\n')
            ;
        customer -> carArray = (Car**) malloc(sizeof(Car*));
        customer -> carArray[0] = NULL;
        customerList = insertCustomerAtEnd(customerList, customer);
    }
    return customerList;
}

char *readInStringFromFile(FILE *fp) {
    char inputChar;
    int newLength = 0;
    char *newString = NULL;
    while(fscanf(fp, "%c", &inputChar) == 1 && inputChar != ';') {
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

void freeCustomerList(CustomerList *customerList) {
    Customer *iter = customerList -> head;
    while(iter != NULL) {
        Customer *temp = iter -> next;
        free(iter -> surName);
        free(iter -> firstName);
        free(iter -> carArray);
        free(iter);
        iter = temp;
    }
    customerList -> head = NULL;
    customerList -> tail = NULL;
    free(customerList);
}
