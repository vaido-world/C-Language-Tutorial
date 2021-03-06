/*
_____________________________________________________________
 File: RegGetValue.c
 Description: Receives registry value from the Registry key
 Notes:
  * https://stackoverflow.com/questions/30812594/how-do-i-determine-the-size-of-a-registry-values-data-using-the-windows-api/30812628#30812628
_____________________________________________________________

This work is marked with CC0 1.0 Universal. 🄍

By marking the work with a CC0 public domain dedication, the creator is giving 
up their copyright and allowing reusers to distribute, remix, adapt, and build 
upon the material in any medium or format, even for commercial purposes.

To view a copy of this license, visit http://creativecommons.org/publicdomain/zero/1.0


*/

// Compiler Pragmatic comments
#pragma comment(lib, "advapi32") // No more Need for "tcc -ladvapi32 RegGetValueA.c"         tcc: error: undefined symbol 'RegGetValueA' 

// Include directives.
#include <stdio.h>               // Prevents a warning:                  test.c:34: warning: implicit declaration of function 'printf'
#include <windows.h>             // Required, else an error:             test.c:12: error: 'DWORD' undeclared
#include <stdlib.h>              // Required, else a warning:            RegGetValue.c:62: warning: implicit declaration of function 'malloc'

// Functions declaration
void openRegistryKey();
void getRegistryKeyValue();
const char* registryHiveInterpretation(hKey);
const char* registryTypeValuesInterpretation(valueType);


// For hKey argument
// Makes Accept both hex values, integer values and string values
// HKEY_LOCAL_MACHINE       80000002      0x80000002
int main(){
	//openRegistryKey();
	getRegistryKeyValue(HKEY_CURRENT_USER);
	return 0;
}



void openRegistryKey(){
	  
	// Types Definitions
	HKEY   hKey;
	LPCSTR lpSubKey;
	DWORD  ulOptions;
	REGSAM samDesired;
	PHKEY  phkResult;
	
	// Pointer to opened registry key
	HKEY OpenedRegistryKey;
	  
	//Error Codes Handling
	LONG ErrorCode;
	  
	  
	// https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regopenkeyexa
	ErrorCode = RegOpenKeyExA(
		hKey      = HKEY_CURRENT_CONFIG,
		lpSubKey  = "System",	  
		ulOptions = 0,
		samDesired = KEY_READ,
		phkResult = &OpenedRegistryKey
	);
	switch(ErrorCode){
		case ERROR_SUCCESS: 
			printf("The registry subkey has been opened successfully\n");
		break;
		
		case ERROR_FILE_NOT_FOUND:
			printf("Registry subkey cannot be found or is incorrect.\n");
		break;
			
		default:
			printf("Less known error, consult Microsoft Documentation: https://docs.microsoft.com/en-us/windows/win32/debug/system-error-codes--0-499-");
		
	}
		printf("Registry hive:   ");
		printf(registryHiveInterpretation(hKey));
		printf(" (Hex value: 0x%x)", hKey);
		printf("\n");
		printf("Registry subkey: \\%s \n", lpSubKey);
		
	ErrorCode = RegCloseKey(OpenedRegistryKey);
	switch(ErrorCode){
		case ERROR_SUCCESS: 
			printf("The registry subkey has been closed successfully\n ");
		break;
		
		default:
			printf("Error code: %x    Consult the Microsoft documentation to understand the error. https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regclosekey  \n", ErrorCode);
	}
	

	
}

