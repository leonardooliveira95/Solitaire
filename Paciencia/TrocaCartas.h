//Função para retirar uma carta do monte e colocar na pilha de cartas viradas
void retiraCartaMonte(Lista_C **lc)
{
	Carta *c;

	pop(&(*lc)->l->cartas,&c);
	(*lc)->l->num_cartas--;
	
	c->virada = 0;

	push(&(*lc)->prox->l->cartas,c);
	(*lc)->prox->l->num_cartas++;
}

//Função para reempilhar o monte quando ele estiver vazio
void reempilhaMonte(Pilha **Viradas,Pilha **Monte)
{
	Carta *c;

	while(!isEmptyPilha(*Viradas))
	{
		pop(Viradas,&c);
		push(Monte,c);
	}
}

//Função para desvirar a carta anterior
void desviraCarta(char tecla_origem,char tecla_destino,Pilha *pont)
{
	//Desvira a carta anterior das temporárias
	if (tecla_origem >= 49 && tecla_origem <= 55 && pont->prox != NULL)
		pont->prox->c->virada = 0;
}

//Função para processar pontuação
void processaPontos(char tecla_origem, char tecla_destino,int *pontos,Pilha *Origem)
{
	//Se a origem for uma pilha de cartas finais, perde pontos
	if (tecla_origem >= 97 && tecla_destino <= 100)
	{
		if (*pontos < 10)
			*pontos = 0;
		else
			*pontos -= 10;
	}
	else
	{
		if (tecla_origem >= 49 && tecla_origem <= 55) //Se a origem for uma temporária, só ganha pontos se a anterior estiver virada
		{
			if (Origem->prox != NULL && Origem->prox->c->virada == 1)
				*pontos += 10;
			else
			if (tecla_destino >= 97 && tecla_destino <= 100) //Se a origem for uma temporária e o destino uma final, ganha pontos
				*pontos += 10;
		}
		else
			*pontos += 10;
	}
}

//Percorre a pilha para devolver ponteiro em seleção múltipla
void posicionaMult(Pilha *comeco,Pilha **pont,int cont)
{
	int i = 0;
	
	*pont = comeco;
	while(*pont != NULL && i < cont)
	{
		*pont = (*pont)->prox;
		i++;
	}
}

//Posiciona o ponteiro na carta da tecla pressionada
void posicionaPonteiro(Lista_C **lc,Lista **Ponteiro,char tecla)
{
	int i;
	
	if(tecla >= 97 && tecla <= 100) // Entre A e D
	{
		*Ponteiro = (*lc)->prox->prox->l;

		for(i = 97;i < tecla;i++)
			*Ponteiro = (*Ponteiro)->prox;
	}
	else
	if(tecla >= 49 && tecla <= 55) // Entre 1 e 7
	{
		*Ponteiro = (*lc)->prox->prox->prox->l;

		for(i = 49;i < tecla;i++)
			*Ponteiro = (*Ponteiro)->prox;
	}
	else
		if(tecla == 43)
			*Ponteiro = (*lc)->prox->l;
}

//Função para efetuar a troca das cartas
void trocaCartas(Lista **Origem,Lista **Destino,int cont)
{
	int i;
	Pilha *aux;
	Carta *c;
	
	initPilha(&aux);
	
	if(cont > 0)
	{
		for(i = 0; i <= cont;i++)
		{
			pop(&(*Origem)->cartas,&c);
			(*Origem)->num_cartas--;

			push(&aux,c);
		}

		for(i = 0;i <= cont;i++)
		{
			pop(&aux,&c);

			push(&(*Destino)->cartas,c);
			(*Destino)->num_cartas++;
		}
	}
	else
	{
		pop(&(*Origem)->cartas,&c);
		(*Origem)->num_cartas--;

		push(&(*Destino)->cartas,c);
		(*Destino)->num_cartas++;
	}
}

//Função para controlar a troca de cartas
void controlaTrocaCartas(Lista_C **lc,char tecla_origem,char tecla_destino,int cont,int *pontos)
{
	Lista *Origem;
	Lista *Destino;
	Pilha *aux;
	
	initLista(&Origem);
	initLista(&Destino);
	initPilha(&aux);

	posicionaPonteiro(lc,&Origem,tecla_origem);
	posicionaPonteiro(lc,&Destino,tecla_destino);
	
	if(!isEmptyLista(Origem) && !isEmptyLista(Destino) && !isEmptyPilha(Origem->cartas))
	{
		//Se a pilha de destino esiver vazia, a carta de origem deve ser um rei ou um ás
		if(isEmptyPilha(Destino->cartas))
		{
			if(tecla_destino >= 97 && tecla_destino <= 100) // Destino são as finais, logo a primeira carta deve ser um ás
			{
				if(Origem->cartas->c->num == 1) //Se a carta for um ás
				{
					processaPontos(tecla_origem, tecla_destino, pontos,Origem->cartas);
					desviraCarta(tecla_origem, tecla_destino, Origem->cartas);
					trocaCartas(&Origem,&Destino,cont);
				}
			}
			else
				if(tecla_destino >= 49 && tecla_destino <= 55) // Destino são as temporárias, logo a primeira carta deve ser um rei
				{
					posicionaMult(Origem->cartas,&aux,cont);

					if (aux->c->num == 13) //Se a carta for um rei
					{
						processaPontos(tecla_origem, tecla_destino, pontos,aux);
						desviraCarta(tecla_origem, tecla_destino, aux);
						trocaCartas(&Origem, &Destino,cont);
					}
				}
		}
		else
		{
			//Se o destino for uma pilha temporária, os naipes devem alternar entre si
			if(tecla_destino >= 49 && tecla_destino <= 55)
			{
				posicionaMult(Origem->cartas,&aux,cont);
				
				//Se os naipes forem de cores diferentes
				if(aux->c->cor == 'V' && Destino->cartas->c->cor == 'P')
				{
					if(aux->c->num + 1 == Destino->cartas->c->num) //Se a carta de origem for um numero menor que a de destino
					{
						processaPontos(tecla_origem, tecla_destino, pontos,aux);
						desviraCarta(tecla_origem, tecla_destino, aux);
						trocaCartas(&Origem,&Destino,cont);
					}
				}
				else
				{
					if(aux->c->cor == 'P' && Destino->cartas->c->cor == 'V')
					{
						if(aux->c->num + 1 == Destino->cartas->c->num)
						{
							processaPontos(tecla_origem, tecla_destino, pontos,aux);
							desviraCarta(tecla_origem, tecla_destino, aux);
							trocaCartas(&Origem,&Destino,cont);
						}
					}
				}
			}
			else
			{
				//Se o destino for uma pilha final, os naipes devem ser iguais
				if(tecla_destino >= 97 && tecla_destino <= 100)
				{
					//Se os naipes forem iguais
					if(Origem->cartas->c->naipe == Destino->cartas->c->naipe)
					{
						if (Origem->cartas->c->num - 1 == Destino->cartas->c->num) //Se a carta de origem for um numero menor que a de destino
						{
							processaPontos(tecla_origem, tecla_destino, pontos,Origem->cartas);
							desviraCarta(tecla_origem, tecla_destino, Origem->cartas);
							trocaCartas(&Origem, &Destino,cont);
						}
					}
				}
			}
		}	
	}
}