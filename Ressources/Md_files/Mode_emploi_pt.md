# Como usar:

- As funções utilizáveis: sin(), cos(), tan(), sqrt(), exp(), log(), abs(), sinc(), inteiro(),
- Para valores negativos, sempre utilizar parênteses ex.: sin(-2) => sin((-2)) ou -4 => (-4)
- Para potências, utilizar '**' ex.: x elevado a y => x**y
- Para números decimais, utilizar '.' e não ','
- todas as letras maiúsculas são consideradas minúsculas

## 1. Início:

A tela inicial facilita o acesso às funcionalidades como o manual de uso, agradecimentos ou o lançamento do modo de exibição, com uma versão 3D em desenvolvimento.

## 2. Retorno:

Para voltar aos menus anteriores sem reiniciar o programa, clique no ícone de casa ou utilize a tecla "Backspace" ou "Retornar".

## 3. Barra superior:

Permite visualizar e modificar até 20 expressões com funcionalidades interativas.

   - Mudança de cor: Selecione uma nova cor em tempo real através do ícone da roda.

   - Ocultar uma função: Oculta ou exibe uma função conforme necessário em tempo real.

   - Eliminar uma função: Clique no ícone da lixeira para eliminar uma expressão.

   - Modificar os limites: Ajuste os limites de uma expressão através de uma janela de texto para maior flexibilidade.

## 4. Visualização das funções:

A avaliação das funções atualiza dinamicamente sua representação gráfica.

   - Zoom: Utilize a roda do mouse para ajustar a exibição do gráfico.

   - Centralização: Redefina a exibição com a tecla "C" ou o botão "Recentrar" em breve.

   - Translação da referência: Arraste no gráfico com um clique direito para percorrer a curva.

   - Avaliador em x: Ative com um clique esquerdo para visualizar as coordenadas de um ponto na curva.

# Nossas estruturas:

Para desenvolver nosso gráfico, utilizamos várias estruturas, especialmente para codificar as funções na forma de árvore e para retornar os diferentes erros possíveis. Esta parte foi fornecida no início do projeto, fizemos algumas modificações que vamos apresentar.
O arquivo que nos foi fornecido continha 6 estruturas diferentes:

- árvore: recebe como parâmetro um token (Typejeton), uma árvore seguinte e uma árvore anterior.
- typejeton: recebe como parâmetro o tipo de valor do token (typelexem) e o valor do token (typevaleur).
- typevaleur: recebe como parâmetro se o valor do token, que pode ser um real, uma função, um operador e decidimos adicionar também a possibilidade de incluir uma variável (char) que tomará como valor x ou y para poder realizar o gráfico de uma função com duas variáveis.
- typefonction: recebe como parâmetro uma função matemática (exp, sin, cos,…)
- typeoperateur: recebe como parâmetro um operador (mais, menos; vezes, dividido, potência)
- typelexem: recebe como parâmetro o nome do tipo de valor do token.

Adicionamos também uma estrutura code_erreur que associa um nome de erro a um código de erro, por exemplo: RACINE_NEGATIVE=301, é impossível calcular a raiz de um número negativo, portanto, se o usuário solicitar para fazer uma, o código retornará este erro.

# Explicação da função Analyse_Lexicale

A função 'Analyse_Lexicale' transforma uma expressão matemática na forma de string em um array de tokens. Permite decompor a expressão em elementos que podem ser facilmente manipulados para operações de análise sintática subsequentes.

Este processo de tokenização é uma etapa preliminar fundamental para a futura análise e avaliação da expressão matemática.

## Funcionamento global

A função recebe quatro parâmetros:
* 'TabToken': array que conterá os tokens gerados
* 'Expression': a string que representa a expressão matemática
* 'erreur': ponteiro para uma variável que conterá o código de erro em caso de problema
* 'Dimension': indica se a expressão é de 1 ou 2 variáveis (0 para uma dimensão, 1 para duas dimensões)

**Importante**: O analisador léxico considera maiúsculas como minúsculas.

**Importante**: Para números decimais, utilizar pontos e não vírgulas.

## Subfunções utilizadas

