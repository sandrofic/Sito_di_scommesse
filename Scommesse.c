#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX 30


struct partita {	// lista delle partite inserite dall'admin
	char squadra1[MAX];
	char squadra2[MAX];
	float quota1;
	float quotaX;
	float quota2;
	int codice;
	int risultato;
	struct partita *next;
};
typedef struct partita partita;

struct partitagiocata{		//Lista delle partite su cui l'utente ha scommesso: se è una scommessa singola next=NULL
	char squadra1[MAX];
	char squadra2[MAX];
	int quotascelta;
	float puntata;
	float quotavinta;
	int codice;// per distinguere la partita (codice univoco)
	int codice2; //per distinguere la scommessa singola dalle multiple (uguale nelle multiple,diverso nelle singole)
	struct partitagiocata *next;
};
typedef struct partitagiocata partitagiocata;


struct schedina{ //lista storico degli utenti

	char squadra1[MAX];
	char squadra2[MAX];
	float puntata;
	float quotagiocata; //quota scelta da utente
	float quotariscossa; //quantità di soldi eventualmente vinti
	int vincita; //vittoria o sconfitta
	int codice2; // cod multipla/singola
};
typedef struct schedina schedina;
struct storico{
	schedina schedina;
	struct storico *next;
};
typedef struct storico storico;

struct utente { //lista degli utenti
        char nome[MAX];
        char cognome[MAX];
        char codicefiscale[17];
        char password[MAX];
        float conto;
        struct storico *head_s;
        struct partitagiocata *head_p; //in attesa
        struct utente *next;
};

typedef struct utente utente;

int ch; //per pulire il buffer

struct amministratore { 	//credenziali admin
        char nomeamm[6];
        char pwamm[6];
};
typedef struct amministratore amministratore;


//funzione che, prendendo la quota in input, decide randomicamente se essa è vincente oppure no
int AlgoritmoQuota(float quota){

        float probabilita= 1/quota*100;
        srand(time(NULL));
        float casuale=1+rand()%100;
                if(casuale<=probabilita)
                        return 0; //LA QUOTA E' VINCENTE
                else return 1; //quota perdente

}


//funzione che cancella il carattere \n dalla fgets usata durante la scrittura su file
void CancellaAndataACapo (char stringa[]){
	int lung=strlen(stringa);

	if (stringa[lung-1]=='\n')
		stringa[lung-1]='\0';

}


void EliminaFileUtente(char codicefiscale[]){

        char filestorico[24];//24
        char filepg[21];//21

        strcpy(filestorico,codicefiscale);
        strcat(filestorico,"_st.txt");

        strcpy(filepg,codicefiscale);
        strcat(filepg,".txt");

        char stringastorico[27];
        char stringapg[24];
        strcpy(stringastorico,"rm ");
        strcpy(stringapg,"rm ");

        strcat(stringastorico,filestorico);
        strcat(stringapg,filepg);

        system(stringapg);
        system(stringastorico);


}

int VerificaFormatoINT (char stringa[]){

        int j=strlen(stringa);
        int k=0;
        for(int i=0;i<j;i++){
                if(stringa[i]<48 || stringa[i]>57)
                        k=1; //lettera
        }
        if (k==0){//la stringa contiene solo numeri
                float conto=atoi(stringa);
                return conto;
        }
        else return -1; //la stringa contiene lettere

}

float VerificaFormato (char stringa[]){

        int j=strlen(stringa);
        int k=0;
        for(int i=0;i<j;i++){
                if((stringa[i]<48 || stringa[i]>57) && stringa[i]!=46)
                        k=1; //lettera
        }
        if (k==0){//la stringa contiene solo numeri
                float conto=atof(stringa);
                return conto;
        }
        else return -1; //la stringa contiene lettere


}

//funzione che controlla che l'utente sia maggiorenne prendendo in input il suo codice fiscale
int calcolaMaggiorenne(char codfisc[]){

	int controllo=0;

	int lung=strlen(codfisc);
	if(lung>17){
		controllo=1;
		return controllo;
	}


        char annochar[3]; //array che copierà le due cifre che indicano l'anno di nascita
        char giornochar[3]; //array che copierà le due cifre che indicano il giorno di nascita
        char mesechar=codfisc[8]; //copio nella variabile mesechar il carattere che indica il mese di nascita
        int meseint=-1; // variabile che conterrà il mese di nascita

                //assegno i caratteri del codice alle rispettive variabili
                annochar[0]=codfisc[6];
                annochar[1]=codfisc[7];
                giornochar[0]=codfisc[9];
                giornochar[1]=codfisc[10];
                //utilizzo un istruzione di selezione switch-case per trasformare il carattere in un numero
                switch (mesechar){
                        case 'a': case 'A': meseint=1;    //Gennaio
                                break;
                        case 'b': case 'B': meseint=2;    //Febbraio
                                break;
                        case 'c': case 'C': meseint=3;    //Marzo
                                break;
                        case 'd': case 'D': meseint=4;    //Aprile
                                break;
                        case 'e': case 'E': meseint=5;    //Maggio
                                break;
                        case 'h': case 'H': meseint=6;    //Giugno
                                break;
                        case 'l': case 'L': meseint=7;    //Luglio
                                break;
                        case 'm': case 'M': meseint=8;    //Agosto
                                break;
                        case 'p': case 'P': meseint=9;    //Settembre
                                break;
                        case 'r': case 'R': meseint=10;   //Ottobre
                                break;
                        case 's': case 'S': meseint=11;   //Novembre
                                break;
                        case 't': case 'T': meseint=12;   //Dicembre
                                break;
                        default: {
                                printf("Hai inserito un codice fiscale sbagliato\n");
                                controllo=1;
                        }
                }
                // utilizo la funzione atoi della stdlib per convertire un carattere numerico in un intero numerico
//		int annoint = atoi(annochar);
//		int giornoint= atoi(giornochar);

                //il programma considera maggiorenni tutti coloro che sono nati fino al 15.06.2002
                //                    (giorno della consegna)

                //controllo maggiorenne eta: se flag==0 utente maggiorenne
                // se flag==1 utente minorenne
		//      f c r l s n 0 0 t 1 9  b  4  2  8  x  /0
		//      0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
		int flag;
                if(annochar[0]=='0' || annochar[0]=='1'){ //prendo in considerazione solo i nati nella fascia 2000-2019
                        if(annochar[1]=='2'){ //caso 2002 (utente può essere sia maggiorenne sia minorenne)
                                if(meseint>6) flag=1; //se è nato dopo giugno è minorenne
                                else if(meseint==6){ //se è nato a giugno bisogna considerare il giorno
                                        if(giornochar[0]=='1' && giornochar[1]=='5' && giornochar[0]=='3' && giornochar[1]=='0') flag=1; //giorno di nascita uomo
                                        else if(giornochar[0]=='4' && giornochar[1]=='5' && giornochar[0]=='7' && giornochar[1]=='0') flag=1; //giorno di nascita donna (+40)
                                        else flag=0;
                                }
                                else flag=0;
                         }  //chiusura secondoif
                        else if(annochar[1]>'2') flag=1; //nati dopo il 2002
                        else flag=0; //nati prima del 2002
                }//chiusura primo if
                else  flag=0;
                if(flag==1){
                        printf("Utente minorenne, uscita dal programma\n");
                        exit(1);
                }
        return controllo;     //chiusura funzione
}

