void kernel_main(){
	char* video_memory = (char*) 0xb8000;

	*video_memory = 'H';
	*(video_memory + 2*1) = 'e';
	*(video_memory + 2*2) = 'l';
	*(video_memory + 2*3) = 'l';
	*(video_memory + 2*4) = 'o';
	*(video_memory + 2*5) = ',';
	*(video_memory + 2*6) = ' ';
	*(video_memory + 2*7) = 'W';
	*(video_memory + 2*8) = 'o';
	*(video_memory + 2*9) = 'r';
	*(video_memory + 2*10) = 'l';
	*(video_memory + 2*11) = 'd';
	*(video_memory + 2*12) = '!';
}