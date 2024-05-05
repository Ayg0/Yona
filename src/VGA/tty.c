# include "tty.h"

extern tty mainTty;

void initTty(tty *tty){
	tty->nbrOfSessions = 5;
	for (uint8_t i = 0; i < tty->nbrOfSessions; i++){
		tty->selectedSession = &tty->AvailableSessions[i];
		tty->selectedSession->cursor.x = 0;
		tty->selectedSession->cursor.y = 0;
	}
	tty->Color = GET_COLOR(VGA_WHITE, VGA_BLACK);
	selectSession(0);
}

void setTtyCursor(int32_t x, int32_t y){
	if (x >= TMWIDTH)
		x = 0, y += 1;
	if (y >= TMHEIGHT)
		y = TMHEIGHT - 1;
	mainTty.selectedSession->cursor.x = x;
	mainTty.selectedSession->cursor.y = y;
}

void updateStatusBar(uint8_t sessionIndex){
	uint16_t i;
	char *s = "Selected Session: X";

	s[18] = sessionIndex + 48;
	for (i = 0; *s; i++, s++)
		mainTty.StatusBar.buff[i] = *s;
	mainTty.StatusBar.buff[i] = 0;
}

void selectSession(uint8_t sessionIndex){
	mainTty.selectedSIndex = sessionIndex;
	mainTty.selectedSession = &mainTty.AvailableSessions[sessionIndex];
	updateStatusBar(sessionIndex);
	printSBarContent();
}
// no cursor Change :) 
void kprintTtyC(char c, uint8_t x, uint8_t y, uint8_t color){
	uint16_t *vMemory = (uint16_t*)VIDEO_MEMORY;

    vMemory[x + y * TMWIDTH] = GET_CHAR(c, color);
}
// print a VGA character cell (16 bit), changesThe Cursor
void kprintTtyVC(uint16_t	vgaCell){
	pos cursor	= mainTty.selectedSession->cursor;
	uint16_t *vMemory = (uint16_t*)VIDEO_MEMORY;

	if (cursor.x >= TMWIDTH)
        cursor.x = 0, cursor.y += 1;
    vMemory[ cursor.x + cursor.y * TMWIDTH] = vgaCell;
	mainTty.selectedSession->cursor = cursor;
}

void	clearTty(){
	for (uint8_t i = 0; i < TMHEIGHT; i++)
		for (uint8_t j = 0; j < TMWIDTH; j++)
			kprintTtyC(' ', j, i, 0);
}

void	printSBarContent(){
	char *statusBar = mainTty.StatusBar.buff;

	for (uint16_t i = 0; *statusBar; i++, statusBar++){
		kprintTtyC(*statusBar, i, 24, GET_COLOR(VGA_WHITE, VGA_BLACK));
	}
}

void	printSessionContent(){
	uint8_t *screenBuff = (uint8_t *)mainTty.selectedSession->sessionBuff;
	clearTty();
	for (uint8_t i = 0; i < TMHEIGHT; i++)
	{
		for (uint8_t j = 0; j < TMWIDTH; j++)
		{
			kprintTtyC(screenBuff[0], j, i, screenBuff[1]);
			screenBuff += 2;
		}
	}
}
// cp a string to the screen buffer
void	sToSession(char *s, uint8_t x, uint8_t y){
	uint16_t *sessionBuffer = (uint16_t *)mainTty.selectedSession->sessionBuff;

	for (uint16_t i = x + y * 80; *s; i++, s++)
		sessionBuffer[i] = GET_CHAR(*s, mainTty.Color);
}
// set new colors to the tty, using -1 keeps the old values
void	setTtyColor(int8_t foregroundColor, int8_t backgroundColor){
	if (foregroundColor == -1)
		foregroundColor = mainTty.Color & 0x00ff;
	else if (backgroundColor == -1)
		backgroundColor = mainTty.Color & 0xff00;
	mainTty.Color = GET_COLOR(foregroundColor, backgroundColor);
}