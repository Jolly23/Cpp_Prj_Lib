#ifndef SYSTIME_H
#define SYSTIME_H


class SysTime
{
public:
    SysTime();
    char* GetTime();
    int yday2mday(int nDays, int nYear);
    void Now(char *rBuff);
};

#endif // SYSTIME_H
