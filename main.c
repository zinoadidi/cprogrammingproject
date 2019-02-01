#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void runGame();
int executeTurn();

void handleUpdates();
void displayJamesState();
int handleEncounters();
void handleJamesChoice();
void saveGame();
void loadGame();

char * getRoomText();
char * getAvailableDirections();
int handleMonsterEncounter();
int getJamesChoice();
void addJamesLives();
int handleCombatChoice();
int checkIfMonsterAttacks();
void escapeAttempt(int *combatEnded);
void attackAttempt(int *combatEnded);

//Room coordinates are x,y,
const int JamesX = 1;
const int JamesY = 0;
const int JamesLives = 100;

struct James {
    int XLocation;
    int YLocation;
    int lives;
};

static struct James James;

static int healingRegen = 0;
static int turns = 0;
static int monsterLives = 100;

static char directionNames[4][10]  = {
        "(u)p",
        "(r)ight",
        "(d)own",
        "(l)eft"
};
// x  y  direction
static int availableDirections[3][3][4]  = {
        //Definition for a room's available directions: Up, Right, Down, Left. 0 - blocked, 1 - available
        {  //x=0
                { 1, 1, 0, 0}, //y=0
                { 1 ,1, 1, 0}, //y=1
                { 0, 1, 1, 0}, //y-2
        },{ //x=1
                { 1, 1, 0, 1}, //y=0
                { 1, 1, 1, 1}, //y=1
                { 0, 1, 1, 1}, //y-2
        },{ //x=2
                { 1, 0, 0, 1}, //y=0
                { 1, 0, 1, 1}, //y=1
                { 0, 0, 1, 1}, //y-2
        }
};

static char roomTexts[3][3][200] = {
        {
                {"You see a volcano blocking your path to the left.\nYou can go:\n"},
                {"You Can go:\n"},
                {"You Can go:\n"},
        },
        {
                {"You find yourself in the land of dinosaurs. You Can go:\n"},
                {"You Can go:\n"},
                {"You Can go:\n"},
        },
        {
                {"You Can go:\n"},
                {"You Can go:\n"},
                {"You you see a lake filled with Spinosaurus. You Can go:\n"},
                
        }
};
//Main Method
int main()
{
    //Initialize James base stats
    James.XLocation = JamesX;
    James.YLocation = JamesY;
    James.lives = JamesLives;
    //enter Game loop
    monsterLives = (JamesLives/5) * 7 + (monsterLives/6);
    runGame();  
    return 0;
}


void runGame(){
    int isGameOver = 0;
    //While the game is not over, execute loop
    while(!isGameOver){
        isGameOver = executeTurn();
    }
}

int executeTurn(){
    int gameOver;
    //incement turn counter
    turns++;
    //handle any uptades, that have to happen each turn
    handleUpdates();
    //output details about James's status: lives, location, etc.
    displayJamesState();
    //handle a random encounter, and check whether it caused the game to end
    gameOver = handleEncounters();
    if(gameOver > 0){
        
        return 1;
    }
    //give James a choice for further exploration
    handleJamesChoice();
    return 0;
}

void handleUpdates(){
    //decrement the amount of turns left until healing is available
    if(healingRegen > 0){
        healingRegen--;
    }
}

void displayJamesState(){
    char* roomText;
    //get room description text
    roomText = getRoomText();
    printf("\n\n\n\n");
    printf("\nJames location: X: %d Y: %d",James.XLocation,James.YLocation);
    printf("\nJames live: %d",James.lives);
    printf("\nCurrent turn: %d \n",turns);
    printf("\n%s",roomText);
    printf("\n\n");
}


//Decide any encounters that should happen in the given room.
int handleEncounters(){
    switch(James.XLocation){
        case 0:
            switch(James.YLocation){
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    break;
            }
            break;
        case 1:
            switch(James.YLocation){
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    break;
            }

            break;
        case 2:
            switch(James.YLocation){
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    //Healing station in X:2 Y:2
                    addJamesLives();
                    break;
            }
            break;
    }

    if(checkIfMonsterAttacks()){
        return handleMonsterEncounter();
    }
    return 0;
}

void handleJamesChoice(){
    printf("(s)ave\nl(o)ad\n(e)nd and save\n(a)bout game\n");
    int choice = getJamesChoice();
    switch(choice){
        //u - up
        case 117:
            if(availableDirections[James.XLocation][James.YLocation][0]){
                printf("\nYou chose to go up.\n\n");
                James.YLocation++;
            } else{
                printf("\nYou cannot go there");
            }

            break;
            //r - right
        case 114:
            if(availableDirections[James.XLocation][James.YLocation][1]){
                printf("\nYou chose to go right.\n\n");
                James.XLocation++;
            } else{
                printf("\nYou cannot go there");
            }
            break;
            //d - down
        case 100:
            if(availableDirections[James.XLocation][James.YLocation][2]){
                printf("\nYou chose to go down.\n\n");
                James.YLocation--;
            } else{
                printf("\nYou cannot go there");
            }
            break;
            //l - left
        case 108:
            if(availableDirections[James.XLocation][James.YLocation][3]){
                printf("\nYou chose to go left.\n\n");
                James.XLocation--;
            } else{
                printf("\nYou cannot go there");
            }
            break;
        case 115:
            saveGame();
            break;
        case 111:
            loadGame();
            break;
        case 101:
        //save and exit
            saveGame();
            exit(0);
            break;
        case 97:
            // about game
            printf("James wakes up at night after a bad day to lights shinning from his window\nHe moves towards his window and finds a time portal spinning mid air.\n He Jumps into this portal and finds himself back in time.");
            printf("\n Press enter .....");
            getchar();
            break;
            
        default:
            printf("Unknown choice %d", choice);
    }
    getchar();

}

