#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#define MAX 78
#define LEVEL 3

typedef struct {
    char exact;
    int wrong[MAX];
} letter;

typedef struct{
    int min;
    int max;
}occurence;

typedef struct Node_list{
    char *string;
    struct Node_list* next;
}node_list;

typedef struct Node{
    char value;
    struct Node *children;
    struct Node *bro;
    bool end_word;
    struct Node_list* last;
} node;

typedef node* ptr_node;
typedef node_list* ptr_node_list;
ptr_node root;
int length_input;
int letters_word_game[MAX];
letter* word_suggestion;
char* output;
int num_filtrate;
occurence letters_suggestion[MAX];
int length_input;
bool inGame = false;
char* word_game; //word to guess
int life;
int size;


ptr_node_list create_new_node_list(char *str) {
    ptr_node_list cur;
    cur = (ptr_node_list) malloc(sizeof(node_list));
    cur->string = malloc(sizeof(char)*(length_input-LEVEL+1));
    strcpy(cur->string,str);
    cur->string[length_input-LEVEL] = '\0';
    cur->next = NULL;
    return cur;
}

void insert_list_end(node_list **list, ptr_node_list new) {
    ptr_node_list current;
    if (*list == NULL || strcmp((*list)->string,new->string) >= 0) {
        new->next = *list;
        *list = new;
    }else {
        current = *list;
        while (current->next != NULL && strcmp(current->next->string,new->string) < 0){
            current = current->next;
        }
        new->next = current->next;
        current->next = new;
    }
}

ptr_node create_new_node_trie(char c) {
    ptr_node cur = (ptr_node) malloc(sizeof(node));
    cur->value = c;
    cur->end_word = false;
    cur->children = NULL;
    cur->bro = NULL;
    cur->last = NULL;
    return cur;
}

ptr_node insert_list_trie(ptr_node *list, char c){

    node* new_node;
    node* current;
    if (*list == NULL || (*list)->value > c) {
        new_node = create_new_node_trie(c);
        new_node->bro = *list;
        *list = new_node;
        return *(list);
    }else {
        if((*list)->value == c) return *(list);
        current = *list;
        while (current->bro != NULL && current->bro->value < c){
            current = current->bro;
        }
        if( current->bro != NULL && c == current->bro->value) return current->bro;
        new_node = create_new_node_trie(c);
        new_node->bro = current->bro;
        current->bro = new_node;
        return new_node;
    }
}


void insert_word_tree(char str[]) {
    ptr_node cur = root;
    for (int i = 0; i < size; ++i) {
        cur = insert_list_trie(&cur->children,str[i]);
    }
    if (length_input <= LEVEL){
        cur->end_word = true;
        return;
    }
    ptr_node_list new = create_new_node_list(str+3);
    insert_list_end(&cur->last,new);
}


bool search_word_list(ptr_node_list list, char *str) {
    ptr_node_list curs = list;
    str[length_input-LEVEL] = '\0';
    while(curs != NULL) {
        if (strcmp(curs->string,str) == 0) return true;
        curs = curs->next;
    }
    return false;
}

ptr_node search_letter_children(ptr_node head, char x){
    ptr_node current = head;
    while (current != NULL && x >= current->value) {
        if (current->value == x)
            return current;
        current = current->bro;
    }
    return NULL;
}

bool search_word_tree(char str[]) {
    ptr_node cur = root;
    for (int i = 0; i < size; i++){
        cur = search_letter_children(cur->children,str[i]);
        if(cur == NULL) return false;
    }
    if(length_input == LEVEL)  return (cur->end_word);
    return search_word_list(cur->last,str+LEVEL);
}


void count_letters(char word[], int letters[]) {
    for (int i = 0; i < strlen(word); ++i) {
        letters[word[i]-45]++;
    }
}

void clear_nuova_partita() {
    memset(letters_word_game,0,sizeof(int)*MAX);
    num_filtrate = 0;
    for (int i = 0; i < MAX; ++i) {
        letters_suggestion[i].min = 0;
        letters_suggestion[i].max = -1;
    }
    for (int i = 0; i < length_input; ++i) {
        word_suggestion[i].exact = ' ';
        for (int j = 0; j < MAX; ++j) {
            word_suggestion[i].wrong[j] = 0;
        }

    }
}

