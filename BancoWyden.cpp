#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h> // Biblioteca para tipo de dado booleano (true, false) 

/* 
Projeto de Avalia��o da disciplina Programa��o de Software B�sico em C 
Sistema Banc�rio 
Feito por: Aryane Santos Andrade
*/ 

// Estrutura para armazenar informa��es do Cliente 
typedef struct {
	char nome[50];
	char cpf[15];
	char senha[8];
	float salario;
	float saldo; 
    float limiteInicial; 
    float limiteCredito;
    float valorEmprestimo;
    float valorPIX; 
    float valorFinalEmprestimo; 
    int parcelasPIX; 
    int parcelasEmprestimo;
    int numeroCartao[16];
    int cvc[3];
    // Estrutura para armazenar datas da primeira parcela do emprestimo e PIX 
    struct tm dataPrimeiraParcelaEmprestimo; 
    struct tm dataPrimeiraParcelaPIX; 
}Cliente;


// Fun��o cadastro do cliente 
void cadastrarCliente(Cliente *cliente){
	printf("\nBem-vindo(a) ao Banco Wyden!\n");
	printf("\nInicie seu cadastro \n");
	printf("\nNome: ");
	scanf("%[^\n]s", cliente->nome);
	printf("CPF: ");
	scanf("%s", cliente->cpf);
	printf("Salario: ");
	scanf("%f", &cliente->salario);
	printf("Senha: ");
	scanf("%s", cliente->senha);
	printf("Cadastro realizado com sucesso! \n");
	
	cliente->saldo = 0;
	
	// Define o valor do limite de credito do cliente multiplicando seu salario por 2.
	cliente->limiteCredito = cliente->salario * 2;  
	cliente->limiteInicial = cliente->limiteCredito;
	
      // Gera n�mero de cart�o aleat�rio
    srand(time(NULL));
    for (int i = 0; i < 16; i++) {
        cliente->numeroCartao[i] = rand() % 10; // d�gitos aleat�rios de 0 a 9
    }
    // Gera n�mero do codigo de verifica��o
    for (int i=0; i < 3; i++) {
        cliente->cvc[i] = rand() % 10; // d�gitos aleat�rios de 0 a 9
    }
}

// Fun��o para autenticar o Cliente (login) 
void autenticarCliente(Cliente *cliente){
	char cpfDigitado[15], senhaDigitada[8];
	bool autenticado = false;
	
	while(!autenticado){
		printf("\nAcesse sua Conta Wyden\n");
		printf("CPF: ");
		scanf("%s", cpfDigitado);
		printf("Senha: ");
		scanf("%s", senhaDigitada);
		
		// Verifica se as cred�nciais est�o corretas 
		if(strcmp(cpfDigitado, cliente->cpf)==0 && strcmp(senhaDigitada, cliente->senha)==0){
			autenticado = true; 
			system("cls");
			printf("\nOla %s, sua conta esta pronta para uso!\n", cliente->nome);	
			printf("\nLimite inicial de credito disponivel R$ %.2f\n", cliente->limiteInicial);
		} else{
			printf("CPF ou senha incorretos. Tente novamente. \n");
		}
	}
}

// Fun��o para depositar Saldo
void depositarSaldo(Cliente *cliente){
	float valor; 
	printf("Informe o valor a ser depositado: ");
	scanf("%f", &valor);
	cliente->saldo += valor;
	printf("Deposito de R$ %.2f adicionado ao saldo da conta com sucesso! \n", valor);
}

