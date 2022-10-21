#LAB01
Alunos: Rafael Pinheiro e Gabriel Bento Villanti

P: Onde aparecem as mensagem enviadas via cout ?

R: As mensagens aparecem no Terminal I/O que emula hardwares de input e output como um display por exemplo.

P: Como entrar dados para leitura via cin?

R: Utilizando a caixa "input" no Terminal I/O que nesse caso emula um dispositivo de input como um teclado.

P: Quais os valores dos símbolos pré-definidos que você escolheu ? o que significam ?

R: _cplusplus: 201703: é definido se o compilador utilizado é c++. Seu valor representa ao padrão C++ que o compilador utiliza para compilar.
   __DATE__: Mar 4 2022: Data da compilação da main utilizada.
   __FILE__: D:\Faculdade\Embarcados\ELF74\ELF74\Lab1\main.cpp: nome do arquivo compilado e executado. É exibido o caminho completo do arquivo.
   
P: Que tipo de instrução assembly foi usada para a soma dos dois valores float ? Qual configuração do projeto no IAR causou o uso desta instrução ?

R: Foi utilizado o comando VLDR(floating-point) em conjunto com VADD. Foi a configuração de floating-points em 32bits onde foi setado para single precision.  