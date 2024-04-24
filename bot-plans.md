## Matriz de inteiros para representar um tabuleiro 

Exemplo:
``` 
 0  0 19  0 23  0  1
32  0  0 14  0  0  0
 0 25  0  1  0 35  0
16  0  0  0  0 14  0
 0 22  0 18  0  0 24
```

- 0 é para água 
- 1 é para o porto

## Representação dos peixes

- Peso máximo que o navio pode carregar: 10kg
- Dividido por 1 dezena Tainha, 2 Cioba, 3 Robalo
    - O resto da divisão é a quantidade de peixes desse tipo 

## Jogo por turnos:

- Estados do navio:
    - Indo para o porto;
- O navio não pode pescar até que seja exaurida a quantidade de peixes em algum local, caso contrário, receberá uma multa.
- O jogo termina quando algum navio chega R$ 10.000, ou após um determinado número de partidas caso nenhum navio consiga. 

**Observações**:
1. Como o bot pode enviar qualquer string para a saída padrão, se ela não for reconhecido como uma das possíveis ações do jogo, a ação será desconsiderada e o bot perde a oportunidade de executar algo no seu turno. Nesse caso, o simulador envia como resultado da ação a string `INVALID`, indicando que foi uma ação inválida.

2. Todo comando de ação deve ser concluído com um salto de linha (`\n`) para que o término do comando seja reconhecido. Assim, para enviar a ação de mover para a esquerda, por exemplo, deve-se enviar `LEFT\n`.

Para o barco do bot se mover na área de pesca, ele deve realizar uma das seguintes ações (enviar a string para a saída padrão):

* `LEFT`: o bot informa que quer se deslocar para esquerda.
* `RIGHT`: o bot informa que quer se deslocar para direita.
* `UP`: o bot informa que quer se deslocar para cima.
* `DOWN`: o bot informa que quer se deslocar para baixo.

O resultado da solicitação de deslocamento é recebido logo em seguida (deve ser lido da entrada padrão) e pode ser uma das seguintes strings:

* `DONE`: indica que o barco se deslocou na direção solicitada.
* `BUSY`: indica que o deslocamento não foi realizado devido à célula para a qual o bot quer se delocar já estar ocupada por outro bot. O único local onde pode haver vários barcos concomitantemente são os portos. Nos demais, apenas um barco pode ocupar o lugar.
* `OUT`: indica que o deslocamento não foi realizado devido à célula para a qual o bot quer se delocar se encontrar fora da área de pesca (limites da matriz). Nesse caso, os órgãos de controle multam o bot por ir além da área de pesca permitida. Ou seja, além de não ter a ação realizada, o bot paga uma multa de R$ 500. Esse valor é automaticamente deduzido do seu saldo.


