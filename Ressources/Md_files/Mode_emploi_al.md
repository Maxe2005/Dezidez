
# Wie man es benutzt:

- Nutzbare Funktionen: sin(), cos(), tan(), sqrt(), exp(), log(), abs(), sinc(), integer(),
- Für negative Werte immer Klammern verwenden, z. B.: sin(-2) => sin((-2)) oder -4 => (-4)
- Für Potenzen ** verwenden, z. B.: x hoch y => x**y
- Für Dezimalzahlen '.' statt ',' verwenden
- Alle Großbuchstaben werden als Kleinbuchstaben betrachtet

## 1. Startseite:

Der Startbildschirm erleichtert den Zugriff auf Funktionen wie das Benutzerhandbuch, Danksagungen oder das Starten des Anzeigemodus, wobei eine 3D-Version in Entwicklung ist.

## 2. Zurück:

Um zu vorherigen Menüs zurückzukehren, ohne das Programm neu zu starten, entweder auf das Haussymbol klicken oder die "Rücktaste" oder "Return"-Taste verwenden.

## 3. Obere Leiste:

Ermöglicht das Anzeigen und Ändern von bis zu 20 Ausdrücken mit interaktiven Funktionen.

- Farbwechsel: Eine neue Farbe in Echtzeit über das Farbrad-Symbol auswählen.
- Funktion ausblenden: Eine Funktion je nach Bedarf in Echtzeit ausblenden oder anzeigen.
- Funktion löschen: Auf das Papierkorbsymbol klicken, um einen Ausdruck zu löschen.
- Grenzen ändern: Die Grenzen eines Ausdrucks über ein Textfenster für mehr Flexibilität anpassen.

## 4. Funktionsanzeige:

Die Auswertung von Funktionen aktualisiert dynamisch ihre grafische Darstellung.

- Zoom: Das Mausrad verwenden, um die Grafikanzeige anzupassen.
- Zentrieren: Die Anzeige mit der Taste "C" oder der Schaltfläche "Neu zentrieren" (kommt bald) zurücksetzen.
- Achsenverschiebung: Mit einem Rechtsklick auf den Graphen ziehen, um die Kurve zu durchlaufen.
- x-Evaluator: Mit einem Linksklick aktivieren, um die Koordinaten eines Punktes auf der Kurve anzuzeigen.

# Unsere Strukturen:

Um unseren Graphenzeichner zu entwickeln, haben wir mehrere Strukturen verwendet, insbesondere um Funktionen als Baum zu codieren, aber auch um die verschiedenen möglichen Fehler zurückzugeben. Dieser Teil wurde uns zu Beginn des Projekts zur Verfügung gestellt; wir haben einige Änderungen vorgenommen, die wir Ihnen vorstellen werden.
Die uns zur Verfügung gestellte Datei enthielt 6 verschiedene Strukturen:

- Baum: nimmt als Parameter ein Token (TokenType), einen nächsten Baum und einen vorherigen Baum.
- tokentype: nimmt als Parameter den Werttyp des Tokens (lexemeType) und den Wert des Tokens (valueType).
- valuetype: nimmt als Parameter, ob der Wert des Tokens, der ein Real, eine Funktion, ein Operator sein kann, und wir haben beschlossen, auch die Möglichkeit hinzuzufügen, eine Variable (char) einzuschließen, die x oder y als ihren Wert annimmt, um eine Funktion mit zwei Variablen grafisch darstellen zu können.
- functiontype: nimmt als Parameter eine mathematische Funktion (exp, sin, cos, ...) an.
- operatortype: nimmt als Parameter einen Operator (plus, minus, mal, dividieren, Potenz) an.
- lexemetype: nimmt als Parameter den Namen des Werttyps des Tokens an.

Wir haben auch eine error_code-Struktur hinzugefügt, die einen Fehlernamen mit einem Fehlercode verknüpft, zum Beispiel: NEGATIVE_WURZEL=301, es ist unmöglich, die Wurzel einer negativen Zahl zu berechnen. Wenn der Benutzer also als Eingabe anfordert, eine zu machen, gibt der Code diesen Fehler zurück.

# Erklärung der Funktion Lexikalische_Analyse