//funzione che restituisce il puntatore della partita cercata tramite codice
partita* trovaPartitaPuntatore(partita *head, partita u){


        while ((head != NULL) && (head->codice != u.codice)){
                  head = head->next;
        }
        return head;

}


//funzione che prende il puntatore di tipo partita, chiede all'admin quale partita voglia simulare, dopodichè restituisce l'esito
void SimulaPartita(partita *p){

        partita temp;
        partita *q;
        char stringa[MAX];

        if (p==NULL) printf("Non ci sono partite da simulare.\n"); //le partite della lista sono tutte simulate
        else {
                printf("Inserisci il codice della partita da simulare:\t");
                scanf("%s",stringa);
                temp.codice=VerificaFormatoINT(stringa);
                while((ch=getchar())!='\n' && ch!=EOF);

                q=trovaPartitaPuntatore(p,temp);
                if (q==NULL){
                        printf("Non c'è alcuna partita con questi parametri\n");
                        return;
                }
                if(q->risultato !=-1){
                        printf("La partita è già stata simulata\n");
                        return;
                }
                printf("Hai selezionato la partita %s - %s\n",q->squadra1,q->squadra2);
                int flag1=AlgoritmoQuota(q->quota1);
                        if (flag1==0) {
                                q->risultato=1;
                                printf("Ha vinto %s\n",q->squadra1);
                        }
                        else {
                                int flagX=AlgoritmoQuota(q->quotaX); //vittoria squadra1
                                if(flagX==0){
                                        q->risultato=0; //pareggio
                                        printf("Le squadre hanno pareggiato\n");
                                }
                                else {
                                        q->risultato=2; //vittoria squadra2
                                        printf("Ha vinto %s\n",q->squadra2);
                                }
                        }
        }
}


//funzione che restituisce 1 se trova il codice fiscale, 0 altrimenti
int trovaCodiceFiscale(utente *head, utente u){
        utente *temp;
        temp = head;
        //scorro tutta la lista per vedere se il codice esiste o meno.
        while ((temp != NULL) && strcmp(temp->codicefiscale, u.codicefiscale)!=0){
                temp = temp->next;
                }
        if (temp==NULL) return 0;
        else  return 1;

}

//funzione che restituisce 1 se trova la partita tramite squadre (non usata)
int trovaPartita(partita *head, partita u){
        partita *p;
        p = head;
        while ((p != NULL) && (strcmp(p->squadra1, u.squadra1)!=0 || strcmp(p->squadra2,u.squadra2)!=0)){
      	          p = p->next;
        }
        if (p==NULL) return 0; // non esiste, torna 0, 
        else return 1; //altrimenti torna 0
}


//funzione che cerca l'utente tramite codice fiscale e password.Restituisce il puntatore
utente* trovaUtentePuntatore(utente *head, utente u){
	while ((head != NULL) &&( strcmp(head->codicefiscale, u.codicefiscale)!=0 || strcmp(head->password,u.password)!=0)){
                  head = head->next;
        }
        return head;

}
//funzione che cerca l'utente tramite codice fiscale.
utente* trovaUtenteCodiceFiscalePtr(utente *head, utente u){
        while ((head != NULL) && strcmp(head->codicefiscale, u.codicefiscale)!=0){
                  head = head->next;
        }
        return head;

}


//funzione che restituisce il puntatore di tipo partitagiocata attraverso la ricerca tramite codice
partitagiocata *CercaPartitaPerCodicePG (partitagiocata *p,int code){

        while ((p!=NULL) && p->codice!=code){
                p=p->next;
	}
        return p;
}


//funzione che restituisce il puntatore di tipo partita attraverso la ricerca tramite codice.
partita *CercaPartitaPerCodice (partita *p,int code){

	while (p!=NULL && p->codice!=code){
		p=p->next;
	}
	return p;
}


int trovaUtente(utente *head, utente u){
        utente *p;
        p = head;
        //scorro tutta la lista per vedere se il codice e la password inseriti corrispondono.
        while ((p != NULL) &&( strcmp(p->codicefiscale, u.codicefiscale)!=0 || strcmp(p->password,u.password)!=0)){
		 p = p->next;
        }
        if (p==NULL) return 0; // non esiste, torna 0. else return 1; //se esiste torna 1
	else return 1;
}