A função 'Analyse_Lexicale' segue uma sequência de processamento em três etapas principais, cada uma utilizando uma subfunção dedicada:

1. 'ExpressionSansLesEspaces'
   Esta função remove todos os espaços da expressão matemática.

2. 'MultiplicationImplicite'
   Esta função detecta e adiciona os operadores de multiplicação implícitos. Por exemplo, transforma '2x' em '2*x'.

3. 'DecompositionToken'
   Esta função decompõe a expressão em tokens individuais e os armazena no array 'TabToken', considerando o parâmetro de dimensão.

## Processo detalhado com exemplos

Vamos considerar a expressão '"3 + cos(5x)"' como exemplo.

### Etapa 1: Remoção dos espaços
'ExpressionSansLesEspaces' transforma '"3 + cos(5x)"' em '"3+cos(5x)"'.

### Etapa 2: Adição das multiplicações implícitas
'MultiplicationImplicite' detecta as multiplicações implícitas entre números e variáveis. Transforma '"3+cos(5x)"' em '"3+cos(5*x)"'.

### Etapa 3: Criação dos tokens
'DecompositionToken' decompõe a expressão em tokens:
* '3' → Token do tipo REAL com valor 3.0
* '+' → Token do tipo OPERADOR com valor MAIS
* 'cos' → Token do tipo FUNÇÃO com valor COS
* '(' → Token do tipo PAR_ABRIR
* '5' → Token do tipo REAL com valor 5.0
* '*' → Token do tipo OPERADOR com valor VEZES
* 'x' → Token do tipo VARIÁVEL com valor 'x'
* ')' → Token do tipo PAR_FECHAR
* Adição de um token FIM no final

## Como 'DecompositionToken' identifica os tokens

A função 'DecompositionToken' analisa caractere por caractere e utiliza várias subfunções para identificar corretamente cada token:

* 'TokenReelPositif': Cria um token para números positivos
* 'TokenReelNegatif': Lida com números negativos (ex: "(-2.5)")
* 'TokenOperateur': Identifica os operadores (+, -, *, /, **)
* 'TokenFonction': Identifica as funções (sin, cos, abs, etc.)
* 'TokenVariable': Identifica as variáveis (x, y) conforme o parâmetro 'Dimension'

## Gestão das variáveis conforme a dimensão

A função 'TokenVariable' utiliza o parâmetro 'Dimension' para saber quais variáveis são permitidas:
* Se 'Dimension' = 0: apenas a variável 'x' é permitida
* Se 'Dimension' = 1: as variáveis 'x' e 'y' são permitidas

## Gestão dos erros

A função sinaliza vários tipos de erros possíveis:
* FONCTION_INCONNUE (101): Função não reconhecida
* NOMBRE_INVALIDE (102): Formato de número incorreto (ex: "1.2.3")
* CARACTERE_INCONNUE (103): Caractere não reconhecido
* VARIABLE_INCONNUE (104): Variável não permitida conforme a dimensão

## Exemplo completo

Para a expressão '"2x + sin(3.5)"' com 'Dimension' = 0:

1. Remoção dos espaços: '"2x+sin(3.5)"'
2. Adição das multiplicações implícitas: '"2*x+sin(3.5)"'
3. Criação dos tokens:
   * REAL (2.0)
   * OPERADOR (VEZES)
   * VARIÁVEL ('x')
   * OPERADOR (MAIS)
   * FUNÇÃO (SIN)
   * PAR_ABRIR
   * REAL (3.5)
   * PAR_FECHAR
   * FIM

# Análise sintática

### createEmptyNode

Utilidade: Retorna uma árvore vazia

Recebe como parâmetro:

-Nada

Funcionamento da função:

-Cria fg e fd vazios

### buildExpressionTree

Utilidade: Constrói recursivamente a árvore a partir de um array de tokens

Recebe como parâmetro:

-um array de tokens
-o índice da primeira caixa a analisar
-o índice da última caixa a analisar
-o erro recuperado pela última chamada recursiva (o erro padrão é 0).

Funcionamento da função:

Se houver erro na última chamada recursiva: retorna uma árvore vazia no final do ramo

