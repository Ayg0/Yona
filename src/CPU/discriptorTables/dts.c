#include "CPU/DiscriptorTables.h"

void	initGdt();
void	initIdt();

void	initDTs(){
	initGdt();
	initIdt();
}