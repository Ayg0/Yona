#include "types.h"
#include "ports.h"
#include "CPU/DiscriptorTables.h"

#define BACK_SPACE_KEY	'B'
#define CTRL_KEY		'C'
#define ALT_KEY			'A'
#define CAPS_LOCK_KEY	'C'
#define INSERT_KEY		'E'
#define DELETE_KEY		'D'
#define LEFT_SHIFT_KEY	'L'
#define RIGHT_SHIFT_KEY	'R'

enum MODIFIER_KEYS {
	SHIFT_MOD,
	CTRL_MOD,
	CAPS_MOD,
	NUMLOCK_MOD,
};

typedef struct keyboardFlags
{
	uint8_t	nl;
	uint8_t	alt;
	uint8_t	modifiers;
} _keyboardFlags;

# define SET_BIT(FLAG, BIT) (FLAG = FLAG | 1 << BIT)
# define RESET_BIT(FLAG, BIT) (FLAG = FLAG & ~(1 << BIT))
# define IS_SET(FLAG, BIT) (FLAG & (1 << BIT))
# define CAP_OPT(MODIFIERS) (MODIFIERS & (1 << SHIFT_MOD | 1 << CAPS_MOD))

void	keyboardHandler(registers Rs);