Die Funktion 'Lexikalische_Analyse' wandelt einen mathematischen Ausdruck in Form einer Zeichenkette in ein Array von Token um. Sie zerlegt den Ausdruck in Elemente, die für nachfolgende Parsing-Operationen leicht manipuliert werden können.

Dieser Tokenisierungsprozess ist ein grundlegender vorbereitender Schritt für die zukünftige Analyse und Auswertung des mathematischen Ausdrucks.

## Globale Operation

Die Funktion nimmt vier Parameter an:
* 'TokenArray': Array, das die generierten Token enthält
* 'Ausdruck': die Zeichenkette, die den mathematischen Ausdruck darstellt
* 'Fehler': Zeiger auf eine Variable, die den Fehlercode im Falle eines Problems enthält
* 'Dimension': gibt an, ob der Ausdruck 1 oder 2 Variablen hat (0 für eine Dimension, 1 für zwei Dimensionen)

**Wichtig**: Der lexikalische Analysator betrachtet Großbuchstaben als Kleinbuchstaben.

**Wichtig**: Für Dezimalzahlen Punkte statt Kommas verwenden.

## Verwendete Unterfunktionen

Die Funktion 'Lexikalische_Analyse' folgt einer dreistufigen Verarbeitungssequenz, wobei jede eine dedizierte Unterfunktion verwendet:

1. 'AusdruckOhneLeerzeichen'
   Diese Funktion entfernt alle Leerzeichen aus dem mathematischen Ausdruck.

2. 'ImpliziteMultiplikation'
   Diese Funktion erkennt und fügt implizite Multiplikationsoperatoren hinzu. Zum Beispiel wandelt sie '2x' in '2*x' um.

3. 'TokenZerlegung'
   Diese Funktion zerlegt den Ausdruck in einzelne Token und speichert sie im Array 'TokenArray', wobei der Dimensionsparameter berücksichtigt wird.

## Detaillierter Prozess mit Beispielen

Nehmen wir den Ausdruck '"3 + cos(5x)"' als Beispiel.

### Schritt 1: Leerzeichenentfernung
'AusdruckOhneLeerzeichen' wandelt '"3 + cos(5x)"' in '"3+cos(5x)"' um.

### Schritt 2: Hinzufügen impliziter Multiplikationen
'ImpliziteMultiplikation' erkennt implizite Multiplikationen zwischen Ziffern und Variablen. Sie wandelt '"3+cos(5x)"' in '"3+cos(5*x)"' um.

### Schritt 3: Token-Erstellung
'TokenZerlegung' zerlegt den Ausdruck in Token:
* '3' → REAL-Typ-Token mit Wert 3.0
* '+' → OPERATOR-Typ-Token mit Wert PLUS
* 'cos' → FUNKTION-Typ-Token mit Wert COS
* '(' → OPEN_PAREN-Typ-Token
* '5' → REAL-Typ-Token mit Wert 5.0
* '*' → OPERATOR-Typ-Token mit Wert MAL
* 'x' → VARIABLE-Typ-Token mit Wert 'x'
* ')' → CLOSE_PAREN-Typ-Token
* Hinzufügen eines ENDE-Tokens am Ende

## Wie 'TokenZerlegung' Token identifiziert

Die Funktion 'TokenZerlegung' analysiert Zeichen für Zeichen und verwendet mehrere Unterfunktionen, um jedes Token korrekt zu identifizieren:

* 'TokenPositivReal': Erstellt ein Token für positive Zahlen
* 'TokenNegativReal': Verarbeitet negative Zahlen (z. B. "(-2.5)")
* 'TokenOperator': Identifiziert Operatoren (+, -, *, /, **)
* 'TokenFunktion': Identifiziert Funktionen (sin, cos, abs usw.)
* 'TokenVariable': Identifiziert Variablen (x, y) basierend auf dem Parameter 'Dimension'

## Variablenverwaltung nach Dimension

Die Funktion 'TokenVariable' verwendet den Parameter 'Dimension', um zu wissen, welche Variablen zulässig sind:
* Wenn 'Dimension' = 0: nur die Variable 'x' ist zulässig
* Wenn 'Dimension' = 1: die Variablen 'x' und 'y' sind zulässig

## Fehlerbehandlung