//funzione che cancella l'utente dalla lista degli utenti. Se il saldo è maggiore di 0 chiede un'ulteriore conferma
int trovaUtentePerCancellazione(utente *head, utente u){
        utente *temp;
        temp = head;
	char c;
        while ((temp != NULL) &&( strcmp(temp->codicefiscale, u.codicefiscale)!=0 || strcmp(temp->password,u.password)!=0)){
                 temp = temp->next;
        }
        if (temp==NULL) return 0; // non esiste, torna 0. else return 1; //se esiste torna 1
        else if (temp->conto==0) return 1;
	else {
		printf("Attenzione! Il tuo saldo di %.2f € andrà perso.\nVuoi comunque continuare? s/n ",temp->conto);
		scanf("%c",&c);
	        while((ch=getchar())!='\n' && ch!=EOF);
		if(c=='s') return 1;
		else if (c=='n') return 0;
		else {
			printf("Errore nella scelta\n");
			return 0;
		}
	}
}


//funzione che stampa tutta la lista degli utenti (per admin)
void StampaUtente(utente* head){
        if (head==NULL)  printf("Nessun utente presente nella lista\n");
        else {
                while(head != NULL){
                      printf("Nome:\t%s\n",head->nome);
                      printf("Cognome:\t%s\n",head->cognome);
                      printf("Codice fiscale:\t%s\n",head->codicefiscale);
              //      printf("%s\t\t",head->password);
                      printf("Conto:\t%.2f\n\n",head->conto);
                      head = head->next;
                }
        }
}


//funzione che stampa tutta la lista delle partite
void StampaPartita( partita* head){
        if (head==NULL)  printf("Non ci sono partite disponibili al momento.\n");
        else {
                while(head != NULL){
                      printf("%s-",head->squadra1);
                      printf("%s\n",head->squadra2);
                      printf("QUOTE-> 1:%.2f\tX:%.2f\t2:%.2f\t%d\t",head->quota1,head->quotaX,head->quota2,head->codice);
		      if(head->risultato==-1) printf("Partita ancora da giocare\n\n");
		      else printf("Partita simulata\n\n");
                      head = head->next;
                }
        }
}


//funzione che inizializza l'utente e lo va ad inserire in testa della  lista degli utenti
utente* InserisciUtenteInTesta (utente **head, utente *l, utente u){

        utente *temp=NULL;
	printf("Inserisci codice fiscale:\t");
	scanf("%s",u.codicefiscale);
	while((ch=getchar())!='\n' && ch!=EOF);
	if (calcolaMaggiorenne(u.codicefiscale)==1) //controllo validità codicefiscale
		 return temp;
	int i=trovaCodiceFiscale(l,u);
		if(i==1){
			printf("Codice già esistente\n");
			return temp;
		}
		else {
	        	temp=(utente*)malloc(sizeof(utente));
//			temp->head=NULL;
  	  		temp->head_s=NULL;
     	 	  	temp->head_p=NULL;
			char stringa[MAX];
			printf("Inserisci Nome: \t");
			fgets(u.nome,MAX,stdin); //standard input
			CancellaAndataACapo(u.nome);
			printf("Inserisci Cognome:\t");
			fgets(u.cognome,MAX,stdin);
			CancellaAndataACapo(u.cognome);
			printf("Nota bene: La tua password non deve contenere spazi.\nInserisci Password:\t");
			scanf("%s",u.password);
			printf("La password memorizzata è:\t %s\n",u.password);
			while((ch=getchar())!='\n' && ch!=EOF);
			printf("Inserisci Conto:\t");
			scanf("%s",stringa);
			while((ch=getchar())!='\n' && ch!=EOF);
			u.conto=VerificaFormato(stringa);
			if (u.conto <=0){
				printf("Conto non valido, uscita dalla registrazione.\n");
				temp=NULL;
				return temp;
			}
	       		strcpy(temp->nome,u.nome);
       			strcpy(temp->cognome,u.cognome);
        		strcpy(temp->codicefiscale,u.codicefiscale);
        		strcpy(temp->password,u.password);
        		temp->conto=u.conto;
       			temp->next = *head;
        		*head = temp;
			return temp;
		}
}


//funzione per l'inserimento dell'utente in coda, usata nella lettura del file
void InserisciCodaLetturaUtente (utente **head, utente u){

        utente *temp;
        utente *nodo;
	nodo=(utente*)malloc(sizeof(utente));
  //    nodo->head=NULL;
	nodo->head_p=NULL;
	nodo->head_s=NULL;

        strcpy(nodo->nome,u.nome);
	strcpy(nodo->cognome,u.cognome);
	strcpy(nodo->codicefiscale,u.codicefiscale);
	strcpy(nodo->password,u.password);
	nodo->conto=u.conto;
        nodo->next = NULL;
        if(*head==NULL){ 
        	*head=nodo;
        	return;
         }

        temp=*head;
        while (temp->next != NULL)
               	temp=temp->next;
        temp->next=nodo;
}


//funzione che inserisce la partita in coda, usata in lettura.
void InserisciCodaPartitaLettura (partita **head, partita u){
        partita *temp;
        partita *nodo;
        nodo=(partita*)malloc(sizeof(partita));
        strcpy(nodo->squadra1,u.squadra1);
        strcpy(nodo->squadra2,u.squadra2);
        nodo->quota1=u.quota1;
	nodo->quotaX=u.quotaX;
	nodo->quota2=u.quota2;
        nodo->codice=u.codice;
	nodo->risultato=u.risultato;
        nodo->next=NULL;
        if(*head==NULL){
        	*head=nodo;
        	return;
         }
        temp=*head;
        while (temp->next != NULL)
                temp=temp->next;
        temp->next=nodo;
}


//funzione che aggiunge un codice alla partita inserita dall'admin. Il codice è +1 rispetto alla partita precedente
void aggiungiCodicePartita(partita *head){

	if  (head->next==NULL){
		head->codice=1;		//Non ci sono partite precedenti, quindi inserisce come codice 1
		return;
	}

	while (head->next->next!=NULL)
		head=head->next;

	head->next->codice=(head->codice)+1;
}


