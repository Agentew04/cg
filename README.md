# Computação Gráfica

## Descrição
Este é um repositório para organizar meus trabalhos da disciplina de
Computação Gráfica(UFSM - ELC1015). Também contém uma versão modificada
da biblioteca Canvas2D do professor Cesar Pozzer.

A pasta Template tem um projeto vazio que contém muitas funções de apoio
e de UI.

## Trabalhos:
* [T1](./Trab1RodrigoAppelt): Manipulação e renderização de imagens BMP
  * UI básica e reutilizável
  * Renderização de imagens
  * Transformações lineares com matrizes
* T2: Visualização da DCT(Transformada Discreta do Cosseno) _(Opcional)_
* [T3](./Trab3RodrigoAppelt): Recriação do jogo [Balls Bounce](https://play.google.com/store/apps/details?id=com.gimica.ballbounce&hl=en_US).
  * Física 2D e operações com vetores
  * Modularidade e reusabilidade de componentes
  * TDD para classes matemáticas
  * Carregamento de arquivos 3D
* T4: Não divulgado
* T5: Não divulgado

## Ferramentas criadas

Dentre as ferramentas e tecnologias criadas para melhorar a qualidade dos trabalhos
e a facilidade de desenvolvimento, estão:
* PersistentStorage: Uma classe que facilita a persistência de dados em arquivos
* UI: Coleção de controles totalmente customizáveis e reutilizáveis usando geometria
* Math: Coleção de componentes matemáticas testadas: Vetores, Matrizes e Polígonos
* ObjLoader: Carregador de arquivos .obj simples para a renderização de formas complexas
  sem usar bmps ou bibliotecas externas. No futuro, será usado para renderizar fontes
  customizadas sem a necessidade de uma biblioteca ttf.

E algumas bibliotecas externas pequenas embutidas direto no código:
* schrift.c: Usado para carregar arquivos .ttf. Não utilizado ainda
* lodepng.c: Usado para carregar arquivos .png. Não utilizado ainda
* htttlib.h: Usado para fazer requisições HTTP. Utilizado para placar online

## Demonstrações:

### T1

### T3

