#include "klibc/types.h"
#include "klibc/strings.h"
#include "klibc/print.h"
#include "drivers/time.h"
#include "drivers/keyboard.h"
#include "klibc/converts.h"
#include "klibc/random.h"
#include "yonaShell/yonaShell.h"

# define GRID_W 78
# define GRID_H 22
# define MAX_FOOD_COUNT 5
# define MAX_FOOD_BAR 15
# define FOOD_DIFFICULTY_MODIFIER 5

typedef enum {
    HEAD = '$',
    BODY = '#',
    EMPTY = ' ',
    H_WALL = '_',   // horizontal Wall
    V_WALL = '|',   // vertical Wall
    ALPHA = '.',    // Small Food
    ORAYN = '@',    // Big Food
    EGG = '0',      // Big Bonus Food
} snakeGameElement;

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NUN,
} direction;

typedef struct pos
{
    int16_t x;
    int16_t y;
}_pos;

typedef struct bodyPart
{
    _pos        pos;
    direction   dir;
    bool    isHead;
} _bodyPart;

typedef struct gameData {
    uint8_t     grid[GRID_H][GRID_W + 1];
    uint8_t     isRunning;
    uint32_t    score;
    _bodyPart   snakeBody[GRID_H * GRID_W];
    uint32_t    snakeSize;
    uint32_t    foodCount;
    uint32_t    gameSpeed;
    uint8_t     difficulty;
    uint8_t     foodBar;
    uint8_t     lastDirection;
    _pos        currentEggPos;
} _gameData;

_gameData gd;

_pos    generateFood(bool generateBonusFlag){
    _pos    foodPos;
    uint8_t x, y;

    do {
        x = rand() % GRID_W;
        y = rand() % GRID_H;
    } while (gd.grid[y][x] != EMPTY);

    foodPos.x = x;
    foodPos.y = y;
    gd.foodCount++;
    if (generateBonusFlag){
        gd.grid[y][x] = EGG;
        gd.currentEggPos.x = x;
        gd.currentEggPos.y = y;
        gd.foodBar = MAX_FOOD_BAR;
        return foodPos;
    }
    gd.grid[y][x] = (rand() % 2) ? ALPHA : ORAYN;
    return foodPos;
}

void    initPositions(void){
    for (uint8_t i = 0; i < GRID_H; i++) {
        for (uint8_t j = 0; j < GRID_W; j++) {
            // init grid positions
            if ((!j || (j == GRID_W - 1)) && i)
                gd.grid[i][j] = V_WALL;
            else if (!i || i == GRID_H - 1)
                gd.grid[i][j] = H_WALL;
            else
                gd.grid[i][j] = EMPTY;
            // snake body Positions
            gd.snakeBody[i * j].pos.x = -1;
            gd.snakeBody[i * j].pos.y = -1;
        }
        gd.grid[i][GRID_W] = '\0'; // to form a string
    }
}

void    displayGameInfo(void){
    char   info[50] = {0};

    SPRINTF(info, COLOR_GREEN"Score: %d"COLOR_DEFAULT, gd.score);
    moveCursor(1, 23);
    PRINT_K("%s", info);

    if (gd.foodBar){
        moveCursor((VGA_WIDTH - MAX_FOOD_BAR) / 2, 23);
        for (uint8_t i = 0; i < gd.foodBar; i++)
            PRINT_K(COLOR_RED"|"COLOR_DEFAULT, 0);
    }

    SPRINTF(info, COLOR_YELLOW"Difficulty: %d"COLOR_DEFAULT, gd.difficulty + 1);
    moveCursor(VGA_WIDTH - strlenIgnoreAnsii(info), 23);
    PRINT_K("%s", info);
}

extern _ttySession tty;

