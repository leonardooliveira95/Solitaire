struct lista
{
	Pilha *cartas;
	int num_cartas;
	struct lista *prox;
};

typedef struct lista Lista;

//Inicializa a estrutura
void initLista(Lista **l)
{
	*l = NULL;
}

//Pergunta se a lista está vazia
char isEmptyLista(Lista *l)
{
	return l == NULL;
}

//Insere um elemento na lista
void insertLista(Lista **l,Pilha *p,int Num_Cartas)
{
	Lista *nova;

	nova = (Lista*)malloc(sizeof(Lista));
	nova->num_cartas = Num_Cartas;
	nova->cartas = p;
	nova->prox = NULL;

	if(!isEmptyLista(*l))
	{
		nova->prox = *l;
		*l = nova;
	}
	else
		*l = nova;
}

//Remove um elemento da lista
void removeLista(Lista **l)
{
	Lista *aux;

	aux = *l;
	*l = (*l)->prox;
	free(aux);
}

void destroiLista(Lista **l)
{
	while(!isEmptyLista(*l))
		removeLista(l);
	
}