
# Cómo usarlo:

- Funciones utilizables: sin(), cos(), tan(), sqrt(), exp(), log(), abs(), sinc(), entero(),
- Para valores negativos, siempre usa paréntesis, p. ej.: sin(-2) => sin((-2)) o -4 => (-4)
- Para potencias usa ** p. ej.: x elevado a y => x**y
- Para números decimales usa '.' no ','
- Todas las letras mayúsculas se consideran minúsculas

## 1. Inicio:

La pantalla de inicio facilita el acceso a funciones como el manual de usuario, agradecimientos o el inicio del modo de visualización, con una versión 3D en desarrollo.

## 2. Regresar:

Para volver a los menús anteriores sin reiniciar el programa, haz clic en el icono de la casa o usa la tecla "Retroceso" o "Regresar".

## 3. Barra superior:

Permite mostrar y modificar hasta 20 expresiones con funciones interactivas.

- Cambio de color: Selecciona un nuevo color en tiempo real a través del icono de la rueda de color.
- Ocultar una función: Oculta o muestra una función según sea necesario en tiempo real.
- Eliminar una función: Haz clic en el icono de la papelera para eliminar una expresión.
- Modificar límites: Ajusta los límites de una expresión a través de una ventana de texto para mayor flexibilidad.

## 4. Visualización de funciones:

La evaluación de funciones actualiza dinámicamente su representación gráfica.

- Zoom: Usa la rueda del ratón para ajustar la visualización del gráfico.
- Centrado: Restablece la visualización con la tecla "C" o el botón "Recentralizar" (próximamente).
- Traslación de ejes: Arrastra en el gráfico con un clic derecho para navegar por la curva.
- Evaluador de x: Activa con un clic izquierdo para mostrar las coordenadas de un punto en la curva.

# Nuestras estructuras:

Para desarrollar nuestro graficador, utilizamos varias estructuras, en particular para codificar funciones como un árbol, pero también para devolver los diferentes errores posibles. Esta parte nos fue proporcionada al inicio del proyecto; hicimos algunas modificaciones que les presentaremos.
El archivo que se nos proporcionó contenía 6 estructuras diferentes:

- árbol: toma como parámetros un token (TipoToken), un árbol siguiente y un árbol anterior.
- tipotoken: toma como parámetros el tipo de valor del token (tipoLexema) y el valor del token (tipoValor).
- tipovalor: toma como parámetros si el valor del token, que puede ser un real, una función, un operador, y decidimos agregar también la posibilidad de incluir una variable (char) que tomará x o y como su valor para poder graficar una función con dos variables.
- tipofuncion: toma como parámetro una función matemática (exp, sin, cos, ...).
- tipooperador: toma como parámetro un operador (más, menos, veces, dividir, potencia).
- tipolexema: toma como parámetro el nombre del tipo de valor del token.

También agregamos una estructura código_error que asocia un nombre de error con un código de error, por ejemplo: RAIZ_NEGATIVA=301, es imposible calcular la raíz de un número negativo, por lo que si el usuario solicita como entrada hacer uno, el código devolverá este error.

# Explicación de la función Análisis_Léxico

La función 'Análisis_Léxico' transforma una expresión matemática en forma de una cadena de caracteres en una matriz de tokens. Descompone la expresión en elementos que pueden manipularse fácilmente para operaciones de análisis sintáctico posteriores.

Este proceso de tokenización es un paso preliminar fundamental para el análisis y evaluación futuros de la expresión matemática.

## Operación global

La función toma cuatro parámetros:
* 'MatrizToken': matriz que contendrá los tokens generados
* 'Expresión': la cadena de caracteres que representa la expresión matemática
* 'error': puntero a una variable que contendrá el código de error en caso de un problema
* 'Dimensión': indica si la expresión es de 1 o 2 variables (0 para una dimensión, 1 para dos dimensiones)

**Importante**: El analizador léxico considera las letras mayúsculas como minúsculas.

**Importante**: Para números decimales, usa puntos, no comas.

## Subfunciones utilizadas

La función 'Análisis_Léxico' sigue una secuencia de procesamiento de tres pasos, cada uno usando una subfunción dedicada:

