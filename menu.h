#ifndef MENU_H
#define MENU_H

/**
@struct Date datum formatumot tarol.
@field  year a datum eve.
@field  month a datum honapja.
@field  day a datum napja.
**/
typedef struct Date {
    unsigned int year, month, day;
} Date;
/**
@struct Customer egy ugyfel adatai.
@field  ID az ugyfel azonositoja.
@field  surName az ugyfel vezetekneve.
@field  firstName az ugyfel keresztneve.
@field  phoneNumber az ugyfel telefonszama.
@field  carArray az ugyfelhez tartozo autokra mutato pointerek tombje.
@field  prev az ugyfelek lista elozo elemere mutato pointer.
@field  next az ugyfelek lista kovetkezo elemere mutato pointer.
**/
typedef struct Customer {
    unsigned int ID;
    char* surName;
    char* firstName;
    unsigned long phoneNumber;
    struct Car **carArray;
    struct Customer *prev, *next;
} Customer;
/**
@struct CustomerList az ugyfelek lista.
@field  head a lista elejere mutato pointer.
@field  tail a lista vegere mutato pointer.
**/
typedef struct CustomerList {
    Customer *head;
    Customer *tail;
} CustomerList;
/**
@struct Car egy auto adatai.
@field  numberPlate az auto rendszama.
@field  active az auto aktiv statusza;
@field  owner az auto tulajdonosa.
@field  brand az auto markaja.
@field  testDate az auto vizsgajanak lejarta.
@field  repairArray az autohoz tartozo javitasok tombje.
@field  prev az autok lista elozo elemere mutato pointer.
@field  next az autok lista kovetkezo elemere mutato pointer.
**/
typedef struct Car {
    char* numberPlate;
    unsigned int active;
    Customer *owner;
    char* brand;
    Date testDate;
    struct Repair **repairArray;
    struct Car *prev, *next;
 } Car;
/**
@struct CarList az autok lista.
@field  head a lista elejere mutato pointer.
@field  tail a lista vegere mutato pointer.
**/
typedef struct CarList {
    Car *head;
    Car *tail;
} CarList;
/**
@enum repairType egy javitas lehetseges neve.
@constant olajcsere .
@constant kerekcsere .
@constant fekcsere .
@constant akkucsere .
@constant gyujtogyertyacsere .
@constant legzsakcsere .
@constant fenyszorocsere .
@constant szenzorcsere .
@constant ablaktorlocsere .
@constant fenyezes .
**/
typedef enum repairType {
    olajcsere = 1,
    kerekcsere = 2,
    fekcsere = 3,
    akkucsere = 4,
    gyujtogyertyacsere = 5,
    legzsakcsere = 6,
    fenyszorocsere = 7,
    szenzorcsere = 8,
    ablaktorlocsere = 9,
    fenyezes = 10
} repairType;
/**
@struct Repair egy javitas adatai.
@field  ID a javitas azonositoja.
@field  car az autora mutato pointer.
@field  name a javitas neve.
@field  done a javitas kesz statusza.
@field  price a javitas ara.
@field  readyDate a javitas elkeszultenek datuma.
@field  prev a javitasok lista elozo elemere mutato pointer.
@field  next a javitasok lista kovetkezo elemere mutato pointer.
**/
typedef struct Repair {
    unsigned int ID;
    Car* car;
    repairType name;
    unsigned int done;
    unsigned int price;
    Date readyDate;
    struct Repair *next, *prev;
} Repair;
/**
@struct RepairList a javitasok lista.
@field  head a lista elejere mutato pointer.
@field  tail a lista vegere mutato pointer.
**/
typedef struct RepairList {
    Repair *head;
    Repair *tail;
} RepairList;
/**
@function menu a fomenu.
@param customerList a jelenlegi ugyfelek listara mutato pointer.
@param carList a jelenlegi autok listara mutato pointer.
@param repairList a jelenlegi javitasok listara mutato pointer.
@return a program futhat-e tovabb vagy be kell zarni.
**/
bool menu(CustomerList *customerList, CarList *carList, RepairList *repairList);
/**
@function scanDecimal egy elojeles egesz szam beolvasasa.
@param min a minimum elfogadhato ertek.
@param max a maximum elfogadhato ertek.
@return a beolvasott egesz ertek.
**/
int scanDecimal(const int min, const int max);
/**
@function errorHandling hiba eseten lehetoseget ad az ujraprobalasra vagy menube visszalepesre.
@return a valasztott menupont sorszama, 0/1 a kilepes/ujraprobalas.
**/
int errorHandling();
/**
@function memoryAllocError memoriafoglalasi hiba eseten kilep a programbol.
**/
void memoryAllocError();
/**
@function fileOpenError fajlmegnyitasi hiba eseten kilep a programbol.
**/
void fileOpenError();
/**
@function exitOnly egyeb hiba eseten kilepteti a felhasznalot a 0 es Enter megnyomasa utan.
**/
void exitOnly();
/**
@function clearInputBuffer kitorli az input bufferben levo karaktereket Enterig bezarolag.
**/
void clearInputBuffer();
/**
@function printASCIIART kirajzolja a program elejen az ASCII karakterekbol allo autokat.
**/
void printASCIIART();
#endif //MENU_H
