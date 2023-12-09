#ifndef NEWCUSTOMER_H
#define NEWCUSTOMER_H

#include "menu.h"
/**
@function newCustomer uj ugyfelet hoz letre.
@param customerList az ugyfelek listara mutato pointer.
@return a program futhat-e tovabb vagy be kell zarni (memoriafoglalasi vagy fajlmegnyitasi hiba eseten).
**/
bool newCustomer(CustomerList *customerList);
/**
@function readInString beolvas egy stringet.
@return a beolvasott string vagy hiba eseten NULL pointer.
**/
char *readInString();
/**
@function writeToCustomerFile az ugyfelek listajat beirja az ugyfelek fajlba.
@param customerList az ugyfelek listara mutato pointer.
@return sikerult-e a fajlbairas.
**/
bool writeToCustomerFile(CustomerList *customerList);
/**
@function insertCustomerAtEnd az ugyfelek listajanak vegere beszurja az uj ugyfelet.
@param customerList az ugyfelek listara mutato pointer.
@param newCustomer az uj ugyfelre mutato pointer.
@return az uj ugyfelek listara mutato pointer vagy hiba eseten NULL pointer.
**/
CustomerList *insertCustomerAtEnd(CustomerList *customerList, Customer *newCustomer);
/**
@function numberOfCustomers az ugyfelek lista elemeit szamolja meg.
@param customerList az ugyfelek listara mutato pointer.
@return az ugyfelek lista elemeinek szama.
**/
unsigned int numberOfCustomers(const CustomerList *customerList);
/**
@function readInStringFromFile egy stringet olvas be a ';' karakterig.
@param fp a fajlra mutato pointer, amibol ovas.
@return a beolvasott string vagy hiba eseten NULL pointer.
**/
char *readInStringFromFile(FILE *fp);
/**
@function readCustomerListFromFile beolvassa a fajlban tarolt ugyfelek lista elemeinek adatait es elkesziti a duplan lancolt listat.
@return az elkeszult listara mutato pointer vagy hiba eseten NULL pointer.
**/
CustomerList *readCustomerListFromFile();
/**
@function freeCustomerList felszabaditja az ugyfelek lista minden elemet, elemek neveit, autolistajat es az ugyfelek listat.
@param customerList az ugyfelek listara mutato pointer.
**/
void freeCustomerList(CustomerList *customerList);

#endif // NEWCUSTOMER_H