void handle_new_partita() {
    clear_nuova_partita();
    if(scanf("%s", word_game) <= 0) return;
    count_letters(word_game,letters_word_game);
    if(scanf("%d", &life) <= 0) return;
    inGame = true;
    num_filtrate = 0;

}

void handle_inserisci_inizio() {
    char input[18];
    while( scanf("%s",input) > 0 && input[0] != '+') {
        insert_word_tree(input);
    }
}


void handle_input_word() {
    char input[18];
    while(scanf("%s",input) > 0) {
        if(strcmp(input,"+nuova_partita") == 0) {
            handle_new_partita();
            return;
        }else {
            if(input[0] != '+') {
                insert_word_tree(input);
            }
        }
    }
}


void handle_end_game(int res) {
    inGame = false;
    if(res == 0) {
        printf("ko\n");
        return;
    }
    printf("ok\n");
    return;
}


int check_1(int count[], int i) {
    for (int j = 0; j < MAX; ++j) {
        if(letters_suggestion[j].max != -1 ) {
            if(letters_suggestion[j].max < count[j] ) {
                return 0;
            }
        }else {
            if(letters_suggestion[j].min > count[j] + length_input - (i + 1)) {
                return 0;
            }
        }
    }
    return 1;
}

int check_2(int count[]) {
    for (int j = 0; j < MAX; ++j) {
        if (letters_suggestion[j].max != -1) {
            if (letters_suggestion[j].max != count[j]) {
                return 0;
            }
        } else {
            if (letters_suggestion[j].min > count[j]) {
                return 0;
            }
        }
    }
    return 1;
}

void update_min_max(occurence letters_attempt[], char input[]) {
    for (int i = 0; i < length_input; ++i) {
        letters_suggestion[input[i]-45].max = letters_attempt[input[i]-45].min;
    }
}


void count_suffix(ptr_node_list suf, int count[]) {

    for (int i = 0; i < length_input - LEVEL; ++i) {

        if (word_suggestion[i + LEVEL].exact != ' ') {
            if (suf->string[i] != word_suggestion[i + LEVEL].exact) {
                for (int j = 0; j < i; ++j) {
                    count[suf->string[j] - 45]--;
                }
                return;
            }

        } else {

            if (word_suggestion[i + LEVEL].wrong[suf->string[i] - 45] == -1) {
                for (int j = 0; j < i; ++j) {
                    count[suf->string[j] - 45]--;
                }
                return;
            }

        }

        count[suf->string[i] - 45]++;
    }
    if (check_2(count) == 1) {
        num_filtrate++;
    }
    for (int i = 0; i < length_input - LEVEL; ++i) {
        count[suf->string[i] - 45]--;
    }
}

void count_list(ptr_node_list suf, int count[]) {
    while(suf != NULL) {
        count_suffix(suf,count);
        suf = suf->next;
    }
}

void count_small_filtrate(ptr_node cur, int i, char build[], int count[]) {
    if(i == length_input) return;
    if(word_suggestion[i].exact != ' ') {
        cur = search_letter_children(cur->children,word_suggestion[i].exact);
        if(cur == NULL) return;
        build[i] = word_suggestion[i].exact;
        count[word_suggestion[i].exact - 45]++;
        if(i + 1 == length_input && cur->end_word == true) {
            if(check_2(count) == 1){
                num_filtrate++;
            }
        }else {
            if(check_1(count,i) == 1) {
                count_small_filtrate(cur,i+1,build,count);
            }
        }
        count[word_suggestion[i].exact-45]--;
    }else {
        cur = cur->children;
        if(cur == NULL) return;
        while(cur != NULL) {
            if (word_suggestion[i].wrong[cur->value - 45] != -1) {
                build[i] = cur->value;
                count[cur->value - 45]++;
                if(i + 1 == length_input && cur->end_word == true) {
                    if(check_2(count) == 1){
                        num_filtrate++;
                    }
                }else {
                    if(check_1(count,i) == 1) {
                        count_small_filtrate(cur,i+1,build,count);
                    }
                }
                count[cur->value - 45]--;
            }
            cur = cur->bro;
        }
    }
}