int getJamesChoice(){
    char selected;
    char* availableDirections;
    availableDirections  = getAvailableDirections();
    printf("\n%s\nWhere will you go? Choose wisely:  ",availableDirections);
    scanf(" %c", &selected);
    /// convert uppercase to lower case
    //selected = tolower(selected);    
    return (int)selected;
}

char * getRoomText(){
    static char fullString[240];
    strcpy(fullString,"");
    strcat(fullString, roomTexts[James.XLocation][James.YLocation]);
    return fullString;
}

char * getAvailableDirections(){
    static char availableDirectionString[40];
    strcpy(availableDirectionString,"");
    //Iterate through all directions, and if the value is 1, add to the James output
    for(int i = 0; i < 4; i++){
        if(availableDirections[James.XLocation][James.YLocation][i]){
            strcat(availableDirectionString, directionNames[i]);
            strcat(availableDirectionString,"\n");
        }
    }
    return availableDirectionString;
}


int checkIfMonsterAttacks(){
    int randomResult;
    time_t t;
    srand((unsigned) time(&t));
    randomResult = rand() % 10;
    //simplest implementation of random in c
    //values 5-9 will produce a monster attack
    if(randomResult > 4){
        return 1;
    } else {
        return 0;
    }
}

int handleMonsterEncounter(){
    printf("A Carnotaurus appears in front of you.\n");
    printf("He has %d lives left\n", monsterLives);
    printf("If you choose to run, you have a 3/6 chance 10 health. \n");
    printf("If you choose to attack, you have a 5/6 chance to do 20 damage. \n");
    return handleCombatChoice();
}


void addJamesLives(){
    if(healingRegen == 0){
        printf("You feel your life essence regenerated. +12 lives.\n");
        James.lives += 12;
        healingRegen += 5;
    } else {
        printf("A Velociraptor stole the health portions... Wait for %d turns \n",healingRegen);
    }
}

int handleCombatChoice(){
    char selected;
    int combatEnded;
    combatEnded = 0;
    printf("\n\nDo you: (r)un or (a)ttack\n");
    //Pass the MEMORY ADDRESS of the selected value to the scanf function
    scanf(" %c", &selected);
    switch(selected){
        case 'r':
            //Pass the reference to the Combat ended variable
            escapeAttempt(&combatEnded);
            break;
        case 'a':
            attackAttempt(&combatEnded);
            break;
        default:
            printf("Invalid option.\n");
            combatEnded = 0;
    }

    if(James.lives <= 0){
        printf("You died!\nGame over\n");
        printf("try again....(press enter)");
        getchar();
        main();
    }
    if(monsterLives <= 0){
        printf("You killed the monster!\n\n");
        printf("The Journey Continues....(press enter)");
        getchar();
        main();
    }
    if(!combatEnded){
        return handleCombatChoice();
    }
    return 0;
}
//accept a pointer to a variable
void escapeAttempt(int *combatEnded){
    int randomResult;
    time_t t;
    srand((unsigned) time(&t));
    randomResult = rand() % 6;
    if(randomResult > 1){
        printf("You have succeeded in running away from the ferocious beast without harm!\n");
        //assign a value to the variable.
        *combatEnded = 1;
    }else{
        printf("You try to run, but the Dino's know their crib.\nYou lose 10 lives\n");
        James.lives -= 12;
        *combatEnded = 1;
    }
}

void attackAttempt(int *combatEnded){
    int randomResult;
    time_t t;
    srand((unsigned) time(&t));
    randomResult = rand() % 6;
    if(randomResult > 1){
        printf("You have succeeded in harming a rare breed of dinosaur!\n");
        monsterLives -= 20;
        *combatEnded = 0;
    }else{
        printf("You try to hit, but creature of mass distruction is too strong for mortals.\nYou lose 20 lives\n");
        James.lives -= 20;
        *combatEnded = 0;
    }
    printf("Monster lives %d\n", monsterLives);
    printf("James lives %d\n\n\n",James.lives);
}


void saveGame(){
    printf("\ngame saved!\n");
    
    FILE *fp;

    fp = fopen("lostintime.txt", "w+");

    fprintf(fp,"%d %d %d %d %d %d", James.XLocation, James.YLocation, James.lives, healingRegen, turns, monsterLives);
    fclose(fp);
    turns -=1;

}

void loadGame(){
    printf("\ngame loaded!\n");
    
    FILE *fp;

    fp = fopen("lostintime.txt", "r");

    fscanf(fp,"%d %d %d %d %d %d", &James.XLocation, &James.YLocation, &James.lives, &healingRegen, &turns, &monsterLives);
    fclose(fp);
    turns -=1;
    
}