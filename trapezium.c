/*************************************************************************************************/
/*** Nome do aplicativo: trapezium	**********************************************************/
/*************************************************************************************************/
/********************* Instruçoes para compilação e execução do programa *************************/
/*
		1.	Para compilar o arquivo, deve-se usar o compilador gcc do linux.
		2.	Usando o terminal, passe o seguinte comando:
		3.	gcc trapezium.c -lm -o <nome do programa que sera gerado>
		4.	Exemplo:  gcc trapezium.c -lm -o teste
		5.	Caso nao tenha ocorrido nenhum erro, o arquivo teste sera gerado
		6.	Para executar-lo, devera usar o seguinte comando
		7.	 ./<nome do programa que sera gerado> <arquivo-entrada> <nome do arquivo de saida>
		8.	Exemplo: ./teste Entrada1.txt saida
		9.	se tudo correr bem, o arquivo de saida com o script R sera gerado.
*/
/*************************************************************************************************/
/*****  Ambiente(s) de Desenvolvimento utilizado(s): Dev-C++ 5.11 e	NetBeans IDE 8.2 *********/
/*************************************************************************************************/
/***** Data de inicio da implementação do codigo: 03/06/2018 *************************************/
/***** Data de verificação final do codigo: 07/06/2018 *******************************************/
/*************************************************************************************************/
/********** Algoritmo para criar um polinomio interpolador com base em uma funcao tabelada     ***/
/********** e apartir desse polinomio, calcular a integral definida da funcao no intervalo a|b ***/
/*************************************************************************************************/
/*************************************************************************************************/
/***** Nome: Pedro Daniel Camargos Soares 		Matricula: 0020640      ******************/
/*************************************************************************************************/
/***** Nome: Lucas Gabriel de Almeida		 	Matricula: 0035333	******************/
/*************************************************************************************************/
/*************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string.h>

float area(float base1, float base2, float h_altura){ //Funcao para calcular a area de um unico trapezio
	float area;
	area = ((base1+base2) * h_altura)/2; //Formula do trapezio
	//area = fabs(area);
	return area;
}

//Funcao que avalia o polinomio pelo metodo de horner
float avaliacao_polinomio(float *coeficientes_polinomio,float ponto, int grau_polinomio){ 
	int i,j;
	float valor_polinomio = 0;
	
	valor_polinomio = coeficientes_polinomio[grau_polinomio]; //Pega o elemento de maior grau
	for(i=grau_polinomio-1;i>=0;i--){
                //Formula do polinomio horner
		valor_polinomio = valor_polinomio * ponto + coeficientes_polinomio[i]; 
	}
	return valor_polinomio;
		
	// Metodo para avaliar o polinomio menos eficiente
        // Estavamos tendo problema com o polinomio de horner, mas corrigimos o problema.
	/* 
	int i,j;
	float valor_polinomio = 0;
	
	for(i=0;i<=grau_polinomio;i++){
		valor_polinomio = valor_polinomio+ coeficientes_polinomio[i]*pow(ponto,i);
	}
	return valor_polinomio;
	*/
        
}

// Calcula a integral da funcao polinomial
float area_total(int t_trap, int n_pontos_tabelados, float b_lim_sup, float a_lim_inf, float *coeficientes_polinomio){
	
	int j,i;
	float y_polinomio[t_trap],h_altura;
	int grau_polinomio = n_pontos_tabelados-1;
	h_altura = (b_lim_sup-a_lim_inf)/t_trap;
	float base1;
	float base2;
	float area_trapezio[t_trap],areatotal = 0;
	
	//printf("\nAltura %.2f\n\n\n",h_altura);
	
	float pontos_calcular[t_trap+1]; //Define a quantidade de pontos para calcular(Quantidade de trapezios + 1)
	
	
	pontos_calcular[0] = a_lim_inf; //Inicio da base de cada trapezio
	for(j=1;j<t_trap+1;j++){
		pontos_calcular[j] = pontos_calcular[j-1]+h_altura;
	}
	
	/*
	for(j=0;j<t_trap+1;j++){
		printf("\nPonto %d: \t%.2f",j, pontos_calcular[j]);
	}
	*/
	
	for(i=0;i<t_trap+1;i++){ //Encontra o y de cada base dos trapezios
		y_polinomio[i] = avaliacao_polinomio(coeficientes_polinomio, pontos_calcular[i],grau_polinomio);
		//printf("\nY do polinomio %d: \t%.2f",i, y_polinomio[i]);
	}
	
	
	for(i=0;i<t_trap;i++){ //Calcula a area dos trapezios
		base1 = y_polinomio[i];
		base2 = y_polinomio[i+1];
		
		area_trapezio[i] = area(base1,base2, h_altura);//Calcula a area de cada trapezio
		//printf("\n\t %.3f", area_trapezio[i]);
		areatotal = areatotal + (area_trapezio[i]); //Calcula a area total da funcao
	}
	return areatotal;
}

