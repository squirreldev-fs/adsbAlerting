#include "aircraftlist.h"

AircraftList::AircraftList()
{
}

bool AircraftList::contains(const QString& icao) const
{
    bool found = false;
    for(int i=0;i<this->size();i++)
    {
        if(this->at(i).getIcao() == icao)
        {
            found = true;
            break;
        }
    }
    return found;
}

int AircraftList::indexOfIcao(const QString& icao) const
{
    int index = -1;
    for(int i=0;i<this->size();i++)
    {
        if(this->at(i).getIcao() == icao)
        {
            index = i;
            break;
        }
    }
    return index;
}

int AircraftList::indexOfReg(const QString& reg) const
{
    int index = -1;
    for(int i=0;i<this->size();i++)
    {
        if(this->at(i).getRegistration() == reg)
        {
            index = i;
            break;
        }
    }
    return index;
}
