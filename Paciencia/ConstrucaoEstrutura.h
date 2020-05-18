#define BARALHO 52

//Cria o baralho
void criaBaralho(Pilha **p)
{
	int i;

	for(i = 0;i < BARALHO; i++)
		push(p,novaCarta(i % 4 + 3,i % 13 + 1));

}

//Embaralha as cartas (Riffle Shuffle)
void embaralhar(Pilha **p)
{
	Pilha *p1;
	Pilha *p2;
	Pilha *aux;

	int i;
	int escolha;

	initPilha(&p1);
	initPilha(&p2);
	aux = *p;

	for(i = 0;i < BARALHO; i++)
	{
		escolha = rand() % 2;
		
		if(escolha == 0)
			push(&p1,aux->c);
		else
			push(&p2,aux->c);

		aux = aux->prox;
	}

	concat(p1,p2,p);
}

//Empilha um número de cartas em uma pilha
void empilhaCartas(Lista **l,Pilha **Origem,char virarUltima)
{
	int i;
	Carta *c;

	for(i = 0;i < (*l)->num_cartas;i++)
	{
		pop(Origem,&c); //Retira uma carta da origem
			
		//Se estiver na última carta, desvira
		if(i == (*l)->num_cartas - 1 && virarUltima == 1)
			c->virada = 0;
		
		push(&(*l)->cartas,c); //Insere a carta retirada no destino
	}
	
}

//Cria as 4 listas
void estruturaInicial(Lista_C **lc,Pilha **Baralho)
{
	int i,j;

	//Aloca memória para as listas
	for(i = 0;i < 4; i++)
		insertListaC(lc,NULL);

	for(i = 0;i < 4;i++)
	{
		if(i == 0) //Inicializa o monte
		{
			initLista(&(*lc)->l);
			insertLista(&(*lc)->l,NULL,24); //Tirando as 28 cartas das pilhas temporárias, sobram 24 no monte
			empilhaCartas(&(*lc)->l,Baralho,0);
		}
		else
		if(i == 1) //Inicializa a pilha de cartas viradas
		{
			initLista(&(*lc)->prox->l);
			insertLista(&(*lc)->prox->l,NULL,0);
		}
		else
		if(i == 2) //Inicializa lista de pilhas de cartas finais
		{
			initLista(&(*lc)->prox->prox->l);
					
			for(j = 0;j < 4;j++)
				insertLista(&(*lc)->prox->prox->l,NULL,0);
		}
		else
		if(i == 3) //Inicializa lista de pilhas de cartas temporárias
		{
			initLista(&(*lc)->prox->prox->prox->l);
						
			for(j = 7;j >= 1;j--)
			{
				insertLista(&(*lc)->prox->prox->prox->l,NULL,j);
				empilhaCartas(&(*lc)->prox->prox->prox->l,Baralho,1);
			}
		}
	}
}

//Cria estrutura inicial de cartas
void montaEstrutura(Lista_C **lc)
{
	int i;
	Pilha *Baralho;

	initPilha(&Baralho);
	
	criaBaralho(&Baralho); //Aloca memória para a pilha de cartas do baralho
	
	//Embaralha as cartas 5 vezes
	for(i = 0;i < 5;i++)
		embaralhar(&Baralho); 

	estruturaInicial(lc,&Baralho); //Monta a lista que guarda a estrutura das cartas

}

//Percorre a estrutura descalocando a memória de cada elemento
void limpaMemoria(Lista_C **lc)
{
	Lista *aux_lista;
	Pilha *aux_pilha;
	Carta *c;

	while(*lc != NULL)
	{
		aux_lista = (*lc)->l;
		
		while(aux_lista != NULL)
		{
			aux_pilha = aux_lista->cartas;
			
			while(aux_pilha != NULL)
			{
				pop(&aux_pilha,&c);
			}

			removeLista(&aux_lista);
		}

		removeListaC(lc);
	}
}