#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define pileSize 7
struct node{
    struct node *next;
    char facedown;
    char card[3];
};


typedef struct node node;

node * piles[pileSize];
node * current = NULL;
node * last = NULL;

char deckArray[52][3];

void initialize();
void loadDeck();
void freeMem();
void append(node * head, char newCard[2]);
int moveCards(int from, int to, char card[2]);
void printTable();
void printColoumnNames();
void printColoumn(node * head);
int readFile(char filename[20]);
int checkCardMove(char cardTo[2], char cardFrom[2]);
node* getLastCard(node * head);


int main() {




    initialize();
    //printTable();


    //printColoumn(piles[0]);
    //puts("\n");
    //append(piles[0], "HK");
    //current = getLastCard(piles[1]);


    char str[50] = "";
    char opcode[3] = "";
    char filename[50];
    char errmsg[50] = "";
    int res = 0;
    char debug = 1;
    int fromColoumn, toColoumn;
    char cardToMove[3] = "";
    while(1){

        printTable();
        printf("LAST Command: %s\n", str);
        printf("Message: %s\n", errmsg);
        printf("INPUT > ");
        sprintf(errmsg, "%s", "");

        gets(str);

        //printf("You entered: %s, length is %lu\n", str, strlen(str));


        opcode[0] = str[0];
        opcode[1] = str[1];
        //printf("Opcode: %s\n", opcode);
        if (strcmp("LD", opcode) == 0){
            if (str[2] != ' '){
                initialize();
                sprintf(errmsg, "%s", "Loaded unshuffled deck");
                continue;

            }
            strncpy(filename,  &str[3], strlen(str)-3);
            filename[strlen(str)-3] = '\0';

            res = readFile(filename);
            if(!res){
                //puts("File does not exist\n");
                sprintf(errmsg, "%s", "File not found");
                continue;
            }
            freeMem();
            loadDeck();
            sprintf(errmsg, "%s", "Loaded chosen deck");

        } else if(opcode[0] == 'C'){
            fromColoumn = ((int) opcode[1]) - 1 - 0x30 ;
            if(str[2] == ':'){
                cardToMove[0] = str[3];
                cardToMove[1] = str[4];
                if(str[7] != 'C'){
                    sprintf(errmsg, "%s", "Illegal move");
                    continue;
                }
                toColoumn = ((int) str[8]) - 1 - 0x30;
            }else{
                strcpy(cardToMove, "");
                if(str[4] != 'C'){
                    sprintf(errmsg, "%s", "Illegal move");
                    continue;
                }
                toColoumn = ((int) str[5]) - 1 - 0x30;
            }

            res = moveCards(fromColoumn, toColoumn, cardToMove);
            if(!res){
                sprintf(errmsg, "%s", "Illegal move");
                continue;
            }
        }else if(strcmp("QQ", opcode) == 0){
            freeMem();
            break;
        }






    }






    return 0;
}

void initialize(){
    int res = readFile("unShuffledDeck.txt");
    if (!res){
        puts("File does not exist");
        return;
    }
    loadDeck();
}


void loadDeck(){
    int coloumnSize = 1, deckCounter = 0;

    for (int i = 0; i < pileSize; ++i) {
        deckCounter = i;    // Decides which index the coloumn starts on, for the deck array
        for (int j = 0; j < coloumnSize; ++j) {

            node * card = (node*) malloc(sizeof (node));

            if(j+5 >= coloumnSize){
                card->facedown = 0;
                //strcpy(card->card, "HH");
                sprintf(card->card, "%s", deckArray[deckCounter]);
            }else{
                card->facedown = 1;
                //strcpy(card->card, "HH");
                sprintf(card->card, "%s", deckArray[deckCounter]);
            }

            if(j == 0){
                piles[i] = card;
                last = card;
            }else{
                last->next = card;
                last = card;
            }
            card->next = NULL;

            if (j >= 5){
                deckCounter += ((pileSize-1)-(j-(pileSize-3)));
            }else{
                deckCounter += pileSize-1;
            }
        }
        if (i == 0){
            coloumnSize+=5;
        }else{
            coloumnSize++;
        }

    }

    //printTable();
}

void freeMem(){
    node *temp = NULL;
    for (int i = 0; i < pileSize; ++i) {
        current = piles[i];
        while (current !=NULL){
            temp = current;
            current = current->next;
            free(temp);
        }
    }
}