//funzione che inizializza la partita e la inserisce alla coda della lista. Torna 0 se è andato tutto bene,1 altrimenti
void InserisciCodaPartita (partita **head, partita u){
        partita *temp;
        partita *nodo;
        char stringa[MAX];
	nodo = (partita *) malloc(sizeof(partita));
	printf("Inserisci squadra 1:\t");
	fgets(u.squadra1,MAX,stdin);
//	while((ch=getchar())!='\n' && ch!=EOF);
	CancellaAndataACapo(u.squadra1);
	printf("Inserisci squadra 2:\t");
	fgets(u.squadra2,MAX,stdin);
//	while((ch=getchar())!='\n' && ch!=EOF);
	CancellaAndataACapo(u.squadra2);
	printf("Inserisci quota 1 (vincitrice squadra 1):\t");
	scanf("%s",stringa);
        while((ch=getchar())!='\n' && ch!=EOF);
	u.quota1=VerificaFormato(stringa);
	printf("Inserisci quota X (pareggio):\t");
	scanf("%s",stringa);
        while((ch=getchar())!='\n' && ch!=EOF);
	u.quotaX=VerificaFormato(stringa);
	printf("Inserisci quota 2 (vincitrice squadra 2):\t");
	scanf("%s",stringa);
        while((ch=getchar())!='\n' && ch!=EOF);
	u.quota2=VerificaFormato(stringa);
	if(u.quota1 < 1 || u.quotaX < 1 || u.quota2 <1){
		printf("Hai inserito una o più quote sbagliate.\nLe quote devono essere maggiori o uguali a 1");
		printf("\nPartita non inserita\n");
		return;
	}
	u.risultato=-1;
        strcpy(nodo->squadra1,u.squadra1);
        strcpy(nodo->squadra2,u.squadra2);
        nodo->quota1=u.quota1;
        nodo->quotaX=u.quotaX;
	nodo->quota2=u.quota2;
        nodo->codice=u.codice;
	nodo->risultato=u.risultato;
        nodo->next = NULL;
        if(*head==NULL){
        	*head=nodo;
        	return;
         }
        temp = *head;
        while (temp->next != NULL)
                temp = temp->next;
        temp->next = nodo;
	aggiungiCodicePartita(*head);
	return;
}


//funzione che legge i dati di tipo partita da file e li inserisce nella rispettiva lista.
void leggiPartita(partita **p){
        partita N;
        FILE *fd;
        fd=fopen("partite.txt","r");
        if(fd==NULL){
                return;
        }
        while(fgets(N.squadra1,MAX,fd)!=NULL && fgets(N.squadra2,MAX,fd)!=NULL && fscanf(fd,"%f %f %f %d %d\n",&(N.quota1),&(N.quotaX),&(N.quota2),&(N.codice),&(N.risultato)) != EOF) {
 
                CancellaAndataACapo(N.squadra1);
                CancellaAndataACapo(N.squadra2);
                InserisciCodaPartitaLettura(p,N); 
		//l'inserimento avviene in coda, altrimenti le partite vengono inserite nella lista al contrario
        }

        fclose(fd);
}

//funzione che legge i dati degli utenti dal file e li inserisce nella lista degli utenti
void leggiUtente(utente** p){

        utente u;//struttura
        FILE *fd;
        fd=fopen("utenti.txt","r");
        if(fd==NULL){
                return;
        }
        while(fgets(u.nome,MAX,fd)!=NULL && fgets(u.cognome,MAX,fd)!=NULL && fscanf(fd,"%s %s %f\n",u.codicefiscale,u.password,&(u.conto)) != EOF) {
		CancellaAndataACapo(u.nome);
		CancellaAndataACapo(u.cognome);
                InserisciCodaLetturaUtente(p,u);//anche qui in lettura va fatto l'inserimento in coda, altrimenti i dati vengono letti al contrario
        }

        fclose(fd);
}

//funzione che prende il nodo da cancellare di tipo partita e collega il precedente al successivo, dopodichè libera memoria occupata dal nodo
void CancellaPartita(partita **head, partita a){
        partita *succ = *head;
	partita *prec = *head;

        while ((succ != NULL) && (succ->codice != a.codice)){
        	prec = succ;
        	succ = succ->next;
        }
        if (succ == NULL)
        	return;
        if (succ==*head)
		*head =(*head)->next; 
        else
        	prec->next = succ->next;


        free(succ);

}

//funzione che cancella il nodo di tipo utente e collega il precedente al suo successivo
void CancellaUtente(utente **head, utente a){
        utente *succ = *head;
	utente *prec = *head;
        while ((succ != NULL) && (strcmp(succ->codicefiscale,a.codicefiscale)!=0 || strcmp(succ->password,a.password)!=0)){
        	prec= succ;
        	succ=succ->next;
        }
        if (succ == NULL)
       	 	return;
        if (succ==*head)
		*head =(*head)->next;
        else
	        prec->next = succ->next;

        free(succ);

}


//funzione che inserisce la partita giocata dall'utente nella coda della lista di tipo partitagiocata
void InserisciPartitaGiocataInCoda (partitagiocata **head,partitagiocata pg){

	partitagiocata *t=NULL;
	t=(partitagiocata*)malloc(sizeof(partitagiocata));
	strcpy(t->squadra1,pg.squadra1);
	strcpy(t->squadra2,pg.squadra2);
	t->quotascelta=pg.quotascelta;
	t->puntata=pg.puntata;
	t->codice=pg.codice;
	t->codice2=pg.codice2;
	t->quotavinta=pg.quotavinta;
	partitagiocata *temp;// = *head;
	t->next=NULL;
	if (*head==NULL){
		*head=t;
		return;
	}
	temp = *head;
	while (temp->next!=NULL)
		temp = temp->next;

	temp->next=t;

}


