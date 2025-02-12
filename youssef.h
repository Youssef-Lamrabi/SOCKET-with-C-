#ifndef YOUSSEF_H
#define YOUSSEF_H

int add(int a, int b);
void exor(char *polyGenerateur, char * tmp);
void GetMessage(char * msgSansFanion, int msgLength, char * trame);
void MultiParPolyGenerateur(char * msgSansFanion, int msgLength);
void decaler(char *tmp, char nextBit);
void Division(char *dividende, char * trame);
void GetMessagePlusRest(char * msgPlusRest, int messageLength, char * trame);
int CrcRecieve(char *trame);
int checkRest(char *tmp);
void CrcTrasnfert(char * trame, char *msgShouldBeSent);
int calculate_next_hop(int current_router);

#endif // YOUSSEF_H