Die Funktion meldet verschiedene Arten von möglichen Fehlern:
* UNBEKANNTE_FUNKTION (101): Unbekannte Funktion
* UNGÜLTIGE_ZAHL (102): Ungültiges Zahlenformat (z. B. "1.2.3")
* UNBEKANNTES_ZEICHEN (103): Unbekanntes Zeichen
* UNBEKANNTE_VARIABLE (104): Variable nicht zulässig gemäß der Dimension

## Vollständiges Beispiel

Für den Ausdruck '"2x + sin(3.5)"' mit 'Dimension' = 0:

1. Leerzeichenentfernung: '"2x+sin(3.5)"'
2. Hinzufügen impliziter Multiplikationen: '"2*x+sin(3.5)"'
3. Token-Erstellung:
   * REAL (2.0)
   * OPERATOR (MAL)
   * VARIABLE ('x')
   * OPERATOR (PLUS)
   * FUNKTION (SIN)
   * OPEN_PAREN
   * REAL (3.5)
   * CLOSE_PAREN
   * ENDE

# Syntaxanalyse

### erstelleLeerenKnoten

Nützlichkeit: Gibt einen leeren Baum zurück

Nimmt als Parameter an:

-Nichts

Funktionsbetrieb:

-Erstellt leere linke Kinder (lc) und rechte Kinder (rc)

### baueAusdrucksbaum

Nützlichkeit: Baut den Baum rekursiv aus einem Array von Token auf

Nimmt als Parameter an:

-ein Array von Token
-den Index der ersten zu analysierenden Zelle
-den Index der letzten zu analysierenden Zelle
-den Fehler, der vom letzten rekursiven Aufruf abgerufen wurde (der Standardfehler ist 0).

Funktionsbetrieb:

Wenn beim letzten rekursiven Aufruf ein Fehler vorliegt: gibt am Ende des Zweigs einen leeren Baum zurück

Wenn der Startindex > Endindex: gibt Fehler = LEERES_MITGLIED zurück

Wenn ein Operator vorhanden ist: teilt den Baum am Operator in linkes Kind und rechtes Kind auf.

Andernfalls gibt es in den folgenden Fällen einen Fehler zurück:

- FUNKTIONS_KLAMMER_PROBLEME: fehlende Klammer nach einer Funktion oder wenn sich die Klammer nicht schließt
- LEERES_MITGLIED: wenn ein Operator nicht von verarbeitbaren Objekten umgeben ist, wenn Klammern leer sind
- GESCHLOSSENE_KLAMMER_1ST_TOKEN: wenn eine Klammer geschlossen wird, ohne geöffnet worden zu sein
- KLAMMERN_ZAHL_PROBLEME: wenn die Anzahl der öffnenden Klammern von der Anzahl der schließenden Klammern abweicht
- PROBLEM_NACH_REAL_ODER_VARIABLE: Vermutlich unmöglich
- ENDE_FEHLT: wenn das Token "ENDE" fehlt

### findeNiedrigstenPrioritätsoperator

Nützlichkeit: Gibt den Operator zurück, wo zwischen linkem Kind und rechtem Kind geteilt werden soll.

Nimmt als Parameter an:

-ein Array von Token
-den Index der ersten zu analysierenden Zelle
-den Index der letzten zu analysierenden Zelle
-den Fehler, der vom letzten rekursiven Aufruf abgerufen wurde (der Standardfehler ist 0).

Funktionsbetrieb:

Initialisiert die Komplexitätstiefe der Klammern auf 0
Setzt den Teilungsindex standardmäßig auf -1.

minimalOperatorIndex = -1, wenn kein Operator in der Funktion vorhanden ist
minimalOperatorPriority stellt den Operator dar, auf dem das Token-Array getrennt werden soll.

Durchläuft das Array Zelle für Zelle und prüft, ob ein Operator vorhanden ist

Wenn die Zelle ein Operator ist, die aktuelle Tiefe der Klammern Null ist und der Operator der aktuellen Zelle für die Teilung besser geeignet ist:
minimalOperatorIndex = aktueller Index
minimalOperatorPriority = aktuelle Zelle

Wenn die Zelle ein OPEN_PAREN ist:
die Tiefe im Zusammenhang mit Klammern erhöht sich

Wenn die Zelle ein CLOSE_PAREN ist:
die Tiefe im Zusammenhang mit Klammern verringert sich

Wenn die Tiefe der Klammern nicht Null ist (=> Anzahl der OPEN_PAREN != Anzahl der CLOSE_PAREN)
Fehler = KLAMMERN_ZAHL_PROBLEME

