#pragma once
#include <Windows.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>


#if !defined(HTAPI_API)
#if defined(BUILDING_HTAPI)
#define HTAPI_API __declspec(dllexport)
#else
#define HTAPI_API __declspec(dllimport)
#endif
#endif


#define SIZE 512


	struct HTAPI_API HTHANDLE       
	{
		HTHANDLE();
		HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[SIZE]);
		int     Capacity;                     
		int     SecSnapshotInterval;         
		int     MaxKeyLength;              
		int     MaxPayloadLength;          
		char    FileName[SIZE];             
		HANDLE  File;                          
		HANDLE  FileMapping;                      
		LPVOID  Addr;                           
		char    LastErrorMessage[SIZE];          
		time_t  lastsnaptime;                 

		HANDLE  snapshotThread;             
		volatile LONG isRunning;                
		HANDLE  hMutex;                   

	};

	struct HTAPI_API Element     
	{
		Element();
		Element(const void* key, int keylength);                                               
		Element(const void* key, int keylength, const void* payload, int  payloadlength);      
		Element(Element* oldelement, const void* newpayload, int  newpayloadlength);           
		~Element();
		const void* key;                    
		int             keylength;         
		const void* payload;               
		int             payloadlength;     
	};

	extern "C" {
		HTAPI_API HTHANDLE* Create                   
		(
			int	  Capacity,					     
			int   SecSnapshotInterval,		       
			int   MaxKeyLength,                   
			int   MaxPayloadLength,               
			const char  FileName[SIZE]             
		); 	      

		HTAPI_API HTHANDLE*  Open                     
		(
			const char    FileName[SIZE]            
		); 	      

		HTAPI_API BOOL  Snap           
		(
			const HTHANDLE* hthandle               
		);


		HTAPI_API BOOL  Close                 
		(
			HTHANDLE* hthandle               
		);	        


		HTAPI_API BOOL  Insert          
		(
			const HTHANDLE* hthandle,              
			const Element* element               
		);	      


		HTAPI_API BOOL  Delete          
		(
			const HTHANDLE* hthandle,               
			const Element* element                
		);	      

		HTAPI_API Element*  Get          
		(
			const HTHANDLE* hthandle,              
			const Element* element                
		); 	      


		HTAPI_API BOOL  Update          
		(
			const HTHANDLE* hthandle,             
			const Element* oldelement,               
			const void* newpayload,				    
			int   newpayloadlength				   
		); 	      

		HTAPI_API char*  HT_GetLastError       
		(
			HTHANDLE* ht                           
		);

		HTAPI_API void  Print                                  
		(
			const Element* element                
		);

	}