//funzione che controlla che le partite della lista partitagiocata siano state simulate dall'admin.
//Viene invocata all'avvio dell'utente
int SchedinaChiusa (partitagiocata *head,partita *head_p){

	partita *p = NULL;

	while (head!=NULL){ // si scorre la lista di tipo partitagiocata per controllare che ogni partitagiocata sia stata simulata o meno
		p=CercaPartitaPerCodice(head_p,head->codice);
		if (p->risultato==-1)
			return 0; //non è ancora stata simulata
		head=head->next;
	}

	return 1; //tutte le partitegiocate sono state simulate.
}


//funzione che aggiunge il codice2 alle partite giocate per lo storico degli utenti.
//Per distinguere le schedine,le multiple avranno lo stesso codice, quelle singole avranno codici diversi
void AggiungiCodiceScommessa (partitagiocata *head,storico *head_s){

	int code;

	if (head_s==NULL) //lista storico vuota. E' la prima partita
		code=1;
	else
		code=head_s->schedina.codice2+1;//altrimenti aumenta di 1 il codice precedente.

	if (head->next==NULL){
		head->codice2=code;
		return;
	}

	while (head->next->next!=NULL)
		head=head->next;
	head->next->codice2=code;
	return;
}
//funzione che torna 1 se la puntata inserita dall'utente è valida, 0 altrimenti
int ControlloPuntata(utente *l,utente u,float puntata){
	if (puntata>l->conto) {
		printf("Hai inserito una puntata troppo alta\n");
		return 0;
	}
	else if(puntata <= 0){
		printf("Hai inserito una puntata non valida\n");
		return 0;
	}
	else return 1;
}

//funzione che copia i risultati in modo ordinato nello storico,
//confronta gli esiti della schedina con gli esiti delle partite simulate
//restituisce all'utente l'eventuale vincita. Restituisce 0 se l'utente ha perso la scommessa
//infine cancella i dati dalla struct partitagiocata
float RiscattaVincite (partitagiocata **head_s,partita *head_p,storico **head){

	float quotatotale=0;
	float soldivinti=0;
	int flag=1;

	partita *p = NULL;
	storico *s = NULL;

	partitagiocata *temp = *head_s;
	storico *newstorico = NULL;
	while (*head_s!=NULL){

		newstorico = (storico*)malloc(sizeof(storico));
        	newstorico->next=NULL;

		p=CercaPartitaPerCodice(head_p,(*head_s)->codice);
		if (p->risultato!=(*head_s)->quotascelta){
			flag=0;
			newstorico->schedina.vincita=0;
		}
		else
			newstorico->schedina.vincita=1;

		quotatotale+=(*head_s)->quotavinta;
		strcpy(newstorico->schedina.squadra1,(*head_s)->squadra1);
		strcpy(newstorico->schedina.squadra2,(*head_s)->squadra2);
		newstorico->schedina.puntata=(*head_s)->puntata;;
		newstorico->schedina.quotagiocata=(*head_s)->quotascelta;
		newstorico->schedina.codice2=(*head_s)->codice2;
		*head_s = (*head_s)->next;
		free(temp);
		temp = *head_s;

		if (*head==NULL)
			*head=newstorico;
		else{
			newstorico->next = *head;
			*head=newstorico;
		}
	}

	if (flag==0)
		soldivinti=0;
	else
		soldivinti = ((*head)->schedina.puntata)*quotatotale;
	s=*head;
	while ((s != NULL) && s->schedina.codice2 == (*head)->schedina.codice2){
		s->schedina.quotariscossa=soldivinti;
		s=s->next;
	}


	return soldivinti;
}

void StampaStorico(storico *head){

	if(head==NULL)
		printf("Lo storico è vuoto.Ancora non hai piazzato alcuna scommessa\n");
	while (head!=NULL){
		printf("Match:\t%s-%s\nPuntata:\t%.2f\nVincita:\t%.2f\nSingola/Multipla:\t%d\n\n",head->schedina.squadra1,head->schedina.squadra2,head->schedina.puntata,head->schedina.quotariscossa,head->schedina.codice2);
		head=head->next;
	}
}
void StampaPartiteGiocate( partitagiocata *head){

	printf("\n\tPARTITE GIOCATE:\n");
	while (head != NULL){
		printf("Match: %s - %s\nEsito:\t%d\nPuntata:\t%.2f\nCodice partita:\t%d\n",head->squadra1,head->squadra2,head->quotascelta,head->puntata,head->codice);
		head=head->next;
	}
}


//funzione che scrive le partite giocate dall'utente su un file apposito e unico per ogni utente.
void SalvaPartiteGiocateSuFile (partitagiocata *head_p,char codicefiscale[]){

	char filename[21];

	strcpy(filename,codicefiscale);
	strcat(filename,".txt");

	FILE *f=NULL;
	f=fopen(filename,"w");

	if (f==NULL)
		return;

	while (head_p!=NULL){

	        fprintf(f,"%s\n%s\n%.2f %d %.2f %d %d\n",head_p->squadra1,head_p->squadra2,head_p->puntata,head_p->quotascelta,head_p->quotavinta,head_p->codice,head_p->codice2);
                head_p=head_p->next;
	}

	fclose(f);
}

//funzione che salva le schedine giocate e chiuse nello storico dell'utente. Anche qui ogni utente ha il suo file apposito.
void SalvaStoricoSuFile (storico *head_s,char codicefiscale[]){
	char filename[24];

        strcpy(filename,codicefiscale);
        strcat(filename,"_st.txt");

	FILE *f=NULL;
        f=fopen(filename,"w");

        if (f==NULL){
                return;
	}

  //      printf("\n%f",head_s->schedina.quotariscossa);
  //      printf("%d\n",head_s->schedina.vincita);

        while (head_s!=NULL){

                fprintf(f,"%s\n%s\n%.2f %.2f %d\n",head_s->schedina.squadra1,head_s->schedina.squadra2,head_s->schedina.puntata,head_s->schedina.quotariscossa,head_s->schedina.codice2);

                head_s=head_s->next;
        }

        fclose(f);
}