// Fun��o para fazer PIX 
void fazerPIX(Cliente *cliente){
	float valor;
	char destinatario[40]; 
	
	printf("\nQual o valor da transferencia? ");
	scanf("%f", &valor);
	printf("\nPara quem voce deseja transferir R$ %.2f ? (Informe o CPF ou a Chave PIX) \n", valor);
	scanf("%s", &destinatario);
	
	int escolha;
	printf("\nEscolha como transferir\n");
	printf("\n[1] Pagar com Saldo da Conta \n");
	printf("[2] Pagar com Cartao de Credito \n");
	printf("\nOpcao: ");
	scanf("%d", &escolha);
	
	switch(escolha){
		
	    case 1:
	    	// Verfica se o saldo � suficiente 
	    	if(valor<=cliente->saldo){
	    		cliente->saldo -= valor; 
	    		printf("Transferencia via PIX realizada com sucesso para %s ! \n", destinatario);
			} else{
				printf("Saldo insuficiente para realizar a transferencia! \n");
			}	
	    	break;
	    		
	    case 2:
	    	// Verifica se o cr�dito � suficiente 
	    	if(valor <= cliente->limiteCredito){
	    	
	    	cliente->valorPIX = valor; // Armazena valor da tranfer�ncia 
	    	
	    	printf("\nInforme o numero de parcelas (maximo 12 vezes): ");
	    	scanf("%d", &cliente->parcelasPIX); 
	    	
	    	// Verifica se o n�mero de parcelas � v�lido
	    	if(cliente->parcelasPIX <= 12 && cliente->parcelasPIX > 0){
	    		cliente->limiteCredito -= valor;
	    	    printf("Transferencia via PIX realizada com sucesso para %s ! \n", destinatario);
	    		// Obt�m a data atual da primeira parcela do PIX 
	    		time_t now = time(NULL);
                struct tm *dataAtual = localtime(&now);
                memcpy(&cliente->dataPrimeiraParcelaPIX, dataAtual, sizeof(struct tm));
			} else {
				printf("Numero de parcelas invalido! \n");
				cliente->parcelasPIX = 0; 
			} 
			} else{
				printf("O valor excede o limite disponivel! \n");
	        }
	    	break;
	    default:
	    	printf("Opcao invalida. \n");
       }
   }

// Fun��o para solicitar Empr�stimo do Banco
void solicitarEmprestimo(Cliente *cliente){
	printf("\nO Banco Wyden oferece uma taxa mensal de 5%% para realizar emprestimos. \n");
	printf("\nInforme o valor do Emprestimo desejado: ");
	scanf("%f", &cliente->valorEmprestimo);
	printf("\nEm quantas vezes deseja parcelar? ");
	scanf("%d", &cliente->parcelasEmprestimo);
	
	// Obter a data atual para a primeira parcela do empr�stimo
    time_t now = time(NULL);
    struct tm *dataAtual = localtime(&now);
    memcpy(&cliente->dataPrimeiraParcelaEmprestimo, dataAtual, sizeof(struct tm));

    // Aplicando a f�rmula de Juros Simples com 5% de taxa mensal 
    float taxaMensal = 0.05;
	float juros = cliente->valorEmprestimo * taxaMensal * cliente->parcelasEmprestimo;
    cliente->valorFinalEmprestimo = cliente->valorEmprestimo + juros;

    cliente->saldo += cliente->valorEmprestimo; 
    printf("\nEmprestimo de R$ %.2f realizado com sucesso!\n", cliente->valorEmprestimo);
    printf("\nValor Final R$ %.2f que sera divido em %d vezes.\n", cliente->valorFinalEmprestimo, cliente->parcelasEmprestimo);
}