void    displayGame(void){
    clearScreenBuffer();
    displayGameInfo();
    moveCursor(0, 1);
    for (uint8_t i = 0; i < GRID_H - 1; i++)
        PRINT_K(" %s\n\r", gd.grid[i]);
    PRINT_K(" %s", gd.grid[GRID_H - 1]);
}

void    setHeadDirection(direction dr){
    bool    dontInverse;
    
    dontInverse = (gd.lastDirection == UP && dr == DOWN) || (gd.lastDirection == DOWN && dr == UP) ||
                (gd.lastDirection == LEFT && dr == RIGHT) || (gd.lastDirection == RIGHT && dr == LEFT);

    if (!dontInverse)
        gd.snakeBody[0].dir = dr;
}

_pos    getNextPos(_bodyPart part){
    _pos    position = part.pos;

    switch (part.dir) {
    case LEFT:
        position.x -= 1;
        break;
    case RIGHT:
        position.x += 1;
        break;
    case UP:
        position.y -= 1;
        break;
    case DOWN:
        position.y += 1;
        break;
    default:
        break;
    }

    return position;
}

void    setBodyPart(uint8_t x, uint8_t y, uint8_t isHead, direction dir){
    gd.snakeBody[gd.snakeSize].pos.x = x;
    gd.snakeBody[gd.snakeSize].pos.y = y;
    gd.snakeBody[gd.snakeSize].isHead = isHead;
    gd.snakeBody[gd.snakeSize].dir = dir;
    gd.grid[y][x] = isHead ? HEAD : BODY;
    gd.snakeSize++;
}

void    addBodyPart(){
    _bodyPart   lastPart = gd.snakeBody[gd.snakeSize - 1];
    _pos        newPos = lastPart.pos;

    switch (lastPart.dir) {
    case RIGHT:
        newPos.x -= 1;
        break;
    case LEFT:
        newPos.x += 1;
        break;
    case UP:
        newPos.y += 1;
        break;
    case DOWN:
        newPos.y -= 1;
        break;
    default:
        break;
    }
    setBodyPart(newPos.x, newPos.y, 0, lastPart.dir);
}


void    handleFood(snakeGameElement element){
    uint32_t    toSub;
    static uint8_t  smallFoodCount;
    static uint32_t foodAte;

    switch (element)
    {
    case ALPHA:
        gd.score += 10;
        smallFoodCount++;
        if (smallFoodCount == 3)
            addBodyPart(), smallFoodCount = 0;
        break;
    case ORAYN:
        gd.score += 30;
        addBodyPart();
        break;
    case EGG:
        gd.score += 50;
        addBodyPart();
        addBodyPart();
        gd.foodBar = 0;
    default:
        break;
    }
    gd.foodCount--;
    if (gd.snakeSize % FOOD_DIFFICULTY_MODIFIER == 0){
        toSub =  30 / gd.difficulty;
        if (gd.gameSpeed > toSub){
            gd.gameSpeed -= toSub;
            gd.difficulty++;
        }
        displayGame();
    }
    foodAte++;
    if (foodAte == 5){
        generateFood(1);
        foodAte = 0;
    }
}


