// RecordList.h

#ifndef _RECORDLIST_h
#define _RECORDLIST_h
#include <iostream>
#include <string>

typedef enum { PERMANENT, BLINK, SHIFT } mode_type;
typedef struct record;
typedef struct record {
	mode_type mode;
	uint16_t delay;
	uint16_t period = 0;
	string text;
	uint16_t ptr = 0;
	record* next = NULL;
};

record* first = NULL;
record* current = NULL;
uint16_t list_length = 0;

uint16_t processLine(string line);
uint16_t addPermanent(string s);
uint16_t addBlink(string s);
uint16_t addShift(string s);
void listRecords(record* r);
void clearRecords();
uint16_t deleteRecord(uint16_t index, record* r);
void nextRecord();
uint16_t parseInt(string s, uint16_t start, uint16_t* end);


#endif

