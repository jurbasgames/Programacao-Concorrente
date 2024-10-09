# Relatório Laboratório 6

## Atividade 1

### O TAD lista implementado nesses arquivos poderia ser compartilhado por threads de um programa concorrente? Com qual finalidade e de que forma?
R: Sim, poderia ser implementado para melhorar a performance do software, e o uso de mecanismos de sincronização como Leitor/Escritor, locks e variáveis de condição são essenciais para manter a proteção das seções críticas e a consistência dos dados.

### O que poderia acontecer se o programa nao implementasse exclusão mútua no acesso as operações da lista encadeada?
R: O que poderia acontecer é a corrida de dados. Se duas threads fossem ler ao mesmo tempo, não haveria problemas, mas se qualquer outra cobinação de operações fosse realizada por exemplo leitura e escrita, escrita e escrita, então teríamos uma corrida de dados, causando inconsistência nos recursos compartilhados.


### O que acontece com o tempo de execução do programa quando aumentamos o número de threads? Por que isso ocorre?
R: O tempo de execução aumenta, pois apenas uma thread pode acessar os recursos da lista de cada vez, fazendo com que elas concorram pela posse do mutex. O aumento de threads só vai aumentar ainda mais a concorrência, fazendo com que elas fique mais tempo bloqueadas esperando a posse do mutex.

## Atividade 2

### Em quais cenários o uso do rwlock pode ser mais vantajoso que o uso do lock de exclusao mútua?
R: O rwlock pode ser mais vantajoso quando temos muitas operações de leitura e poucas operações de escrita, porque o rwlock vai permitir que várias threads façam leituras ao mesmo tempo, se não houver ninguém escrevendo, ou seja, seu uso se justifica se a proporção de escritas é baixa comparado a de leituras.

## Atividade 3

### Análise de prioridade de escrita

Como essa implementação fiz o uso de mutex e variáveis de condição para garantir que não haja leitura e escrita ao mesmo tempo, permitindo que várias leituras sejam feitas simultaneamente. O mutex é passado para as funções que foram implementadas e é usado para garantir que não haja alterações simultâneas nas variáveis compartilhadas.

As variáveis globais `escritor_fila`, `escritores`, `leitores` foram usadas para controlar a prioridade e o acesso ao recurso compartilhado. Onde a primeira é para verificar se há algum esritor em espera e as outras duas são para contagem de escritores e leitores, respectivamente.

A prioridade da escrita está funcionando da seguinte forma, os leitores podem ler simultaneamente desde que não haja escritores ativos ou esperando na fila e quando um escritor solicita acesso, incrementando na fila, os novos leitores não podem iniciar a leitura até que o escritor termine sua operação. 

Para a análise foram adicionadas mensagens de debug para observar a prioridade de escrita sendo realizada. Para viés de testagem reduzi o número de operações no arquivo principal.

Executando o programa com 10 threads 
```bash
./lab6 10 > output.txt
```

```txt
(...)
(01) Último leitor acabou, liberando escritores.
(02) Escritor começou a escrever. Escritores ativos: 1
(03) Escritor acabou de escrever.
(04) Não há mais escritores na fila, liberando leitores.
(05) Escritor esperando leitores finalizarem.
(06) Último leitor acabou, liberando escritores.
(07) Leitor bloqueado. Escritores ativos ou na fila.
(08) Escritor começou a escrever. Escritores ativos: 1
(09) Leitor bloqueado. Escritores ativos ou na fila.
(10) Escritor acabou de escrever.
(11) Não há mais escritores na fila, liberando leitores.
(...)
```

Na linha (01), vemos que o último  leitor acabou sua leitura e como havia um escritor esperando, ele foi acordado. O escritor iniciou a escrita, linha (02) e, após concluir a escrita (03), liberou os leitores na linha (04). 

Dentro desse lote de leituras, um novo escritor entrou na fila (05) e esperou até que todos os leitores terminassem suas operações (06). Após as operações de leitura, o escritor foi liberado para iniciar sua escrita na linha (08).

Na linha (07), vemos que um leitor tentou começar a leitura, mas foi bloqueado, pois havia escritores ativos ou na fila, respeitando a prioridade de escrita. Após o finalizar a escrita na linha (10), os leitores foram novamente liberados na linha (11).