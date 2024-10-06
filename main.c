#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "lists.h"

typedef struct {
	char suit;
	char rank;
} Card;

const char SUITS[] = {'C', 'D', 'H', 'S'};
const char RANKS[] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};

void fillDeck(DList *deck) {
  for (int i = 0; i < sizeof(SUITS); ++i) {
	for (int j = 0; j < sizeof(RANKS); ++j) {
	  Card *card = (Card *) malloc(sizeof(Card));
	  card->suit = SUITS[i];
	  card->rank = RANKS[j];
	  DListNode *cardNode = createNode(card);
	  insertLast(deck, cardNode);
	}
  }
}

void foldDeck(DList *deck) {
  for (int i = 0; i < 300; ++i) {
	DListNode *card = pop2(deck);
	int deckSize = deck->size;
	int insertionIndex = rand() % (deckSize + 1);
	if (insertionIndex == 0) insertFirst(deck, card);
	else if (insertionIndex == deckSize) insertLast(deck, card);
	else insertBefore(deck, card, insertionIndex);
  }
}

/// @brief Re-calculate the hand total value.
/// @param hand The pointer to hand to update.
void updateSum(DList *hand) {
  int total = 0;
  int aceCount = 0;
  
  for (int i = 0; i < hand->size; ++i) {
	DListNode *node = get(hand, i);
	Card *card = node->value;
	char rank = card->rank;
	
	switch (rank) {
	  case 'T':
	  case 'J':
	  case 'Q':
	  case 'K':
		total += 10;
		break;
	  case 'A':
		total += 11;
		aceCount++;
		break;
	  default:
		total += (int) (rank - '0'); // Convert char to int
		break;
	}
  }
  
  // Adjust for aces if the total exceeds 21
  while (total > 21 && aceCount > 0) {
	total -= 10; // Change ace from 11 to 1
	aceCount--;
  }
  
  hand->total = total;
  hand->isSoft = (aceCount > 0); // isSoft is true if there's still an ace counted as 11
}

void printCard(void *value) {
  Card *card = (Card *) value;
  printf("%c%c ", card->rank, card->suit);
}

void printHand(DList *hand) {
  print(hand, printCard);
  printf("(");
  if (hand->isSoft && hand->total != 21) printf("%d/", (hand->total) - 10);
  printf("%d)\r\n", hand->total);
}

int canSplit(DList *hand) {
  if (hand->size != 2) return 0;
  char card1Rank = ((Card *) (((DListNode *) get(hand, 0))->value))->rank;
  char card2Rank = ((Card *) (((DListNode *) get(hand, 1))->value))->rank;
  if (card1Rank == card2Rank) return 1;
  if ((card1Rank == 'T' || \
        card1Rank == 'J' || \
        card1Rank == 'Q' || \
        card1Rank == 'K') && \
      (card2Rank == 'T' || \
        card2Rank == 'J' || \
        card2Rank == 'Q' || \
        card2Rank == 'K'))
	return 1;
  return 0;
}

int main() {
  while (1) {
	printf("--------------------------------------------\r\n");
	bool casinoBJ = false, playerBJ = false;
	char choice;
	srand(time(NULL));
	DList *deck = createList();
	// Fill deck with all cards ordered then fold it
	fillDeck(deck);
	foldDeck(deck);
	
	while (deck->size > 14) {
	  DList *playerHand = createList();
	  DList *casinoHand = createList();
	  
	  // Initialisation : both get 2 card, the casino have one down
	  insertLast(playerHand, pop2(deck));
	  insertLast(casinoHand, pop2(deck));
	  insertLast(playerHand, pop2(deck));
	  DListNode *downCard = pop2(deck);
	  updateSum(playerHand);
	  updateSum(casinoHand);
	  
	  // Check if casino have blackjack :
	  insertFirst(casinoHand, downCard);
	  updateSum(casinoHand);
	  if (casinoHand->total == 21)
		casinoBJ = true;
	  else {
		downCard = pop2(casinoHand);
		updateSum(casinoHand);
	  }
	  
	  // Check if player have blackjack :
	  if (playerHand->total == 21)
		playerBJ = true;
	  
	  printHand(casinoHand);
	  printf("\r\n");
	  printHand(playerHand);
	  printf("\r\n\r\n");
	  
	  if (casinoBJ || playerBJ) {
		if (casinoBJ && playerBJ) {
		  printf("END : Push\r\n\r\n");
		  continue;
		}
		if (casinoBJ) {
		  printf("END : Lost\r\n\r\n");
		  continue;
		}
		printf("END : Blackjack\r\n\r\n");
		continue;
	  }
	  
	  
	  choice:
	  printf("S = Stand, H = Hit, 2 = Double");
	  if (canSplit(playerHand)) printf(", / = Split");
	  printf("\r\n");
	  scanf(" %c", &choice);
	  fflush(stdin);
	  switch (choice) {
		case 'S':
		  break;
		case 'H':
		  insertLast(playerHand, pop2(deck));
		  updateSum(playerHand);
		  printHand(casinoHand);
		  printf("\r\n");
		  printHand(playerHand);
		  printf("\r\n\r\n");
		  if (playerHand->total < 21) goto choice;
		  break;
		case '2':
		  break;
		case '/':
		  break;
		default:
		  //printf("\x1b[1F");
		  //printf("\x1b[2K");
		  goto choice;
	  }
	  // EOG
	  // Reveal Casino second card
	  insertLast(casinoHand, downCard);
	  updateSum(casinoHand);
	  
	  if (playerHand->total > 21) {
		printHand(casinoHand);
		printf("\r\n");
		printHand(playerHand);
		printf("\r\n\r\n");
		
		printf("END : Burst\r\n\r\n");
		continue;
	  }
	  
	  // Fill casino hand until soft 17
	  while (casinoHand->total < 17) {
		insertLast(casinoHand, pop2(deck));
		updateSum(casinoHand);
	  }
	  
	  // Display final hands
	  printHand(casinoHand);
	  printf("\r\n");
	  printHand(playerHand);
	  printf("\r\n\r\n");
	  
	  if (casinoHand->total > 21) {
		printf("END : Win\r\n\r\n");
		continue;
	  }
	  if (playerHand->total == casinoHand->total) {
		printf("END : Push\r\n\r\n");
		continue;
	  }
	  if (playerHand->total > casinoHand->total) {
		printf("END : Win\r\n\r\n");
		continue;
	  }
	  printf("END : Loose\r\n\r\n");
	}
  }
  return 0;
}