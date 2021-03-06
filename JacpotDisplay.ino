#include "RecordList.h"
#define DDR_DS DDRA
#define DDR_SS DDRC
#define PORT_DS PORTA
#define PORT_SS PORTC
#define DS 8
#define BS 1024
#define DELAY 1000


unsigned long delay_timer = 0;
unsigned long delay_period = 0;

String help = "Jacpot display\n@_COMMAND/MODE_DELAY[1s]/INDEX_,_[PERIOD[100ms]]_,_TEXT_NewLine\n\
			   COMMAND/MODE - L_list, D_delete, C_clear, P_add-permanent, B_add-blink, S_add-shift\n\
			   DELAY/INDEX  - How long text stay on display or delete index\n\
			   PERIOD       - State change time for blink and shift\n";

uint8_t chars[] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
					0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x91, 0x5, 0x0, 0x0, 0x0, 0x0, 0x4,
					0x66, 0x33, 0x0, 0x0, 0x40, 0x8, 0x80, 0x49, 0x77, 0x11, 0x6B, 0x3B, 0x1D, 0x3E, 0x7E, 0x17, 0x7F, 0x3F, 0x0, 0x0,
					0x60, 0x28, 0x30, 0xCB, 0x0, 0x5F, 0x7C, 0x68, 0x79, 0x6E, 0x4E, 0x76, 0x5C, 0x10, 0x71, 0x5E, 0x64, 0x57, 0x58, 0x78,
					0x4F, 0x1F, 0x48, 0x36, 0x6C, 0x70, 0x75, 0x7D, 0x5D, 0x3D, 0x63, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5F, 0x7C, 0x68,
					0x79, 0x6E, 0x4E, 0x76, 0x5C, 0x10, 0x71, 0x5E, 0x64, 0x57, 0x58, 0x78, 0x4F, 0x1F, 0x48, 0x36, 0x6C, 0x70, 0x75, 0x7D,
					0x5D, 0x3D, 0x63, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
					0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
					0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
					0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
					0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
					0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
					0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

bool d_state = false;
void setup() {
	pinMode(13, OUTPUT);
	DDR_DS = 0xFF;
	DDR_SS = 0xFF;
	PORT_DS = 0xFF;
	PORT_SS = 0xFF;
	Serial.begin(115200);

	for (size_t i = 0; i < 5; i++)
	{
		digitalWrite(13, d_state);
		delay(500);
		d_state = !d_state;
	}
}

void loop() {
	update();
	if (Serial.available() > 0) {
		String s = Serial.readStringUntil('\n');
		if (s.length() > 1) {
			uint16_t ret_val = processLine(s);
			if (ret_val == 0) {
				Serial.println("Syntax Error");
				Serial.print(help);
			}
		}
		else {
			Serial.println("Syntax Error");
			Serial.print(help);
		}

	}
	update();
}

void update() {
	for (int16_t i = 0; i < DS; i++)
	{
		PORT_DS = 0x00;
		PORT_SS = chars[text[i]];
		PORT_DS = 0x1 << i;
		delay(1);
	}
}



void slideText() {
	b_start = (b_start + 1) % b_length;
	b_end = (b_start + DS) % b_length;
	
	if (b_start < b_end) {
		memcpy(text, &buffer[b_start], DS);
	}
	else {
		uint16_t part_size = b_length - b_start;
		memcpy(text, &buffer[b_start], part_size);
		memcpy(&text[part_size], &buffer[0], DS - part_size);
	}
}
