/*
Scrivere un programma per gestire un magazzino di articoli generici.
Per ogni articolo viene memorizzato:
    - un codice univoco, 
    - una descrizione di al più 30 caratteri, 
    - la quantità effettiva di giacenza,
    - la quantità minima sotto la quale viene avviato il processo di riordino.

Non è noto a priori la quantità di articoli che il magazzino dovrà gestire. (LISTA)

Devono essere proposte all’utente, attraverso un menù di scelta, le seguenti funzionalità:
    - inserisci un nuovo articolo acquisendo tutti i dati dall’utente
    - cancella un articolo inserito dato il suo codice
    - visualizza i dati di un articolo dato il suo codice
    - visualizza gli articoli da riordinare (con quantità effettiva minore di quella minima)
    - riordina un articolo dato il suo codice (incrementa la quantità effettiva come specificato     dall’utente)
    - visualizza la lista completa di articoli presenti nel magazzino (uno per riga)
    - svuota il magazzino
    - memorizza i dati del magazzino su file (sovrascrivere eventuali file già presenti), chiedere all’utente il nome del file da utilizzare
    - carica i dati del magazzino da un file precedentemente creato (elimina eventuali articoli presenti in memoria prima del caricamento da file), 
    chiedere all’utente il nome del file da utilizzare.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define L 31 // lunghezza descrizione artiocolo

typedef struct articolo{
    unsigned long int codice_articolo;
    char descrizione[L];
    unsigned int qty_giacenza;
    unsigned int qty_riordino;
    struct articolo *next;
} art_t;

void inserisci_articolo(art_t** head);
void cancella_articolo(art_t** head);
void visualizza_articolo(art_t** head);
void visualizza_daRiordinare(art_t** head);
void riordina_articolo(art_t** head);
void visualizza_tutto(art_t** head);
void salva_file(art_t** head);
void carica_file(art_t** head);
void svuota_magazzino(art_t** head);
int esiste_gia(art_t** head, unsigned long int cod);

void errore(char err[]){
    printf("\nErrore: %s! ", err);
    system("PAUSE");
}

char stampa_menu(void){
    char num;
    system("CLS");
    printf("\n");
    printf(" 1 - inserisci un nuovo articolo acquisendo tutti i dati dall'utente.\n");
    printf(" 2 - cancella un articolo dato il suo codice.\n");
    printf(" 3 - visualizza i dati di un articolo dato il suo codice.\n");
    printf(" 4 - visualizza gli articoli da riordinare (con quantita' effettiva minore di quella minima).\n");
    printf(" 5 - riordina un articolo dato il suo codice.\n");
    printf(" 6 - visualizza la lista completa di articoli presenti nel magazzino.\n");
    printf(" 7 - salva i dati del magazzino su file.\n");
    printf(" 8 - carica i dati del magazzino da un file precedentemente creato.\n");
    printf(" 9 - svuota il magazzino.\n");
    printf(" 0 - Esci.\n");
    printf("Inserire il numero corrispondente alla scelta: ");
    scanf("\n%c", &num);
    return num;
}

int main(){
    char scelta; 
    art_t *head = NULL;

    do{
        scelta = stampa_menu();
        switch(scelta){
            case '1': // nuovo articolo
                inserisci_articolo(&head);
                printf("Ho aggiunto il prodotto in magazzino.\n\n");
                system("PAUSE");
                break;
            case '2':
                cancella_articolo(&head);
                printf("Ho cancellato l'articolo.\n\n");
                system("PAUSE");
                break;
            case '3':
                visualizza_articolo(&head);
                break;
            case '4':
                visualizza_daRiordinare(&head);
                system("PAUSE");
                break;
            case '5':
                riordina_articolo(&head);
                break;
            case '6':
                visualizza_tutto(&head);
                break;
            case '7':
                salva_file(&head);
                break;
            case '8':
                carica_file(&head);
                break;
            case '9':
                svuota_magazzino(&head);
                break;
            case '0':
                printf("Arrivederci.\n");
                break;
            default:
                if(scelta < '0' || scelta > '9'){
                    errore("Generico");
                    scelta = '0';
                    salva_file(&head);
                }
                break;
        }
    } while(scelta > '0');
    svuota_magazzino(&head);
    return 0;
}

void inserisci_articolo(art_t** head){
    // alloco lo spazio in memoria
    art_t *prodotto = (art_t *) malloc(sizeof(art_t));
    if(prodotto == NULL){   // errore nell'allocazione
        printf("Errore.");
        return;
    }
    
    // inserisco prima il codice, controllo se esiste gia'
    printf("Inserire specifiche articolo:\n");
    printf("Codice Prodotto: ");
    scanf("%lu", &(prodotto->codice_articolo));
    
    if(esiste_gia(head, prodotto->codice_articolo) == 1){
        *head = prodotto->next;
        free(prodotto);
        errore("Il prodotto con questo codice esiste gia'! Procedere prima eliminando il duplicato.");
        return;
    }
    //   ---- INSERIMENTO IN TESTA
    prodotto->next = *head; // se head e' NULL non ho il problema, primo elemento
    *head = prodotto;

    // inserisco tutti gli altri valori
    printf("Descrizione del prodotto (no spazi): ");
    //fscanf("\n");   // rimuove il bug: premendo invio potrebbe creare problemi
    char c;
    c = getchar();
    fgets(prodotto->descrizione, L, stdin);
    int len = strlen(prodotto->descrizione);
    prodotto->descrizione[len-1] = '\0';
    printf("Quantita' in magazzino: ");
    scanf("%u", &(prodotto->qty_giacenza));
    printf("Minima quantita' avvio riordino: ");
    scanf("%u", &(prodotto->qty_riordino));
}

void cancella_articolo(art_t** head){
    art_t *aux = *head;
    art_t *prev = NULL;
    unsigned long int code;
    printf("Inserire codice articolo da eliminare: ");
    scanf("%lu", code);

    while(aux != NULL){ // finche' non finisco la lista
        if(aux->codice_articolo == code){
            // elimino il prodotto
            if(*head == aux){ // primo articolo
                *head = aux->next;
            } else {
                prev->next = aux->next;
            }   
        }
        prev = aux;
        aux = aux->next;
    }
    printf("Ho cancellato l'articolo.\n\n");
    system("PAUSE");
}

void visualizza_articolo(art_t** head){
    unsigned long int code;
    printf("Inserire il codice dell'articolo da visualizzare: ");
    scanf("%lu", &code);

    art_t *aux = *head;
    while(aux != NULL){
        if(aux->codice_articolo == code){
            printf(" Codice articolo: %lu\n", aux->codice_articolo);
            printf("     Descrizione: %s\n", aux->descrizione);
            printf("       Quantita': %u\n", aux->qty_giacenza);
            printf(" Minimo riordino: %u\n\n", aux->qty_riordino);
            system("PAUSE");
            return;
        }   
        aux = aux->next;
    }
    printf(" Non ho trovato nessun articolo con il codice %lu.", code);
    printf("\n\n");
    system("PAUSE");
}

void visualizza_tutto(art_t** head){
    art_t *aux = *head;
    printf("   Codice    \t   Quantita'  \t   Minimo riordino  \t  Descrizione\n");
    while(aux != NULL){
        printf("  %-10lu         %3u     \t       %3u                %-30s\n", aux->codice_articolo, aux->qty_giacenza, aux->qty_riordino, aux->descrizione); 
        aux = aux->next;
    }
    printf("\n\n");
    system("PAUSE");
}

void salva_file(art_t** head){
    char nome_magazzino[L]; // nome del file da salvare
    char c;
    printf("Inserire il nome con cui si desidera salvare il file (non inserire estensioni): ");
    scanf("%s", nome_magazzino);
    strcat(nome_magazzino, ".txt");
    if(fopen(nome_magazzino, "r") != NULL){ 
        printf("Attenzione! Un file con questo nome esiste gia': sovrascrivere? [y/n] ");
        scanf("\n%c", &c);
        if(c != 'y'){
            printf(" Operazione annullata.\n");
            system("PAUSE");
            return;
        }
    }
    FILE* out = fopen(nome_magazzino, "w");
    if(out == NULL){
        errore("Impossibile aprire il file.");
        return;
    }
    art_t *aux = *head;
    fprintf(out, "    Codice                    Descrizione              Quantita'     Minimo riordino\n");
    while(aux != NULL){
        fprintf(out, " %10lu        %30s        %3u              %3u      \n", aux->codice_articolo, aux->descrizione, aux->qty_giacenza, aux->qty_riordino); 
        aux = aux->next;
    }
    printf("Ho salvato tutto.\n\n");
    system("PAUSE");
    fclose(out);
}

void carica_file(art_t** head){
    char c;
    char nome_magazzino[L]; // nome del file da leggere
    if(*head != NULL){
        printf("Attenzione: proseguendo elimino tutti i dati caricati attualmente. Proseguire? [y/n] ");
        scanf("\n%c", &c);
        if(c != 'y'){
            errore(" La lista non e' vuota, non posso proseguire.");
            return;
        }
        svuota_magazzino(head);
    }
    
    printf("Inserire il nome del file contenete il magazzino (non inserire estensioni): ");
    scanf("%s", nome_magazzino);
    strcat(nome_magazzino, ".txt");
    FILE* in = fopen(nome_magazzino, "r");
    if(in == NULL){
        errore(" Impossibile aprire il file.");
        return;
    }
    // leggo la prima riga ma la scarto subito
    do{
        c = fgetc(in);
    } while(feof(in) == 0 && c != '\n');
    // leggo le righe e creo una lista
    while(feof(in) == 0){
        art_t *p = (art_t *) malloc(sizeof(art_t));
        if(*head == NULL){ // primo elemento
            *head = p;
            p->next = NULL;
        } else {
            p->next = *head;
            *head = p;
        }
        fscanf(in, " %10lu     %30s        %3u              %3u      \n", &(p->codice_articolo), p->descrizione, &(p->qty_giacenza), &(p->qty_riordino));
    }
    printf("Ho caricato tutto.\n");
    system("PAUSE");
    fclose(in);
}

void svuota_magazzino(art_t** head){
    art_t *aux = *head;
    if(*head == NULL){
        errore(" Magazzino vuoto!");
        return;
    }
    while(aux != NULL){
        *head = aux->next;
        free(aux);
        aux = aux->next;
    }
    printf("Ho svuotato il magazzino.\n\n");
    system("PAUSE");
}

int esiste_gia(art_t** head, unsigned long int cod){
    art_t *aux = *head;
    if(*head == NULL){
        //errore(" Magazzino vuoto!");
        return 0;
    }
    while(aux != NULL){
        if(cod == aux->codice_articolo){
            return 1;
        }
        aux = aux->next;
    }
    return 0;
}

void visualizza_daRiordinare(art_t** head){
    art_t *aux = *head;
    if(*head == NULL){
        errore("Il magazzino e' vuoto.");
        return;
    }
    printf("Articoli da riordinare:\n");
    while(aux != NULL){
        if(aux->qty_giacenza <= aux->qty_riordino){
            printf(" > cod: %10lu - in magazzino: %u pezzi\n", aux->codice_articolo, aux->qty_giacenza);
        }
        aux = aux->next;
    }
}

void riordina_articolo(art_t** head){
    art_t *aux = *head;
    if(*head == NULL){
        errore("Il magazzino e' vuoto.");
        return;
    }
    visualizza_daRiordinare(head);
    long unsigned int n;
    printf("\nInserire il codice dell'articolo per cui devi modificare la quantita' in magazzino: ");
    scanf("%lu", &n);
    while(aux != NULL){
        if(n == aux->codice_articolo){
            printf(" > in magazzino: %u pezzi\n", aux->qty_giacenza);
            printf("Inserire la nuova quantita': ");
            scanf("%u", &(aux->qty_giacenza));
        }
        aux = aux->next;
    }
    printf("Quantita' modificata correttamente.\n");
}