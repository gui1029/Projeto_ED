#include "svg.h"
#include "arquivo.h"
#include "estabelecimento.h"
#include <string.h>
#include <stdlib.h>

struct svg{
  arquivo saida;
};

svg criaSVG(char * caminho){
  struct svg *this;
  this = (struct svg*) malloc(sizeof(struct svg));
  this->saida = abreArquivo(caminho, ESCRITA);
  if(this->saida == NULL){
    free(this);
    return NULL;
  }
  escreveLinha(this->saida, "<svg xmlns=\"http://www.w3.org/2000/svg\" fill-opacity='0.6'>\n");
  return (void *)this;
}


void escreveTexto(svg arq_svg, texto text){
  struct svg *this;
  float x,y;
  char *aux;
  this = (struct svg *) arq_svg;
  x = getXtexto(text);
  y = getYtexto(text);
  aux = getTexto(text);
  escreveLinha(this->saida, "<text x=\"%.1f\" y=\"%.1f\">%s</text>", x, y, aux);
}


void escreveLinhaSVG(svg arq_svg, char* texto){
  struct svg *this;
  this = (struct svg *) arq_svg;
  escreveLinha(this->saida,"%s", texto);
}

void *getSvgArq(svg arquivo_svg){
  struct svg *this;
  this = (struct svg *) arquivo_svg;
  return this->saida;
}

void desenhaFigura(svg arq_svg, figura fig){
  struct svg* this;
  float x, y, r, w, h;
  char *cor1, *cor2, *espessura;
  char *aux;
  this = (struct svg *) arq_svg;

  x = getXfig(fig);
  y = getYfig(fig);
  w = getW(fig);
  h = getH(fig);
  r = getR(fig);
  cor1 = getCor1(fig);
  cor2 = getCor2(fig);
  espessura = getEspessura(fig);

  switch(getTipoFigura(fig)){

    case CIRCULO:
      escreveLinha(this->saida, "<circle cx=\"%.1f\" cy=\"%.1f\" r=\"%.1f\" stroke=\"%s\" fill=\"%s\" stroke-width=\"%s\"/>", x, y, r, cor1, cor2, espessura);
      // printf("ENDERECO: %p  | %p   | %p  | %p\n", this->saida, cor1, cor2, espessura);
      break;

    case RETANGULO:
      escreveLinha(this->saida, "<rect x=\"%.1f\" y=\"%.1f\" width=\"%.1f\" height=\"%.1f\" stroke=\"%s\" fill=\"%s\" stroke-width=\"%s\"/>", x, y, w, h, cor1, cor2, espessura);
      break;

  }

}


void desenhaQuadra(svg arq_svg, quadra q){
  struct svg* this;
  this = (struct svg *) arq_svg;
  figura rect;
  rect = getQuadraRect(q);
  desenhaFigura(arq_svg, rect);
  escreveLinha(this->saida, "<text x=\"%.1f\" y=\"%.1f\" text-anchor=\"middle\" font-size=\"10px\">%s</text>", getXfig(rect) + getW(rect)/4, getYfig(rect) + getH(rect)/4, getCep(q));
}


void desenhaEquipamento(svg arq_svg, equipamento h){
  figura circ;
  texto text;
  circ = getEquipamentoCirculo(h);
  text = getEquipamentoTexto(h);
  desenhaFigura(arq_svg, circ);
  escreveTexto(arq_svg, text);
}

void desenhaLineSVG(svg arq_svg, segmento s){
  struct svg* this;
  this = (struct svg *) arq_svg;
  float x1, y1, x2, y2;
  x1 = segGetX1(s);
  x2 = segGetX2(s);
  y1 = segGetY1(s);
  y2 = segGetY2(s);

  escreveLinha(this->saida, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" style=\"stroke:rgb(0,0,0);stroke-width:2\" />", x1, y1, x2, y2);
}


void desenhaPredio(svg arq_svg, predio p){
  struct svg* this;
  this = (struct svg *) arq_svg;
  figura rect;
  rect = getPredioRect(p);
  desenhaFigura(arq_svg, rect);
  float x,y,xr,yr,w,h;

  xr = getXfig(rect);
  yr = getYfig(rect);
  w = getW(rect);
  h = getH(rect);

  if(!strcmp(getPredioFace(p), "N")){
    x = xr + w/2;
    y = yr + h - 1;
  }else if(!strcmp(getPredioFace(p), "S")){
    x = xr + w/2;
    y = yr + 3;
  }else if(!strcmp(getPredioFace(p), "L")){
    x = xr;
    y = yr + h/2;
  }else if(!strcmp(getPredioFace(p), "O")){
    x = xr + w;
    y = yr + h/2;
  }
  escreveLinha(this->saida, "<text x=\"%.1f\" y=\"%.1f\" text-anchor=\"middle\" font-size=\"3px\">%s</text>", x ,y, getPredioCep(p));
}

void desenhaEstab(svg arq_svg, void* estabelecimento){
  struct svg* this;
  this = (struct svg *) arq_svg;
  figura rect;
  rect = criaRetangulo(-1,5,5,getEstabX(estabelecimento),getEstabY(estabelecimento),"green","black","1.0");
  desenhaFigura(arq_svg, rect);
  apagaFigura(rect);
}


void fechaSVG(svg arq_svg){
  struct svg *this;
  this = (struct svg *) arq_svg;
  escreveLinha(this->saida, "</svg>");
  fechaArquivo(this->saida);
  free(this);
}