//Gera o script do [R]
float construcao_script(int n_pontos_tabelados, float *x_tabela, float *y_tabela,int i_num_pont, float *p_pontos,float *y_polinomio, float *coeficientes_polinomio,int t_trap,int argc, char** argv){
	int i,j;


	FILE *saida;

	saida = fopen(argv[2],"w"); //Abre o arquivo
	//saida = fopen("saida.txt","w"); //Abre o arquivo

        //Gera um arquivo de saida com os dados encontrados pela iterpolacao da funcao
	
	fprintf(saida,"######################################################################\n");
	fprintf(saida,"# Script automatico gerado por 'trapezium', software de interpolação #\n");
	fprintf(saida,"# e integracao numerica #\n");
	fprintf(saida,"######################################################################\n\n");
	fprintf(saida,"#\n# Parametros. Devem ser preenchidos pelo software\n#\n\n");
	fprintf(saida,"# Nome da figura\n");
	fprintf(saida,"nome <- '%s.png'\n\n",argv[2]);
	fprintf(saida,"# Dados tabelados\n");
	
	
	

	fprintf(saida,"x.tab <- c( ");
	fprintf(saida,"%.3f",x_tabela[0]);
	for(i=1;i<n_pontos_tabelados;i++){
		fprintf(saida,", %.3f",x_tabela[i]);
	}
	fprintf(saida,");\n");
	
	fprintf(saida,"y.tab <- c( ");
	fprintf(saida,"%.3f",y_tabela[0]);
	for(i=1;i<n_pontos_tabelados;i++){
		fprintf(saida,", %.3f",y_tabela[i]);
	}
	fprintf(saida,");\n\n");
	

	fprintf(saida,"# Pontos interpolados, calculados pelo 'trapezium'\n");
	
	fprintf(saida,"x.int <- c( ");
	fprintf(saida,"%.3f",p_pontos[0]);
	for(i=1;i<i_num_pont;i++){
		fprintf(saida,", %.3f",p_pontos[i]);
	}
	fprintf(saida,");\n");
	
	fprintf(saida,"y.int <- c( ");
	fprintf(saida,"%.3f",y_polinomio[0]);
	for(i=1;i<i_num_pont;i++){
		fprintf(saida,", %.3f",y_polinomio[i]);
	}
	fprintf(saida,");\n\n");
	
	
	fprintf(saida,"# Coeficientes do polinomio interpolador\n");
	
	fprintf(saida,"coef <- c( ");
	fprintf(saida,"%.3f",coeficientes_polinomio[0]);
	for(i=1;i<n_pontos_tabelados;i++){
		fprintf(saida,", %f",coeficientes_polinomio[i]);
	}
	fprintf(saida,");\n\n");
	
	
	fprintf(saida,"# Numero de pontos da tabela\n");
	fprintf(saida,"n.tab <- %d;\n\n",n_pontos_tabelados);

	fprintf(saida,"# Numero de pontos a interpolar\n");
	fprintf(saida,"n.int <- %d;\n\n",i_num_pont);

	fprintf(saida,"# Numero de trapezios\n");
	fprintf(saida,"n.tpz <- %d;\n\n",t_trap);

	fprintf(saida,"# Titulo\n");
	fprintf(saida,"titulo <- 'P(x) = ");
	fprintf(saida,"%.3f",coeficientes_polinomio[0]);
	j=1;
	for(i=1;i<n_pontos_tabelados;i++){
		fprintf(saida,", %f*x^%d",coeficientes_polinomio[i],i);
		if(j==3){
			fprintf(saida,"\\n");
			j=-1;
		}
		j++;
	}
	fprintf(saida,"'\n\n");

	fprintf(saida,"#\n");
	fprintf(saida,"# Esta parte do script deve funcionar desde que os parametros\n");
	fprintf(saida,"# acima estejam devidamente preenchidos. E' a parte estatica\n");
	fprintf(saida,"# do script. Copiar exatamente desta forma no arquivo de saida.\n");
	fprintf(saida,"#\n\n");
	
	fprintf(saida,"# Calcula o valor interpolado para o pto x\n");	
	fprintf(saida,"polinomio <- function(x, coef, n){\n");
	fprintf(saida,"\tresultado <- 0;\n");
	fprintf(saida,"\tfor(i in 1:n){\n");
	fprintf(saida,"\t\tresultado <- resultado + coef[i]*(x^(i-1));\n");
	fprintf(saida,"\t}\n");
	fprintf(saida,"\treturn(resultado);\n");
	fprintf(saida,"}\n\n");

	fprintf(saida,"#\n");
	fprintf(saida,"# Aqui comecam os comandos para plotar os resultados\n");
	fprintf(saida,"#\n\n");
	
	fprintf(saida,"# Cria o arquivo .png\n");
	fprintf(saida,"png(filename = nome);\n\n");
	
	fprintf(saida,"# Gerando figura com 100 pontos\n");
	fprintf(saida,"gap <- (max(x.tab) - min(x.tab)) / 100;\n");
	fprintf(saida,"x <- seq(min(x.tab), max(x.tab), gap);\n");
	fprintf(saida,"y <- polinomio(x, coef, n.tab);\n");
	fprintf(saida,"plot(x,y,type='l', main=titulo);\n\n");
	
	fprintf(saida,"# Plota os trapezios\n");
	fprintf(saida,"h <- (max(x.tab) - min(x.tab)) / n.tpz;\n");
	fprintf(saida,"xp <- seq(min(x.tab), max(x.tab), h);\n");
	fprintf(saida,"yp <- polinomio(xp, coef, n.tab);\n");
	fprintf(saida,"for(i in 1:(n.tpz)){\n");
	fprintf(saida,"\tpolygon(c(xp[i], xp[i], xp[i+1], xp[i+1], xp[i]),\n");
	fprintf(saida,"\tc(0, yp[i], yp[i+1], 0, 0),\n");
	fprintf(saida,"\tcol='yellow', border='black', lty=2, lwd=1.3);\n");
	fprintf(saida,"}\n\n");
	
	fprintf(saida,"# Pontos da tabela\n");
	fprintf(saida,"for(i in 1:n.tab){\n");
	fprintf(saida,"\tpoints(x.tab[i], y.tab[i], col='red', pch=19);\n");
	fprintf(saida,"}\n\n");
	
	fprintf(saida,"# Pontos interpolados\n");
	fprintf(saida,"for(i in 1:n.tab){\n");
	fprintf(saida,"\tpoints(x.int[i], y.int[i], col='blue', pch=19);\n");
	fprintf(saida,"}\n\n");
	fprintf(saida,"# Encerra a criacao do arquivo .png\n");	
	fprintf(saida,"dev.off();\n");
	
	fclose(saida);
}

