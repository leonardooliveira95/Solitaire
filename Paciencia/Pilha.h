struct carta
{
	int num;
	char naipe;
	char cor;
	char virada;
};

struct pilha
{
	struct carta *c;
	struct pilha *prox;
};

typedef struct carta Carta;
typedef struct pilha Pilha;

//Inicializa estrutura
void initPilha(Pilha **p)
{
	*p = NULL;
}

//Pergunta se a pilha está vazia
char isEmptyPilha(Pilha *p)
{
	return p == NULL;
}

//Retorna a carta no topo da pilha
Carta* topo(Pilha *p)
{
	if(!isEmptyPilha(p))
		return p->c;
	
	return NULL;
}

//Insere carta na pilha
void push(Pilha **p,Carta *c)
{
	Pilha *nova;
	
	nova = (Pilha*)malloc(sizeof(Pilha)); 
	
	nova->c = c; 
	nova->prox = *p; 
	
	*p = nova; 
}

//Retira carta da pilha
void pop(Pilha **p,Carta **x)
{
	Pilha *aux;

	if(!isEmptyPilha(*p))
	{
		aux = *p;
		*x = (*p)->c;
		*p = (*p)->prox;
		free(aux);
	}
	else
		x = NULL;
}

//Exibe a pilha
void show(Pilha *p)
{
	if(p != NULL)
	{
		printf("Numero - %d, Naipe - %c\n",p->c->num,p->c->naipe);
		show(p->prox);
	}
}

//Concatena p1 e p2 e devolve o resultado em p3
void concat(Pilha *p1,Pilha *p2,Pilha **p3)
{
	Pilha *temp;
	Carta *c = NULL;
	
	initPilha(&temp);
	initPilha(p3);

	while(p1 != NULL)
	{
		pop(&p1,&c);
		push(&temp,c);
	}

	while(temp != NULL)
	{
		pop(&temp,&c);
		push(p3,c);
	}

	while(p2 != NULL)
	{
		pop(&p2,&c);
		push(&temp,c);
	}

	while(temp != NULL)
	{
		pop(&temp,&c);
		push(p3,c);
	}

}

//Inverte a ordem da pilha
void invertePilha(Pilha **p)
{
	Pilha *aux;
	Carta *c;

	initPilha(&aux);

	while(!isEmptyPilha(*p))
	{
		pop(p,&c);
		push(&aux,c);
	}

	*p = aux;
}

//Copia pilha p1 em p2
void copiaPilha(Pilha *p1,Pilha **p2)
{
	Pilha *aux_pilha;
	Carta *aux_carta;

	initPilha(&aux_pilha);

	while(p1 != NULL)
	{
		push(&aux_pilha,p1->c);
		p1 = p1->prox;
	}

	while(!isEmptyPilha(aux_pilha))
	{
		pop(&aux_pilha,&aux_carta);
		push(p2,aux_carta);
	}
}

//Função para destruir uma pilha
void destroiPilha(Pilha **p)
{
	Carta *c;

	while(!isEmptyPilha(*p))
	{
		pop(p,&c);
		free(c);
	}
}

//Cria uma nova carta
Carta* novaCarta(char naipe,int num)
{
	Carta *Nova;
	
	Nova = (Carta*)malloc(sizeof(Carta));
	Nova->naipe = naipe;
	Nova->num = num;
	Nova->cor = naipe == 3 || naipe == 4 ? 'V' : 'P';
	Nova->virada = 1;

	return Nova;
}