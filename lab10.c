#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct TrieNode
{
    struct TrieNode *children[26];
    int count;
};

struct TrieNode *createTrieNode()
{
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    for (int i = 0; i < 26; i++)
    {
        node->children[i] = NULL;
    }
    node->count = 0;
    return node;
}

void insert(struct TrieNode *root, char *word)
{
    struct TrieNode *current = root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        
        // Check if index is valid
        if (index < 0 || index >= 26)
        {
            printf("Invalid character in word: %c\n", word[i]);
            return;
        }
        
        if (current->children[index] == NULL)
        {
            current->children[index] = createTrieNode();
            if (current->children[index] == NULL)
            {
                printf("Memory allocation error\n");
                return;
            }
        }
        
        current = current->children[index];
    }
    current->count++;
}


int numberOfOccurrences(struct TrieNode *root, char *word)
{
    struct TrieNode *current = root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (current->children[index] == NULL)
        {
            return 0; 
        }
        current = current->children[index];
    }
    return current->count;
}

void deallocateTrie(struct TrieNode *node)
{
    if (node == NULL)
    {
        return;
    }
    for (int i = 0; i < 26; i++)
    {
        if (node->children[i] != NULL)
        {
            deallocateTrie(node->children[i]);
        }
    }
    free(node);
}

int readDictionary(char *filename, char ***pInWords)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        exit(1);
    }

    int numWords = 0;
    char **words = NULL;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file))
    {
        buffer[strcspn(buffer, "\n")] = 0;
        numWords++;
        words = (char **)realloc(words, numWords * sizeof(char *));
        if (words == NULL)
        {
            printf("Memory allocation error.\n");
            exit(1);
        }
        words[numWords - 1] = strdup(buffer);
    }

    fclose(file);
    *pInWords = words;
    return numWords;
}

int main(void)
{
    char **inWords = NULL;

    int numWords = readDictionary("dictionary.txt", &inWords);

    struct TrieNode *pTrie = createTrieNode();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    deallocateTrie(pTrie);

    for (int i = 0; i < numWords; ++i)
    {
        free(inWords[i]);
    }
    free(inWords);

    return 0;
}