//funzione che legge le partite giocate dall'utente e le inserisce in lista
void CaricaPartiteGiocateDaFile (partitagiocata **head,char codicefiscale[]){

	char filename[21];

        strcpy(filename,codicefiscale);
        strcat(filename,".txt");
	partitagiocata p;

        FILE *fd=NULL;
        fd=fopen(filename,"r");

        if(fd==NULL){
                return;
        }
        while(fgets(p.squadra1,MAX,fd)!=NULL && fgets(p.squadra2,MAX,fd)!=NULL && fscanf(fd,"%f %d %f %d %d\n",&(p.puntata),&(p.quotascelta),&(p.quotavinta),&(p.codice),&(p.codice2))!=EOF){
		CancellaAndataACapo(p.squadra1);
                CancellaAndataACapo(p.squadra2);
                InserisciPartitaGiocataInCoda(head,p);
		//l'inserimento avviene tramite lettura in coda per evitare che le partite vengano lette al contrario
	}

	fclose(fd);
}

//funzione che inserisce gli elementi in una pila
/*
void InserisciStoricoInPila(storico **head_s, storico s){
	storico *nuovo=NULL;
	nuovo=(storico*)malloc(sizeof(storico));
	if(nuovo==NULL)
		printf("Memoria piena\n");
	else{
		strcpy(nuovo->squadra1,s.squadra1);
		strcpy(nuovo->squadra2,s.squadra2);
		nuovo->puntata=s.puntata;
		nuovo->quotagiocata=s.quotagiocata;
		nuovo->quotariscossa=s.quotariscossa;
		nuovo->vincita=s.vincita;
		nuovo->codice2=s.codice2;
		nuovo->next=*head_s;
		*head_s=nuovo;
	}
}

storico *InvertiPila (storico **head_s){

	storico *stack=NULL;
	storico *nuovo=NULL;

	while (*head_s!=NULL){

		nuovo=(*head_s);
		nuovo->next=stack;

		stack=nuovo;

		*head_s=(*head_s)->next;

	}

	return stack;
}
*/
void InserisciStoricoInCoda( storico **head_s,storico s){


	storico *temp;
	storico *st;
	st=(storico *) malloc(sizeof(storico));
	strcpy(st->schedina.squadra1,s.schedina.squadra1);
	strcpy(st->schedina.squadra2,s.schedina.squadra2);
	st->schedina.puntata = s.schedina.puntata;
	st->schedina.quotariscossa=s.schedina.quotariscossa;
	st->schedina.vincita=s.schedina.vincita;
	st->schedina.codice2=s.schedina.codice2;
	st->next = NULL;
	if(*head_s==NULL){ // lista vuota
		*head_s=st;
		return;
	}

	temp = *head_s;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = st;
}

//funzione che legge lo storico dell'utente che si è collegato.
void CaricaStoricoDaFile (storico **head_s,char codicefiscale[]){

        char filename[24];

        strcpy(filename,codicefiscale);
        strcat(filename,"_st.txt");

        storico s;

        FILE *fd=NULL;
        fd=fopen(filename,"r");

        if(fd==NULL){
                return;
        }
        while(fgets(s.schedina.squadra1,MAX,fd)!=NULL && fgets(s.schedina.squadra2,MAX,fd)!=NULL && fscanf(fd,"%f %f %d\n",&(s.schedina.puntata),&(s.schedina.quotariscossa),&(s.schedina.codice2))!=EOF){
                CancellaAndataACapo(s.schedina.squadra1);
                CancellaAndataACapo(s.schedina.squadra2);
                InserisciStoricoInCoda(head_s,s);
//		*head_s=InvertiPila(head_s);

        }

        fclose(fd);
}

//funzione che  scrive le informazioni dell'utente su file
void scriviUtente(utente* u){

        FILE *fd=NULL;
        fd=fopen("utenti.txt","w");
        if (fd==NULL){
                return;
        }
        while(u!=NULL){
                fprintf(fd,"%s\n%s\n%s %s %.2f\n",u->nome,u->cognome,u->codicefiscale,u->password,u->conto);
                u=u->next;
        }
        fclose(fd);
}

//funzione che scrive su file le informazioni relative alla lista delle partite
void scriviPartita(partita* p){
        FILE *fd=NULL;
        fd=fopen("partite.txt","w");
        if (fd==NULL){
                return;
        }
        while(p!=NULL){
                fprintf(fd,"%s\n%s\n%.2f %.2f %.2f %d %d\n",p->squadra1,p->squadra2,p->quota1,p->quotaX,p->quota2,p->codice,p->risultato);
                p=p->next;
        }
        fclose(fd);
}


//funzione che aggiorna il saldo dell'utente.
void AggiornaVincitaUtente(utente **l, float soldivinti, float puntata){


	if (soldivinti != 0)
		(*l)->conto=(*l)->conto+soldivinti;
	else
		(*l)->conto=(*l)->conto-puntata;

}


void LiberaMemoriaUtente (utente *u){
	storico *s=u->head_s;
	partitagiocata *p=u->head_p;
 
	while (u->head_p !=NULL) {
		u->head_p=u->head_p->next;
		free(p);
		p=u->head_p;
        }
	while (u->head_s !=NULL) {
		u->head_s=u->head_s->next;
		free(s);
		s=u->head_s;
    	}
}

void RiscuotiConto(utente **l){

	printf("\nIl tuo conto è di %.2f euro.\n Premi invio per riscuotere il denaro\n",(*l)->conto);
	getchar();
        (*l)->conto = 0;
	printf("Operazione andata a buon fine. Il tuo conto è di %.2f euro\n",(*l)->conto);
}

