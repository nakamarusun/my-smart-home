#pragma once

#include <Arduino.h>
#include <SSD1306Wire.h>

#include "util.h"

namespace LCD {
	const uint8_t splashLogo[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 
	0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xC0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x07, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 
	0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x3F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x7F, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xF0, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x0F, 0xE0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0xE0, 0x01, 0xF8, 0x01, 0x00, 
	0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0xC0, 
	0x03, 0x8C, 0x01, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x30, 0x00, 0xC0, 0x03, 0x8C, 0xF9, 0x87, 0x8F, 0x77, 0xFE, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0xC0, 0x07, 0x1C, 0xB0, 0x0D, 
	0x18, 0xCE, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x80, 
	0x07, 0xF8, 0xB0, 0x0D, 0x18, 0x06, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x06, 0x00, 0x80, 0x07, 0xC0, 0xB1, 0x8D, 0x1F, 0x06, 0x18, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x80, 0x07, 0x8C, 0xB1, 0xCD, 
	0x18, 0x06, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x80, 
	0x07, 0x8C, 0xB1, 0xCD, 0x1C, 0x06, 0x18, 0x01, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x01, 0x00, 0x80, 0x07, 0xFC, 0xB8, 0x9D, 0xBB, 0x3F, 0xF0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x80, 0x07, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0xC0, 
	0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x00, 0x00, 0xC0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 
	0x80, 0x07, 0x1E, 0x00, 0x80, 0x00, 0x00, 0xC0, 0x03, 0xFE, 0x00, 0x00, 
	0x00, 0x00, 0x0C, 0x00, 0x00, 0x06, 0x18, 0x00, 0x80, 0x0F, 0x00, 0xE0, 
	0x03, 0x8C, 0x01, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x06, 0x18, 0x00, 
	0x00, 0xF0, 0x01, 0xE0, 0x03, 0x0C, 0xE3, 0x83, 0x8F, 0x77, 0xEC, 0xE0, 
	0x03, 0x06, 0x18, 0x00, 0x80, 0x0F, 0x06, 0xE0, 0x03, 0x0C, 0x33, 0xC6, 
	0x18, 0xCE, 0x9C, 0x31, 0x06, 0x06, 0x18, 0x00, 0x80, 0x79, 0x1C, 0xF0, 
	0x01, 0x0C, 0x1B, 0x6C, 0x30, 0x06, 0x0C, 0x1B, 0x0C, 0x06, 0x18, 0x00, 
	0x00, 0xC3, 0x71, 0xF8, 0x01, 0x0C, 0x1B, 0x6C, 0x30, 0x06, 0x0C, 0xFB, 
	0x0F, 0x06, 0x18, 0x00, 0x00, 0x06, 0xC7, 0xFC, 0x01, 0x0C, 0x1B, 0x6C, 
	0x30, 0x06, 0x0C, 0x1B, 0x00, 0x06, 0x18, 0x00, 0x00, 0x0C, 0xBF, 0xFD, 
	0x00, 0x8C, 0x31, 0xC6, 0x18, 0x06, 0x9C, 0x31, 0x0C, 0x06, 0x18, 0x00, 
	0x00, 0x38, 0x7F, 0xFE, 0x00, 0xFE, 0xE0, 0x83, 0x8F, 0x3F, 0xEE, 0xE0, 
	0xC7, 0x3F, 0xFF, 0x00, 0x00, 0xE0, 0xFE, 0x7C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x79, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x3E, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x03, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
};
	SSD1306Wire display(0x3C, SDA_PIN, SCL_PIN);
	String displayText = "An Attest";
	bool visible = false;
	long onUntil;
	const long onDuration = 10000;

	// Update lcd dengan teks dari flash memory.
	void updateLcd() {
		// camera_fb_t* frame = NULL;
        // frame = esp_camera_fb_get();
        
		display.displayOn();
		display.clear();
		display.setTextAlignment(TEXT_ALIGN_LEFT);
		display.setFont(ArialMT_Plain_16);
		Serial.println("Writing to LCD..");
		display.drawStringMaxWidth(0, 0, 128, displayText);
		display.display();

		// Setel LCDnya agar mati nanti.
		visible = true;
		onUntil = millis() + onDuration;
        // esp_camera_fb_return(frame); // TEST WITH THIS PLEASE
	}

	void loadLcdText() {
		displayText = Util::readFileAsString("/message");
	}

	void lcdLoop() {
		if (visible && (onUntil < millis())) {
			visible = false;
			display.displayOff();
		}
	}
};