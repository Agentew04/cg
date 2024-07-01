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
* [T4](./Trab4RodrigoAppelt): Simulação de Pistão, Virabrequim, Engrenagens e Eixo Cardã em tempo real. Iluminação e rasterização.
  * Modelagem de objetos
  * Coordenadas polares
  * Projeção e 3D
  * Técnicas de iluminação
* T5: Não divulgado

## Ferramentas criadas e utilizadas

Dentre as ferramentas e tecnologias criadas para melhorar a qualidade dos trabalhos
e a facilidade de desenvolvimento, estão:
* **PersistentStorage:** Uma classe que facilita a persistência de dados em arquivos
* **UI:** Coleção de controles totalmente customizáveis e reutilizáveis usando geometria
* **Math:** Coleção de componentes matemáticas testadas: Vetores, Matrizes e Polígonos
* **ObjLoader:** Carregador de arquivos .obj simples para a renderização de formas complexas
  sem usar bmps ou bibliotecas externas. No futuro, será usado para renderizar fontes
  customizadas sem a necessidade de uma biblioteca ttf.
* **TaskManager:** Um gerenciador simples de corrotinas usado principalmente para mostrar
  informações de depuração por um tempo na tela.
* **ParticleManager:** Um gerenciador de partículas que usa objetos 3D para criar efeitos
  especiais.
* **WebManager:** Uma classe para simplificar o envio de requisições HTTP usando a biblioteca
  [httplib](https://github.com/Agentew04/cg/tree/main/Trab3RodrigoAppelt).
* **Perspective:** Classe que contém todas as operações e transformações que podem ser
   aplicadas a primitivas modelos 3D. Suporta manipulação de vetores.
* **Rasterizer:** Uma classe encarregada apenas de criar uma renderização pixel a pixel
   da cena com o máximo de performance possível(considerando apenas CPU e OpenGL immediate).

E algumas bibliotecas externas pequenas embutidas direto no código:
* ~**schrift.c:** Usado para carregar arquivos .ttf. Não utilizado ainda~
* ~**lodepng.cpp:** Usado para carregar arquivos .png. Não utilizado ainda~
* **httplib.h:** Usado para fazer requisições HTTP. Utilizado para placar online no T3
* **mingw.thread.h:** Usado para simular a funcionalidade de std::thread do MinGW em contextos
   que a versão do C++ não o suporta.

## Demonstrações:

### T1
TODO
### T3
TODO
### T4
TODO
