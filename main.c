#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debugmalloc.h"
#include "menu.h"
#include "newCustomer.h"
#include "newCar.h"
#include "repairs.h"
/*meghivja a fajl beolvaso fuggvenyeket, ha van adat, annak memoriat foglal, betolti a fajlbol.
Ciklusban hivja meg a menu-t, az addig fut, amig kilepes parancsot nem kap. Meghivja a listat felszabadito fuggvenyeket.*/
int main(void) {
    CustomerList *customerList = readCustomerListFromFile();  //Az ugyfellista beolvasasa
    if (customerList == NULL) {
        memoryAllocError();
        return 0;
    }
    CarList *carList = NULL;
    RepairList *repairList = NULL;
    if (customerList -> head != NULL) {
        carList = readCarListFromFile(customerList);     //Az autolista beolvasasa
        if (carList == NULL) {
            freeCustomerList(customerList);
            memoryAllocError();
            return 0;
        }
        if(carList -> head != NULL) {
            repairList = readRepairListFromFile(carList); //A javitaslista beolvasasa
            if (repairList == NULL) {
                freeCustomerList(customerList);
                freeCarList(carList);
                memoryAllocError();
                return 0;
            }
        }
        else { //ures autok lista eseten a javitasok lista is ures kell legyen
            repairList = (RepairList*) malloc(sizeof(RepairList)); //memoriafoglalas a javitaslista strazsait tartalmazo strukturanak
            if (repairList == NULL)
                return 0;
            repairList -> head = NULL;
            repairList -> tail = NULL;
        }
    }
    else { //ures ugyfelek lista eseten az autok es javitasok lista is ures kell legyen
        carList = (CarList*) malloc(sizeof(CarList)); //memoriafoglalas az autolista strazsait tartalmazo strukturanak
        if (carList == NULL)
            return 0;
        carList -> head = NULL;
        carList -> tail = NULL;
        repairList = (RepairList*) malloc(sizeof(RepairList)); //memoriafoglalas a javitaslista strazsait tartalmazo strukturanak
        if (repairList == NULL)
            return 0;
        repairList -> head = NULL;
        repairList -> tail = NULL;
    }
    bool run = true;
    printASCIIART();
    while(run) {
        run = menu(customerList, carList, repairList);
    }
    freeCustomerList(customerList); //Az ugyfellista felszabaditasa
    freeCarList(carList);           //Az autolista felszabaditasa
    freeRepairList(repairList);     //A javitaslista felszabaditasa
    return 0;
}
