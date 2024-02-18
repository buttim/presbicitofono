#ifndef __IAP_H__
#define __IAP_H__

#define CMD_IDLE 0
#define CMD_READ 1
#define CMD_PROGRAM 2
#define CMD_ERASE 3

#define ENABLE_IAP 0x80

#define IAP_ADDRESS 0x1200

void Delay(BYTE n);
void IapIdle(void);
BYTE IapReadByte(WORD addr);
BOOL IapProgramByte(WORD addr, BYTE dat);
void IapEraseSector(WORD addr);
#endif