void AggiungiDenaro(utente **l){

        char stringa[MAX];
	float liquidi;
        printf("\nInserisci importo in euro:\t");
        scanf("%s",stringa);
        while((ch=getchar())!='\n' && ch!=EOF);
	liquidi=VerificaFormato(stringa);
	if (liquidi==-1){
		printf("Operazione non riuscita\n");
		return;
	}
	(*l)->conto=(*l)->conto+liquidi;
        printf("Operazione andata a buon fine. Il tuo saldo è di %.2f euro\n",(*l)->conto);
}


//menù utente. Viene invocata se l'accesso è stato effettuato con successo, oppure dopo la registrazione dell'utente
void menuUtente(partita **p,utente *l){
	int scelta=-1;
	int flag;
	int i=-1;
	int code;
	float soldivinti;
	utente u;
	partita *selezione = *p;
	partitagiocata pg;
	pg.puntata=0;
	CaricaPartiteGiocateDaFile(&(l->head_p),l->codicefiscale);
	CaricaStoricoDaFile(&(l->head_s),l->codicefiscale);

	if (l->head_p!=NULL && SchedinaChiusa(l->head_p,*p)){

		StampaPartiteGiocate(l->head_p);
		printf("\nLa scommessa è stata chiusa! Ti verranno mostrati ora i risultati.\nPremi Invio per scoprire il risultato\n");
		getchar();
		soldivinti=RiscattaVincite(&(l->head_p),*p,&(l->head_s));
		if(soldivinti!=0)
			printf("\nComplimenti, hai vinto %.2f euro!\n",soldivinti);
		else
			printf("\nHai perso.\n");

		AggiornaVincitaUtente(&l,soldivinti,l->head_s->schedina.puntata);
		//printf("%f\n",l->head_s->puntata);
	}
	printf("\nCiao %s!\n",l->nome);
	printf("Accesso effettuato con successo!");
        while (scelta!=0) {
                printf("\n\tMenù Utente:");
                printf("\n\t0.Esci");
                printf("\n\t1.Visualizza Partite");
                printf("\n\t2.Scommetti");
                printf("\n\t3.Visualizza Storico");
                printf("\n\t4.Cancella Profilo");
		printf("\n\t5.Riscuoti Denaro");
		printf("\n\t6.Aggiungi Denaro");
		printf("\n\t7.Visualizza Denaro\n");
                scanf("%d",&scelta);
                while((ch=getchar())!='\n' && ch!=EOF);
                switch (scelta) {
                	case 0: SalvaPartiteGiocateSuFile(l->head_p,l->codicefiscale);
			        SalvaStoricoSuFile(l->head_s,l->codicefiscale);
				LiberaMemoriaUtente(l);
				return;
                        case 1: StampaPartita(*p); //visualizzazione partite
                                break;
                        case 2: scelta=2;
				selezione=NULL;
				code=-1;
				pg.codice=-1;
				if (l->head_p==NULL){ //nessuna pg.
					printf("Inserire quota da puntare:\t");
                                	scanf("%f",&(pg.puntata));
                                	while (ch=getchar()!='\n' && ch!=EOF);
					if (ControlloPuntata(l,u,pg.puntata)==0){
						break; //errore nella puntata
						}
					while (scelta==2){

						printf("Scegli la partita su cui scommettere tramite codice identificativo:\n");
						printf("Codice:\t");
						scanf("%d",&code);
						while (ch=getchar()!='\n' && ch!=EOF);
						selezione=CercaPartitaPerCodice(*p,code);
						pg.codice=code;//aggiorno il codice delle partite giocate per il controllo della schedina che si sta compilando
						if (selezione==NULL)
							printf("La partita cercata non esiste\n");
						else if (selezione->risultato!=-1)
							printf("La partita cercata è già stata simulata!\n");
						else if (CercaPartitaPerCodicePG(l->head_p,pg.codice)!=NULL)
							printf("Hai già scommesso su questa partita\n");
						else{
							pg.codice=selezione->codice;
							strcpy(pg.squadra1,selezione->squadra1);
							strcpy(pg.squadra2,selezione->squadra2);
							pg.quotascelta=0;
							while (pg.quotascelta != 1 && pg.quotascelta != 2 && pg.quotascelta != 3){
								printf("\t1.Vittoria %s\n",pg.squadra1);
								printf("\t2.Vittoria %s\n",pg.squadra2);
								printf("\t3.Pareggio\n");
								printf("Inserire esito:\t");
								scanf("%d",&(pg.quotascelta));
		                                        	while (ch=getchar()!='\n' && ch!=EOF);
								switch(pg.quotascelta){
									case 1: pg.quotavinta=selezione->quota1;
										break;
									case 2:	pg.quotavinta=selezione->quota2;
										break;
									case 3: pg.quotavinta=selezione->quotaX;
										break;
									default:printf("Hai scelto un'opzione sbagliata\n");
										break;
								}
							}

								InserisciPartitaGiocataInCoda (&(l->head_p),pg);
                                                        	AggiungiCodiceScommessa(l->head_p,l->head_s);
								StampaPartiteGiocate(l->head_p);



						}
						scelta=-1;
						while(scelta != 1 && scelta != 2) {

	                                                printf("\nVuoi aggiungere un'altra partita?\n");
							printf("\t1.No\n\t2.Si\n\t");
							scanf("%d",&scelta);
		                                 	while (ch=getchar()!='\n' && ch!=EOF);
							pg.quotascelta=0;
							selezione=NULL;
						}
					}
                               		break;
				}
				else{
					printf("\nPrima di scommettere nuovamente la scommessa in corso deve essere chiusa!\n");
					break;
				}
                        case 3: StampaStorico(l->head_s);
                                break;
                        case 4: printf("Per questa opzione è necessaria un'ulteriore conferma:");
				printf("\nConferma Codice fiscale:\t");
                                scanf("%s",u.codicefiscale);
                                printf("Conferma password:\t");
                                scanf("%s",u.password);
                                flag=trovaUtentePerCancellazione(l,u); //se è 1 l'ha trovata
                                if (flag==1){

					LiberaMemoriaUtente(l); //cancello dati utente
					EliminaFileUtente(l->codicefiscale);
					CancellaUtente(&l,u);
					printf("\nCancellazione effettuata con successo.\nUscita dal programma\n");
					exit(1);
				}
                                else printf("La cancellazione NON è andata a buon fine\n");
                                break;
			case 5: RiscuotiConto(&l);
				break;
			case 6: AggiungiDenaro(&l);
				break;
			case 7: printf("\nAttualmente hai %.2f euro.\n",l->conto);
				break;
                        default: printf("Errore nella scelta, devi scegliere un numero tra 0 e 7.\n");
                                break;
                }
        }

}