void    moveSnake(){
    _pos    newPos;
    snakeGameElement    element;
    static uint32_t     lastUpdate = 0;

    if (gd.snakeBody[0].dir == NUN)
        return ;

    if (!lastUpdate){
        lastUpdate = getMsElapsed();
        return ;
    }

    if (getMsElapsed() - lastUpdate < gd.gameSpeed)
        return ;

    gd.foodBar -= gd.foodBar ? 1: 0; 

    if (gd.foodBar == 1){
        gd.foodBar = 0;
        gd.grid[gd.currentEggPos.y][gd.currentEggPos.x] = ' ';
        gd.currentEggPos.x = -1;
        gd.currentEggPos.y = -1;
    }

    lastUpdate = getMsElapsed(); // Update lastUpdate after the time check
    // move the snake
    for (uint32_t i = 0; i < gd.snakeSize; i++)
    {
        if (gd.snakeBody[i].dir == NUN)
            gd.snakeBody[i].dir = gd.snakeBody[i - 1].dir;
        newPos = getNextPos(gd.snakeBody[i]);
        element = gd.grid[newPos.y][newPos.x];
        if (i == 0 && (element == BODY || element == H_WALL || element == V_WALL)){
            gd.isRunning = 0;
            return ;
        }
        if (element == ALPHA || element == ORAYN || element == EGG)
            handleFood(element);
        gd.grid[newPos.y][newPos.x] = gd.grid[gd.snakeBody[i].pos.y][gd.snakeBody[i].pos.x];
        gd.grid[gd.snakeBody[i].pos.y][gd.snakeBody[i].pos.x] = EMPTY;
        gd.snakeBody[i].pos.x = newPos.x;
        gd.snakeBody[i].pos.y = newPos.y;
    }

    // update direction of the parts
    for (uint32_t i = gd.snakeSize - 1; i > 0; i--)
        gd.snakeBody[i].dir = gd.snakeBody[i - 1].dir;
    gd.lastDirection = gd.snakeBody[0].dir;
}

void    inputFunc(uint8_t letter){
    switch (letter) {
    case 'a':
        setHeadDirection(LEFT);
        break;
    case 'w':
        setHeadDirection(UP);
        break;
    case 's':
        setHeadDirection(DOWN);
        break;
    case 'd':
        setHeadDirection(RIGHT);
        break;
    default:
        break;
    }
}

void    initGameData(){
    gd.score = 0;
    gd.snakeSize = 0;
    gd.isRunning = 1;
    gd.foodCount = 0;
    gd.gameSpeed = 250;
    gd.difficulty = 1;
    gd.foodBar = 0;
    gd.currentEggPos.x = -1;
    gd.currentEggPos.x = -1;
    gd.lastDirection = NUN;
    setBodyPart(10, 10, 1, NUN);
    gd.grid[gd.snakeBody[0].pos.y][gd.snakeBody[0].pos.x] = HEAD;
}

bool	gameHelpers(char *args){

	if (strcmp(args, "--help") == 0)
        return PRINT_K("it's the snake game, for more info use --rules\r\n", 0);

	if (strcmp(args, "--rules") == 0){
        PRINT_K("********** SNAKE GAME RULES **********\r\n", 0);
        PRINT_K("-> The game is simple, you control a snake and you need to eat food to grow.\r\n", 0);
        PRINT_K("-> The game will end if you hit the wall or the snake body.\r\n", 0);
        PRINT_K("-> The game speed will increase every %d food eaten.\r\n", FOOD_DIFFICULTY_MODIFIER);
        PRINT_K("-> '%c' are the smaller version of food you need 3 of them to grow\r\n", ALPHA);
        PRINT_K("-> '%c' are the bigger version of food you need 1 of them to grow\r\n", ORAYN);
        PRINT_K("-> '%c' are the bonus food you need 1 of them to grow 2 body elements.\r\n", EGG);
        PRINT_K("-> HAVE FUN!\r\n", 0);
        PRINT_K("********************************\r\n", 0);
        return 1;
    }
    return 0;
}

void	snakeGame(char *args){

    if (gameHelpers(args))
    	return ;
    initPositions();
    initGameData();
    displayGame();
    setKeyPressHandler(inputFunc);
    disableCursor();
    while (gd.isRunning != 0){
        msSleep(50);
        displayGame();
        moveSnake();
        if (gd.foodCount < MAX_FOOD_COUNT)
            generateFood(0);
    }
    clearScreenBuffer();
    enableCursor(14, 15);
    PRINT_K("\r\n\r\n********** GAME OVER **********\r\n\r\n", 0);
    PRINT_K("       Your score: %d", gd.score);
    PRINT_K("\r\n\r\n*******************************\r\n\r\n", 0);
    setKeyPressHandler(NULL);
}