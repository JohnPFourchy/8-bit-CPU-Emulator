#ifndef BUS_H
#define BUS_H

#include "..\utils\defines.h"

void writeBus(Byte address, Byte data);
Byte readBus(Byte address);

#endif /*BUS_H*/