struct highScore
{
	char nome[4];
	unsigned long int pontos;
};

typedef struct highScore HighScore;

//Função para exibir o High Score
void exibeHighScore(ALLEGRO_FONT *fonte)
{
	HighScore hs;
	FILE *arq;
	int espaco = 0;

	al_clear_to_color(al_map_rgb(0, 0, 0));

	fonte = al_load_font("segoeuil.ttf", 40, 0);

	arq = fopen("HighScore.dat", "r");
	if (arq != NULL)
	{
		rewind(arq);
		
		al_draw_text(fonte, al_map_rgb(255, 255, 255), LARGURA / 2, 100, ALLEGRO_ALIGN_CENTRE, "High Scores");

		fread(&hs, sizeof(HighScore), 1, arq);
		while (!feof(arq))
		{
			al_draw_textf(fonte, al_map_rgb(255, 255, 255), LARGURA / 2, ALTURA / 2 + espaco, ALLEGRO_ALIGN_CENTRE, "Player: %s    Score: %d", hs.nome, hs.pontos);
			al_flip_display();
			fread(&hs, sizeof(HighScore), 1, arq);
			espaco += 40;
		}

		fclose(arq);
	}
	else
		al_draw_textf(fonte, al_map_rgb(255, 255, 255), LARGURA / 2, ALTURA / 2, ALLEGRO_ALIGN_CENTRE, "No high scores");

	al_flip_display();
	al_rest(10);
}

//Função para gravar o HighScore
void gravaHighScore(HighScore hs)
{
	HighScore hs2;
	int quant;

	FILE *arq;
	arq = fopen("HighScore.dat", "rb+");
	if (arq == NULL)
		arq = fopen("HighScore.dat", "wb+");

	fseek(arq, 0, 2);
	fwrite(&hs, sizeof(HighScore), 1, arq);

	fseek(arq, 0, 2);
	quant = ftell(arq) / sizeof(HighScore)-1;

	fseek(arq, quant*sizeof(HighScore), 0);
	fread(&hs, sizeof(HighScore), 1, arq);
	fseek(arq, (quant - 1)*sizeof(HighScore), 0);
	fread(&hs2, sizeof(HighScore), 1, arq);

	while (hs.pontos > hs2.pontos && quant > 0)
	{
		fseek(arq, (quant - 1)*sizeof(HighScore), 0);
		fwrite(&hs, sizeof(HighScore), 1, arq);
		fseek(arq, quant*sizeof(HighScore), 0);
		fwrite(&hs2, sizeof(HighScore), 1, arq);

		quant--;

		fseek(arq, quant*sizeof(HighScore), 0);
		fread(&hs, sizeof(HighScore), 1, arq);
		fseek(arq, (quant - 1)*sizeof(HighScore), 0);
		fread(&hs2, sizeof(HighScore), 1, arq);
	}


	fclose(arq);
}

//Função para ler o high score
void recebeHighScore(ALLEGRO_FONT *fonte, ALLEGRO_EVENT evento, ALLEGRO_EVENT_QUEUE *filaEventos, int pontos)
{
	int fim = 0;
	HighScore hs;

	strcpy(hs.nome, "");
	hs.pontos = pontos;

	fonte = al_load_font("segoeuil.ttf", 40, 0);

	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_textf(fonte, al_map_rgb(255, 255, 255), LARGURA / 2, ALTURA / 2, ALLEGRO_ALIGN_CENTRE, "Player: %s", hs.nome);
	al_flip_display();

	do
	{
		al_wait_for_event(filaEventos, &evento);
		if (evento.type == ALLEGRO_EVENT_KEY_CHAR)
		{
			//String temporária para guardar a tecla que foi pressionada
			char temp[] = { evento.keyboard.unichar, '\0' };

			if (strlen(hs.nome) < 3)
			{
				if (evento.keyboard.unichar == ' ')
				{
					strcat(hs.nome, temp);
				}
				else
				if (evento.keyboard.unichar >= '0' && evento.keyboard.unichar <= '9')
				{
					strcat(hs.nome, temp);
				}
				else if (evento.keyboard.unichar >= 'A' && evento.keyboard.unichar <= 'Z')
				{
					strcat(hs.nome, temp);
				}
				else if (evento.keyboard.unichar >= 'a' && evento.keyboard.unichar <= 'z')
				{
					strcat(hs.nome, temp);
				}
			}

			if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(hs.nome) != 0)
			{
				hs.nome[strlen(hs.nome) - 1] = '\0';
			}

			if (evento.type == ALLEGRO_EVENT_KEY_CHAR && evento.keyboard.keycode == ALLEGRO_KEY_ENTER)
			{
				fim = 1;
			}

		}

		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_textf(fonte, al_map_rgb(255, 255, 255), LARGURA / 2, ALTURA / 2, ALLEGRO_ALIGN_CENTRE, "Player: %s", hs.nome);
		al_flip_display();

	} while (!fim);

	gravaHighScore(hs);
}