//funzione di registrazione utente. Chiama la funzione di inserimento, dopodichè chiama il menù se tutto è andato bene
void RegistraUtente(utente **head, utente *l, utente u,partita **p){
	utente* flag;
		flag=InserisciUtenteInTesta(head,l,u);
		if (flag!=NULL) menuUtente(p,flag);
		else printf("Registrazione fallita.\n");
}

//funzione per l'accesso dell'utente, se è andato tutto bene chiama il menù utente, altrimenti torna al menù di partenza
void accediUtente(utente **u, utente n,partita **p){
	utente* flag;
	printf("Inserisci codice fiscale:\t");
	scanf("%s",n.codicefiscale);
        while((ch=getchar())!='\n' && ch!=EOF);
	printf("Inserisci password:\t");
	scanf("%s",n.password);
        while((ch=getchar())!='\n' && ch!=EOF);

	flag=trovaUtentePuntatore(*u,n);
	if (flag!=NULL)
		menuUtente(p,flag);
	else {
		printf("Nome utente o password sbagliati\n");
		return;
	}

}

//menù amministratore. Al suo interno viene fatta anche la verifica delle credenziali
void accediAmministratore(utente *listautenti,partita **p, partita m,utente *head){
        amministratore user;
	utente *cerca;
	utente v;
        partita u;

        //credenziali inserite in input
        printf("Username:\t");
        scanf("%s",user.nomeamm);
        while((ch=getchar())!='\n' && ch!=EOF);
        printf("Password:\t");
        scanf("%s",user.pwamm);
        while((ch=getchar())!='\n' && ch!=EOF);


        //credenziali per l'accesso
        int v1=strcmp(user.nomeamm,"admin");
        int v2=strcmp(user.pwamm,"admin");
        int scelta=-1;
	partita *temp;
        if (v1==0 && v2==0){
		printf("Login effettuato con successo!\n");
                while (scelta!=0){
			printf("\nEcco il menù amministratore:\n");
 	                printf("\t0.Torna al Menù principale\n");
 	                printf("\t1.Visualizza Partite\n");
			printf("\t2.Aggiungi Partita\n");
			printf("\t3.Cancella Partita\n");
 	                printf("\t4.Simula Partite\n");
        	        printf("\t5.Visualizza Storico\n");
			printf("\t6.Visualizza Utenti\n");
                        scanf("%d",&scelta);
		        while((ch=getchar())!='\n' && ch!=EOF);

                        switch (scelta) {
                                case 0: break;
                                case 1:	StampaPartita(*p);
					break;
                                case 2: InserisciCodaPartita(p,m);
					break;
				case 3: printf("\nATTENZIONE: FUNZIONE RISCHIOSA.\nSI RACCOMANDA DI USARLA SOLO SE SI E' CERTI CHE TUTTI GLI UTENTI NON ABBIANO LA PARTITA DA CANCELLARE IN SOSPESO\n\n");
					printf("Scrivi il codice della partita che vuoi cancellare:\t");
					scanf("%d",&(u.codice));
		                        while((ch=getchar())!='\n' && ch!=EOF);
					temp=trovaPartitaPuntatore(*p,u);
					if (temp!= NULL)
						CancellaPartita(p,u);//funzioni simula partita
                                        else printf("Non è stata trovata alcuna partita associata alle due squadre\n");
					break;
                                case 4: SimulaPartita(*p);
                                        break;
				case 5: printf("\nScrivi il codice fiscale dell'utente:\t");
                                        scanf("%s",v.codicefiscale);
                                        while((ch=getchar())!='\n' && ch!=EOF);
                                        cerca=trovaUtenteCodiceFiscalePtr(listautenti,v);
					StampaUtente(cerca);
					if (cerca==NULL)
						break;
					CaricaStoricoDaFile(&(cerca->head_s),v.codicefiscale);
					StampaStorico(cerca->head_s);//visualizza storico utente scelto
					break;
				case 6:	StampaUtente(head);
					break;
				default: printf("\nErrore di scelta: devi scegliere un numero tra 0 e 6\n");
                        }
                }
        }
        else {
                printf("Nome utente o password errati\n");
        }
}



//il main contiene il menù di partenza del programma, in base alla scelta chiama gli altri menù
int main(){

	utente n;
	utente *u=NULL;
	partita m;
	partita *p=NULL;
	int scelta=-1;
	leggiUtente(&u);
	leggiPartita(&p);
	printf("Benevuto nella piattaforma di scommesse!\n");
	while(scelta!=0){
                printf("\n\t0.Esci");
                printf("\n\t1.Registrati");
                printf("\n\t2.Accedi");
                printf("\n\t3.Accedi come Amministratore\n");
                scanf("%d",&scelta);
	        while((ch=getchar())!='\n' && ch!=EOF);
                switch (scelta){
                        case 0: break;
                        case 1: RegistraUtente(&u,u,n,&p);
                                break;
                        case 2: accediUtente(&u,n,&p);
                                break;
                        case 3: accediAmministratore(u,&p,m,u);
                                break;
                        default: printf("\nErrore:Devi scegliere un numero tra 0 e 3\n");
                        }
        }
        printf("\n\tArrivederci!\n");

	scriviUtente(u);
	scriviPartita(p);

}
