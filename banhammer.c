#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <math.h>
#include <unistd.h>
#include <sys/stat.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>

#include "bf.h"
#include "bst.h"
#include "ht.h"
#include "node.h"
#include "parser.h"
#include "set.h"

#define WORD "[a-zA-Z0-9'_-]+"

typedef enum { HELP, STATS, THOUGHTCRIME, NEWSPEAK } commands;
int main(int argc, char **argv) {
    // Constructing necessary variables
    FILE *badspeak = fopen("badspeak.txt", "r");
    FILE *newspeak = fopen("newspeak.txt", "r");
    uint32_t hash_size = pow(2, 16);
    uint32_t bloom_size = pow(2, 20);
    Set set_commands = empty_set();
    regex_t re;

    struct stat sb, sn;
    stat("badspeak.txt", &sb);
    char *badwords = (char *) malloc(sb.st_size);
    stat("newspeak.txt", &sn);
    char *oldword = (char *) malloc(sn.st_size);
    char *newword = (char *) malloc(sn.st_size);

    // Opting users for preferences
    int opt = 0;
    while ((opt = getopt(argc, argv, "ht:f:s")) != -1) {
        switch (opt) {
        case 'h': set_commands = insert_set(HELP, set_commands); break;
        case 't': hash_size = (uint32_t) atoi(optarg); break;
        case 'f': bloom_size = (uint32_t) atoi(optarg); break;
        case 's': set_commands = insert_set(STATS, set_commands); break;
        }
    }

    // if help is opted print out the following message
    if (member_set(HELP, set_commands)) {
        printf("SYNOPSIS\n");
        printf("  A word filtering program for the GPRSC.\n");
        printf("  Filters out and reports bad words parsed from stdin.\n\n");
        printf("USAGE\n");
        printf("  ./banhammer [-hs] [-t size] [-f size]\n\n");
        printf("OPTIONS\n");
        printf("   -h          Program usage and help.\n");
        printf("   -s          Program program statistics.\n");
        printf("   -t size     Specify hash table size (default: 2^16).\n");
        printf("   -f size     Specify Bloom filter size (default: 2^20).\n");

        free(badwords);
        free(oldword);
        free(newword);
        return 0;
    }

    // Create the bloomfilter, hashtable, badspeak bst, and newspeak bst
    BloomFilter *bf = bf_create(bloom_size);
    HashTable *ht = ht_create(hash_size);
    if (!ht) {
        return 1;
    }
    Node *bs = bst_create();
    Node *ns = bst_create();

    // Iterate through the badspeak and newspeak files and store the data in
    // bloomfilter and hashtable
    while (fscanf(badspeak, "%s\n", badwords) != EOF) {
        bf_insert(bf, badwords);
        ht_insert(ht, badwords, NULL);
    }

    while (fscanf(newspeak, "%s %s\n", oldword, newword) != EOF) {
        bf_insert(bf, oldword);
        ht_insert(ht, oldword, newword);
    }

    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex. \n");
        return 1;
    }

    // Iterate throught stdin and filter out the words
    char *word = NULL;
    while ((word = next_word(stdin, &re)) != NULL) {
        for (int i = 0; i < strlen(word); i++) {
            word[i] = tolower((unsigned char) word[i]);
        }

        if (bf_probe(bf, word)) {
            if (ht_lookup(ht, word) && !ht_lookup(ht, word)->newspeak) {
                set_commands = insert_set(THOUGHTCRIME, set_commands);
                bs = bst_insert(bs, word, NULL);
            } else if (ht_lookup(ht, word) && ht_lookup(ht, word)->newspeak) {
                set_commands = insert_set(NEWSPEAK, set_commands);
                ns = bst_insert(ns, word, ht_lookup(ht, word)->newspeak);
            }
        }
    }

    // Print out stats and end the program
    if (member_set(STATS, set_commands)) {
        double branches_traversed = (double) branches / (double) lookups;
        double hash_load = 100 * ((double) ht_count(ht) / (double) ht_size(ht));
        double bf_load = 100 * ((double) bf_count(bf) / (double) bf_size(bf));

        printf("Average BST size: %0.6f\n", ht_avg_bst_size(ht));
        printf("Average BST height: %0.6f\n", ht_avg_bst_height(ht));
        printf("Average branches traversed: %0.6f\n", branches_traversed);
        printf("Hash table load: %0.6f%%\n", hash_load);
        printf("Bloom filter load: %0.6f%%\n", bf_load);

        bf_delete(&bf);
        ht_delete(&ht);
        bst_delete(&bs);
        bst_delete(&ns);
        free(badwords);
        free(oldword);
        free(newword);
        regfree(&re);
        free(word);
        fclose(badspeak);
        fclose(newspeak);
        clear_words();
        return 0;
    }

    // Print out the correct letter of punishment
    if (member_set(THOUGHTCRIME, set_commands) && member_set(NEWSPEAK, set_commands)) {
        printf("Dear beloved citizen of the GPRSC,\n\n");
        printf("We have some good news, and we have some bad news.\n");
        printf("The good news is that there is bad news. The bad news is that you will\n");
        printf("be sent to joycamp and subjected to a week-long destitute existence.\n");
        printf("This is the penalty for using degenerate words, as well as using\n");
        printf("oldspeak in place of newspeak. We hope you can correct your behavior.\n\n");
        printf("Your transgressions, followed by the words you must think on:\n\n");
        bst_print(bs);
        bst_print(ns);
    } else if (member_set(THOUGHTCRIME, set_commands)) {
        printf("Dear beloved citizen of the GPRSC,\n\n");
        printf("You have been caught using degenerate words that may cause\n");
        printf("distress among the moral and upstanding citizens of the GPSRC.\n");
        printf("As such, you will be sent to joycamp. It is there where you will\n");
        printf("sit and reflect on the consequences of your choice in language.\n\n");
        printf("Your transgressions:\n\n");
        bst_print(bs);
    } else if (member_set(NEWSPEAK, set_commands)) {
        printf("Dear beloved citizen of the GPRSC,\n\n");
        printf("We recognize your efforts in conforming to the language standards\n");
        printf("of the GPSRC. Alas, you have been caught uttering questionable words\n");
        printf("and thinking unpleasant thoughts. You must correct your wrongspeak\n");
        printf("and badthink at once. Failure to do so will result in your deliverance\n");
        printf("to joycamp.\n\n");
        printf("Words that you must think on:\n\n");
        bst_print(ns);
    }

    // Free memory
    bf_delete(&bf);
    ht_delete(&ht);
    bst_delete(&bs);
    bst_delete(&ns);
    free(badwords);
    free(oldword);
    free(newword);
    clear_words();
    regfree(&re);
    free(word);
    fclose(badspeak);
    fclose(newspeak);
    return 0;
}
