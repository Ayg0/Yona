#include "tty.h"
#include "vga.h"
#include "mem.h"
#include "kLibStd.h"
#include "timer.h"
#include "serialio.h"

extern _tty	tty;
extern uint8_t direction;

// inits the tty
void	initTty(){
	tty.sessionsNb = 5;
	tty.SelectedIndex = 0;
	tty.color = GET_COLOR(VGA_WHITE, VGA_BLACK);
	tty.currentSession = &tty.Sessions[tty.SelectedIndex];

	for (uint8_t i = 0; i < tty.sessionsNb; i++)
		initSession(tty.Sessions + i);
	updateStatusBar();
	enable_cursor(0, 31);
}
// Inits the Session
void	initSession(_ttySession *Session){
	Session->cursor.x = 0;
	Session->cursor.y = 0;	
}
// change current Color, use -1 to keep the old value
void	changeTtyColor(int8_t fgColor, int8_t bgColor){
	if (fgColor == -1)
		fgColor = tty.color & 0x0f;
	else if (bgColor == -1)
		bgColor = tty.color & 0xf0;
	tty.color = GET_COLOR(fgColor, bgColor);
}

void	scrollSession(){
	uint16_t	*sessionBuff = tty.currentSession->buff;

	for (uint16_t i = 0; i < (TTY_HEIGHT - 1); i++)
		memcpy(&sessionBuff[i * 80], &sessionBuff[(i + 1) * 80], 160);
	CLEAR_MEM(uint16_t, tty.currentSession->buff + 1840, 80);
	CLEAR_MEM(uint16_t, VIDEO_MEMORY + 1840, 80);
	printTtySession();
}

void	setTtyCursor(int16_t x, int16_t y){
	if (x >= TTY_WIDTH)
		x = 0, y++;
	if (y >= TTY_HEIGHT){
		scrollSession();
		y = TTY_HEIGHT - 1;
	}
	tty.currentSession->cursor.x = x;
	tty.currentSession->cursor.y = y;
	update_cursor(x, y); // updates the VGA curson and not just my imaginary one
}
// print the VGA cell to the VM
void	cellToVM(uint16_t cell, int16_t x, int16_t y){
	uint16_t	*videoMemory = (uint16_t *) VIDEO_MEMORY;

	videoMemory[x + y * 80] = cell;
}

void	ttyAddChar(uint8_t c){
	uint16_t	*sessionBuff = tty.currentSession->buff;
	_pos		cursor = tty.currentSession->cursor;

	if (c == '\r')
		return setTtyCursor(0, cursor.y);
	else if (c == '\n')
		return setTtyCursor(cursor.x, cursor.y + 1);
	sessionBuff[cursor.x + cursor.y * 80] = GET_CHAR(c, tty.color);
	cellToVM(GET_CHAR(c, tty.color), cursor.x, cursor.y);
	setTtyCursor(cursor.x + 1, cursor.y);
}
// add char to Session (no Edit on Cursor nor preventing of errors)
void	ttyAddCharPos(uint8_t c, int16_t x, int16_t y){
	uint16_t	*sessionBuff = tty.currentSession->buff;

	sessionBuff[x + y * 80] = GET_CHAR(c, tty.color);
	cellToVM(GET_CHAR(c, tty.color), x, y);
}

void	ttyAddStr(char *s){
	while (*s)
	{
		ttyAddChar(*s);
		s++;
	}
}
// add str to Session (no Edit on Cursor nor preventing of errors)
void	ttyAddStrPos(uint8_t *s, int16_t x, int16_t y){
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

void	ttyAddNbr(uint32_t a, uint32_t len, char *base){
	if (a >= len)
		ttyAddNbr((a / len), len, base);
	ttyAddChar(base[a % len]);
}

void clearTtySession(){
	uint16_t	*sessionBuff = tty.currentSession->buff;

	CLEAR_MEM(uint16_t, sessionBuff, 1920);		// clear buffer
	CLEAR_MEM(uint16_t, VIDEO_MEMORY, 1920);	// clear the screen
	setTtyCursor(0, 0);
}

void clearTtyStatusBar(){

	CLEAR_MEM(uint16_t, tty.statusBar.buff, 80);
	CLEAR_MEM(uint16_t, VIDEO_MEMORY + 1920, 80);
}

void printTtySession(){
	uint16_t	*sessionBuff = tty.currentSession->buff;
	uint16_t	*videoMemory = (uint16_t *) VIDEO_MEMORY;

	for (uint16_t i = 0; i < 1920; i++)
		videoMemory[i] = sessionBuff[i];
}

void printTtyStatusBar(){
	uint16_t	*sessionBuff = tty.statusBar.buff;
	uint16_t	*videoMemory = (uint16_t *) VIDEO_MEMORY + 1920;

	for (uint16_t i = 0; i < 80; i++)
		videoMemory[i] = sessionBuff[i];
}
void	updateStatusBar(){
	static char s[80];
	char *str = s;
	uint16_t *statusBar = tty.statusBar.buff;
	_time date = getDate();
	formatString("Session: %u ", &str, tty.SelectedIndex + 1);
	uint8_t i = 0;
	while (s[i]){
		statusBar[i] = GET_CHAR(s[i], GET_COLOR(VGA_WHITE, VGA_BLACK));
		i++;
	}
	formatString("%2u/%2u/%4u %2u:%2u:%2u", &str, date.d, date.mo, date.y,\
	 date.h, date.m, date.s);
	statusBar += 61;
	i = 0;
	while (s[i]){
		statusBar[i] = GET_CHAR(s[i], GET_COLOR(VGA_WHITE, VGA_BLACK));
		i++;
	}

	if (direction)
		str = " <*_*>";
	else
		str = "<*_*> ";
	i = 0;
	statusBar = tty.statusBar.buff;
	statusBar += 36;
	while (str[i]){
		statusBar[i] = GET_CHAR(str[i], GET_COLOR(VGA_WHITE, VGA_BLACK));
		i++;
	}
	printTtyStatusBar();
}

void switchSession(uint8_t sessionIndex){
	if (sessionIndex >= tty.sessionsNb)
		return;
	tty.SelectedIndex = sessionIndex;
	tty.currentSession = &tty.Sessions[sessionIndex];
	updateStatusBar();
	printTtySession();
	setTtyCursor(tty.currentSession->cursor.x, tty.currentSession->cursor.y);
}