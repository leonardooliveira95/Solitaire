struct lista_controle
{
	Lista *l;
	struct lista_controle *prox;
};

typedef struct lista_controle Lista_C;

void initListaControle(Lista_C **lc)
{
	*lc = NULL;
}

char isEmptyListaC(Lista_C *lc)
{
	return lc == NULL;
}

void insertListaC(Lista_C **lc,Lista *l)
{
	Lista_C *nova;

	nova = (Lista_C*)malloc(sizeof(Lista_C));
	nova->l = l;
	nova->prox = NULL;

	if(!isEmptyListaC(*lc))
	{
		nova->prox = *lc;
		*lc = nova;
	}
	else
		*lc = nova;
}

void removeListaC(Lista_C **lc)
{
	Lista_C *aux;

	aux = *lc;
	*lc = (*lc)->prox;
	free(aux);
}

void destroiListaC(Lista_C **lc)
{
	while(!isEmptyListaC(*lc))
		removeListaC(lc);
}