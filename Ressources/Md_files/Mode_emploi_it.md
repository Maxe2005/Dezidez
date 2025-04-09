# Come utilizzarlo:

- Le funzioni utilizzabili: sin(), cos(), tan(), sqrt(), exp(), log(), abs(), sinc(), intero(),
- Per i valori negativi utilizzare sempre le parentesi es. sin(-2) => sin((-2)) o -4 => (-4)
- Per le potenze utilizzare '**' es. x alla potenza y => x**y
- Per i numeri decimali utilizzare '.' e non ','
- tutte le lettere maiuscole sono considerate come minuscole

## 1. Home:

Lo schermo iniziale facilita l'accesso alle funzionalità come il manuale d'uso, i ringraziamenti o l'avvio della modalità di visualizzazione, con una versione 3D in sviluppo.

## 2. Ritorno:

Per tornare ai menu precedenti senza riavviare il programma, fare clic sull'icona della casa o utilizzare il tasto "Backspace" o "Indietro".

## 3. Barra superiore:

Consente di visualizzare e modificare fino a 20 espressioni con funzionalità interattive.

   - Cambio colore: Selezionare un nuovo colore in tempo reale tramite l'icona della ruota.

   - Nascondi una funzione: Nascondere o visualizzare una funzione in base alle esigenze in tempo reale.

   - Elimina una funzione: Fare clic sull'icona del cestino per eliminare un'espressione.

   - Modifica i limiti: Regolare i limiti di un'espressione tramite una finestra di testo per maggiore flessibilità.

## 4. Visualizzazione delle funzioni:

La valutazione delle funzioni aggiorna dinamicamente la loro rappresentazione grafica.

   - Zoom: Utilizzare la rotellina del mouse per regolare la visualizzazione del grafico.

   - Centratura: Reimpostare la visualizzazione con il tasto "C" o il pulsante "Ricentra" in arrivo.

   - Traslazione del riferimento: Trascinare sul grafico con un clic destro per esplorare la curva.

   - Valutatore in x: Attivare con un clic sinistro per visualizzare le coordinate di un punto sulla curva.

# Le nostre strutture:

Per sviluppare il nostro grafico abbiamo utilizzato diverse strutture, in particolare per codificare le funzioni sotto forma di albero e per restituire i vari errori possibili. Questa parte ci è stata fornita all'inizio del progetto, vi abbiamo apportato alcune modifiche che vi presenteremo.
Il file che ci è stato fornito conteneva 6 strutture diverse:

- albero: prende come parametro un token (Typejeton), un albero successivo e un albero precedente.
- typejeton: prende come parametro il tipo di valore del token (typelexem) e il valore del token (typevaleur).
- typevaleur: prende come parametro se il valore del token, che può essere un reale, una funzione, un operatore e abbiamo deciso di aggiungere anche la possibilità di includere una variabile (char) che assumerà come valore x o y per poter eseguire il grafico di una funzione a due variabili.
- typefonction: prende come parametro una funzione matematica (exp, sin, cos,…)
- typeoperateur: prende come parametro un operatore (più, meno; per, div, potenza)
- typelexem: prende come parametro il nome del tipo di valore del token.

Abbiamo aggiunto anche una struttura code_erreur che associa un nome di errore a un codice di errore, ad esempio: RACINE_NEGATIVE=301, è impossibile calcolare la radice di un numero negativo, quindi se l'utente richiede di farlo, il codice restituirà questo errore.

# Spiegazione della funzione Analyse_Lexicale

La funzione 'Analyse_Lexicale' trasforma un'espressione matematica in forma di stringa in un array di token. Permette di scomporre l'espressione in elementi che possono essere facilmente manipolati per operazioni di analisi sintattica successive.

Questo processo di tokenizzazione è un passaggio preliminare fondamentale per la futura analisi ed valutazione dell'espressione matematica.

## Funzionamento globale

La funzione prende quattro parametri:
* 'TabToken': array che conterrà i token generati
* 'Expression': la stringa che rappresenta l'espressione matematica
* 'erreur': puntatore a una variabile che conterrà il codice di errore in caso di problema
* 'Dimension': indica se l'espressione è a 1 o 2 variabili (0 per una dimensione, 1 per due dimensioni)

**Importante**: L'analizzatore lessicale considera le maiuscole come minuscole.

**Importante**: Per i numeri decimali, utilizzare punti e non virgole.

## Sottofunzioni utilizzate

