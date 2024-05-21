#pragma once

# include "types.h"
# define period_ms uint32_t

void	setSound(uint32_t frequency);
void	nosound();
void	playSound(uint32_t fr, period_ms time);