void append(node * head, char newCard[2]){
    node * new_node = (node*) malloc(sizeof(node));
    //strcpy(new_node->card, newCard);
    sprintf(new_node->card, "%s",newCard);

    if(head == NULL){
        head = new_node;
        return;
    }
    else{
        while(head->next != NULL){
            head = head->next;
        }
        head->next = new_node;
        return;
    }

}



node* getLastCard(node * head){
    if(head == NULL){
        return NULL;
    }
    else{

        while(head->next != NULL){

            head = head->next;

        }

        return head;
    }
}




int moveCards(int from, int to, char card[3]){
    node * temp = NULL;
    node * fromPtr = NULL;
    node * toPtr = NULL;


    toPtr = getLastCard(piles[to]);


    fromPtr = piles[from];
    if (piles[from]->next == NULL){

        if(!checkCardMove(toPtr->card, fromPtr->card)){
            //puts("Illegal move");
            return 0;
        }
        piles[from] = NULL;
    }

    if(fromPtr == NULL){
        puts("No card to move");
        return 0;
    }
    if (fromPtr->next == NULL){
        temp = fromPtr;
    }
    else{

        if (strcmp(card, "") == 0){
            while(fromPtr->next != NULL){
                temp = fromPtr;
                fromPtr = fromPtr->next;

            }
            if(!checkCardMove( ((toPtr == NULL) ? "##" : toPtr->card), fromPtr->card)){
                //puts("Illegal move");
                return 0;
            }
            temp->next = NULL;
            temp = fromPtr;
        }else{
            if(!checkCardMove(toPtr->card, card)){
                //puts("Illegal move");
                return 0;
            }
            node * cardMatch = NULL;
            while(fromPtr->next != NULL){
                if (strcmp(card, fromPtr->card) == 0){
                    cardMatch = fromPtr;
                    break;
                }
                temp = fromPtr;
                fromPtr = fromPtr->next;

            }


            if(cardMatch == NULL){
                puts("Card to move not found i coloumn");
                return 0;
            }

        }

    }


    if(temp->facedown == 1){
        temp->facedown = 0;
    }
    if(piles[to] == NULL){
        piles[to] = fromPtr;
        temp->next = NULL;
        //printTable();
        return 1;
    }
    toPtr->next = fromPtr;
    temp->next = NULL;
    //printTable();

}

int checkCardMove(char cardTo[3], char cardFrom[3]) {
    if(cardFrom[1] == cardTo[1]){
        return 0;
    }
    if(cardTo[0] =='#'){
        if(cardFrom[0] != 'K'){
            return 0;
        }
        return 1;
    }
    char arr[14] = "A123456789TJQK";
    for (int i = 0; i < strlen(arr); ++i) {
        if(cardTo[0] == arr[i]){
            if(cardFrom[0] != arr[i-1] ){
                return 0;

            }
            return 1;
        }
    }
    return 0;
}

void printTable(){

    printColoumnNames();
    int i = 0;
    int cardCounter = 0;
    int fCounter = 1;
    char rowCheck = 0;
    while (1){
        rowCheck = 0;
        for (int j = 0; j < pileSize; ++j) {
            cardCounter = 0;
            current = piles[j];


            while (current != NULL && cardCounter < i){
                cardCounter++;
                current = current->next;
            }
            if(current == NULL){
                printf("  \t");
                continue;
            }
            rowCheck = 1;
            printf("%s\t", (current->facedown) ? "[]" : current->card);

        }
        if(i < 7 && i % 2 == 0){
            printf("\t[]\tF%d\n", fCounter);
            fCounter++;
        }else{
            puts("");
        }
        if (!rowCheck && i >= 7){
            break;
        }


        i++;

    }


}
int readFile(char filename[20]){
    FILE* ptr;
    char ch[5];
    ptr = fopen(filename, "r");

    if (NULL == ptr) {
        //puts("file can't be opened \n");
        return 0;
    }

    //printf("content of this file are \n");
    int i = 0;
    while (fgets(ch, 5, ptr) != NULL) {
        //printf("%s", ch);
        memcpy(deckArray[i], ch, 2);
        i++;
    }
    puts("");

    fclose(ptr);

    return 1;

}

void printColoumn(node * head){
    int counter = 1;
    while (head !=NULL){
        //printf("Card nr. %d: %s ", counter, head->card);
        printf("%s\t", head->card);
        counter++;
        head = head->next;
    }
}
void printColoumnNames(){
    printf("\nC1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");
}


