#include "ccapi.h"
#include <cstdio>
#include <iostream>
#include <fstream>

static CCAPI ccapi;

#define BLOCK_LENGTH 1024;

using namespace std;

int chooseProc()
{
	cout << "Printing Processes..." << endl;
	std::vector<CCAPI::ProcessInfo> procs = ccapi.GetProcessList();

	for (int i = 0; i<procs.size(); i++)
		cout << i << " Process PID: " << procs[i].pid << " : " << procs[i].name << std::endl;
	cout << endl;

	cout << "Enter a # Process to search: ";
	int itr;
	cin >> itr;
	ccapi.AttachProcess(procs[itr].pid);

	return 0;
}

int certSearch()
{
	static const char certStart[] = "-----BEGIN CERTIFICATE-----";
	const int startLen = 27;
	static const char firstChar = '-';

	static const char certEnd[] = "-----END CERTIFICATE-----";
	const int endLen = 25;

	char data[1024];
	u64 address = 0;
	memset(&data, 0, 1024);
	for (u64 i = 0; i < 0x100000000; i = i + 1024)
	{
		if (ccapi.ReadMemory(i, 1024, &data) == CCAPI_OK)
		{
			for (int j = 0; j < 1024; j++)
			{
				if (data[j] == firstChar)
				{
					int x = strncmp(certStart, data + j, startLen);
					if (x == 0)
					{
						cout << data + j << endl;
					}
				}
			}
		}
	}

	return 0;
}

int writeBytesToFile(char* data,int length)
{
	
	
	
	return 0;
}

int memDump()
{
	ofstream fout;
	fout.open("file.bin", ios::binary | ios::out);
	const int length = 0x10000;
	const int block = 0x10000000;
	char data[length];
	u64 address = 0;
	memset(&data, 0, length);
	int count = 0;

	cout << "Writing Memory to disk..." << endl;
	for (u64 i = 0; i < block; i = i + length)
	{
		if (ccapi.ReadMemory(i, length, &data) == CCAPI_OK)
		{
			if (fout.write(data, length))
			{
				count++;
				if (!(count % 10)) cout << ".";
				continue;
			}
			else
			{
				cout << "Error writing data to file. Address: " << i << endl;
			}
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;


	if (!ccapi.GetLibraryState())
	{
		cout << "CCAPI Library couldn't be loaded." << endl
			<< "Check that you are using the correct CCAPI.dll." << endl;
	}
	else
	{
		vector<CCAPI::ConsoleInfo> cInfo = ccapi.GetConsoleList();

		for (u32 c = 0; c < cInfo.size(); ++c)
		{
			cout << "Console: " << cInfo.at(c).name << " Ip:" << cInfo.at(c).ip << endl;
		}

		if (ccapi.Connect("192.168.88.138") != CCAPI_OK) //_PS3_IP_HERE_
		{
			cout << "Couldn't establish a connection with your PS3." << endl
				<< "Verify your ps3 ip." << endl;
		}
		else
		{
			cout << "Connected to your PS3." << endl;
			ccapi.VshNotify(CCAPI::NotifyTrophy2, "Connected to CCAPI");
			ccapi.RingBuzzer(CCAPI::BuzzerTriple);

			u32 firmware, version;
			CCAPI::ConsoleType cType;
			int cell, rsx;
			int ret = ccapi.GetFirmware(&firmware) |
				ccapi.GetConsoleType(&cType) |
				ccapi.GetVersion(&version) |
				ccapi.GetTemperature(&cell, &rsx);

			if (ret != CCAPI_OK)
			{
				cout << "An error occured while retrieving console details." << endl;
			}
			else
			{
				cout << "CCAPI Version: " << version << endl;
				cout << "CCAPI Library version: " << ccapi.GetDllVersion() << endl;
				cout << "Firmware: " << CCAPI::FirmwareToString(firmware) << endl;
				cout << "Console Type: " << CCAPI::ConsoleTypeToString(cType) << endl;
				cout << "CELL Temp: " << cell << endl;
				cout << "RSX Temp: " << rsx << endl;

				
				chooseProc();
				memDump();
				u8 i = ccapi.ReadMemory<u8>(0x50000);
				cout << "Read at 0x50000: " << hex << (u32)i << endl;

				/*
				ccapi.WriteMemory<std::string>(0x,"hello");
				ccapi.SetConsoleIds(CCAPI::Idps,"0123456789ABCDEF0123456789ABCDEF");
				*/

			}
		}
	}
	ccapi.Disconnect();

	cout << "Press any key to exit" << endl;
	getchar();

	return 0;
}
