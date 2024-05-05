# pragma once

# include "vga.h"


typedef struct _pos{
    int32_t x;
    int32_t y;
} pos;
// Text Mode Screen
typedef struct _TMSession{
    // pos         WByH;                   // specify width and height of the Session
    pos         cursor;                 // cursor inside the Session
    uint16_t    sessionBuff[1920];       // buffer to keep note of all characters inside the Session (1920 Max)
} TMSession;
// info bar at the buttom
typedef struct _statusBar
{
    uint8_t    buff[80];
} statusBar;
// the whole screen setup
typedef struct _tty
{
    TMSession	AvailableSessions[5];	// max 5 Sessions :( WAITING FOR ALLOCATION
    TMSession	*selectedSession;
    uint8_t		selectedSIndex;
    uint32_t	nbrOfSessions;
	uint8_t		Color;
	statusBar	StatusBar;
} tty;


void	clearTty();
void	initTty(tty *Tty);
void	printSessionContent();
void	kprintTtyVC(uint16_t	vgaCell);
void	kprintTtyC(char c, uint8_t x, uint8_t y, uint8_t color);
void	selectSession(uint8_t sessionIndex);
void	setTtyCursor(int32_t x, int32_t y);
void	sToSession(char *s, uint8_t x, uint8_t y);
void	setTtyColor(int8_t foregroundColor, int8_t backgroundColor);
void	printSBarContent();
