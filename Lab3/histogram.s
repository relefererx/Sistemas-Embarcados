 
 
THUMB

  PUBLIC EightBitHistogram
  SECTION .text: CODE
  
  
InicializaHistograma   ; Preenche histograma com zeros
  STRH R8, [R3], #2
  ADD R7, #1
  CMP R7, #256 ;verifica se ja atingiu os 256 bits
  
  BNE InicializaHistograma ; loop
  
  BX LR ; retorna para EightBitHistogram
   
    
PreencheHistograma

  MOV R3, R9         ; recupera inicio do vetor do histograma armazenado em R9
  LDRB R8, [R2], #1  ; lê o valor da posicao atual da imagem e incremneta R2 = R2 +1 para avançar um pixel
  ADD R8, R8         ; dobra o valor de R8, pois o vetor do histograma está em 16 bits
  
  LDR R10, [R3, R8]  ; lê o valor atual na posição do vetor e incrmenta um
  ADD R10, #1        ; incrementa 1
  
  ADD R3, R8         ; incrementa a posição do vetor no número do pixel correspondente e guarda o valor atualizado
  STR R10, [R3]

  ADD R7, #1   ; incrementa em 1 R7 
  CMP R7, R6   ; compara se chegou no final da imagem
  
  BNE PreencheHistograma
  BX LR

 
EightBitHistogram
    
  ;R0 = WIDTH
  ;R1 = HEIGHT
  ;R2 = ponteiro vetor da imagem
  ;R3 = vetor do histograma
  PUSH {R6-R10}
  MUL R6, R0, R1 ; altura x largura = tamanho da imagem e armazena em em R6
  MOV R9, R3     ; salva em R9 o inicio do vetor de histograma

  CMP R6, #65536 ; verifica se a imagem é menor que 64K (65.536) pixels
  ITT GT
    MOVGT R0, #0  ;retorna  0
    BXGT LR ; retorna ao main.cpp

  MOV R7, #0   ;zera registradores
  MOV R8, #0   ;zera registradores
  PUSH {LR}    ; armazena na pilha o ponto de retorno para main.cpp
  BL InicializaHistograma ;salto para funcao
  POP {LR}     ; salva em LR o o ponto de retorno para main.cpp
    
  MOV R7, #0   ;zera registradores
  PUSH {LR}     ; armazena na pilha o ponto de retorno para main.cpp
  BL PreencheHistograma ;salto para funcao
  POP {LR}    ; salva em LR o o ponto de retorno para main.cpp
  MOV R0, R6 ;Guarda o tamanho da imagem em R0 e retorna

  POP {R6-R10}
      
  BX LR
	
  END