La funzione 'Analyse_Lexicale' segue una sequenza di elaborazione in tre fasi principali, ciascuna utilizzando una sottofunzione dedicata:

1. 'ExpressionSansLesEspaces'
   Questa funzione rimuove tutti gli spazi dall'espressione matematica.

2. 'MultiplicationImplicite'
   Questa funzione rileva e aggiunge gli operatori di moltiplicazione impliciti. Ad esempio, trasforma '2x' in '2*x'.

3. 'DecompositionToken'
   Questa funzione scompone l'espressione in token individuali e li memorizza nell'array 'TabToken', tenendo conto del parametro di dimensione.

## Processo dettagliato con esempi

Prendiamo l'espressione '"3 + cos(5x)"' come esempio.

### Fase 1: Rimozione degli spazi
'ExpressionSansLesEspaces' trasforma '"3 + cos(5x)"' in '"3+cos(5x)"'.

### Fase 2: Aggiunta delle moltiplicazioni implicite
'MultiplicationImplicite' rileva le moltiplicazioni implicite tra numeri e variabili. Trasforma '"3+cos(5x)"' in '"3+cos(5*x)"'.

### Fase 3: Creazione dei token
'DecompositionToken' scompone l'espressione in token:
* '3' → Token di tipo REEL con valore 3.0
* '+' → Token di tipo OPERATEUR con valore PLUS
* 'cos' → Token di tipo FONCTION con valore COS
* '(' → Token di tipo PAR_OUV
* '5' → Token di tipo REEL con valore 5.0
* '*' → Token di tipo OPERATEUR con valore FOIS
* 'x' → Token di tipo VARIABLE con valore 'x'
* ')' → Token di tipo PAR_FERM
* Aggiunta di un token FIN alla fine

## Come 'DecompositionToken' identifica i token

La funzione 'DecompositionToken' analizza carattere per carattere e utilizza diverse sottofunzioni per identificare correttamente ogni token:

* 'TokenReelPositif': Crea un token per i numeri positivi
* 'TokenReelNegatif': Gestisce i numeri negativi (es: "(-2.5)")
* 'TokenOperateur': Identifica gli operatori (+, -, *, /, **)
* 'TokenFonction': Identifica le funzioni (sin, cos, abs, ecc.)
* 'TokenVariable': Identifica le variabili (x, y) in base al parametro 'Dimension'

## Gestione delle variabili in base alla dimensione

La funzione 'TokenVariable' utilizza il parametro 'Dimension' per sapere quali variabili sono consentite:
* Se 'Dimension' = 0: è consentita solo la variabile 'x'
* Se 'Dimension' = 1: sono consentite le variabili 'x' e 'y'

## Gestione degli errori

La funzione segnala diversi tipi di errori possibili:
* FONCTION_INCONNUE (101): Funzione non riconosciuta
* NOMBRE_INVALIDE (102): Formato di numero errato (es: "1.2.3")
* CARACTERE_INCONNUE (103): Carattere non riconosciuto
* VARIABLE_INCONNUE (104): Variabile non consentita in base alla dimensione

## Esempio completo

Per l'espressione '"2x + sin(3.5)"' con 'Dimension' = 0:

1. Rimozione degli spazi: '"2x+sin(3.5)"'
2. Aggiunta delle moltiplicazioni implicite: '"2*x+sin(3.5)"'
3. Creazione dei token:
   * REEL (2.0)
   * OPERATEUR (FOIS)
   * VARIABLE ('x')
   * OPERATEUR (PLUS)
   * FONCTION (SIN)
   * PAR_OUV
   * REEL (3.5)
   * PAR_FERM
   * FIN

# Analisi sintattica

### createEmptyNode

Utilità: Restituisce un albero vuoto

Prende come parametro:

-Niente

Funzionamento della funzione:

-Crea fg e fd vuoti

### buildExpressionTree

Utilità: Costruisce ricorsivamente l'albero a partire da un array di token

Prende come parametro:

