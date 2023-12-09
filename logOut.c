#include <stdio.h>
#include <stdlib.h>
#include "debugmalloc.h"
#include "menu.h"
#include "logOut.h"
/*a main hivja meg, parameterul kapja az autok es javitasok listat, visszaadja, hogy a program futhat-e tovabb, vagy ki kell leptetni (mindig kilepteti).
Kilep a programbol.*/
bool logOut() {
    printf("Viszontlatasra!\n\n");
    return false;
}