int main (int argc, char** argv){
	int i,j,k,cont;
	
	//Define as variaveis utilizadas para a leitura do arquivo
	char c; 	 //Ira ler os caracteres de comentario
	int n_pontos_tabelados = 0;   //Numero de pontos tabelados
	int i_num_pont = 0;   //Numero de pontos no intervalo a interpolar
	int t_trap = 0;   //Numero de trapezios a serem implementados
	float *x_tabela = NULL; //Armazena os valores de X(entrada) da tabela
	float *y_tabela = NULL; //Armazena os valores de X(saida) da tabela
	float a_lim_inf = 0; //Limite inferior da integral definida
	float b_lim_sup = 0; //Limite superior da integral definida
	float *p_pontos = NULL; //Numero de pontos a interpolar
	
	FILE *Entrada; //Cria um ponteiro para o arquivo de entrada
	
	Entrada = fopen(argv[1],"r"); //Abre o arquivo
	//Entrada = fopen("dados.txt", "r");
	/////////////
	
	
	if (Entrada == NULL){ //Verifica se o arquivo foi aberto com sucesso
		printf("O arquivo nao pode ser aberto!\n");
		return 0;
	}
	
	
	do{ //Percorre todos os comentarios que estao em sequencia ate nao houver mais nenhum
	fscanf(Entrada, "%c", &c); //Percorre o comentario
		i=0;
		if(c == '#'){
			i=1;
			do{
				fscanf(Entrada, "%c", &c); //Percorre o comentario
			} while (c != '\n' );
		}
	}while(i!=0);
	
	fscanf(Entrada, "\n%d", &n_pontos_tabelados); //Le o numero de pontos tabelados
	
	//Aloca memoria para o numero de pontos tabelados
	x_tabela = malloc(n_pontos_tabelados * sizeof(float)); 
	y_tabela = malloc(n_pontos_tabelados * sizeof(float));
	
	
	fscanf(Entrada, "\n"); //Linha de baixo
	do{//Percorre todos os comentarios que estao em sequencia ate nao houver mais nenhum
	fscanf(Entrada, "%c", &c); //Percorre o comentario
		i=0;
		if(c == '#'){
			i=1;
			do{
				fscanf(Entrada, "%c", &c); //Percorre o comentario
			} while (c != '\n' );
		}
	}while(i!=0);
	
	fscanf(Entrada, "%f", &x_tabela[0]); //Le o primeiro x
	for(cont = 1; cont<n_pontos_tabelados; cont++){ //Le o restante dos numeros
		fscanf(Entrada, "%f", &x_tabela[cont]);
	}
	
	fscanf(Entrada, "\n"); //Linha de baixo
	do{//Percorre todos os comentarios que estao em sequencia ate nao houver mais nenhum
	fscanf(Entrada, "%c", &c); //Percorre o comentario
		i=0;
		if(c == '#'){
			i=1;
			do{
				fscanf(Entrada, "%c", &c); //Percorre o comentario
			} while (c != '\n' );
		}
	}while(i!=0);
	
	fscanf(Entrada, "%f", &y_tabela[0]); //Le o primeiro y
	for(cont = 1; cont<n_pontos_tabelados; cont++){ //Le o restante dos numeros
		fscanf(Entrada, "%f", &y_tabela[cont]);
	}
	
	fscanf(Entrada, "\n"); //Linha de baixo
	do{//Percorre todos os comentarios que estao em sequencia ate nao houver mais nenhum
	fscanf(Entrada, "%c", &c); //Percorre o comentario
		i=0;
		if(c == '#'){
			i=1;
			do{
				fscanf(Entrada, "%c", &c); //Percorre o comentario
			} while (c != '\n' );
		}
	}while(i!=0);	
	
	fscanf(Entrada, " %f", &a_lim_inf); //Le o Liminte inferior
	
	fscanf(Entrada, "\n"); //Linha de baixo
	do{//Percorre todos os comentarios que estao em sequencia ate nao houver mais nenhum
	fscanf(Entrada, "%c", &c); //Percorre o comentario
		i=0;
		if(c == '#'){
			i=1;
			do{
				fscanf(Entrada, "%c", &c); //Percorre o comentario
			} while (c != '\n' );
		}
	}while(i!=0);
	
	fscanf(Entrada, "%f", &b_lim_sup); //Le o Liminte superior


	fscanf(Entrada, "\n"); //Linha de baixo
	do{//Percorre todos os comentarios que estao em sequencia ate nao houver mais nenhum
	fscanf(Entrada, "%c", &c); //Percorre o comentario
		i=0;
		if(c == '#'){
			i=1;
			do{
				fscanf(Entrada, "%c", &c); //Percorre o comentario
			} while (c != '\n' );
		}
	}while(i!=0);	
	
	fscanf(Entrada, "%d",&i_num_pont); //Le o numero de pontos no intervalo a interpolar
	p_pontos = malloc(i_num_pont * (sizeof(float))); // Aloca memoria para armazenar os pontos a serem interpolados	
	
	fscanf(Entrada, "\n"); //Linha de baixo
	do{//Percorre todos os comentarios que estao em sequencia ate nao houver mais nenhum
	fscanf(Entrada, "%c", &c); //Percorre o comentario
		i=0;
		if(c == '#'){
			i=1;
			do{
				fscanf(Entrada, "%c", &c); //Percorre o comentario
			} while (c != '\n' );
		}
	}while(i!=0);
	
	fscanf(Entrada, "%f", &p_pontos[0]); //Le o primeiro ponto a ser interpolado
	
	if (p_pontos[0] < a_lim_inf || p_pontos[0]> b_lim_sup){ //Verificador
		printf("Erro, o ponto a ser interpolado esta fora dos limites!");
		return 0;
	}
	
	for(cont = 1; cont<i_num_pont; cont++){
		fscanf(Entrada,"%f",&p_pontos[cont]); //Le o restante dos pontos a serem interpolados
			if (p_pontos[cont] < a_lim_inf || p_pontos[cont]> b_lim_sup){
				printf("Erro, o ponto a ser interpolado esta fora dos limites!");
			return 0;
		}
	}
	
	fscanf(Entrada, "\n"); //Linha de baixo
	do{//Percorre todos os comentarios que estao em sequencia ate nao houver mais nenhum
	fscanf(Entrada, "%c", &c); //Percorre o comentario
		i=0;
		if(c == '#'){
			i=1;
			do{
				fscanf(Entrada, "%c", &c); //Percorre o comentario
			} while (c != '\n' );
		}
	}while(i!=0);
	
	fscanf(Entrada,"%d", &t_trap); //Le o numero de trapezios
	
	fclose(Entrada);
	
	
	
	
	float coeficientes_polinomio[n_pontos_tabelados];
	float pivo, aux, mult,y_tabela_gauss[n_pontos_tabelados];
	int linha_pivo;
	float mat[n_pontos_tabelados][n_pontos_tabelados],matriz[n_pontos_tabelados][n_pontos_tabelados];
	float somatorio;
	float integral_polinomio;
	int grau_polinomio = n_pontos_tabelados-1;
                
        
	///////////////////
	//montando a tabela
	for(i=0;i<n_pontos_tabelados;i++){
		for(j=0;j<n_pontos_tabelados;j++){
			mat[i][j] = pow(x_tabela[i],j);
			matriz[i][j] = mat[i][j];
			//printf("%.2f\t",mat[i][j]);
		}
		//printf("\n\n");
	}


	//eliminação por gauss
	///////////////////
	
	for(i=0;i<n_pontos_tabelados;i++){
		y_tabela_gauss[i] = y_tabela[i];
	}
	
	for(i=0; i<(n_pontos_tabelados-1); i++){
		pivo = mat[i][i]; //Define a primeira linha como pivo
		linha_pivo = i;
		for(k=(i+1); k<n_pontos_tabelados; k++){ //Encontra a nova linha pivo
			if(abs(mat[k][i])> abs(pivo)){
				pivo = mat[k][i];
				linha_pivo = k;
			}
		}
		if(linha_pivo != i){ //Realiza a troca de linhas
			for(j=0; j<n_pontos_tabelados; j++){
				aux = mat[i][j];
				mat[i][j] = mat[linha_pivo][j];
				mat[linha_pivo][j] = aux;
			}
			aux = y_tabela_gauss[i];
			y_tabela_gauss[i] = y_tabela_gauss[linha_pivo];
			y_tabela_gauss[linha_pivo] = aux;
		}
		for(j=(i+1); j<n_pontos_tabelados; j++){ //Encontra o multiplicador
			mult = mat[j][i]/mat[i][i];
			for(k=0; k<n_pontos_tabelados; k++){
				mat[j][k] = mat[j][k] - mult*mat[i][k];
			}
			
			y_tabela_gauss[j] = y_tabela_gauss[j] - mult* y_tabela_gauss[i]; //Atualiza o valor de b
			
		}
	}
	
        //Polinomios interpoladores
	coeficientes_polinomio[n_pontos_tabelados] = y_tabela_gauss[n_pontos_tabelados]/mat[n_pontos_tabelados][n_pontos_tabelados];
	
	for(k=(n_pontos_tabelados-1); k>=0; k--){ 
		somatorio = 0;
		for(j=(k+1); j<n_pontos_tabelados; j++){
			somatorio = somatorio + mat[k][j] * coeficientes_polinomio[j];
		}
		coeficientes_polinomio[k] = (y_tabela_gauss[k] - somatorio) / mat[k][k];
	}
	
	
        //Saida no console
	printf("Trapezium: Interpolador/Integrador Numerico\n\n");
	printf("Polinomio Interpolador:\n\n");
	printf("p(x) = %f ",coeficientes_polinomio[0]);
	for(i=1;i<n_pontos_tabelados;i++){
		printf(" + %f*x^%d",coeficientes_polinomio[i],i); //Mostra na tela
	}
	printf("\n\nValores Interpolados:\n\n");
	
	
	float y_polinomio[i_num_pont];
	for(i=0;i<i_num_pont;i++){
		y_polinomio[i] = avaliacao_polinomio(coeficientes_polinomio, p_pontos[i],grau_polinomio);
		printf("P(%.2f) = %f\n",p_pontos[i], y_polinomio[i]);
	}
        
        //Calcula a integral do polinomio
	integral_polinomio = area_total(t_trap, n_pontos_tabelados, b_lim_sup, a_lim_inf, coeficientes_polinomio);
	printf("\n\nIntegral em [%.2f, %.2f] = %f\n",a_lim_inf,b_lim_sup,integral_polinomio);
		
	
	//Chama a funcao para construir o script
	 construcao_script(n_pontos_tabelados, x_tabela, y_tabela, i_num_pont, p_pontos,y_polinomio,coeficientes_polinomio,t_trap,argc, argv);
	
	
}
