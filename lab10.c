#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Trie
{
    struct Trie *children[26]; 
    int isEndOfWord;
    int count; 
};


struct Trie *createTrie()
{
    struct Trie *node = (struct Trie *)malloc(sizeof(struct Trie));
    if (node) {
        node->isEndOfWord = 0;
        node->count = 0;
        for (int i = 0; i < 26; i++) {
            node->children[i] = NULL;
        }
    }
    return node;
}


void insert(struct Trie *pTrie, char *word)
{
    struct Trie *curr = pTrie;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (!curr->children[index]) {
            curr->children[index] = createTrie();
        }
        curr = curr->children[index];
    }
    curr->isEndOfWord = 1;
    curr->count++;
}


int numberOfOccurances(struct Trie *pTrie, char *word)
{
    struct Trie *curr = pTrie;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (!curr->children[index]) {
            return 0;
        }
        curr = curr->children[index];
    }
    return (curr != NULL && curr->isEndOfWord) ? curr->count : 0;
}


struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (!pTrie) {
        return NULL;
    }
    for (int i = 0; i < 26; i++) {
        if (pTrie->children[i]) {
            deallocateTrie(pTrie->children[i]);
        }
    }
    free(pTrie);
    return NULL;
}

int readDictionary(char *filename, char **pInWords)
{

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 0;
    }

    char word[256];
    int numWords = 0;

    while (fscanf(file, "%s", word) == 1) {
        pInWords[numWords] = strdup(word);
        numWords++;
    }
    fclose(file);

    return numWords;
}

int main(void)
{
    char *inWords[256];


    int numWords = readDictionary("dictionary.txt", inWords);

    struct Trie *pTrie = createTrie();
    for (int i=0;i<numWords;i++)
    {
        insert(pTrie, inWords[i]);
    }
  
    char *pWords[] = {"12", "not", "note", "ucf", "notaword", "notawordeither", "notaword", "ucf", "note", "ucf", "notawordeithereither", "not", "ucf"};
    int pWordsSize = sizeof(pWords) / sizeof(pWords[0]);
    for (int i=0;i<pWordsSize;i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");
    return 0;
}