void gerarArquivoHTML(Cliente *cliente){
	FILE *arquivoHTML;
    arquivoHTML = fopen("conta_cliente.html", "w");

    if (arquivoHTML == NULL) {
        printf("Erro ao abrir o arquivo HTML.\n");
        return;
    }
    fprintf(arquivoHTML, "<!DOCTYPE html>");
    fprintf(arquivoHTML, "<head>");
    fprintf(arquivoHTML, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
    fprintf(arquivoHTML, "<title>Sistema Bancario</title>");
    fprintf(arquivoHTML, "<style>");
    // Estilos CSS para o HTML gerado
    fprintf(arquivoHTML, "        /* Estilos b�sicos para o corpo do documento */");
    fprintf(arquivoHTML, "        body {");
    fprintf(arquivoHTML, "            font-family: 'Montserrat', sans-serif;");
    fprintf(arquivoHTML, "            margin: 0;");
    fprintf(arquivoHTML, "            padding: 0;");
    fprintf(arquivoHTML, "            background-color: ##f5f5f5;");
    fprintf(arquivoHTML, "        }");
    fprintf(arquivoHTML, "        /* Estilos para o conte�do principal */");
    fprintf(arquivoHTML, "        .main {");
    fprintf(arquivoHTML, "            top: 0px;");
    fprintf(arquivoHTML, "            flex-grow: 1;");
    fprintf(arquivoHTML, "            padding: 20px;");
    fprintf(arquivoHTML, "            position: relative;");
    fprintf(arquivoHTML, "        }");
    fprintf(arquivoHTML, "        /* Estilos para o container principal */");
    fprintf(arquivoHTML, "        .container {");
    fprintf(arquivoHTML, "            display: flex;");
    fprintf(arquivoHTML, "            height: 130vh;");
    fprintf(arquivoHTML, "        }");
    fprintf(arquivoHTML, "        /* Estilos para a barra lateral */");
    fprintf(arquivoHTML, "        .barra-lateral {");
    fprintf(arquivoHTML, "            background-color: #242726;");
    fprintf(arquivoHTML, "            width: 220px;");
    fprintf(arquivoHTML, "            padding: 20px;");
    fprintf(arquivoHTML, "            position: relative;");
    fprintf(arquivoHTML, "        }\n");
    fprintf(arquivoHTML, "        .img-icone {");
    fprintf(arquivoHTML, "            width: 40px;");
    fprintf(arquivoHTML, "            height: 30px;");
    fprintf(arquivoHTML, "            background-color: #fff;");
    fprintf(arquivoHTML, "            border-radius: 50%;");
    fprintf(arquivoHTML, "            margin-right: 10px;");
    fprintf(arquivoHTML, "        }");
    fprintf(arquivoHTML, "        .img-icone img {");
    fprintf(arquivoHTML, "            width: 40px;");
    fprintf(arquivoHTML, "            height: 30px;");
    fprintf(arquivoHTML, "            object-fit: cover;");
    fprintf(arquivoHTML, "        }");
    fprintf(arquivoHTML, "        /* Estilos para a se��o de informa��es do usu�rio */");
    fprintf(arquivoHTML, "        .info-cliente {");
    fprintf(arquivoHTML, "            background-color: #fff;");
    fprintf(arquivoHTML, "            color: #000;");
    fprintf(arquivoHTML, "            padding: 15px;");
    fprintf(arquivoHTML, "            margin-bottom: 20px;");
    fprintf(arquivoHTML, "            border-radius: 8px;");
    fprintf(arquivoHTML, "            display: flex;");
    fprintf(arquivoHTML, "            align-items: center;");
    fprintf(arquivoHTML, "        }");
    fprintf(arquivoHTML, "        /* Estilos para a se��o de informa��es do cart�o */");
    fprintf(arquivoHTML, "        .info-cartao {");
    fprintf(arquivoHTML, "            display: flex;");
    fprintf(arquivoHTML, "            align-items: center;");
    fprintf(arquivoHTML, "            margin-bottom: 20px;");
    fprintf(arquivoHTML, "        }");
    fprintf(arquivoHTML, "        /* Estilos para a imagem do cart�o */");
    fprintf(arquivoHTML, "        .info-cartao .img-cartao {");
    fprintf(arquivoHTML, "            width: 200px; ");
    fprintf(arquivoHTML, "            height: 120px; ");
    fprintf(arquivoHTML, "            margin-right: 20px;");
    fprintf(arquivoHTML, "        }");
    fprintf(arquivoHTML, "        /* Estilos para a imagem do cart�o */");
    fprintf(arquivoHTML, "        .info-cartao .img-cartao img {");
    fprintf(arquivoHTML, "            width: 200px;");
    fprintf(arquivoHTML, "            height: 120px;");
    fprintf(arquivoHTML, "            object-fit: cover;");
    fprintf(arquivoHTML, "        }");
    fprintf(arquivoHTML, "        /* Estilos para os detalhes do cart�o */");
    fprintf(arquivoHTML, "        .info-cartao .detalhes-cartao {");
    fprintf(arquivoHTML, "            display: flex;");
    fprintf(arquivoHTML, "            flex-direction: column;");
    fprintf(arquivoHTML, "        }");
    fprintf(arquivoHTML, "        /* Estilos para cada detalhe do cart�o */");
    fprintf(arquivoHTML, "        .info-cartao .detalhes-cartao .detail {");
    fprintf(arquivoHTML, "            margin-bottom: 5px;");
    fprintf(arquivoHTML, "            font-family: sans-serif;");
    fprintf(arquivoHTML, "        }");
    fprintf(arquivoHTML, "        /* Estilos para a se��o do cart�o de cr�dito */");
    fprintf(arquivoHTML, "        .cartao-credito {");
    fprintf(arquivoHTML, "            display: flex;");
    fprintf(arquivoHTML, "            align-items: center;");
    fprintf(arquivoHTML, "            justify-content: space-between;");
    fprintf(arquivoHTML, "            margin-bottom: 20px;");
    fprintf(arquivoHTML, "        }");
    fprintf(arquivoHTML, "        /* Estilos para as informa��es do cart�o de cr�dito */");
    fprintf(arquivoHTML, "        .cartao-credito .info {");
    fprintf(arquivoHTML, "            display: flex;");
    fprintf(arquivoHTML, "            flex-direction: column;");
    fprintf(arquivoHTML, "        }");
    fprintf(arquivoHTML, "        /* Estilos para o status \"Limite dispon�vel\" */");
    fprintf(arquivoHTML, "        .cartao-credito .info .bill .status.available {");
    fprintf(arquivoHTML, "            background-color: #4CAF50;");
    fprintf(arquivoHTML, "        }");
    fprintf(arquivoHTML, "        /* Estilos para o gr�fico do cart�o de cr�dito */");
    fprintf(arquivoHTML, "        .cartao-credito .grafico {");
    fprintf(arquivoHTML, "            position: absolute;");
    fprintf(arquivoHTML, "            top: 110px;");
    fprintf(arquivoHTML, "            right: 450px;");
    fprintf(arquivoHTML, "            width: 150px;");
    fprintf(arquivoHTML, "            height: 150px;");
    fprintf(arquivoHTML, "        }");
    fprintf(arquivoHTML, "        /* Estilos para a legenda do gr�fico */");
    fprintf(arquivoHTML, "        .cartao-credito .grafico .legend {");
    fprintf(arquivoHTML, "            display: flex;");
    fprintf(arquivoHTML, "            flex-direction: column;");
    fprintf(arquivoHTML, "            align-items: center;");
    fprintf(arquivoHTML, "            margin-top: 10px;");
    fprintf(arquivoHTML, "        }");
    fprintf(arquivoHTML, "        /* Estilos para cada item da legenda */");
    fprintf(arquivoHTML, "        .cartao-credito .grafico .legend .item {");
    fprintf(arquivoHTML, "            display: flex;");
    fprintf(arquivoHTML, "            align-items: center;");
    fprintf(arquivoHTML, "            margin-bottom: 4px;");
    fprintf(arquivoHTML, "        }");
    fprintf(arquivoHTML, "        /* Estilos para a cor do item da legenda */");
    fprintf(arquivoHTML, "        .cartao-credito .grafico .legend .item .color {");
    fprintf(arquivoHTML, "            display: inline-block;");
    fprintf(arquivoHTML, "            width: 10px;");
    fprintf(arquivoHTML, "            height: 10px;");
    fprintf(arquivoHTML, "            border-radius: 50%;");
    fprintf(arquivoHTML, "            margin-right: 5px;");
    fprintf(arquivoHTML, "        }");
    fprintf(arquivoHTML, "        /* Estilos para a cor \"Limite total\" */");
    fprintf(arquivoHTML, "        /* Estilos para o texto do item da legenda */");
    fprintf(arquivoHTML, "        .cartao-credito .grafico .legend .item .text {");
    fprintf(arquivoHTML, "            font-size: 12px;");
    fprintf(arquivoHTML, "            color: #666;");
    fprintf(arquivoHTML, "        }");
    fprintf(arquivoHTML, "		  table {");
    fprintf(arquivoHTML, " 		  border-collapse: collapse;");
    fprintf(arquivoHTML, "		  }");
    fprintf(arquivoHTML, "		  th, td {");
    fprintf(arquivoHTML, "  	  border-bottom: 1px solid #ddd;");
    fprintf(arquivoHTML, "  	  text-align: left;");
    fprintf(arquivoHTML, "  	  padding: 8px;");
    fprintf(arquivoHTML, " 		  padding-left: 50px;"); 
    fprintf(arquivoHTML, "  	  padding-right: 50px;"); 
    fprintf(arquivoHTML, "		  }");
    fprintf(arquivoHTML, "		  th {");
    fprintf(arquivoHTML, "  	  background-color: #f2f2f2;");
    fprintf(arquivoHTML, "		  }");
    fprintf(arquivoHTML, "		  td {");
    fprintf(arquivoHTML, "  	  padding-left: 50px;"); 
    fprintf(arquivoHTML, "        padding-right: 50px;");
    fprintf(arquivoHTML, "		  }");
    fprintf(arquivoHTML, "		  strong {");
    fprintf(arquivoHTML, "  	   font-size: 18px;");
    fprintf(arquivoHTML, "		  }");
    fprintf(arquivoHTML, "        .info-saldo {\n");
	fprintf(arquivoHTML, "            border: 0px;\n"); 
	fprintf(arquivoHTML, "            padding: 10px;\n");
	fprintf(arquivoHTML, "            box-shadow: 3px 3px 5px rgba(0, 0, 0, 0.02);\n"); 
	fprintf(arquivoHTML, "            display: inline-block;\n"); 
	fprintf(arquivoHTML, "            background-color: #f2f2f2;\n");  
	fprintf(arquivoHTML, "            border-radius: 10px;\n");  
	fprintf(arquivoHTML, "        }\n");
    fprintf(arquivoHTML, "    </style>");
    fprintf(arquivoHTML, "</head>");
    fprintf(arquivoHTML, "<body>");
    fprintf(arquivoHTML, "    <div class=\"container\">");
    fprintf(arquivoHTML, "        <!-- Barra lateral -->");
    fprintf(arquivoHTML, "        <div class=\"barra-lateral\">");
    fprintf(arquivoHTML, "            <!-- Se��o de informa��es do Cliente -->");
    fprintf(arquivoHTML, "            <div class=\"info-cliente\">");
    fprintf(arquivoHTML, "                <!-- �cone do usu�rio -->");
    fprintf(arquivoHTML, "                <div class=\"img-icone\">");
    fprintf(arquivoHTML, "                    <img src=\"icone.ico\" alt=\"icone\">");
    fprintf(arquivoHTML, "                </div>");
    fprintf(arquivoHTML, "                <!-- Nome do Cliente -->");
    fprintf(arquivoHTML, "                <span>Ol�, %s</span>", cliente->nome);
    fprintf(arquivoHTML, "            </div>");
    fprintf(arquivoHTML, "        </div>\n");
    fprintf(arquivoHTML, "        <!-- Conte�do principal -->");
    fprintf(arquivoHTML, "        <div class=\"main\">");
    fprintf(arquivoHTML, "            <!-- Adicionando o Saldo Dispon�vel-->");
    fprintf(arquivoHTML, "           <p class=\"info-saldo\">  <strong> Conta <br>  </strong> Saldo Dispon�vel: R$ %.2f </p>", cliente->saldo);
    fprintf(arquivoHTML, "            <!-- Se��o do cart�o de cr�dito -->");
    fprintf(arquivoHTML, "            <div class=\"cartao-credito\">");
    fprintf(arquivoHTML, "                <!-- Informa��es do cart�o de cr�dito -->");
    fprintf(arquivoHTML, "                <div class=\"info\">");
    fprintf(arquivoHTML, "                    <!-- Se��o de informa��es do cart�o -->");
    fprintf(arquivoHTML, "                    <div class=\"info-cartao\">");
    fprintf(arquivoHTML, "                        <!-- Imagem do cart�o -->");
    fprintf(arquivoHTML, "                        <div class=\"img-cartao\" >");
    fprintf(arquivoHTML, "                            <img src=\"cartao.png\" alt=\"Cartao de Credito\">");
    fprintf(arquivoHTML, "                        </div>");
    fprintf(arquivoHTML, "                        <!-- Detalhes do cart�o -->");
    fprintf(arquivoHTML, "                        <div class=\"detalhes-cartao\">");
    fprintf(arquivoHTML, "                            <h3>Cart�o de Cr�dito</h3>");
    fprintf(arquivoHTML, "                            <div class=\"detail\">Nome: &nbsp <strong> %s </strong></div>", cliente->nome);
    fprintf(arquivoHTML, "                            <div class=\"detail\">N�mero: &nbsp");
    
    for (int i = 1; i <= 16; i++) {  
    	fprintf(arquivoHTML, "<strong>%d</strong>", cliente->numeroCartao[i]);
    	if(i==4 || i==8 || i == 12){
    		 fprintf(arquivoHTML, "  ");
		}   
    }
    fprintf(arquivoHTML, "</div>");
    fprintf(arquivoHTML, "                            <div class=\"detail\">CVC: &nbsp");
    for (int i = 0; i < 3; i++) {
        fprintf(arquivoHTML, "<strong>%d</strong>", cliente->cvc[i]);
    }
    fprintf(arquivoHTML, "</div>");
    fprintf(arquivoHTML, "                        </div>");
    fprintf(arquivoHTML, "                    </div>");
    fprintf(arquivoHTML, "                    <!-- Lista de faturas -->");
    fprintf(arquivoHTML, "<h3>Hist�rico de Faturas</h3>");
    fprintf(arquivoHTML, "<table >"); 
    fprintf(arquivoHTML, "<tr><th>Fatura</th><th>Vencimento</th><th>Descri��o</th><th>Valor</th></tr>");
    
	// Calcula o valor de cada parcela do PIX (com juros)
	float valorParcelaEmprestimo =  cliente->valorFinalEmprestimo / cliente->parcelasEmprestimo;
    float valorParcelaPIX = (cliente->valorPIX / cliente->parcelasPIX) * 1.03; 
    
    int parcelaEmprestimoAtual = 1; // Contador de parcelas do empr�stimo
    int parcelaPIXAtual = 1;       // Contador de parcelas do PIX
    
    // Copia a data da primeira parcela para manipular
    struct tm dataParcelaEmprestimo = cliente->dataPrimeiraParcelaEmprestimo;
    struct tm dataParcelaPIX = cliente->dataPrimeiraParcelaPIX;

    // Obter a data atual do sistema
    time_t now = time(NULL);
    struct tm *dataAtual = localtime(&now);

    while (parcelaEmprestimoAtual <= cliente->parcelasEmprestimo || parcelaPIXAtual <= cliente->parcelasPIX) {
        char mes[10];
        strftime(mes, sizeof(mes), "%B", &dataParcelaEmprestimo); // Formata o m�s
        int ano = dataParcelaEmprestimo.tm_year + 1900; // Calcula o ano

        float valorTotalParcela = 0; // Valor total da parcela (empr�stimo + PIX)
        char descricao[50] = ""; // Descri��o da parcela
        char faturaNome[20] = ""; // Nome da fatura

        if (parcelaEmprestimoAtual <= cliente->parcelasEmprestimo) {
            valorTotalParcela += valorParcelaEmprestimo;
            strcat(descricao, "Empr�stimo "); // Adiciona "Empr�stimo" � descri��o
            parcelaEmprestimoAtual++;
        }

        if (parcelaPIXAtual <= cliente->parcelasPIX) {
            valorTotalParcela += valorParcelaPIX;
            if (strlen(descricao) > 0) {
                strcat(descricao, " e PIX"); // Adiciona "e PIX" � descri��o se j� houver "Empr�stimo"
            } else {
                strcat(descricao, "PIX"); // Adiciona "PIX" � descri��o
            }
            parcelaPIXAtual++;
        }

        if (valorTotalParcela > 0) {
            // Verificar se a data da parcela atual � igual ao m�s atual do sistema
            if (dataParcelaEmprestimo.tm_mon == dataAtual->tm_mon &&
                dataParcelaEmprestimo.tm_year == dataAtual->tm_year) {
                strcpy(faturaNome, "Fatura Atual"); // Se a data � atual, define como "Fatura Atual"
            } else {
                strcpy(faturaNome, "Fatura Parcial"); // Se a data n�o � atual, define como "Fatura Parcial"
            }

            fprintf(arquivoHTML, "<tr><td>%s</td><td>%s de %d</td><td>%s</td><td> R$ %.2f</td></tr>", faturaNome, mes, ano, descricao, valorTotalParcela);
        }

        // Avan�a para o pr�ximo m�s para o empr�stimo
        if (parcelaEmprestimoAtual <= cliente->parcelasEmprestimo) {
            dataParcelaEmprestimo.tm_mon++;
            if (dataParcelaEmprestimo.tm_mon > 11) {
                dataParcelaEmprestimo.tm_mon = 0;
                dataParcelaEmprestimo.tm_year++;
            }
        }

        // Avan�a para o pr�ximo m�s para o PIX
        if (parcelaPIXAtual <= cliente->parcelasPIX) {
            dataParcelaPIX.tm_mon++;
            if (dataParcelaPIX.tm_mon > 11) {
                dataParcelaPIX.tm_mon = 0;
                dataParcelaPIX.tm_year++;
            }
        }
    }
    fprintf(arquivoHTML, "</table>");
    fprintf(arquivoHTML, "                <!-- Gr�fico do cart�o de cr�dito -->");
    fprintf(arquivoHTML, "                <div class=\"grafico\">");
    fprintf(arquivoHTML, "                    <!-- Tela do gr�fico -->");
    fprintf(arquivoHTML, "                    <canvas id=\"creditCardChart\"></canvas>");
    fprintf(arquivoHTML, "                    <!-- Legenda do gr�fico -->");
    fprintf(arquivoHTML, "                    <div class=\"legend\">");
    fprintf(arquivoHTML, "                        <!-- Item da legenda \"Limite total\" -->");
    fprintf(arquivoHTML, "                        <div class=\"item\">");
    fprintf(arquivoHTML, "                            <!-- Cor do item da legenda -->");
    fprintf(arquivoHTML, "                            <div class=\"color total\"></div>");
    fprintf(arquivoHTML, "                            <!-- Texto do item da legenda -->");
    fprintf(arquivoHTML, "                            <div class=\"text\"> <h4> Limite total: R$ %.2f </h4> </div>", cliente->limiteInicial);
    fprintf(arquivoHTML, "                        </div>");
    fprintf(arquivoHTML, "                    </div>");
    fprintf(arquivoHTML, "                </div>");
    fprintf(arquivoHTML, "            </div>");
    fprintf(arquivoHTML, "        </div>");
    fprintf(arquivoHTML, "    </div>");
    fprintf(arquivoHTML, "    <!-- Script do Grafico.js -->");
    fprintf(arquivoHTML, "    <script src=\"https://cdn.jsdelivr.net/npm/chart.js\"></script>");
    fprintf(arquivoHTML, "    <!-- Script para o gr�fico do cart�o de cr�dito -->");
    fprintf(arquivoHTML, "    <script>");
    fprintf(arquivoHTML, "        // Obt�m o contexto do gr�fico\n");
    fprintf(arquivoHTML, "        const creditCardChart = document.getElementById('creditCardChart').getContext('2d');\n");
    fprintf(arquivoHTML, "        // Cria um gr�fico de pizza (doughnut)\n");
    fprintf(arquivoHTML, "        new Chart(creditCardChart, {\n");
    fprintf(arquivoHTML, "            // Tipo de gr�fico\n");
    fprintf(arquivoHTML, "            type: 'doughnut',\n");
    fprintf(arquivoHTML, "            // Dados do gr�fico\n");
    fprintf(arquivoHTML, "            data: {\n");
    fprintf(arquivoHTML, "                // Labels para cada fatia do gr�fico\n");
    fprintf(arquivoHTML, "                labels: ['Limite Utilizado', 'Limite Disponivel'],\n");
    fprintf(arquivoHTML, "                // Conjuntos de dados do gr�fico\n");
    fprintf(arquivoHTML, "                datasets: [{\n");
    fprintf(arquivoHTML, "                    // Dados para cada fatia do gr�fico\n");
    fprintf(arquivoHTML, "                    data: [%.2f, %.2f],\n",  cliente->limiteInicial - cliente->limiteCredito, cliente->limiteCredito);
    fprintf(arquivoHTML, "                    // Cores das fatias do gr�fico\n");
    fprintf(arquivoHTML, "                    backgroundColor: ['#FF3131', '#7ED957'],\n");
    fprintf(arquivoHTML, "                    // Largura da borda das fatias\n");
    fprintf(arquivoHTML, "                    borderWidth: 0.03\n");
    fprintf(arquivoHTML, "                }]\n");
    fprintf(arquivoHTML, "            },\n");
    fprintf(arquivoHTML, "            // Op��es de configura��o do gr�fico\n");
	fprintf(arquivoHTML, "            options: {\n");
	fprintf(arquivoHTML, "                cutout: '40%'\n");
	fprintf(arquivoHTML, "            }\n");
    fprintf(arquivoHTML, "        });\n");
    fprintf(arquivoHTML, "    </script>\n");
    fprintf(arquivoHTML, "</body>");
    fprintf(arquivoHTML, "</html>");
    fclose(arquivoHTML);
    printf("Arquivo HTML gerado com sucesso em conta_cliente.html!\n");
}


int main(){

	// Declara uma variavel cliente do tipo Cliente
	Cliente cliente; 
	cliente.valorEmprestimo = 0;
	cliente.parcelasEmprestimo = 0;
	cliente.valorPIX = 0;
	cliente.valorEmprestimo = 0; 
	
	// Chama a fun��o para cadastrar o cliente 
	cadastrarCliente(&cliente);
	// Chama a fun��o para autentica��o do cliente
	autenticarCliente(&cliente);

	int op=0;
	
	do{
		printf("\nConta\n");
		printf("\n[1] Depositar Saldo \n");
		printf("[2] Fazer transferencia PIX \n");
		printf("[3] Solicitar um Emprestimo \n");
		printf("[4] Gerar Interface HTML \n");
		printf("[5] Sair \n");
	    printf("\nEscolha uma opcao: ");
	    scanf("%d", &op);
	    system("cls");
	    
	    switch(op){
	    	
	    	case 1:
	    		depositarSaldo(&cliente);
	    		break;
	    		
	    	case 2:
	    		fazerPIX(&cliente);
	    		break;
	    	
	    	case 3:
	    		solicitarEmprestimo(&cliente);
	    		break;
	    		
	    	case 4:
	    		gerarArquivoHTML(&cliente);
	    		break;
	    		
	    	case 5:
	    	    printf("Saindo do Sistema... \n");
	    		break;
	    		
	    	default:
	    		printf("Op��o inv�lida. Tente novamente. \n");
	    		return 0; 
		}
	}while(op != 5 );
	
	return 0; 
}
