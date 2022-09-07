#include <stdio.h>
#include <stdlib.h>

typedef struct List List;
typedef struct Node Node;

struct Node{
   int item;
   Node * next;
};

struct List{
   Node * firtsNode;
};

void add(List * lista, int elemento){
    Node * y = malloc(sizeof(Node));
    y->item = elemento;
    y->next = NULL;
    
    if(lista->firtsNode == NULL){
        lista->firtsNode = y;        
    }
    else{
        Node * nodo = lista->firtsNode;
        if(nodo->next == NULL){
            nodo->next = y;   
        }
        else{
            while(nodo->next){
                nodo = nodo->next;
            }
            y->next = NULL;
            nodo->next = y;
        }
    }
}

void remove_list_entry(List * lista, Node * entry){
    Node * indirect = lista->firtsNode;
    while(&(*indirect) != &(*entry)){
        indirect = indirect->next;
    }
    *indirect = *entry->next;
}

void remove_list_entry_university(List * lista, Node * entry){
    Node * walk = lista->firtsNode;
    Node * prev = NULL;
    while(&(*walk) != &(*entry)){
        prev = walk;
        walk = walk->next;
    }
    if(!prev)
        lista->firtsNode = entry->next;
    else
        prev->next = entry->next;
}

void display(List * lista){
    printf("List: ");
    Node * nodo = lista->firtsNode;
    while(nodo->next){
        printf("%d ", nodo->item);
        nodo = nodo->next;
    }
    printf("%d ", nodo->item);
    printf( "\n" );
    return;
}

int main(){
    List lista;
    add(&lista, 1);
    add(&lista, 2);
    add(&lista, 3);
    add(&lista, 4);
    add(&lista, 5);
    display(&lista);

    List * listaP = &lista;
    Node * y = listaP->firtsNode;
    remove_list_entry(&lista, y);
    display(&lista);
    Node * x = listaP->firtsNode->next;
    remove_list_entry_university(&lista, x);
    display(&lista);
    add(&lista, 20);
    x = listaP->firtsNode->next;
    remove_list_entry_university(&lista, x);
    display(&lista);
}