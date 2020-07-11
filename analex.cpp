#include <bits/stdc++.h>

using namespace std;

char prox_char = ' ';
string simbolo;
int linha;
ifstream arquivo;
bool pula_linha = false, terminar = false;
FILE *out = fopen("tokens.txt","w");

//conjunto de caracteres especiais
set<char> spec_char = {'.',';',',','(',')',':','=','<','>','+','-','*'};

//conjunto de palavras reservadas
set<string> reservadas = {"PROGRAM","LABEL","VAR","PROCEDURE","FUNCTION","BEGIN","END",
"IF","THEN","ELSE","WHILE","DO","OR","AND","DIV","NOT"};

//dicionario de simbolos
map<string,string> dic_simb = {{".","C_PONTO"},{";","C_PONTO_VIRGULA"},{",","C_VIRGULA"},
{"(","C_ABRE_PAR"},{")","C_FECHA_PAR"},{":","C_DOIS_PONTOS"},{"=","C_IGUAL"},
{"<","C_MENOR"},{">","C_MAIOR"},{"+","C_SOMA"},{"-","C_SUB"},{"*","C_MULT"},{":=","C_ATRIB"}};

void proximo(){
    char c;
    arquivo.get(c);
    //quebra de linha
    if(c == '\n'){
        linha++;
       pula_linha = true;
    }

    //acabou o arquivo
    if(arquivo.eof()){   
        terminar = true;
    }
    prox_char = toupper(c);
}

string codigo(string in, int is_simb){
    if(is_simb){
        return dic_simb[in];
    }
    return "C_" + in;
}

void erro(int flag){
    printf("\n\nErro na linha %d: ",linha);
    switch(flag){
        case 1:
            printf("Identificador ou numero mal formatado\n");
        break;
        case 2:
            printf("Simbolo especial desconhecido\n");
        break;
    }
    arquivo.close();
    fclose(out);
    exit(0);
}

void analisador_lexico(){
    //se for um espaço vazio, ignorar
    string atomo;
    while((prox_char == ' ' || prox_char == '\t' || prox_char == '\n') && !terminar){
        if(pula_linha)
            fprintf(out,"\n%d ",linha);
            pula_linha = false;
        proximo();
    }
    
    if(terminar)
        return;

    //simbolos especiais
    if(spec_char.find(prox_char) != spec_char.end()){   //verifica se prox_char está no conjunto spec_char
        string s;
        s.push_back(prox_char);     //s = prox_char
        proximo();
        if(s.compare(":") == 0){    //se s == ':'
            if(prox_char == '='){
                s.push_back('=');   //s = ':='
                proximo();
            }
        }
        simbolo = codigo(s,1);
        
    //palavra reservada ou identificador
    }else if(isalpha(prox_char)){       //se prox_char é uma letra
        do{
            atomo.push_back(prox_char);
            proximo();
        }while(isalnum(prox_char) && !terminar);     //enquanto prox_char é uma letra ou um numero
        if(reservadas.find(atomo) != reservadas.end()){  //se atomo é uma palavra reservada
            simbolo = codigo(atomo,0);
        }else
            simbolo = "ID_"+atomo;            //identificador

    //numero
    }else if(isdigit(prox_char)){       //se prox_char é um digito
        do{
            atomo.push_back(prox_char);
            proximo();
        }while(isdigit(prox_char) && !terminar);
        if(isalpha(prox_char)){         //se prox_char é uma letra
            erro(1);
        }
        simbolo = "NUM_"+atomo;                //numero
    }else{
        if(!terminar)                   //se não for um caracter de fim de arquivo - erro
            erro(2);
    }
    fprintf(out,"%s ",simbolo.c_str());
    if(pula_linha){
        fprintf(out,"\n%d ",linha);
        pula_linha = false;
    }
}

int main(){
    linha = 1;
    char str[256];
    cout << "Nome do arquivo de entrada: " << flush;
    cin.get(str,256);
    arquivo.open(str);
    //primeira linha
    fprintf(out,"%d ",linha);          
    //enquanto o arquivo não chegar ao fim
    while(!terminar){
        analisador_lexico();
    }
    arquivo.close();
    fclose(out);
}