Se o índice do início > índice do fim: retorna erro = MEMBRO_VAZIO

Se houver um operador: divisão da árvore no operador em fg e fd.

Caso contrário, retorna um erro nos seguintes casos

- PROBLEME_PARENTHESES_FONCTIONS: falta um parêntese após uma função ou se o parêntese não se fecha
- MEMBRO_VAZIO: se um operador não está cercado por objetos tratáveis, se os parênteses estão vazios
- PARENTHESE_FERMEE_1_ER_JETON: se um parêntese é fechado sem ter sido aberto
- PROBLEMES_NOMBRE_PARENTHESES: se o número de parênteses abertos é diferente do número de parênteses fechados
- PROBLEME_APRES_REEL_OU_VARIABLE: Suposto impossível
- ABSENCE_FIN: se o token "FIM" está ausente

### findLowestPriorityOperator

Utilidade: Retorna o operador onde fazer a divisão entre fg e fd.

Recebe como parâmetro:

-um array de tokens
-o índice da primeira caixa a analisar
-o índice da última caixa a analisar
-o erro recuperado pela última chamada recursiva (o erro padrão é 0).

Funcionamento da função:

Inicializa a profundidade de complexidade dos parênteses a
Define o índice de divisão como -1 por padrão.

indiceOperateurMinimal = -1 se não houver operador na função
prioritéOperateurMinimal representa o operador no qual separar o array de tokens.

Percorre o array caixa por caixa e verifica se há um operador

Se a caixa é um operador, a profundidade atual dos parênteses é nula e o operador da caixa atual é mais adequado para a divisão:
indiceOperateurMinimal = índice atual
prioriteOperateurMinimal = caixa atual

Se a caixa é uma PAR_ABRIR:
a profundidade relacionada aos parênteses aumenta

Se a caixa é uma PAR_FECHAR:
a profundidade relacionada aos parênteses diminui

