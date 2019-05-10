//---------------------------------------------------------------------------


#pragma hdrstop

#include "jvsn.h"
#include "md5.h"
#include <time.h>
#include <stdio.h>
#include <vcl\Registry.hpp>
//---------------------------------------------------------------------------

#pragma package(smart_init)

AnsiString JvFile = "jvinfo.ini";

void SaveJVIni(JV_INFO *info)
{
        TIniFile *ini;

      //  ini = new TIniFile(
      //          ChangeFileExt( Application->ExeName, ".INI" ) );
        ini = new TIniFile(JvFile);
        if(!ini)
        {
            //ShowMessage("对不起,文件打不开!");
            return;
        }

        ini->WriteInteger("Generate", "OemId", info->dwOemId);       
        ini->WriteInteger("Generate", "Group", info->dwGroup);
        ini->WriteInteger("Generate", "Version", info->dwVersion);
        ini->WriteInteger("Generate", "CheckerID", info->nCheckerID);
        ini->WriteInteger("Generate", "User", info->dwUser);
        ini->WriteInteger("Generate", "ID", info->nCurID);

        delete ini;
}

int ReadJVIni(JV_INFO *info)
{
        TIniFile *ini;
        //ini = new TIniFile(
        //        ChangeFileExt( Application->ExeName, ".INI" ) );
        ini = new TIniFile(JvFile);
        if(!ini)
        {
            //ShowMessage("对不起,文件打不开!");
            return -1;
        }

        info->dwSubsystemID = 0xE71AFFFF;
	info->dwVolume = 0xF097765B;
        info->dwOemId = ini->ReadInteger("Generate", "OemId", 0);
        info->dwGroup = ini->ReadInteger("Generate", "Group", 'A');
        info->dwVersion = ini->ReadInteger("Generate", "Version", 0x01000001);
        info->nCheckerID = ini->ReadInteger("Generate", "CheckerID", 1);
        info->dwUser = ini->ReadInteger("Generate", "User", 0x00010001);
        info->nCurID = ini->ReadInteger("Generate", "ID", 0);
        delete ini;

        return 0;
}

static void MD5Build(BYTE *string, int len, BYTE *digest)
{
    MD5_CTX context;

    MD5_Init (&context);
    MD5_Update (&context, string, len);
    MD5_Final (&context, digest);
}

