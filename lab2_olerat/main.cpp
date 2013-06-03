#include <iostream>
#include <fstream>
#include <Stack>
#include <string>
#include <stdlib.h>
using namespace std;

struct orderLexema{
	string lex;
	int order;
};

stack <orderLexema> st;
orderLexema orderLex[21];
bool flagVar = false, flagIF = false, flagIFBegin = false, flagGoto = false, flagArray = false, flagPName = false;
int countVar = 0, countIF = 0, sizeArray = 2;
fstream opz;

void popStackOnOrder(orderLexema curOrderLex){	
	while (!st.empty() && st.top().order >= curOrderLex.order){
		opz << st.top().lex << " ";
		cout << st.top().lex << " ";
		st.pop();
	}
}

void popStackBeforeLexema(string lexema)
{
	while (!st.empty() && st.top().lex != lexema){
		opz << st.top().lex << " ";
		cout << st.top().lex << " ";
		st.pop();
	}	
}

void popStackLexema(string lexema)
{
	if (!st.empty() && st.top().lex == lexema)
		st.pop();	
}

void workWithW(string lexema){
	
			if (lexema == "W1"){//Program
				flagPName = true;
			}else if (lexema == "W2"){//Begin
				if (flagVar)
					flagVar = false;			
				else if (flagIF)						
					flagIFBegin = true;
			}else if (lexema == "W3"){//Var
				countVar = 0;
				flagVar = true;
			}else if (lexema == "W4"){//End			
				if (flagIFBegin)
					flagIFBegin = false;						
			}else if (lexema == "W5"){//If
				st.push(orderLex[11]);
				flagIF = true;
				countIF++;
			}else if (lexema == "W6"){//then			
				popStackBeforeLexema("W5");											
				opz << "M"<< countIF << " UPL\n";
				cout << "M"<< countIF << " UPL\n";
			}else if (lexema == "W7"){//else			
 				popStackBeforeLexema("W5");
				countIF++;
				opz << "M"<< countIF << " BP\n";	
				cout << "M"<< countIF << " BP\n";	
			}else if (lexema == "W8"){//Integer				
				opz << countVar << "int ";
				cout << countVar << "int ";
				countVar=0;
			}else if (lexema == "W9"){//Real				
				opz << countVar << "real ";
				cout << countVar << "real ";
				countVar = 0;				
			}else if (lexema == "W10"){//Array					
			}else if (lexema == "W11"){;//of				
			}else if(lexema == "W12"){//readln
				opz << " W12 ";
				cout << " W12 ";			
			}else if(lexema == "W13"){//writeln
				opz << " W13 ";
				cout << " W13 ";
			}else if(lexema == "W14"){//goto
				orderLexema curOrderLex = orderLex[12];								
				popStackOnOrder(curOrderLex);
				st.push(curOrderLex);
				flagGoto = true;
			}
}


void workWithR(string lexema){	
	switch (lexema[1]){
		case '2':
			if (flagArray)
				sizeArray++;
			break;
		case '3'://;
		//встретили ; в конце условия, при этом не стоит операторных скобок
			popStackBeforeLexema("W5");	
			if (flagIF && !flagIFBegin){
				popStackBeforeLexema("W5");											
				popStackLexema("W5");
				opz << "M" << countIF << ":";
				cout << "M" << countIF << ":";
				flagIF = false;
			}			
			else if (flagGoto){
				flagGoto = false;
				opz << " BP ";
				cout << " BP ";
			}
			break;
		case '4'://(
			st.push(orderLex[13]);
			break;
		case '5'://(
			popStackBeforeLexema("R4");
			popStackLexema("R4");
			break;
		case '6'://[
			flagArray = true;
			st.push(orderLex[14]);
			break;
		case '7'://]	
			if (flagArray){
				flagArray = false;
				popStackBeforeLexema("R6");
				popStackLexema("R6");
				opz << sizeArray << "AEM ";	
				cout << sizeArray << "AEM ";
				sizeArray = 2;
			}
			break;
	}
}

void workWithI(string lexema){	
	//сразу выводим идентификатор
	if (flagPName){
		flagPName = false;
		return;
	}
	opz << lexema<< " ";
	cout << lexema<< " ";
	
	if (flagVar)
		countVar++;	
}

void workWithN(string lexema){	
	//срузу заносим в файл
	opz << lexema<< " ";
	cout << lexema<< " ";
}

void workWithC(string lexema){
	//срузу заносим в файл	
	opz << lexema << " ";
	cout << lexema << " ";
}

void workWithO(string lexema){	
	orderLexema curOrderLex;
	bool flagLexema = false;
	for (int i = 0; i < sizeof(orderLex)/sizeof(orderLex[0]); i++)
		if (lexema == orderLex[i].lex){
			curOrderLex = orderLex[i];
			flagLexema = true;
			break;
		}
		if (!flagLexema)
			return;
		popStackOnOrder(curOrderLex);
		st.push(curOrderLex);
}

void initOrderLexema(){
	orderLex[0].lex  = "O1"; orderLex[0].order = 2;//+	
	orderLex[1].lex  = "O2"; orderLex[1].order = 2;//-	
	orderLex[2].lex  = "O3"; orderLex[2].order = 3;//*	
	orderLex[3].lex  = "O4"; orderLex[3].order = 3;///	
	orderLex[4].lex  = "O6"; orderLex[4].order = 1;//:=	
	orderLex[5].lex  = "O8"; orderLex[5].order = 4;//>	
	orderLex[6].lex  = "O9"; orderLex[6].order = 4;//<	
	orderLex[7].lex  = "O10"; orderLex[7].order = 4;//=	
	orderLex[8].lex  = "O11"; orderLex[8].order = 4;//<>	
	orderLex[9].lex  = "O12"; orderLex[9].order = 4;//<=	
	orderLex[10].lex = "O13"; orderLex[10].order = 4;//>=	
	orderLex[11].lex = "W5"; orderLex[11].order = 0;//IF	
	orderLex[12].lex = "W14"; orderLex[12].order = 1;//goto
	orderLex[13].lex = "R4"; orderLex[13].order = 0;//(	
	orderLex[14].lex = "R6"; orderLex[14].order = 0;//[	
}

int main()
{
	fstream inp("d:\output.txt",ios_base::in);//файл с лексемами
	opz.open("d:\opz.txt",ios_base::out|ios_base::trunc);//файл с ОПЗ
	
	char c;
	string lexema;	
	initOrderLexema();

	while(!(inp.eof()))
	{
		lexema = "";
		c = inp.get();
		if (c == '\n')
		{
			opz<<c;
			cout << c;
		}
		while (c == ' ')
			c = inp.get();
		if (c == 'W' ||  c == 'R' || c == 'O' || c == 'N' || c == 'I' || c == 'C'){		
				lexema += c;
				c = inp.get();
				if (c >= '0' && c <= '9')
					lexema += c;
				else
					lexema = "";
				c = inp.get();
				if (c >= '0' && c <= '9')
					lexema += c;

		}
		if (lexema != ""){		
			switch (lexema[0]) {
				case 'W' : workWithW(lexema); break;
				case 'R' : workWithR(lexema); break;
				case 'I' : workWithI(lexema); break;
				case 'N' : workWithN(lexema); break;		
				case 'C' : workWithC(lexema); break;
				case 'O' : workWithO(lexema); break;
			}	
		}

	}//end while

	while (!st.empty())
	{
		opz << st.top().lex << " ";
		cout << st.top().lex << " ";
		st.pop();
	}

	inp.close();
	opz.close();
}
	