Se a profundidade dos parênteses não é nula (=> número de PAR_ABRIR != número de PAR_FECHAR
erro = PROBLEMES_NOMBRE_PARENTHESES

### findExpressionLength

Utilidade: Retorna o comprimento do array

Recebe como parâmetro:

-Um array de tokens

Funcionamento da função:

Percorre o array e se a caixa é uma caixa final, a função para
Se nenhum FIM for encontrado, então fim = -1

### checkParenthesesBalance

Utilidade: Retorna um booleano em relação ao equilíbrio do número de parênteses.

Recebe como parâmetro:

-índice do início do array a analisar
-índice do fim do array a analisar
-o array de tokens

Funcionamento da função:

Percorre o array e conta o número de PAR_ABRIR e de PAR_FECHAR

### buildSyntaxTree

Utilidade: Constrói a árvore sintática completa a partir de uma sequência de tokens

Recebe como parâmetro:

-o array de tokens
-o último erro retornado por buildExpressionTree (0 para a primeira chamada)

Funcionamento da função:

Verifica se o token FIM existe
Se existir, a árvore é construída chamando buildExpressionTree
Caso contrário, erro = ABSENCE_FIN

# PARTE AVALIADOR:

O objetivo do avaliador é calcular uma função (que está na forma de uma árvore) a partir das variáveis x e y fornecidas. Seu papel é essencial para traçar curvas, pois permite obter os pontos a serem exibidos no gráfico. Nosso código se divide em duas funções principais: avaliador, nossa função principal que interpreta a árvore, e calculer_fonction, uma função auxiliar que aplica funções matemáticas padrão.
Função avaliador:

Tem como objetivo decodificar a árvore de entrada. Recebe como entrada:
- A função na forma de uma árvore
- Os valores de x e y
- O ponteiro code_erreur.
Retorna o resultado da função de entrada, calculado a partir das variáveis x e y.
Esta função é recursiva e possui quatro casos de parada. O primeiro é se o ponteiro code_erreur mudar de valor, o que significa que há um erro na função a ser calculada e, portanto, a decodificação da árvore deve ser interrompida assim que um erro ocorrer. O segundo caso de parada é se o token principal da árvore não possuir um tipo. Por fim, a função parará se o token principal da árvore for um real ou uma variável, o que significa que chegamos a uma folha da árvore.
Depois disso, verifica-se com uma função switch o tipo do token principal da árvore de entrada, se for:

- Uma variável: retorna o valor da variável recuperado na entrada da função.
- Um real: retorna o valor do real.
- Um operador: utiliza um método de dividir para conquistar para calcular a operação.
- Uma função: utiliza então a segunda função do nosso programa.
Função calculer_fonction:

Esta função recebe como entrada o tipo da função matemática f, a variável x sobre a qual aplicar f e o ponteiro code_erreur. Retorna a aplicação f(x). Os tipos das funções possíveis são as funções matemáticas universais como sin, cos ou ln.
Primeiro, verifica-se com uma função switch de qual função se trata, depois verifica se não há erros, antes de calcular f(x).
Em caso de erro:
Em caso de erro (ex: divisão por zero, logaritmo de um número negativo...), a função retorna um valor incorreto e modifica o ponteiro code_erreur para sinalizar o problema. Isso permite, por exemplo, interromper a avaliação ou exibir uma mensagem apropriada ao usuário.

# Exibição gráfica

## 1. Início

A tela inicial facilita a interação com o usuário. Dá acesso às diferentes funcionalidades, como a consulta do manual de uso, a exibição dos agradecimentos ou o lançamento de um modo de exibição. Atualmente, o programa oferece uma exibição 2D, mas uma versão 3D para funções com duas variáveis está em desenvolvimento pelos nossos engenheiros.

## 2. Funcionalidades de retorno

Uma funcionalidade de retorno permite que o usuário volte aos menus anteriores sem reiniciar o programa. É possível voltar à tela inicial clicando no ícone em forma de casa nos menus de texto ou pressionando a tecla "Backspace" ou "Retornar" do teclado.

## 3. Barra superior

O usuário pode exibir até 20 expressões. Por padrão, uma função básica é apresentada para cada caso. O usuário pode então clicar em uma expressão para modificá-la. Além disso, ao digitar em uma das três áreas de texto da barra superior, ele pode interagir com as seguintes funções:

### 3.1 Mudança de cor

O programa oferece a possibilidade de personalizar a cor das funções através de um menu de seleção de cor. Este menu é ativado clicando no ícone da roda de cores. Uma vez aberto, permite alterar a cor das funções em tempo real (mesmo mantendo o clique esquerdo).

### 3.2 Ocultar uma função

Esta funcionalidade permite que o usuário oculte ou exiba as funções de sua escolha em tempo real, conforme necessário.

### 3.3 Eliminar uma função

O usuário pode eliminar expressões clicando no ícone da lixeira com o botão esquerdo do mouse.

### 3.4 Modificar os limites

Uma janela de texto permite que o usuário modifique os limites de uma expressão em tempo real, para ganhar tempo e maior flexibilidade.

## 4. Processamento da expressão

Uma vez inserida a expressão, ela é submetida a uma análise léxica e, em seguida, sintática, para preparar seu processamento posterior.

## 5. Exibição das funções

A árvore gerada durante a análise é utilizada para associar a cada valor do intervalo sua imagem correspondente na tela, graças à função de avaliação. Este processo se repete em tempo real, permitindo assim atualizações dinâmicas da exibição.

### 5.1 Zoom

Para uma melhor análise gráfica, o programa permite dar zoom in e zoom out usando a roda do mouse. Isso modifica o intervalo exibido da função.

### 5.2 Centralização

A funcionalidade de centralização permite redefinir rapidamente a exibição. Basta pressionar a tecla "C" do teclado. Em breve, um botão "Recentrar" será adicionado na barra direita da janela.

### 5.3 Translação da referência

Pressione o botão "clique direito" do mouse para ativar um modo de "deslizamento" que permite ao usuário percorrer o gráfico de maneira intuitiva.

### 5.4 Avaliador em x

O avaliador, ativado por um clique esquerdo, permite que o usuário visualize duas retas que intersectam a primeira curva da lista de expressões. Essas retas seguem a posição do mouse no eixo das abscissas. Uma exibição fornece o valor de 'x' e o valor da função no ponto de clique.
