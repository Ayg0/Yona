#include "tty.h"
#include "vga.h"
#include "mem.h"
extern _tty	mainTty;

/*
	- [X] init the Tty
	- [X] init Session
	- [X] Change Color
	- [X] add a character to the Session
	- [X] add a string to the Session
	- [X] clear the Tty Screen
	- [X] clear the StatusBar
	- [X] print the StatusBar content
	- [X] print the ScreenContent
	- [X] update StatusBar
	- [X] switch Session
*/
// inits the mainTty
void	initTty(){
	mainTty.sessionsNb = 5;
	mainTty.SelectedIndex = 0;
	mainTty.currentSession = &mainTty.Sessions[mainTty.SelectedIndex];
	mainTty.color = GET_COLOR(VGA_WHITE, VGA_BLACK);

	for (uint8_t i = 0; i < mainTty.sessionsNb; i++)
		initSession(mainTty.Sessions + i);
	updateStatusBar();
}
// Inits the Session
void	initSession(_ttySession *Session){
	Session->cursor.x = 0;
	Session->cursor.y = 0;
}
// change current Color, use -1 to keep the old value
void	changeTtyColor(int8_t fgColor, int8_t bgColor){
	if (fgColor == -1)
		fgColor = mainTty.color & 0xff00;
	else if (bgColor == -1)
		bgColor = mainTty.color & 0x00ff;
	mainTty.color = GET_COLOR(fgColor, bgColor);
}
// print the VGA cell to the VM
void	cellToVM(uint16_t cell, int16_t x, int16_t y){
	uint16_t	*videoMemory = (uint16_t *) VIDEO_MEMORY;

	videoMemory[x + y * 80] = cell;
}

void	ttyAddChar(uint8_t c){
	_pos		cursor = mainTty.currentSession->cursor;
	uint16_t	*sessionBuff = mainTty.currentSession->buff;

	if (cursor.x >= TTY_WIDTH)
		cursor.x = 0, cursor.y++;
	if (cursor.y >= TTY_HEIGHT)
		cursor.y = TTY_HEIGHT - 1; // fix You need to Scroll
	sessionBuff[cursor.x + cursor.y * 80] = GET_CHAR(c, mainTty.color);
	cellToVM(GET_CHAR(c, mainTty.color), cursor.x, cursor.y);
	cursor.x++;
	mainTty.currentSession->cursor = cursor;
}
// add char to Session (no Edit on Cursor nor preventing of errors)
void	ttyAddCharPos(uint8_t c, int16_t x, int16_t y){
	uint16_t	*sessionBuff = mainTty.currentSession->buff;

	sessionBuff[x + y * 80] = GET_CHAR(c, mainTty.color);
	cellToVM(GET_CHAR(c, mainTty.color), x, y);
}

void	ttyAddStr(uint8_t *s){
	while (*s)
	{
		ttyAddChar(*s);
		s++;
	}
}
// add str to Session (no Edit on Cursor nor preventing of errors)
void	ttyAddStrPos(uint8_t *s, int16_t x, int16_t y){
	uint16_t	*sessionBuff = mainTty.currentSession->buff;

	while (*s)
	{
		ttyAddCharPos(*s, x, y);
		s++;
		x++;
		if (x >= TTY_WIDTH)
			x = 0, y++;
		if (y >= TTY_HEIGHT)
			y = TTY_HEIGHT - 1;
	}
}

void clearTtySession(){
	uint16_t	*sessionBuff = mainTty.currentSession->buff;

	CLEAR_MEM(uint16_t, sessionBuff, 1920);		// clear buffer
	CLEAR_MEM(uint16_t, VIDEO_MEMORY, 1920);	// clear the screen
}

void clearTtyStatusBar(){

	CLEAR_MEM(uint16_t, mainTty.statusBar.buff, 80);
	CLEAR_MEM(uint16_t, VIDEO_MEMORY + 1920, 80);
}

void printTtySession(){
	uint16_t	*sessionBuff = mainTty.currentSession->buff;
	uint16_t	*videoMemory = (uint16_t *) VIDEO_MEMORY;

	for (uint16_t i = 0; i < 1920; i++)
		videoMemory[i] = sessionBuff[i];
}

void printTtyStatusBar(){
	uint16_t	*sessionBuff = mainTty.statusBar.buff;
	uint16_t	*videoMemory = (uint16_t *) VIDEO_MEMORY + 1920;

	for (uint16_t i = 0; i < 80; i++)
		videoMemory[i] = sessionBuff[i];
}

void	updateStatusBar(){
	char *s = "Selected Session: X";
	uint16_t *statusBar = mainTty.statusBar.buff;
	s[18] = mainTty.SelectedIndex + 48;

	while (*s)
	{
		*statusBar = GET_CHAR(*s, GET_COLOR(VGA_WHITE, VGA_BLACK));
		s++, statusBar++;
	}
	printTtyStatusBar();
}

void switchSession(uint8_t sessionIndex){
	if (sessionIndex >= mainTty.sessionsNb)
		return;
	mainTty.SelectedIndex = sessionIndex;
	mainTty.currentSession = &mainTty.Sessions[sessionIndex];
	updateStatusBar();
	printTtySession();
}