1. 'ExpresiónSinEspacios'
   Esta función elimina todos los espacios de la expresión matemática.

2. 'MultiplicaciónImplícita'
   Esta función detecta y agrega operadores de multiplicación implícitos. Por ejemplo, transforma '2x' en '2*x'.

3. 'DescomposiciónToken'
   Esta función descompone la expresión en tokens individuales y los almacena en la matriz 'MatrizToken', teniendo en cuenta el parámetro de dimensión.

## Proceso detallado con ejemplos

Tomemos la expresión '"3 + cos(5x)"' como ejemplo.

### Paso 1: Eliminación de espacios
'ExpresiónSinEspacios' transforma '"3 + cos(5x)"' en '"3+cos(5x)"'.

### Paso 2: Agregar multiplicaciones implícitas
'MultiplicaciónImplícita' detecta multiplicaciones implícitas entre dígitos y variables. Transforma '"3+cos(5x)"' en '"3+cos(5*x)"'.

### Paso 3: Creación de tokens
'DescomposiciónToken' descompone la expresión en tokens:
* '3' → token de tipo REAL con valor 3.0
* '+' → token de tipo OPERADOR con valor MÁS
* 'cos' → token de tipo FUNCIÓN con valor COS
* '(' → token de tipo PAR_ABRE
* '5' → token de tipo REAL con valor 5.0
* '*' → token de tipo OPERADOR con valor VECES
* 'x' → token de tipo VARIABLE con valor 'x'
* ')' → token de tipo PAR_CIERRA
* Agregar un token FIN al final

## Cómo 'DescomposiciónToken' identifica los tokens

La función 'DescomposiciónToken' analiza carácter por carácter y utiliza varias subfunciones para identificar correctamente cada token:

* 'TokenRealPositivo': Crea un token para números positivos
* 'TokenRealNegativo': Maneja números negativos (p. ej., "(-2.5)")
* 'TokenOperador': Identifica operadores (+, -, *, /, **)
* 'TokenFunción': Identifica funciones (sin, cos, abs, etc.)
* 'TokenVariable': Identifica variables (x, y) según el parámetro 'Dimensión'

## Gestión de variables por dimensión

La función 'TokenVariable' usa el parámetro 'Dimensión' para saber qué variables están permitidas:
* Si 'Dimensión' = 0: solo se permite la variable 'x'
* Si 'Dimensión' = 1: se permiten las variables 'x' e 'y'

## Gestión de errores

La función informa varios tipos de errores posibles:
* FUNCIÓN_DESCONOCIDA (101): Función no reconocida
* NÚMERO_INVÁLIDO (102): Formato de número incorrecto (p. ej., "1.2.3")
* CARÁCTER_DESCONOCIDO (103): Carácter no reconocido
* VARIABLE_DESCONOCIDA (104): Variable no permitida según la dimensión

## Ejemplo completo

Para la expresión '"2x + sin(3.5)"' con 'Dimensión' = 0:

1. Eliminación de espacios: '"2x+sin(3.5)"'
2. Agregar multiplicaciones implícitas: '"2*x+sin(3.5)"'
3. Creación de tokens:
   * REAL (2.0)
   * OPERADOR (VECES)
   * VARIABLE ('x')
   * OPERADOR (MÁS)
   * FUNCIÓN (SIN)
   * PAR_ABRE
   * REAL (3.5)
   * PAR_CIERRA
   * FIN

# Análisis sintáctico

### crearNodoVacío

Utilidad: Devuelve un árbol vacío

Toma como parámetro:

-Nada

Operación de la función:

-Crea hijos izquierdo (hi) y derecho (hd) vacíos

### construirÁrbolExpresión

Utilidad: Construye recursivamente el árbol a partir de una matriz de tokens

Toma como parámetro:

-una matriz de tokens
-el índice de la primera celda a analizar
-el índice de la última celda a analizar
-el error recuperado por la última llamada recursiva (el error predeterminado es 0).

Operación de la función:

Si hay un error en la última llamada recursiva: devuelve un árbol vacío al final de la rama

Si el índice de inicio > índice de fin: devuelve error = MIEMBRO_VACÍO