void find_suffix(ptr_node_list suf, char build[], int count[]) {

    for (int i = 0; i < length_input - LEVEL; ++i) {
        if (word_suggestion[i + LEVEL].exact != ' ') {
            if (suf->string[i] != word_suggestion[i + LEVEL].exact) {
                for (int j = 0; j < i; ++j) {
                    count[suf->string[j] - 45]--;
                }
                return;
            }

        } else {

            if (word_suggestion[i + LEVEL].wrong[suf->string[i] - 45] == -1) {
                for (int j = 0; j < i; ++j) {
                    count[suf->string[j] - 45]--;
                }
                return;
            }

        }

        count[suf->string[i] - 45]++;
        build[i+LEVEL] = suf->string[i];
    }
    if (check_2(count) == 1) {
        build[length_input]='\0';
        printf("%s\n",build);
    }
    for (int i = 0; i < length_input - LEVEL; ++i) {
        count[suf->string[i] - 45]--;
    }
}

void find_list(ptr_node_list suf, char build[], int count[]) {
    while(suf != NULL) {
        find_suffix(suf,build,count);
        suf = suf->next;
    }
}




void count_filtrate(ptr_node cur, int i, int count[]) {
    if(i == 3) {
        count_list(cur->last,count);
        return;
    }

    if(word_suggestion[i].exact != ' ') {
        cur = search_letter_children(cur->children,word_suggestion[i].exact);
        if(cur == NULL) return;
        count[word_suggestion[i].exact - 45]++;
        if(check_1(count,i) == 1) {
            count_filtrate(cur,i+1,count);
        }
        count[word_suggestion[i].exact-45]--;

    }else {
        cur = cur->children;
        if(cur == NULL) return;
        while(cur != NULL) {
            if (word_suggestion[i].wrong[cur->value - 45] != -1) {
                count[cur->value - 45]++;
                if(check_1(count,i) == 1) {
                    count_filtrate(cur,i+1,count);
                }
                count[cur->value - 45]--;
            }
            cur = cur->bro;
        }
    }
}

void update_num_filtrate() {
    int count[78] = {0};
    char build[length_input+1];
    if(length_input <= LEVEL) count_small_filtrate(root,0,build,count);
    count_filtrate(root,0,count);
    return;
}

void merge_attempt(occurence letters_attempt[], char input[]) {
    for (int i = 0; i < length_input; ++i) {
        if(letters_suggestion[input[i]-45].max == -1) {

            if(letters_attempt[input[i]-45].max != -1) {
                letters_suggestion[input[i]-45].max = letters_attempt[input[i]-45].max;
            }else {
                if(letters_attempt[input[i]-45].min > letters_suggestion[input[i]-45].min) {
                    letters_suggestion[input[i]-45].min = letters_attempt[input[i]-45].min;
                }
            }
        }
    }
}


void update_info(char input[], occurence letters_attempt[]) {
    bool flag_all_char_found = true;
    for (int i = 0; i < length_input; ++i) {
        if(output[i] == ' ') {
            if(letters_word_game[input[i]-45] > 0) {
                if(letters_word_game[input[i]-45] - letters_attempt[input[i]-45].min > 0) {
                    letters_attempt[input[i]-45].min++;
                    output[i] = '|';
                    word_suggestion[i].wrong[input[i]-45] = -1;

                }else {
                    letters_attempt[input[i]-45].max = letters_attempt[input[i]-45].min;
                    output[i] = '/';
                    flag_all_char_found = false;
                    word_suggestion[i].wrong[input[i]-45] = -1;
                }
            }else {
                letters_attempt[input[i]-45].max = 0;
                letters_attempt[input[i]-45].min = 0;
                output[i] = '/';
                flag_all_char_found = false;
                for (int j = 0; j < length_input; ++j) {
                    word_suggestion[i].wrong[input[i]-45] = -1;
                }//ridondante
            }
        }
    }
    if(flag_all_char_found == true) {
        update_min_max(letters_attempt,input);
    }else {
        merge_attempt(letters_attempt,input);
    }
    num_filtrate = 0;
    update_num_filtrate();
    output[length_input] = '\0';
    printf("%s\n",output);
    printf("%d\n",num_filtrate);
}


