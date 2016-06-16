
#include "RecordList.h"
#include <iostream>
#include <string>
using namespace std;
uint16_t processLine(string line) {
	uint16_t ret_val = 0;
	if (line[0] != '@')
		return 0;
	switch (line[1])
	{
	case 'L':
		// List all records
		listRecords(first);
		ret_val = 1;
		break;
	case 'D':
		// Delete record with number
		uint16_t index = parseInt(line, 2, NULL);
		if (index < 1 || index > list_length)
			break;
		ret_val = deleteRecord(index, first);
		break;
	case 'C':
		// Clear all records
		clearRecords();
		break;
	case 'P':
		ret_val = addPermanent(line);
		break;
	case 'B':
		ret_val = addBlink(line);
		break;
	case 'S':
		break;
	default:
		return 0;
		break;
	}
}

uint16_t addPermanent(string s) {
	uint16_t end = 0;
	uint16_t delay = parseInt(s, 2, &end);
	if (delay == 0)
		return 0;
	record* n_rec = (record*)malloc(sizeof(record));
	n_rec->delay = delay;
	n_rec->mode = PERMANENT;
	n_rec->ptr = 0;
	n_rec->text = s.substr(end);
	n_rec->next = first;
	first = n_rec;
	return 3;
}

uint16_t addBlink(string s) {
	uint16_t end = 0;
	uint16_t delay = parseInt(s, 2, &end);
	uint16_t period = parseInt(s, end, &end);
	if (delay == 0 || period == 0)
		return 0;
	record* n_rec = (record*)malloc(sizeof(record));
	n_rec->delay = delay;
	n_rec->period = period;
	n_rec->mode = BLINK;
	n_rec->ptr = 0;
	n_rec->text = s.substr(end);
	n_rec->next = first;
	first = n_rec;
	return 4;
}

uint16_t addShift(string s) {
	uint16_t end = 0;
	uint16_t delay = parseInt(s, 2, &end);
	uint16_t period = parseInt(s, end, &end);
	if (delay == 0 || period == 0)
		return 0;
	record* n_rec = (record*)malloc(sizeof(record));
	n_rec->delay = delay;
	n_rec->period = period;
	n_rec->mode = SHIFT;
	n_rec->ptr = 0;
	n_rec->text = s.substr(end);
	n_rec->next = first;
	first = n_rec;
	return 5;
}

void listRecords(record* r) {
	if (r == NULL) {
		Serial.println("List empty.");
	}
	while (r != NULL) {
		Serial.println(r->text);
		r = r->next;
	}
}

void clearRecords() {
	uint16_t count = 0;
	record* to_delete = NULL;
	current = NULL;
	while (first != NULL) {
		to_delete = first;
		first = first->next;
		Serial.println(to_delete->text);
		count++;
		free(to_delete);
	}
	Serial.print(count);
	Serial.println(" records deleted. List empty.");
}

uint16_t deleteRecord(uint16_t index, record* r) {
	if (r == NULL)
		return 0;
	record* to_delete = NULL;
	if (index == 1) {
		to_delete = first;
		first = first->next;
	}
	else {
		while (index > 2 && r != NULL) {
			index--;
			r = r->next;
		}
		if (r != NULL && r->next != NULL) {
			to_delete = r->next;
			r->next = r->next->next;
		}
	}
	if (to_delete == NULL)
		return 0;
	Serial.println(to_delete->text);
	if (current == to_delete) {
		nextRecord();
	}
	free(to_delete);
	return 2;
}

void nextRecord() {
	if (current == NULL)
		return;
	if (current->next == NULL) {
		current = first;
	}
	else {
		current = current->next;
	}
}


uint16_t parseInt(string s, uint16_t start, uint16_t* end) {
	uint16_t i_num = 0;
	for (uint16_t i = start; i < s.length(); i++)
	{
		uint16_t c = s[i];
		if (c >= '0' && c <= '9') {
			i_num *= 10;
			c = c - '0';
			i_num + c;
		}
		else {
			if (end != NULL)
				*end = i;
			break;
		}
	}
	return i_num;
}