int JvSnGenerate(JV_INFO *info, char *out)
{
        DWORD n, n1, dwID = info->nCurID + 1;
		GUID guid;
		HRESULT result;
		CoInitialize(NULL);
		result = CoCreateGuid(&guid);
		CoUninitialize();

		for (n=0; n<4; n++)
		{
			out[0x00+n] = (UCHAR)((guid.Data1 >> (DWORD)(n*8)) & 0xFF);
		}
		
		for (n=0; n<2; n++)
		{
			out[0x04+n] = (UCHAR)((guid.Data2 >> (WORD)(n*8)) & 0xFF);
			out[0x06+n] = (UCHAR)((guid.Data3 >> (WORD)(n*8)) & 0xFF);
		}

		for (n=0; n<8; n++)
		{
			out[0x08+n] = (UCHAR)guid.Data4[n];
		}

	//	out[0] = 0xA0;
	//	out[1] = 0xB0;
	//	out[2] = 0xC0;
	//	out[3] = 0xD0;

		time_t t;
		time(&t);
	
		UCHAR volume[4], datetime[4], serial[4], subsys[4], ver[4], user[4], group[4], oemid[4];
		//volume：卷标；datetime：生产日期；id：序列号；subsys：板卡类型（C800，C900）oemid: OEM厂商号码各占2个字节；ver：硬件版本；
		//user：用户group:编组号。
		UCHAR temp[4];
		UCHAR digest[16];
		for (n=0; n<4; n++)
		{
			volume[n] = UCHAR(info->dwVolume >> (DWORD)(n*8));
			datetime[n] = UCHAR(t >> (DWORD)(n*8));
			serial[n] = UCHAR(dwID >> (DWORD)(n*8)); //serial
			subsys[n] = UCHAR(info->dwSubsystemID >> (DWORD)(n*8));
			ver[n] = UCHAR(info->dwVersion >> (DWORD)(n*8)); 
			user[n] = UCHAR(info->dwUser >> (DWORD)(n*8));
			group[n] = UCHAR(info->dwGroup>> (DWORD)(n*8));
			oemid[n] = UCHAR(info->dwOemId>> (DWORD)(n*8));

			out[0x10+n] =  volume[n];
			out[0x14+n] =  datetime[n];
			out[0x18+n] =  serial[n];
			out[0x1C+n] =  subsys[n];
			out[0x20+n] =  ver[n];
			out[0x24+n] =  user[n];
			out[0x28+n] =  group[n];
			out[0x2c+n] =  oemid[n];
		}
		//加密点1
		for (n=0; n<4; n++)
			temp[n] = datetime[n] ^ out[0x06+n];
		MD5Build(temp, 4, digest);
		for (n=0; n<4; n++)
			out[0x30+n] = digest[n];
		//加密点2
		for (n=0; n<4; n++)
			temp[n] = volume[n] ^ serial[n];
		MD5Build(temp, 4, digest);
		for (n=0; n<4; n++)
			out[0x34+n] = digest[n];
		//加密点3
		for (n=0; n<4; n++)
			temp[n] = ver[n] ^ datetime[n];
		MD5Build(temp, 4, digest);
		for (n=0; n<4; n++)
			out[0x38+n] = digest[n];
		//加密点4
		for (n=0; n<4; n++)
			temp[n] = user[n] ^ datetime[n];
		MD5Build(temp, 4, digest);
		for (n=0; n<4; n++)
			out[0x3C+n] = digest[n];
		//加密点5
		for (n=0; n<4; n++)
			temp[n] = group[n] ^ subsys[n];
		MD5Build(temp, 4, digest);
		for (n=0; n<4; n++)
			out[0x40+n] = digest[n];
		//加密点6
		for (n=0; n<4; n++)
			temp[n] = subsys[n] ^ serial[n];
		MD5Build(temp, 4, digest);
		for (n=0; n<4; n++)
			out[0x44+n] = digest[n];
		//加密点7
		for (n=0; n<4; n++)
			temp[n] = out[0x03+n*2];
		MD5Build(temp, 4, digest);
		for (n=0; n<4; n++)
			out[0x48+n] = digest[n];
		//加密点8
		for (n=0; n<4; n++)
			temp[n] = out[0x05+n*3];
		MD5Build(temp, 4, digest);
		for (n=0; n<4; n++)
			out[0x4C+n] = digest[n];

		unsigned char* plain;
		plain = &(out[0x30]);
		MD5Build(plain, 0x10, digest);
		for (n=0; n<16; n++)
			out[0x10+n] ^= digest[n];

		plain = &(out[0x40]);
		MD5Build(plain, 0x10, digest);
		for (n=0; n<16; n++)
			out[0x20+n] ^= digest[n];

		out[0x50] = 0xFF;
		for (n=0;n<0x50;n++)
		{
			out[0x50] ^= out[n];
		}

		for (n=0;n<0x51;n++)
		{
			out[0x51 + n] = out[n] ^ ((n*2+3)%5 + 19);
			out[0x51*2 + n] = out[n] ^ ((n*3+2)%7 + 23);
		}

		out[0xF3] = 0xFF;
		for (n=0;n<0xF3;n++)
		{
			out[0xF3] ^= out[n];
		}


		out[0xF4] = 'J';
		out[0xF5] = 'o';
		out[0xF6] = 'v';
		out[0xF7] = 'i';
		out[0xF8] = 's';
		out[0xF9] = 'i';
		out[0xFA] = 'o';
		out[0xFB] = 'n';
		out[0xFC] = 'E';
		out[0xFD] = '7';
		out[0xFE] = '1';
		out[0xFF] = 'A';

    SYSTEMTIME st;
    char strItem[256];
    GetLocalTime(&st);
    sprintf(strItem, "%04d-%04d%02d%02d.txt", info->nCheckerID, st.wYear, st.wMonth, st.wDay);
    HANDLE fh = CreateFile(strItem, GENERIC_WRITE, FILE_SHARE_READ, NULL,
             OPEN_ALWAYS, 0, 0);
    if (fh == INVALID_HANDLE_VALUE)
    {
        return 1;
    }
    SetFilePointer(fh, 0, 0, FILE_END);
    sprintf(strItem, "%08X %08X %08X-%04X-%04X-%02X%02X%02X%02X%02X%02X%02X%02X %08X %04X %04X\r\n",
				dwID, t, guid.Data1, guid.Data2, guid.Data3,
				guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7],
				info->dwVolume,info->dwOemId,info->dwGroup);
    if(!WriteFile(fh, strItem, strlen(strItem), &n, 0))
    {
        //MessageBox(NULL, "写文件失败！", "检验器", MB_OK|MB_ICONERROR);
        return 2;
    }
	     /*	CString strPath;
		strPath.Format("%s\\reg\\%d\\%02d", info->config.acSelfPath, gmtime(&t)->tinfo->year+1900, gmtime(&t)->tinfo->mon+1);
		if (GetFileAttributes(strPath) != FILE_ATTRIBUTE_DIRECTORY)
			CreateAllDirectories(strPath);

		CString strFile;
		strFile.Format("%s\\%04d-%04d%02d%02d.txt", strPath, info->nCheckerID, gmtime(&t)->tinfo->year+1900, gmtime(&t)->tinfo->mon+1, gmtime(&t)->tinfo->mday);
		CStdioFile file;
		if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
		{
			file.SeekToEnd();
			CString strItem;
			strItem.Format("%08X %08X %08X-%04X-%04X-%02X%02X%02X%02X%02X%02X%02X%02X %08X %04X %04X\n",
				dwID, t, guid.Data1, guid.Data2, guid.Data3,
				guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7],
				info->dwVolume,info->dwOemId,info->dwGroup);
			file.WriteString(strItem);
			file.Close();
		}
		else
		{
			MessageBox("写文件失败！", "检验器", MB_OK|MB_ICONERROR);
			return;
		}         */
    CloseHandle(fh);
    info->nCurID = dwID;
    SaveJVIni(info);
    return 0;
}