### findeAusdruckslänge

Nützlichkeit: Gibt die Größe des Arrays zurück

Nimmt als Parameter an:

-Ein Array von Token

Funktionsbetrieb:

Durchläuft das Array und wenn die Zelle eine Endzelle ist, stoppt die Funktion
Wenn kein ENDE gefunden wird, dann Ende = -1

### prüfeKlammerbilanz

Nützlichkeit: Gibt einen Booleschen Wert basierend auf dem Gleichgewicht der Anzahl der Klammern zurück.

Nimmt als Parameter an:

-Index des Starts des zu analysierenden Arrays
-Index des Endes des zu analysierenden Arrays
-das Array von Token

Funktionsbetrieb:

Durchläuft das Array und zählt die Anzahl der OPEN_PAREN und CLOSE_PAREN

### baueSyntaxbaum

Nützlichkeit: Baut den vollständigen Syntaxbaum aus einer Sequenz von Token auf

Nimmt als Parameter an:

-das Array von Token
-der letzte von baueAusdrucksbaum zurückgegebene Fehler (0 für den ersten Aufruf)

Funktionsbetrieb:

Prüft, ob das ENDE-Token existiert
Wenn es existiert, wird der Baum durch Aufrufen von baueAusdrucksbaum aufgebaut
Andernfalls Fehler = ENDE_FEHLT

# EVALUATOR-TEIL:

Das Ziel des Evaluators ist es, eine Funktion (die in Form eines Baumes vorliegt) aus gegebenen x- und y-Variablen zu berechnen. Seine Rolle ist für das Zeichnen von Kurven unerlässlich, da er es ermöglicht, die Punkte zu erhalten, die auf dem Graphen angezeigt werden sollen.
Unser Code ist in zwei Hauptfunktionen unterteilt: Evaluator, unsere Hauptfunktion, die den Baum interpretiert, und berechne_funktion, eine Hilfsfunktion, die Standard-Mathematikfunktionen anwendet.
Evaluator-Funktion:

Ihr Zweck ist es, den Eingabebaum zu dekodieren. Sie nimmt als Eingabe an:
- Die Funktion in Form eines Baumes
- Die Werte von x und y
- Den Fehlercode-Zeiger.
Sie gibt das Ergebnis der Eingabefunktion zurück, berechnet aus den x- und y-Variablen.
Diese Funktion ist rekursiv und hat vier Stoppfälle. Der erste ist, wenn der Fehlercode-Zeiger seinen Wert ändert, was bedeutet, dass ein Fehler in der zu berechnenden Funktion vorhanden ist und daher das Dekodieren des Baumes gestoppt werden muss, sobald ein Fehler auftritt. Der zweite Stoppfall ist, wenn das Haupttoken des Baumes keinen Typ hat. Schließlich stoppt die Funktion, wenn das Haupttoken des Baumes ein Real oder eine Variable ist, was bedeutet, dass wir ein Blatt des Baumes erreicht haben.
Sobald dies erledigt ist, schauen wir uns mit einer Switch-Funktion den Typ des Haupttokens des Eingabebaumes an, wenn es sich um Folgendes handelt:

- Eine Variable: Wir geben den Wert der Variablen zurück, die wir als Funktionseingabe abgerufen haben.
- Ein Real: Wir geben den Wert des Reals zurück.
- Ein Operator: Wir verwenden eine Teile-und-Herrsche-Methode, um die Operation zu berechnen.
- Eine Funktion: Wir verwenden dann die zweite Funktion unseres Programms.
Berechne_funktion-Funktion:

Diese Funktion nimmt als Eingabe den Typ der mathematischen Funktion f, die Variable x, auf die f angewendet wird, und den Fehlercode-Zeiger an. Sie gibt die Anwendung f(x) zurück. Die möglichen Funktionstypen sind universelle mathematische Funktionen wie sin, cos oder ln.
Wir schauen uns zuerst mit einer Switch-Funktion an, welche Funktion es ist, dann überprüfen wir, ob kein Fehler vorliegt, bevor wir f(x) berechnen.
Im Fehlerfall:
Im Fehlerfall (z. B. Division durch Null, Logarithmus einer negativen Zahl...) gibt die Funktion einen falschen Wert zurück und ändert den Fehlercode-Zeiger, um das Problem zu signalisieren. Dies ermöglicht es beispielsweise, die Auswertung zu stoppen oder eine geeignete Nachricht für den Benutzer anzuzeigen.

