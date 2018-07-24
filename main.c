#include <stdio.h>
#include <stdlib.h>
#define __STRINGLENGTH__ 100
#define MAX_CORN_SUPPORTED 100

// Logic. Corn structure definition and global array.
struct Corn 
{
    char color;
    int priceInCents;
    int age; // Anything over 30 days is assumed to be bad.
    int lengthInCM;
};

struct Corn corns[MAX_CORN_SUPPORTED];
int currentCorns = 0;

/*
Operations
*/

int getChoice(int minimum, int maximum, int attempt){
	int choice;
	printf("Enter any number between %d and %d.\n", minimum, maximum);
	scanf("%d", &choice);
	if(attempt >= 5){
		printf("Error! User unable to input the correct choice. Layer 8 issue. Exiting");
		exit(1);
	}
	if(choice < minimum || choice > maximum){
		printf("Invalid choice!");
		return getChoice(minimum, maximum, attempt+1);
	}
	return choice;
}

void readFileToMem(){
	// Does our file exist?
	FILE* cornStorage;
	int piecesOfCorn;
	// an izisulu is a place to store maize on a farm.
	cornStorage = fopen("izisulu.bin", "rb");
	if(cornStorage == NULL){
		printf("Error! File does not exist!\n");
		cornStorage = fopen("izisulu.bin", "wb");
		printf("Creating, please re-preform this operation.\n");
	}else{
		fscanf(cornStorage, "%d\n", &piecesOfCorn);
	}
	currentCorns = piecesOfCorn;
	for(piecesOfCorn=0; piecesOfCorn<currentCorns; piecesOfCorn++){
		fscanf(cornStorage, "%c %d %d %d\n", &corns[piecesOfCorn].color, &corns[piecesOfCorn].priceInCents, &corns[piecesOfCorn].age, &corns[piecesOfCorn].lengthInCM);
	}
	fclose(cornStorage);
}

void writeSpecificCorn(int i, char color, int price, int age, int length){
	FILE* cornStorage;
	// an izisulu is a place to store maize on a farm.
	cornStorage = fopen("izisulu.bin", "ab");
	// fprintf(cornStorage, "%d\n", currentCorns);
	printf("(Printing) %c %d %d %d\n", corns[i].color, corns[i].priceInCents, corns[i].age, corns[i].lengthInCM);
	fprintf(cornStorage, "%c %d %d %d\n", corns[i].color, corns[i].priceInCents, corns[i].age, corns[i].lengthInCM);
	fclose(cornStorage);
}

void writeToFile(){
	int i;
	FILE* cornStorage;
	cornStorage = fopen("izisulu.bin", "wb");
	
	fprintf(cornStorage, "%d\n", currentCorns);
	fclose(cornStorage);
	
	for(i = 0; i < currentCorns; i++){
		writeSpecificCorn(i,corns[i].color, corns[i].priceInCents, corns[i].age, corns[i].lengthInCM);
	}
}

void pause(){
	system("pause");
}

/*
Drawing Related.
draw - writes a string to screen, pads with standard spacing.
draw - writes a character to screen the amount of standard spacing.
drawMiddle - writes a string to middle, pads with standard spacing on either side.
*/
void draw(char* string, int length){
	int i, j;
	// Print out each character in string before index 100.
	for(i = 0; i < length && j < __STRINGLENGTH__; i++, j++){
		printf("%c", string[i]);	
	}
	// We could accomplish this with printflags, but the printf standard can/will vary by console.
	for(i = length; i < __STRINGLENGTH__; i++){
		printf(" ");
	}	
	printf("\n");
}

void drawLetter(char string){
	int i = 0;
	for(i = 0; i < __STRINGLENGTH__; i++)
		printf("%c",string);
	printf("\n");
}

