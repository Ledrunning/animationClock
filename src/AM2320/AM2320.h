#ifndef AM2320_H
#define AM2320_H

#define AM2320_address (0xB8 >> 1) 

void AM2320_init(void);
int AM2320_read(void);


#endif
