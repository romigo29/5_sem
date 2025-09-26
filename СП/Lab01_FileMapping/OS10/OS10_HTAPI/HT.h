#pragma once
#include "pch.h" 
#define SIZE 512

namespace HT      
{

	struct HTHANDLE       
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
	};

	struct Element     
	{
		Element();
		Element(const void* key, int keylength);                                               
		Element(const void* key, int keylength, const void* payload, int  payloadlength);      
		Element(Element* oldelement, const void* newpayload, int  newpayloadlength);           
		const void* key;                    
		int             keylength;         
		const void* payload;               
		int             payloadlength;     
	};

	HTHANDLE* Create                   
	(
		int	  Capacity,					     
		int   SecSnapshotInterval,		       
		int   MaxKeyLength,                   
		int   MaxPayloadLength,               
		const char  FileName[SIZE]             
	); 	      

	HTHANDLE* Open                     
	(
		const char    FileName[SIZE]            
	); 	      

	BOOL Snap           
	(
		const HTHANDLE* hthandle               
	);


	BOOL Close                 
	(
		const HTHANDLE* hthandle               
	);	        


	BOOL Insert          
	(
		HTHANDLE* hthandle,              
		const Element* element               
	);	      


	BOOL Delete          
	(
		HTHANDLE* hthandle,               
		const Element* element                
	);	      

	Element* Get          
	(
		const HTHANDLE* hthandle,              
		const Element* element                
	); 	      


	BOOL Update          
	(
		HTHANDLE* hthandle,             
		const Element* oldelement,               
		const void* newpayload,				    
		int   newpayloadlength				   
	); 	      

	char* GetLastError       
	(
		HTHANDLE* ht                           
	);

	void print                                  
	(
		const Element* element                
	);
};
