# pragma once
# include "types.h"
# include "keyboard.h"

# define TTY_WIDTH	80
# define TTY_HEIGHT	24	// -1 for the status bar;

typedef struct pos
{
	int16_t	x;
	int16_t	y;
} _pos;


typedef struct ttySession
{
	uint16_t	buff[1920]; // 80 by 24
	_kbdBuffer	kbdBuffer;
	_pos		cursor;
} _ttySession;

typedef struct statusBar
{
	uint16_t	buff[80];
} _statusBar;

typedef struct tty
{
	_ttySession	Sessions[5];		// max Sessions are 5 no malloc :)
	_ttySession	*currentSession;
	uint8_t		SelectedIndex;		// currently selected Session Index
	uint8_t		sessionsNb;	// number of available Sessions
	_statusBar	statusBar;
	uint8_t		color;
}_tty;


// inits
void	initTty();
void	initSession(_ttySession *Session);

// Session Helpers
void	ttyAddChar(uint8_t c);
void	ttyAddCharPos(uint8_t c, int16_t x, int16_t y);
void	ttyAddStr(char *s);
void	ttyAddStrPos(uint8_t *s, int16_t x, int16_t y);
void	ttyAddNbr(uint32_t a, uint32_t len, char *base);
void	switchSession(uint8_t sessionIndex);

// Screen Helpers
void	clearTtySession();
void	clearTtyStatusBar();
void	printTtySession();
void	printTtyStatusBar();
void	updateStatusBar();
// miscellaneous
void	cellToVM(uint16_t cell, int16_t x, int16_t y);
void	changeTtyColor(int8_t fgColor, int8_t bgColor);
void	setTtyCursor(int16_t x, int16_t y);
// Printfs
void	printfTty(const char *FmtS, ...);

