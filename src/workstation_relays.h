#ifndef WORKSTATON_RELAYS_H
#define WORKSTATION_RELAYS_H

void WorkstationRelaysInit(void);
void WorkstationRelaysMain(void); 


void WorkstationRelaysActivateRelay1(void);
void WorkstationRelaysDeactivateRelay1(void);
void WorkstationRelaysActivateRelay2(void);
void WorkstationRelaysDeactivateRelay2(void);
void WorkstationRelaysActivateRelay3(void);
void WorkstationRelaysDeactivateRelay3(void);
void WorkstationRelaysActivateRelay4(void);
void WorkstationRelaysDeactivateRelay4(void);

boolean WorkstationRelaysReadRelay1Status(void);
boolean WorkstationRelaysReadRelay2Status(void);
boolean WorkstationRelaysReadRelay3Status(void);
boolean WorkstationRelaysReadRelay4Status(void);
#endif
