#pragma once
# include "drivers/vga/textMode/vgaTextMode.h"
# include "arch/serial.h"
# include "yonaData.h"

typedef uint8_t (*putCharFnc)(char);
extern _yonaData    yona;

typedef struct buff
{
	char		*buff;
	uint32_t	index;
}_buff;


# define COLOR_BLACK        "\033[30m"
# define COLOR_BLUE         "\033[34m"
# define COLOR_GREEN        "\033[32m"
# define COLOR_CYAN         "\033[36m"
# define COLOR_RED          "\033[31m"
# define COLOR_MAGENTA      "\033[35m"
# define COLOR_BROWN        "\033[33m"
# define COLOR_LIGHT_GREY   "\033[37m"
# define COLOR_DARK_GREY    "\033[90m"
# define COLOR_LIGHT_BLUE   "\033[94m"
# define COLOR_LIGHT_GREEN  "\033[92m"
# define COLOR_LIGHT_CYAN   "\033[96m"
# define COLOR_LIGHT_RED    "\033[91m"
# define COLOR_LIGHT_MAGENTA "\033[95m"
# define COLOR_YELLOW       "\033[93m"
# define COLOR_WHITE        "\033[97m"
# define COLOR_DEFAULT      "\033[39m"



uint8_t appendsprintBuff(char c);
void    setSprintBuff(char *buff);
int     print(putCharFnc fnc, char *fmtString, ...);

// printf clone: Uses the VGA.
#define PRINT_K(FMT_STRING, ...) print(kPutC, FMT_STRING, __VA_ARGS__)
// printf clone: Uses the serial Communication.
#define PRINT_SC(FMT_STRING, ...) print(SPutC, FMT_STRING, __VA_ARGS__)
// sprintf clone:
#define SPRINTF(BUFF, FMT_STRING, ...) do{\
    setSprintBuff(BUFF);\
    print(appendsprintBuff, FMT_STRING, __VA_ARGS__);\
    setSprintBuff(NULL);\
} while (0)

// Prints a string to the serial Communication and updates the state of the yonaData.
#define S_INFO(FMT_STRING, ...)    print(SPutC, "\033[34mINFO\033[39m [%s] " FMT_STRING, __func__, __VA_ARGS__)

#define S_SUCC(FMT_STRING, ...)    do {\
                                        print(SPutC, "\033[92mSUCC\033[39m [%s] " FMT_STRING, __func__, __VA_ARGS__);\
                                        setYonaStatus(YONA_STATUS_STABLE);\
                                    } while (0)
#define S_ERR(FMT_STRING, ...)     do {\
                                        print(SPutC, "\033[31mERR\033[39m [%s] " FMT_STRING, __func__, __VA_ARGS__);\
                                        setYonaStatus(YONA_STATUS_ERROR);\
                                    } while (0)
#define S_DEBUG(FMT_STRING, ...)   print(SPutC, "\033[93mDEBUG\033[39m [%s] " FMT_STRING, __func__, __VA_ARGS__)

// Prints a string to the VGA doesn't update the state of the yonaData.
#define K_INFO(FMT_STRING, ...)    print(kPutC, "\033[34mINFO\033[39m [%s] " FMT_STRING, __func__, __VA_ARGS__)
#define K_SUCC(FMT_STRING, ...)    print(kPutC, "\033[92mSUCC\033[39m [%s] " FMT_STRING, __func__, __VA_ARGS__)
#define K_ERR(FMT_STRING, ...)     print(kPutC, "\033[31mERR\033[39m [%s] " FMT_STRING, __func__, __VA_ARGS__)
#define K_DEBUG(FMT_STRING, ...)   print(kPutC, "\033[93mDEBUG\033[39m [%s] " FMT_STRING, __func__, __VA_ARGS__)