Si hay un operador: divide el árbol en el operador en hijo izquierdo e hijo derecho.

De lo contrario, devuelve un error en los siguientes casos:

- PROBLEMAS_PARÉNTESIS_FUNCIÓN: falta un paréntesis después de una función o si el paréntesis no se cierra
- MIEMBRO_VACÍO: si un operador no está rodeado de objetos procesables, si los paréntesis están vacíos
- PARÉNTESIS_CERRADO_1ER_TOKEN: si se cierra un paréntesis sin haberse abierto
- PROBLEMAS_NÚMERO_PARÉNTESIS: si el número de paréntesis de apertura es diferente del número de paréntesis de cierre
- PROBLEMA_DESPUÉS_REAL_O_VARIABLE: Supuestamente imposible
- AUSENCIA_FIN: si falta el token "FIN"

### encontrarOperadorDeMenorPrioridad

Utilidad: Devuelve el operador donde dividir entre el hijo izquierdo y el hijo derecho.

Toma como parámetro:

-una matriz de tokens
-el índice de la primera celda a analizar
-el índice de la última celda a analizar
-el error recuperado por la última llamada recursiva (el error predeterminado es 0).

Operación de la función:

Inicializa la profundidad de complejidad de los paréntesis a 0
Establece el índice de división a -1 de forma predeterminada.

índiceOperadorMínimo = -1 si no hay operador en la función
prioridadOperadorMínimo representa el operador en el que separar la matriz de tokens.

Itera a través de la matriz celda por celda y verifica si hay un operador

Si la celda es un operador, la profundidad actual de los paréntesis es cero y el operador de la celda actual es más adecuado para la división:
índiceOperadorMínimo = índice actual
prioridadOperadorMínimo = celda actual

Si la celda es un PAR_ABRE:
la profundidad relacionada con los paréntesis aumenta

Si la celda es un PAR_CIERRA:
la profundidad relacionada con los paréntesis disminuye

Si la profundidad de los paréntesis no es cero (=> número de PAR_ABRE != número de PAR_CIERRA)
error = PROBLEMAS_NÚMERO_PARÉNTESIS

### encontrarLongitudExpresión

Utilidad: Devuelve el tamaño de la matriz

Toma como parámetro:

-Una matriz de tokens

Operación de la función:

Itera a través de la matriz y si la celda es una celda final, la función se detiene
Si no se encuentra ningún FIN, entonces fin = -1

### verificarEquilibrioParéntesis

Utilidad: Devuelve un booleano basado en el equilibrio del número de paréntesis.

Toma como parámetro:

-índice del inicio de la matriz a analizar
-índice del final de la matriz a analizar
-la matriz de tokens

Operación de la función:

Itera a través de la matriz y cuenta el número de PAR_ABRE y PAR_CIERRA

### construirÁrbolSintaxis

Utilidad: Construye el árbol de sintaxis completo a partir de una secuencia de tokens

Toma como parámetro:

-la matriz de tokens
-el último error devuelto por construirÁrbolExpresión (0 para la primera llamada)

Operación de la función:

Verifica si existe el token FIN
Si existe, entonces el árbol se construye llamando a construirÁrbolExpresión
De lo contrario, error = AUSENCIA_FIN

# PARTE EVALUADOR:

El objetivo del evaluador es calcular una función (que está en forma de árbol) a partir de las variables x e y dadas. Su función es esencial para trazar curvas, ya que permite obtener los puntos para mostrar en el gráfico.
Nuestro código se divide en dos funciones principales: evaluador, nuestra función principal que interpreta el árbol, y calcular_función, una función auxiliar que aplica funciones matemáticas estándar.
Función evaluador:

Su propósito es decodificar el árbol de entrada. Toma como entrada:
- La función en forma de árbol
- Los valores de x e y
- El puntero código_error.
Devuelve el resultado de la función de entrada, calculado a partir de las variables x e y.
Esta función es recursiva y tiene cuatro casos de parada. El primero es si el puntero código_error cambia de valor, lo que significa que hay un error en la función a calcular y, por lo tanto, la decodificación del árbol debe detenerse una vez que aparece un error. El segundo caso de parada es si el token principal del árbol no tiene un tipo. Finalmente, la función se detendrá si el token principal del árbol es un real o una variable, lo que significa que hemos llegado a una hoja del árbol.
Una vez hecho esto, miramos usando una función switch el tipo del token principal del árbol de entrada, si es:

