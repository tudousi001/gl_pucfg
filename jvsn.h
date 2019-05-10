//---------------------------------------------------------------------------

#ifndef jvsnH
#define jvsnH
//---------------------------------------------------------------------------

#include <windows.h>

typedef struct
{
    DWORD dwSubsystemID;
    DWORD dwVolume;
    DWORD dwOemId;
    DWORD dwGroup;
    DWORD dwVersion;
    DWORD nCheckerID;
    DWORD dwUser;
    DWORD nCurID;
}JV_INFO;

int ReadJVIni(JV_INFO *info);
int JvSnGenerate(JV_INFO *info, char *out);

#endif
