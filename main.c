/* 
 * File:   main.c
 * Author: littlefool
 *
 * Created on December 13, 2012, 10:47 PM
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Start Stack */
typedef struct StackElement {
    struct StackElement *previous;
    char *value;
} StackElement;

typedef struct Stack {
    StackElement *head;
} Stack;

Stack* createStack() {
    // first we need some space for the Stack itself
    Stack *top = (Stack*) malloc(sizeof (Stack));

    // malloc returns NULL if there is no memory available
    // we do the same...
    if (top == NULL) {
        return NULL;
    }

    // the head of the new Stack is  at NULL (cos its empty)
    top->head = NULL;

    return top;
}

int isEmpty(Stack *top) {
    if (top->head == NULL || top == NULL) {
        return 1;
    }

    return 0;
}

void push(Stack *top, char *value) {
    // alloc memory for a new StackElement
    StackElement *new = (StackElement*) malloc(sizeof (StackElement));

    if (new == NULL) {
        return;
    }

    // first set the properties of the new element
    // the previous object of the new one will be the current head
    new->value = value;
    new->previous = top->head;

    // and the new object becomes the head
    top->head = new;
}

char* pop(Stack *top) {
    // if the stack is empty there is nothing to return
    if (isEmpty(top)) {
        return NULL;
    }

    // value is the value we have to return
    char *value = top->head->value;
    // tmp will hold the current head cos we need to free that
    StackElement *tmp = top->head;
    // set the head to the previous object
    top->head = tmp->previous;
    // and free the old head
    free(tmp);

    return value;
}

void destroyStack(Stack *top) {
    // if the stack isnt empty we make it empty
    while(!isEmpty(top)) {
        pop(top);
    }
    free(top);
    top = NULL;
}

/* End Stack */


int domainTransfer(char* fileName) {
    FILE *file;
    int temp, i=0, spalte=1;
    char domain[256], authcode[256];
    Stack *s = createStack();
    
    // createStack() returns NULL if malloc failed to allocate memory for us
    if (s == NULL) {
        printf("There is not enought memory for the stack! We have to exit here!\n");
        return EXIT_FAILURE;
    }
    
    file = fopen(fileName, "r");
    
    if (file == NULL) {
        printf("Can't open file: %s\n",fileName);
        destroyStack(s);
        return EXIT_FAILURE;
    }
    
    while ((temp = fgetc(file)) != EOF) {
        if(temp == '\n') {
            authcode[i] = 0;
            spalte = 1;
            i = 0;
            
            char *domainc = (char*) malloc(sizeof(domain));
            char *authcodec = (char*) malloc(sizeof(authcode));
            
            memcpy(domainc, domain, sizeof(domain));
            memcpy(authcodec, authcode, sizeof(authcode));
            push(s, domainc);
            push(s, authcodec);
            continue;
        }
        
        if(temp == ';') {
            domain[i] = 0;
            spalte = 2;
            i = 0;
            continue;
        }
        
        if( spalte == 1) {
            domain[i] = temp;
        } else {
            authcode[i] = temp;
        }
        
        i++;
    }
    fclose(file);
    
    file = fopen("transferAPI.txt", "w");
    if (file == NULL) {
        printf("Can't write the file here! Check folder permissions or chose another path.\n");
        return EXIT_FAILURE;
    }
    
    while(!isEmpty(s)) {
        char *auth = pop(s);
        char *dom = pop(s);
        
        fprintf(file, "command = TransferDomain\n");
        fprintf(file, "domain = %s\n", dom);
        fprintf(file, "auth = %s\n\n", auth);
        
        free(auth);
        free(dom);
    }
    fclose(file);
    destroyStack(s);
    
    printf("transferAPI.txt has been written.\n");
    return (EXIT_SUCCESS);
}

int changeNS(char* fileName, char* ns1, char* ns2, char* ns3) {
    FILE *file;
    int temp, i=0;
    char domain[256];
    Stack *s = createStack();
    
    // createStack() returns NULL if malloc failed to allocate memory for us
    if (s == NULL) {
        printf("There is not enought memory for the stack! We have to exit here!\n");
        return EXIT_FAILURE;
    }
    
    file = fopen(fileName, "r");
    
    if (file == NULL) {
        printf("Can't open file: %s\n",fileName);
        destroyStack(s);
        return EXIT_FAILURE;
    }
    
    while ((temp = fgetc(file)) != EOF) {
        if(temp == '\n') {
            i = 0;
            domain[i] = 0;
            
            char *domainc = (char*) malloc(sizeof(domain));
            memcpy(domainc, domain, sizeof(domain));
            push(s, domainc);
            continue;
        }
        
        i++;
    }
    fclose(file);
    
    file = fopen("nsChangeAPI.txt", "w");
    if (file == NULL) {
        printf("Can't write the file here! Check folder permissions or chose another path.\n");
        return EXIT_FAILURE;
    }
    
    while(!isEmpty(s)) {
        char *auth = pop(s);
        char *dom = pop(s);
        
        fprintf(file, "command = TransferDomain\n");
        fprintf(file, "domain = %s\n", dom);
        fprintf(file, "auth = %s\n\n", auth);
        
        free(auth);
        free(dom);
    }
    fclose(file);
    destroyStack(s);
    
    printf("nsChangeAPI.txt has been written.\n");
    return (EXIT_SUCCESS);
}

/*
 * 
 */
int main(int argc, char** argv) {
    if (argc != 3) {
        goto usage;
    } else {
        if (strcmp(argv[1], "transfer") == 0) {
            domainTransfer(argv[2]);
        }
        
        else {
            usage:
            printf("Usage %s transfer [fileName]\n", argv[0]);
        }
    }
    
    
}

