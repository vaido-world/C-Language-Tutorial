// Pragmatic compiler instructions
// Pragmatic comment for advapi32 library.
#pragma comment(lib, "advapi32") // No more Need for "tcc -ladvapi32 RegGetValueA.c"         tcc: error: undefined symbol 'RegGetValueA' 

// Include directives.
#include <stdio.h>               // Prevents a warning:                  test.c:34: warning: implicit declaration of function 'printf'
#include <windows.h>             // Required, else an error:             test.c:12: error: 'DWORD' undeclared
#include <stdlib.h>              // Required, else a warning:            RegGetValue.c:62: warning: implicit declaration of function 'malloc'

/* 
Useful information 
   Function:  RegGetValueA
   Reference: https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-reggetvaluea
   
                                 //Required, else an error:               tcc: error: undefined symbol 'RegGetValueA'
 Proper compilation method/tutorial 
   tcc -impdef advapi32.dll -o .\lib\advapi32.def
   tcc -ladvapi32 RegGetValueAPathEnvironmentVariable.c
More Documentation
   System Path variable locations
   https://stackoverflow.com/questions/573817/where-are-environment-variables-stored-in-registry/573818#573818
   User Variables
   HKEY_CURRENT_USER\Environment
   System Variables
   Computer\HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Environment
*/



void main(){
	
	// Buffer Overflow
	// Notice: the program silently crashes if there are not enough characters allocated
	// https://docs.microsoft.com/en-us/windows/win32/seccrypto/retrieving-data-of-unknown-length
	// https://stackoverflow.com/questions/9137157/no-out-of-bounds-error/9137188#9137188
	// Allocating 2048 bytes = 2kb for the Path variable output
	//char value[2048];
	
	
	
	
	// Define the settings for the RegGetValueA function  
	// https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-reggetvaluea#parameters
	
	// Minimal Buffer size
	#define BUFFER  8192
	DWORD BufferSize = BUFFER;
	
	// Types Definitions for RegGetValue function arguments
	HKEY    hkey;
	LPCSTR  lpSubKey;
	LPCSTR  lpValue;
	DWORD   dwFlags;
	LPDWORD pdwType; 
	PVOID   pvData; 
	LPDWORD pcbData;
	
	// Retrieves the size of the pvData registry value and place it into pcbData variable
	RegGetValueA(
		hkey     = HKEY_LOCAL_MACHINE,
		lpSubKey = "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment",
		lpValue  = "Path",
		dwFlags  = RRF_RT_ANY,
		pdwType  = NULL, // This default NULL value, states that we do not request specific registry value type  https://docs.microsoft.com/en-us/windows/win32/sysinfo/registry-value-types
		pvData   = NULL, // This default NULL value, states that we do not request the data, only the Size of it
		pcbData  = &BufferSize
	);

	// Allocate memory for the size of retrieved pcbData value
	pvData = (int*)malloc(*pcbData);
	
	// Retrieve the content of the registry value and store it in pvData
	RegGetValueA(
		hkey,
		lpSubKey,
		lpValue,
		dwFlags,
		pdwType,
		pvData,
		pcbData
	);
	
	// Debug information
		printf("                  ---");
		printf("\n");
		printf("%s", pvData);
		printf("\n");
		printf("                  ---");
		printf("\n");
		printf("\nRetrieved information about array size: %i\n",*pcbData);
		printf("Actual size of the array: %i\n", strlen(pvData));
		printf("Manually Allocated bytes for the array: %zu", sizeof(pvData));
		printf("\nSince malloc was used, the dynamic Allocation makes the Manual Allocation results strange.");
		// https://stackoverflow.com/questions/37447198/dynamically-allocated-variable-size
	/*
	if (BufferSize >= strlen(value)) {
		printf("                  ---");
		printf("\n");
		printf("%s", value);
		printf("\n");
		printf("                  ---");
		printf("\n");
		printf("Retrieved information about array size: %i\n",BufferSize);
		printf("Actual size of the array: %i\n", strlen(value));
		printf("Manually Allocated bytes for the array: %i", sizeof(value));
	
	} else {
		
		// This is not going to be seen, 
		// the program crashes when the array is too small, therefore out of bounds
		// printf("Is too small");
	}
	*/
	// https://www.educative.io/edpresso/splitting-a-string-using-strtok-in-c
	// https://stackoverflow.com/questions/308695/how-do-i-concatenate-const-literal-strings-in-c/308712#308712

	// https://docs.microsoft.com/en-us/windows/win32/fileio/listing-the-files-in-a-directory
	// https://stackoverflow.com/questions/2314542/listing-directory-contents-using-c-and-windows/2315808#2315808
	
}