-un array di token
-l'indice della prima casella da analizzare
-l'indice dell'ultima casella da analizzare
-l'errore recuperato dall'ultima chiamata ricorsiva (l'errore predefinito è 0).

Funzionamento della funzione:

Se è presente un errore nell'ultima chiamata ricorsiva: restituisce un albero vuoto alla fine del ramo

Se l'indice di inizio > indice di fine: restituisce errore = MEMBRE_VIDE

Se c'è un operatore: scissione dell'albero sull'operatore in fg e fd.

Altrimenti, restituisce un errore nei seguenti casi

- PROBLEME_PARENTHESES_FONCTIONS: manca una parentesi dopo una funzione o se la parentesi non si chiude
- MEMBRE_VIDE: se un operatore non è circondato da oggetti trattabili, se le parentesi sono vuote
- PARENTHESE_FERMEE_1_ER_JETON: se una parentesi è chiusa senza essere stata aperta
- PROBLEMES_NOMBRE_PARENTHESES: se il numero di parentesi aperte è diverso dal numero di parentesi chiuse
- PROBLEME_APRES_REEL_OU_VARIABLE: Supposto impossibile
- ABSENCE_FIN: se il token "FIN" è mancante

### findLowestPriorityOperator

Utilità: Restituisce l'operatore su cui effettuare la scissione tra fg e fd.

Prende come parametro:

-un array di token
-l'indice della prima casella da analizzare
-l'indice dell'ultima casella da analizzare
-l'errore recuperato dall'ultima chiamata ricorsiva (l'errore predefinito è 0).

Funzionamento della funzione:

Inizializza la profondità di complessità delle parentesi a
Imposta l'indice di scissione a -1 per impostazione predefinita.

indiceOperateurMinimal = -1 se non ci sono operatori nella funzione
prioritéOperateurMinimal rappresenta l'operatore su cui separare l'array di token.

Scorre l'array casella per casella e verifica se c'è un operatore

Se la casella è un operatore, la profondità attuale delle parentesi è nulla e l'operatore della casella attuale è più adatto alla scissione:
indiceOperateurMinimal = indice attuale
prioriteOperateurMinimal = casella attuale

Se la casella è una PAR_OUV:
la profondità legata alle parentesi aumenta

Se la casella è una PAR_FERM:
la profondità legata alle parentesi diminuisce

Se la profondità delle parentesi non è nulla (=> numero di PAR_OUV != numero di PAR_FERM
errore = PROBLEMES_NOMBRE_PARENTHESES

### findExpressionLength

Utilità: Restituisce la lunghezza dell'array

Prende come parametro:

-Un array di token

Funzionamento della funzione:

Scorre l'array e se la casella è una casella fine, la funzione si ferma
Se nessun FIN viene trovato, allora fine = -1

### checkParenthesesBalance

Utilità: Restituisce un booleano relativo all'equilibrio del numero di parentesi.

Prende come parametro:

-indice di inizio dell'array da analizzare
-indice di fine dell'array da analizzare
-l'array di token

Funzionamento della funzione:

Scorre l'array e conta il numero di PAR_OUV e di PAR_FERM

### buildSyntaxTree

Utilità: Costruisce l'albero sintattico completo a partire da una sequenza di token

Prende come parametro:

-l'array di token
-l'ultimo errore restituito da buildExpressionTree (0 per la prima chiamata)

Funzionamento della funzione:

Verifica se il token FIN esiste
Se esiste, l'albero si costruisce chiamando buildExpressionTree
Altrimenti errore = ABSENCE_FIN

# PARTE EVALUATOR:

L'obiettivo dell'evaluator è calcolare una funzione (che è sotto forma di albero) a partire dalle variabili x e y date. Il suo ruolo è essenziale per tracciare le curve, poiché permette di ottenere i punti da visualizzare sul grafico. Il nostro codice si divide in due funzioni principali: evaluateur, la nostra funzione principale che interpreta l'albero, e calculer_fonction, una funzione ausiliaria che applica funzioni matematiche standard.
Funzione evaluateur:

Ha l'obiettivo di decodificare l'albero di input. Prende in input:
- La funzione sotto forma di albero
- I valori di x e y
- Il puntatore code_erreur.
Restituisce il risultato della funzione di input, calcolato a partire dalle variabili x e y.
Questa funzione è ricorsiva e possiede quattro casi di arresto. Il primo è se il puntatore code_erreur cambia valore, il che significa che è presente un errore nella funzione da calcolare e quindi bisogna interrompere la decodifica dell'albero una volta rilevato un errore. Il secondo caso di arresto è se il token principale dell'albero non possiede un tipo. Infine, la funzione si fermerà se il token principale dell'albero è un reale o una variabile, il che significa che si è arrivati a una foglia dell'albero.
Una volta fatto ciò, si verifica con una funzione switch il tipo del token principale dell'albero di input, se si tratta di:

- Una variabile: restituisce il valore della variabile recuperato in input alla funzione.
- Un reale: restituisce il valore del reale.
- Un operatore: si utilizza un metodo di divide et impera per calcolare l'operazione.
- Una funzione: si utilizza quindi la seconda funzione del nostro programma.
Funzione calculer_fonction:

Questa funzione prende in input il tipo della funzione matematica f, la variabile x su cui applicare f e il puntatore code_erreur. Restituisce l'applicazione f(x). I tipi delle funzioni possibili sono le funzioni matematiche universali come sin, cos o ln.
Si verifica prima con una funzione switch di quale funzione si tratta, poi si controlla che non ci siano errori, prima di calcolare f(x).
In caso di errore:
In caso di errore (es: divisione per zero, logaritmo di un numero negativo...), la funzione restituisce un valore errato e modifica il puntatore code_erreur per segnalare il problema. Questo permette, ad esempio, di interrompere la valutazione o di visualizzare un messaggio appropriato all'utente.

# Visualizzazione grafica

## 1. Home

Lo schermo iniziale facilita l'interazione con l'utente. Dà accesso alle diverse funzionalità, come la consultazione del manuale d'uso, la visualizzazione dei ringraziamenti o l'avvio di una modalità di visualizzazione. Attualmente, il programma offre una visualizzazione 2D, ma una versione 3D per le funzioni a due variabili è in fase di sviluppo da parte dei nostri ingegneri.

## 2. Funzionalità di ritorno

Una funzionalità di ritorno permette all'utente di tornare ai menu precedenti senza riavviare il programma. È possibile tornare alla schermata iniziale facendo clic sull'icona a forma di casa nei menu di testo o premendo il tasto "Backspace" o "Indietro" della tastiera.

## 3. Barra superiore

L'utente può visualizzare fino a 20 espressioni. Per impostazione predefinita, viene presentata una funzione di base per ogni caso. L'utente può quindi fare clic su un'espressione per modificarla. Inoltre, quando scrive in una delle tre aree di testo della barra superiore, può interagire con le seguenti funzioni:

### 3.1 Cambio colore

Il programma offre la possibilità di personalizzare il colore delle funzioni tramite un menu di selezione del colore. Questo menu si attiva facendo clic sull'icona della ruota dei colori. Una volta aperto, permette di cambiare il colore delle funzioni in tempo reale (anche mantenendo il clic sinistro).

### 3.2 Nascondi una funzione

Questa funzionalità permette all'utente di nascondere o visualizzare le funzioni di sua scelta in tempo reale, in base alle proprie esigenze.

### 3.3 Elimina una funzione

L'utente può eliminare le espressioni facendo clic sull'icona del cestino con il tasto sinistro del mouse.

### 3.4 Modifica i limiti

Una finestra di testo permette all'utente di modificare i limiti di un'espressione in tempo reale, per risparmiare tempo e avere maggiore flessibilità.

## 4. Elaborazione dell'espressione

Una volta inserita l'espressione, viene sottoposta a un'analisi lessicale e poi sintattica, per prepararne l'elaborazione successiva.

## 5. Visualizzazione delle funzioni

L'albero generato durante l'analisi viene utilizzato per associare a ogni valore dell'intervallo la sua immagine corrispondente sullo schermo, grazie alla funzione di valutazione. Questo processo si ripete in tempo reale, permettendo così aggiornamenti dinamici della visualizzazione.

### 5.1 Zoom

Per una migliore analisi grafica, il programma permette di effettuare lo zoom in avanti e indietro utilizzando la rotellina del mouse. Questo modifica l'intervallo visualizzato della funzione.

### 5.2 Centratura

La funzionalità di centratura permette di reimpostare rapidamente la visualizzazione. Basta premere il tasto "C" della tastiera. Presto verrà aggiunto un pulsante "Ricentra" sulla barra destra della finestra.

### 5.3 Traslazione del riferimento

Premendo il tasto "clic destro" del mouse, l'utente attiva una modalità di "scorrimento" che gli permette di esplorare il grafico in modo intuitivo.

### 5.4 Valutatore in x

Il valutatore, attivato con un clic sinistro, permette all'utente di visualizzare due rette che intersecano la prima curva dell'elenco delle espressioni. Queste rette seguono la posizione del mouse sull'asse delle ascisse. Una visualizzazione fornisce il valore di 'x' e il valore della funzione nel punto di clic.