int handle_check(char input[], occurence letters_attempt[]){
    bool found = true;
    for (int i = 0; i < MAX; ++i) {
        letters_attempt[i].min = 0;
        letters_attempt[i].max = -1;
    }
    for (int i = 0; i < length_input; ++i) {
        if (input[i] == word_game[i]) {
            word_suggestion[i].exact = input[i];
            letters_attempt[input[i]-45].min++;
            output[i] = '+';
        }else {
            found = false;
            output[i] = ' ';
        }
    }
    if(found == true) return 1;
    return 0;
}

void handle_attempt(char input[]) {
    int res;
    if(search_word_tree(input) == 0) {
        printf("not_exists\n");
        return;
    }
    occurence letters_attempt[MAX];
    res = handle_check(input,letters_attempt);
    if(res == 1) {
        handle_end_game(1);
        return;
    }
    life--;
    update_info(input,letters_attempt);
    if(life == 0){
        handle_end_game(0);
        return;
    }
}


void print_filtrate(ptr_node cur, int i, char build[], int count[]) {
    if(i == 3) {
        find_list(cur->last,build,count);
        return;
    }

    if(word_suggestion[i].exact != ' ') {
        cur = search_letter_children(cur->children,word_suggestion[i].exact);
        if(cur == NULL) return;
        build[i] = word_suggestion[i].exact;
        count[word_suggestion[i].exact - 45]++;
        if(check_1(count,i) == 1) {
            print_filtrate(cur,i+1,build,count);
        }
        count[word_suggestion[i].exact-45]--;

    }else {
        cur = cur->children;
        if(cur == NULL) return;
        while(cur != NULL) {
            if (word_suggestion[i].wrong[cur->value - 45] != -1) {
                build[i] = cur->value;
                count[cur->value - 45]++;
                if(check_1(count,i) == 1) {
                    print_filtrate(cur,i+1,build,count);
                }
                count[cur->value - 45]--;
            }
            cur = cur->bro;
        }

    }

}



void print_small_filtrate(ptr_node cur, int i, char build[], int count[]) {
    if(i == length_input) return;
    if(word_suggestion[i].exact != ' ') {
        cur = search_letter_children(cur->children,word_suggestion[i].exact);
        if(cur == NULL) return;
        build[i] = word_suggestion[i].exact;
        count[word_suggestion[i].exact - 45]++;
        if(i + 1 == length_input && cur->end_word == true) {
            if(check_2(count) == 1){
                build[length_input] = '\0';
                printf("%s\n",build);
            }
        }else {
            if(check_1(count,i) == 1) {
                print_small_filtrate(cur,i+1,build,count);
            }
        }
        count[word_suggestion[i].exact-45]--;
    }else {
        cur = cur->children;
        if(cur == NULL) return;
        while(cur != NULL) {
            if (word_suggestion[i].wrong[cur->value - 45] != -1) {
                build[i] = cur->value;
                count[cur->value - 45]++;
                if(i + 1 == length_input && cur->end_word == true) {
                    if(check_2(count) == 1){
                        build[length_input] = '\0';
                        printf("%s\n",build);
                    }
                }else {
                    if(check_1(count,i) == 1) {
                        print_small_filtrate(cur,i+1,build,count);
                    }
                }
                count[cur->value - 45]--;
            }
            cur = cur->bro;
        }
    }
}




void handle_stampa_filtrate(){
    char buffer[length_input+1];
    int count[78] = {0};
    if(length_input <= LEVEL) print_small_filtrate(root,0,buffer,count);
    else print_filtrate(root,0,buffer,count);
}

void handle_input(char input[]) {
    if (inGame == false) {
        if (input[1] == 'n') {
            handle_new_partita();
            return;
        }
        handle_inserisci_inizio();
        return;
    }

    if(input[0] != '+') {
        handle_attempt(input);
        return;
    }

    if(input[1] == 's') {
        handle_stampa_filtrate();
        return;
    }
    handle_inserisci_inizio();
    return;
}


int main() {
    if (scanf("%d", &length_input) <= 0) return 0;
    char input[18];
    word_game = (char *) malloc(sizeof(char) * length_input + 1);
    output = (char *) malloc(sizeof(char) * length_input + 1);
    word_suggestion = (letter *) malloc(sizeof(letter) * length_input);
    life = 0;
    num_filtrate = 0;
    root = create_new_node_trie('#');
    if(length_input < LEVEL) size = length_input;
    else size = LEVEL;
    handle_input_word();
    while (scanf("%s", input) > 0) {
        handle_input(input);
    }
    return 0;
}