- Una variable: devolvemos el valor de la variable que recuperamos como entrada de función.
- Un real: devolvemos el valor del real.
- Un operador: usamos un método de divide y vencerás para calcular la operación.
- Una función: entonces usamos la segunda función de nuestro programa.
Función calcular_función:

Esta función toma como entrada el tipo de la función matemática f, la variable x en la que se aplica f y el puntero código_error. Devuelve la aplicación f(x). Los tipos de funciones posibles son funciones matemáticas universales como sin, cos o ln.
Primero miramos usando una función switch qué función es, luego verificamos que no haya ningún error antes de calcular f(x).
En caso de error:
En caso de error (p. ej., división por cero, logaritmo de un número negativo...), la función devuelve un valor incorrecto y modifica el puntero código_error para señalar el problema. Esto permite, por ejemplo, detener la evaluación o mostrar un mensaje apropiado al usuario.

# Visualización gráfica

## 1. Inicio

La pantalla de inicio facilita la interacción con el usuario. Proporciona acceso a varias funciones, como consultar el manual de usuario, mostrar agradecimientos o iniciar un modo de visualización. Actualmente, el programa ofrece una visualización 2D, pero nuestros ingenieros están desarrollando una versión 3D para funciones con dos variables.

## 2. Funciones de retorno

Una función de retorno permite al usuario volver a los menús anteriores sin reiniciar el programa. Pueden volver a la pantalla de inicio haciendo clic en el icono en forma de casa en los menús de texto o presionando la tecla "Retroceso" o "Regresar" en el teclado.

## 3. Barra superior

El usuario puede mostrar hasta 20 expresiones. De forma predeterminada, se presenta una función básica para cada caso. El usuario puede hacer clic en una expresión para modificarla. Además, al escribir en una de las tres áreas de texto de la barra superior, pueden interactuar con las siguientes funciones:

### 3.1 Cambio de color

El programa ofrece la posibilidad de personalizar el color de las funciones a través de un menú de selección de color. Este menú se activa haciendo clic en el icono de la rueda de color. Una vez abierto, permite cambiar el color de las funciones en tiempo real (incluso manteniendo el clic izquierdo).

### 3.2 Ocultar una función

Esta función permite al usuario ocultar o mostrar las funciones de su elección en tiempo real, según sus necesidades.

### 3.3 Eliminar una función

El usuario puede eliminar expresiones haciendo clic en el icono de la papelera con el clic izquierdo del ratón.

### 3.4 Modificar límites

Una ventana de texto permite al usuario modificar los límites de una expresión en tiempo real, para ahorrar tiempo y tener más flexibilidad.

## 4. Procesamiento de expresiones

Una vez que se introduce la expresión, se somete a un análisis léxico y luego sintáctico para prepararla para su posterior procesamiento.

## 5. Visualización de funciones

El árbol generado durante el análisis se utiliza para asociar cada valor del intervalo con su imagen correspondiente en la pantalla, utilizando la función de evaluación. Este proceso se repite en tiempo real, lo que permite actualizaciones dinámicas de la visualización.

### 5.1 Zoom

Para un mejor análisis gráfico, el programa permite acercar o alejar con la rueda del ratón. Esto modifica el intervalo mostrado de la función.

### 5.2 Centrado

La función de centrado permite restablecer rápidamente la visualización. Simplemente presiona la tecla "C" en el teclado. Pronto se agregará un botón "Recentralizar" a la barra derecha de la ventana.

### 5.3 Traslación de ejes

Al presionar el botón "clic derecho" del ratón, el usuario activa un modo de "arrastre" que les permite navegar por el gráfico de forma intuitiva.

### 5.4 Evaluador de x

El evaluador, activado por un clic izquierdo, permite al usuario visualizar dos líneas que se cruzan con la primera curva en la lista de expresiones. Estas líneas siguen la posición del ratón en el eje x. Una pantalla proporciona el valor de 'x' así como el valor de la función en el punto de clic.
```
