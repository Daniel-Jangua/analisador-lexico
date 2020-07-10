#include <bits/stdc++.h>

using namespace std;

char prox_char;
string simbolo;
int linha;
ifstream arquivo;
bool pula_linha = false;

//conjunto de caracteres especiais
set<char> spec_char = {'.',';',',','(',')',':','=','<','>','+','-','*'};

//conjunto de palavras reservadas
set<string> reservadas = {"PROGRAM","LABEL","VAR","PROCEDURE","FUNCTION","BEGIN","END",
"IF","THEN","ELSE","WHILE","DO","OR","AND","DIV","NOT"};

//dicionario de simbolos
map<string,string> dic_simb = {{".","C_PONTO"},{";","C_PONTO_VIRGULA"},{",","C_VIRGULA"},
{"(","C_ABRE_PAR"},{")","C_FECHA_PAR"},{":","C_DOIS_PONTOS"},{"=","C_IGUAL"},
{"<","C_MENOR"},{">","C_MAIOR"},{"+","C_SOMA"},{"-","C_SUB"},{"*","C_MULT"},{":=","C_ATRIB"}};

int proximo(){
    char c;
    arquivo.get(c);
    //quebra de linha
    if(c == '\n'/* && arquivo.get(c)*/){
        linha++;
       pula_linha = true;
    }

    //acabou o arquivo
    if(arquivo.eof()){   
        arquivo.close();      
        exit(0);
        return 0;
    }
    prox_char = toupper(c);
    return 1;
}

string codigo(string in, int is_simb){
    if(is_simb){
        return dic_simb[in];
    }
    return "C_" + in;
}

void erro(int flag){
    //cout << "Erro na linha " << linha << ": " << flush;
    printf("\n\nErro na linha %d: ",linha);
    switch(flag){
        case 1:
            //cout << "Identificador ou numero mal formatado\n" << flush;
            printf("Identificador ou numero mal formatado\n");
        break;
        case 2:
            //cout << "Simbolo especial desconhecido\n" << flush;
            printf("Simbolo especial desconhecido\n");
        break;
    }
    arquivo.close();
    exit(0);
}

int main(){
    linha = 1;
    char str[256];
    cout << "Nome do arquivo de entrada: " << flush;
    cin.get(str,256);
    arquivo.open(str);
    //cout << linha + " ";
    printf("%d ",linha);
    proximo();
    //enquanto o arquivo não chegar ao fim
    while(true){
        //se for um espaço vazio, ignorar
        string atomo;
        while(prox_char == ' ' || prox_char == '\t' || prox_char == '\n'){
            if(pula_linha)
                printf("\n%d ",linha);
                pula_linha = false;
            proximo();
        }
        
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
            //cout << " " + simbolo + " ";
        //palavra reservada ou identificador
        }else if(isalpha(prox_char)){       //se prox_char é uma letra
            do{
                atomo.push_back(prox_char);
                proximo();
            }while(isalnum(prox_char));     //enquanto prox_char é uma letra ou um numero
            //arquivo.unget();                //volta o ponteiro
            if(reservadas.find(atomo) != reservadas.end())  //se atomo é uma palavra reservada
                simbolo = codigo(atomo,0);
            else
                simbolo = atomo;            //identificador
        //numero
        }else if(isdigit(prox_char)){       //se prox_char é um digito
            do{
                atomo.push_back(prox_char);
                proximo();
            }while(isdigit(prox_char));
            if(isalpha(prox_char)){         //se prox_char é uma letra
                erro(1);
            }
            //arquivo.unget();                //volta o ponteiro
            simbolo = atomo;                //numero
        }else{
            erro(2);
        }
        //cout << simbolo + " " << flush;
        printf("%s ",simbolo.c_str());
        if(pula_linha){
            printf("\n%d ",linha);
            pula_linha = false;
        }
    }
    arquivo.close();
}