#ifndef SEARCHBYCUSTOMER_H
#define SEARCHBYCUSTOMER_H

#include "menu.h"
/**
@function searchByCustomer egy ugyfelet keres meg az ugyfelek listaban es kiirja az adatait, a hozza tartozo auto(k) adatait es az auto(k)hoz tartozo elkeszult javitasok adatait.
@param customerList az ugyfelek listara mutato pointer.
@param carList az autok listara mutato pointer.
@param repairList az javitasok listara mutato pointer.
@return a program futhat-e tovabb vagy be kell zarni (memoriafoglalasi hiba eseten).
**/
bool searchByCustomer(CustomerList *customerList, CarList *carList, RepairList *repairList);
/**
@function customerSearch az ugyfelet keresi meg az ugyfelek listaban a nev vagy a nev egy resze alapjan.
@param customerList az ugyfelek listara mutato pointer.
@return az ugyfel azonositoja vagy a hibakod.
**/
int customerSearch(CustomerList *customerList);

#endif //SEARCHBYCUSTOMER_H
