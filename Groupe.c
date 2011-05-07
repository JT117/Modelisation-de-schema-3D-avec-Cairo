#include "Groupe.h"

Groupe_initialiser( Groupe* groupe, Groupe* pere, int nb )
{
    groupe->groupePere = pere;
    groupe->id = nb;
}