# Grafische Anzeige

## 1. Startseite

Der Startbildschirm erleichtert die Interaktion mit dem Benutzer. Er bietet Zugriff auf verschiedene Funktionen, wie das Abrufen des Benutzerhandbuchs, das Anzeigen von Danksagungen oder das Starten eines Anzeigemodus. Derzeit bietet das Programm eine 2D-Anzeige, aber eine 3D-Version für Funktionen mit zwei Variablen wird von unseren Ingenieuren entwickelt.

## 2. Rückkehrfunktionen

Eine Rückkehrfunktion ermöglicht es dem Benutzer, zu vorherigen Menüs zurückzukehren, ohne das Programm neu zu starten. Sie können zum Startbildschirm zurückkehren, indem sie auf das Haussymbol in den Textmenüs klicken oder die "Rücktaste" oder "Return"-Taste auf der Tastatur drücken.

## 3. Obere Leiste

Der Benutzer kann bis zu 20 Ausdrücke anzeigen. Standardmäßig wird für jeden Fall eine Basisfunktion angezeigt. Der Benutzer kann dann auf einen Ausdruck klicken, um ihn zu ändern. Wenn er in eines der drei Textfelder der oberen Leiste schreibt, kann er mit den folgenden Funktionen interagieren:

### 3.1 Farbwechsel

Das Programm bietet die Möglichkeit, die Farbe der Funktionen über ein Farbauswahlmenü anzupassen. Dieses Menü wird durch Klicken auf das Farbrad-Symbol aktiviert. Nach dem Öffnen ermöglicht es das Ändern der Farbe der Funktionen in Echtzeit (auch durch Halten des Linksklicks).

### 3.2 Funktion ausblenden

Diese Funktion ermöglicht es dem Benutzer, die Funktionen seiner Wahl je nach Bedarf in Echtzeit auszublenden oder anzuzeigen.

### 3.3 Funktion löschen

Der Benutzer kann Ausdrücke löschen, indem er mit dem Linksklick der Maus auf das Papierkorbsymbol klickt.

### 3.4 Grenzen ändern

Ein Textfenster ermöglicht es dem Benutzer, die Grenzen eines Ausdrucks in Echtzeit zu ändern, um Zeit zu sparen und mehr Flexibilität zu erhalten.

## 4. Ausdrucksverarbeitung

Sobald der Ausdruck eingegeben wurde, wird er einer lexikalischen und dann einer syntaktischen Analyse unterzogen, um ihn für die weitere Verarbeitung vorzubereiten.

## 5. Funktionsanzeige

Der während der Analyse generierte Baum wird verwendet, um jedem Wert des Intervalls sein entsprechendes Bild auf dem Bildschirm zuzuordnen, indem die Auswertungsfunktion verwendet wird. Dieser Prozess wird in Echtzeit wiederholt, wodurch dynamische Aktualisierungen der Anzeige ermöglicht werden.

### 5.1 Zoom

Für eine bessere grafische Analyse ermöglicht das Programm das Vergrößern oder Verkleinern mit dem Mausrad. Dadurch wird das angezeigte Intervall der Funktion geändert.

### 5.2 Zentrieren

Die Zentrierungsfunktion ermöglicht das schnelle Zurücksetzen der Anzeige. Drücken Sie einfach die Taste "C" auf der Tastatur. Eine Schaltfläche "Neu zentrieren" wird bald zur rechten Leiste des Fensters hinzugefügt.

### 5.3 Achsenverschiebung

Durch Drücken der Taste "Rechtsklick" der Maus aktiviert der Benutzer einen "Ziehen"-Modus, der es ihm ermöglicht, den Graphen intuitiv zu navigieren.

### 5.4 x-Evaluator

Der Evaluator, der durch einen Linksklick aktiviert wird, ermöglicht es dem Benutzer, zwei Linien zu visualisieren, die die erste Kurve in der Ausdrucksliste schneiden. Diese Linien folgen der Mausposition auf der x-Achse. Eine Anzeige liefert den Wert von 'x' sowie den Wert der Funktion am Klickpunkt.
