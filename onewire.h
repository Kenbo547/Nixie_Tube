
#ifndef ONEWIRE_H
#define ONEWIRE_H

#include <inttypes.h>

#define ONEWIRE_ERROR_OK 	0
#define ONEWIRE_ERROR_COMM 	1

extern uint8_t onewireInit( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask );
extern uint8_t onewireWriteBit( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, uint8_t bit );
extern void onewireWrite( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, uint8_t data );
extern uint8_t onewireReadBit( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask );
extern uint8_t onewireRead( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask );

#endif