void drawMiddle(char* string, int length, char padding){
	int i;
	for(i = 0; i < (__STRINGLENGTH__/2) - (length/2); i++){
		printf("%c", padding);
	}
	printf("%s", string);
	for(i = 0; i < (__STRINGLENGTH__/2) - (length/2) && length%2==0; i++){
		printf("%c", padding);
	}
	for(i = 0; i < (__STRINGLENGTH__/2) - (length/2)-1 && length%2==1; i++){
		printf("%c", padding);
	}
	printf("\n");
}

/*
Draws and Logic.
Prints each menu and submenu, taking user feedback as appropriate.
*/

void drawInventory(){
	// Ensure reading is updated.
	readFileToMem();
	// Iterate through the stored corn.
	int i;
	for(i = 0; i < currentCorns; i++){
		printf("%08d. 	%c 		$%d.%02d 		%d cm 		%d days old.\n", i+1, corns[i].color, corns[i].priceInCents/100, corns[i].priceInCents%100, corns[i].lengthInCM, corns[i].age);
	}
	pause();
}

void addInventory(){
	struct Corn tempCorn;
	// First, let's make sure everything is current.
	readFileToMem();
	currentCorns++;
	// Are we going to surpass our maximum amount of corn?
	if(MAX_CORN_SUPPORTED <= currentCorns){
		// We've eclipsed supported storage.
		printf("(Warning) Can not add corn due to maximum corn limit reached.\n");
		pause();
		return;
	}
	// Great, now let's add our new corn.
	// Let's make a temporary corn!
	printf("Please enter the corn's color code.\n");
	scanf(" %c", &tempCorn.color);
	printf("Please enter the corn's price in cents.\n");
	scanf("%d", &tempCorn.priceInCents);
	printf("Please enter the corn's age in days.\n");
	scanf("%d", &tempCorn.age);
	printf("Please enter the corn's length in CM.\n");
	scanf("%d", &tempCorn.lengthInCM);
	corns[currentCorns-1] = tempCorn;
	pause();
	// Flush
	writeToFile();
}

void modInventory(){
	printf("\n");
}

void skuLookup(){
	int i, sku;
	readFileToMem();
	printf("Please enter desired SKU.\n");
	scanf("%d", &sku);
	printf("SKU    			COLOR         PRICE		AGE  	     LENGTH\n");
	for(i = 0; i < currentCorns; i++){
		if(i==sku){
			printf("%08d		%c	 	%d		 %d		 %d\n", sku, corns[i].color, corns[i].priceInCents, corns[i].age, corns[i].lengthInCM);
		}
	}
	system("pause");
}

void salesHistory(){
	
}

void checkPerson(){
	
}

void specificSearch(){
	
}


void drawMainMenu(){
	system("cls");
	// Let's make a greeting.
	drawMiddle("", 0, '#');
	drawMiddle("   Welcome to the Corn Hub!   ", 30, '#');
	drawMiddle("", 0, '#');
	// Present User with their options
	printf("\n");
	drawMiddle("", 0, '#');
	drawMiddle("                     1. View Inventory                           ", 65, '#');
	drawMiddle("                     2. Modify Invetory                          ", 65, '#');
	drawMiddle("                     3. Lookup by SKU                            ", 65, '#');
	drawMiddle("                     4. View Sales History                       ", 65, '#');
	drawMiddle("                     5. Checkout Person                          ", 65, '#');
	drawMiddle("                     6. Lookup by Artifact                       ", 65, '#');
	drawMiddle("                     7. Exit The Program                         ", 65, '#');
	drawMiddle("", 0, '#');
	int choice = getChoice(1,7,1);
	switch(choice){
		case 1:
			drawInventory();
			break;
		case 2:
			modInventory();
			break;
		case 3:
			skuLookup();
			break;
		case 4:
			salesHistory();
			break;
		case 5:
			checkPerson();
			break;
		case 6: 
			specificSearch();
			break;
		case 7:
			exit(0);				
	}
	return;
}


int main(int argc, char *argv[]) {
	while(1){
		drawMainMenu();
	}
}