// This source code is in Ansi mode.
void getRegistryKeyValue(HKEY KeyHandle){


		// Holds size of the buffer for pvData value in bytes
		// Variable is overwritten when returning, so making a macro that holds the initial value would be needed, to preserve the value after the function return.
		// https://stackoverflow.com/questions/26069085/visual-c-reggetvalue-fails-in-program-where-it-shouldnt/26069277#26069277
		DWORD BufferSize;
		
		// Holds the type of the registry key value
		DWORD valueType;
	
		// Error Codes Handling
		LONG ErrorCode;
	

		// Types Definitions for RegGetValue function arguments
		HKEY    hkey;
		LPCSTR  lpSubKey;
		LPCSTR  lpValue;
		DWORD   dwFlags;
		LPDWORD pdwType; 
		PVOID   pvData; 
		LPDWORD pcbData;
		
		printf("[!]Getting the size of the registry key value to be returned (pvData) \n");
		
		// https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-reggetvaluea
		// RegGetValue, Retrieves the type and data for the specified registry value. https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-reggetvaluea
		ErrorCode = RegGetValueA(
			hkey     = HKEY_LOCAL_MACHINE,
			lpSubKey = "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment",
			lpValue  = "Path",
			dwFlags  = RRF_RT_ANY,
			pdwType  = &valueType, // This default NULL value, states that we do not request specific registry value type  https://docs.microsoft.com/en-us/windows/win32/sysinfo/registry-value-types
			pvData   = NULL, // This default NULL value, states that we do not request the data, only the Size of it
			pcbData  = &BufferSize
		);
		printf("[!]Setting the pointer to registry address of &BufferSize \n");
		printf("[!]The size of pvData will be stored in the variable &BufferSize \n");
		printf("[!]The size of the pvData will be returned in a variable &BufferSize pointed by pcbData \n");
		printf(TEXT("_________________________________________ \n"));
		printf(TEXT("_______________RegGetValue_______________ \n"));
		if (strcmp(lpSubKey, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment") == 0){
			if (strcmp(lpValue, "Path") == 0){
				printf("Note: ");
				printf("System Environment Variables");
				printf("\n");
				
			}
		}
		// For RegGetValueA only
		// RegGetValueW needs printfw
		printf(TEXT("Information \n"));
		printf("  hkey: "); 
		printf("     ");
		printf(registryHiveInterpretation(hkey));
		printf(" (value in Hexadecimal: 0x%x)", hkey);
		
		printf("\n");
		printf(TEXT("  lpSubKey: '%s' \n"), lpSubKey);
		printf(TEXT("  lpValue:  '%s' "), lpValue);
		printf("\n");
		
		// https://www.debugcn.com/en/article/32924235.html
		printf("  pdwType: ");
		printf("  ");
		printf(registryTypeValuesInterpretation(valueType));
		printf(" (Hex value: %i)", valueType);
		printf("\n");
		
		/*
		printf("https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-rprn/25cce700-7fcf-4bb6-a2f3-0f6d08430a55 \n");
		printf("Reference\\example: RRF_RT_REG_SZ: %i\n",RRF_RT_REG_SZ);
		*/
		if (ErrorCode != ERROR_SUCCESS)
			{
				if (ErrorCode == ERROR_FILE_NOT_FOUND){
					printf("\n");
					printf("\n");
					printf(TEXT("  [ERROR! on line %d]: \n "), __LINE__);
					printf(TEXT("  The Windows Registry Subkey:  \n "));
					printf(TEXT("    '%s' could not be opened or is not found. \n "), lpSubKey );
					printf(TEXT("  Error code: %x (ERROR_FILE_NOT_FOUND)"), ErrorCode);
					printf("\n");
					printf("\n");
					printf("TODO: implement open registry loop and check what registry location is not available");
					printf("\n");
					exit(0);
				} else {
					wprintf(L"Please consult the error https://docs.microsoft.com/en-us/windows/win32/debug/system-error-codes--0-499-. Error code: %x\n", lpSubKey, ErrorCode);
					
				}
			}
	
		// Allocate memory for the pvData in size of retrieved pcbData value
		printf("\n"); 
		printf("[!]%i bytes is the size of pvData\n", *pcbData); 
		printf("[!]Allocating heap memory pvData as retrieved by pcbData \n");
		pvData = (int*)malloc(*pcbData);
	
		printf("[!]Getting values of the Registry Key \n");
		ErrorCode = RegGetValueA(
				hkey,
				lpSubKey,
				lpValue,
				dwFlags,
				pdwType,
				pvData,
				pcbData
			);

		if (ErrorCode != ERROR_SUCCESS)
			{
				if (ErrorCode == ERROR_FILE_NOT_FOUND){
					printf(TEXT("  ERROR on line %d: The '%s' subkey could not be opened. Error code: %x\n"), __LINE__, lpSubKey, ErrorCode );
				} else {
					wprintf(L"Please consult the error https://docs.microsoft.com/en-us/windows/win32/debug/system-error-codes--0-499-. Error code: %x\n", lpSubKey, ErrorCode);
					
				}
			}
		printf("\n");
		printf("              [Registry Key Values]");
		printf("\n");
		printf("                  ----Output---");
		printf("\n");
		
		
		
		
		int tokensCounted = 0;
		char *ptr = pvData;
		while((ptr = strchr(ptr, ';')) != NULL) {
			tokensCounted++;
			ptr++;
		}		
		

		
		// https://wiki.sei.cmu.edu/confluence/display/c/STR06-C.+Do+not+assume+that+strtok%28%29+leaves+the+parse+string+unchanged
		char *copy = (char *)malloc(strlen(pvData) + 1);
		if (copy == NULL) {
		  /* Handle error */
		}
		strcpy(copy, pvData);
		char * token = strtok(copy, ";");
		int tokenCount = 0;
		for(tokenCount; token != NULL; token = strtok(NULL, ";"), tokenCount++){
			
			printf("%i", tokenCount);
			printf("   ");
		
			printf( "%s", token ); //printing each token
			if (tokenCount != tokensCounted-1){
				printf("\n");
			}
			
			
		}
		free(copy);
		copy = NULL;
		
		printf("\n");
		printf("\n");
		printf("                  ---");
		printf("-Debug---- \n");
		printf(pvData);
		printf("\n");
		printf("\nRetrieved information about array size: %i bytes.\n",*pcbData);
		printf("Actual size of the array: %i characters counted.\n", strlen(pvData));
		printf("Manually Allocated bytes for the array: %zu bytes.", sizeof(pvData));
		printf("\nSince malloc was used, the dynamic Allocation makes the Manual Allocation results strange.");
}


/* Predefined Registry Hives*/
// Enumeration is defined as the process of extracting user names, machine names, network resources, shares and services from a system.
// Manual enumeration https://docs.microsoft.com/en-us/windows/win32/sysinfo/enumerating-registry-subkeys
//Registry Hive values are to be found in tcc\include\winapi\winreg.h
// Microsoft also uses switch statements to translate HEX to String values: https://stackoverflow.com/questions/58510869/c-sharp-get-basekey-from-registrykey/58547945#58547945
const char* registryHiveInterpretation(hKey){
	switch ((int) hKey){
		case HKEY_CLASSES_ROOT:
			return "HKEY_CLASSES_ROOT";
			
		case HKEY_CURRENT_USER:
			return "HKEY_CURRENT_USER";
			
		case HKEY_LOCAL_MACHINE:
			return "HKEY_LOCAL_MACHINE";
			
		case HKEY_USERS:
			return "HKEY_USERS";
			
		case HKEY_CURRENT_CONFIG:
			return "HKEY_CURRENT_CONFIG";
		
		default:
			printf("Notice:");
			printf("Less expected hKey '0x%x', the known ones are: \n", hKey);
			printf(" HKEY_CLASSES_ROOT   | Integer value: 80000000 \n");
			printf(" HKEY_CURRENT_USER   | Integer value: 80000001 \n");
			printf(" HKEY_LOCAL_MACHINE  | Integer value: 80000002 \n");
			printf(" HKEY_USERS          | Integer value: 80000003 \n");
			printf(" HKEY_CURRENT_CONFIG | Integer value: 80000005 \n");
			printf("For more information consult: https://docs.microsoft.com/en-us/windows/win32/sysinfo/predefined-keys \n");
			return "";
			
	}

	
}

// https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-rprn/25cce700-7fcf-4bb6-a2f3-0f6d08430a55
// tcc\include\winapi\winnt.h
const char* registryTypeValuesInterpretation(valueType){
	switch (valueType){
		case REG_SZ:
			return "REG_SZ";
			
		case REG_DWORD:
			return "REG_DWORD";
			
		case 4207424:
			return "Unknown";
			
		default:
			return "Truly unknown value type";
	}
	
}

