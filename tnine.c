#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

//syntax spustenia -> ./tnine CISLO <seznam.txt
//CISLO je retazec cisiel -->> pre zjednodusenie budem referovat na tento string ako target[]
//seznam.txt je zoznam kontaktov kde input string bude v main programe -->>  buffer


//funkce na najdenie input sekvence cisiel v stringu zo zoznamu
bool find_number(char* input_number, char* target, bool s_parameter){
    bool result = false;
    int input_number_size = strlen(input_number), target_size = strlen(target), pointer = 0;

    //cyklus kontroluje kazdy znak v input stringu
    for(int idx = 0; idx < input_number_size; idx++){
        //ak sa znak nerovna target[0] ide dal
        if(input_number[idx] != target[pointer]) continue; 
        else{
            //algoritmus bez -s parametru
            if(s_parameter == false){
                //ak sa znak rovna kontroluje dalsie znaky s target[1.2.3...atd]
                for(int jdx = 0; jdx < target_size; jdx++){     
                    if(input_number[idx+jdx] != target[pointer]) break;
                    else pointer++;
                }
                //ak sa pocet zhod rovna s dlzkou target[] stringu tak sa True
                if(pointer == target_size){
                    result = true;
                    break;
                }
                pointer = 0;
            }
            //algoritmus hladanania s -s parametrom
            else if(s_parameter == true){
                //ten isty algoritmus ako bez -s parametru akurat mu neprekazaju 
                //medzery medzi target[] ciframi
                for(int jdx = 0; jdx < (input_number_size - idx); jdx++){
                    if(input_number[idx+jdx] == target[pointer]) pointer++;
                    if(pointer == target_size) break;
                }
                //ak sa pocet zhod rovna s dlzkou target[] stringu tak sa True
                if(pointer == target_size){
                    result = true;
                    break;
                }
                pointer = 0;
            }
        }
    }    
    return result;
}

//vymena nenumerickych znakov za numericke v stringu
char *alpha_to_num(char* input_str, char* output){
    int length = strlen(input_str);
    for(int idx = 0; idx < length; idx++){
        char input_char = tolower(input_str[idx]);
        if(input_char >= 'a' && input_char <= 'c') output[idx] = '2';
        else if(input_char >= 'd' && input_char <= 'f') output[idx] = '3';
        else if(input_char >= 'g' && input_char <= 'i') output[idx] = '4';
        else if(input_char >= 'j' && input_char <= 'l') output[idx] = '5';
        else if(input_char >= 'm' && input_char <= 'o') output[idx] = '6';
        else if(input_char >= 'p' && input_char <= 's') output[idx] = '7';
        else if(input_char >= 't' && input_char <= 'v') output[idx] = '8';
        else if(input_char >= 'w' && input_char <= 'z') output[idx] = '9';
        else if(input_char == '+') output[idx] = '0';
        else output[idx] = input_char;
    }
    //ukoncenie stringu \0
    output[length] = '\0';
    return output;
}

//zistenie ci je string cislo
bool is_numerical(char *string){
    //prebehne cely string skontroluje isdigit
    for (int idx = 0; string[idx] != '\0'; idx++) {
        if (!isdigit(string[idx])) {
            return false;  
        }
    }
    return true;    
}

//ocistenie stringu od \n
void  clean_string(char *string){
    int string_len = strlen(string);
    for(int idx = 0; idx < string_len; idx++){
        if(string[idx] == '\n') string[idx] ='\0';
    }
}



//main program
int main(int argc, char* target_num[]){
    //input stringy na uchovavanie dvoch zaznamov
    char buffer[101], last_buffer[101];
    int line = 0, counter = 0;
    
    //kontrola argumentov
    if((argc == 2) || (argc == 3 && strcmp(target_num[1],"-s") == 0)){
        //kontrola ciselnosti argumentov
        if(((argc == 2) && (is_numerical(target_num[1]) == true)) ||
           ((argc == 3) && (is_numerical(target_num[2]) == true))){

            //nacitava zo zoznamu do input stringu buffer[]
            while(fgets(buffer, sizeof(buffer), stdin) != NULL){
                //kontrola dlzky buffer[], ak vyse ako povolene vypise hlasenie
                if((buffer[strlen(buffer)-1] != '\n' && !feof(stdin)) || (strlen(buffer) == 1)){
                    fprintf(stderr, "False input -> input file has a wrong format\n");
                    return 1;
                }

                //ak je buffer[] v poriadku ide dalej
                else{
                    char output1[101] = {};

                    //ak najde zhodu target[] v input stringu -> (buffer) tak vypise spravny vystup
                    //skareda if podminka kvoli obmedzeniu na 100 znakov na line codu
                //ak (argc=2 && find_number=1) || (argc=3 && (find_number=1 || find_number(-s)=1))
                    if(((argc == 2) && (find_number(alpha_to_num(buffer, output1), target_num[1],
                    false) == 1)) || ((argc == 3) && (find_number(alpha_to_num(buffer, output1),
                    target_num[2], true) == 1  || find_number(alpha_to_num(buffer, output1),
                    target_num[2], false) == 1))){
                        clean_string(buffer);

                        //zistenie riadku a podla toho korektny vystup
                        if(line % 2 == 1){
                            clean_string(last_buffer);
                            fprintf(stdout, "%s, %s\n", last_buffer, buffer);
                            counter++;
                        }
                        else{
                            fgets(last_buffer, 101, stdin);
                            clean_string(last_buffer);
                            fprintf(stdout, "%s, %s\n", buffer, last_buffer);
                            line++;
                            counter++;
                        }
                    }
                    //copy do pomocneho stringu
                    strcpy(last_buffer, buffer);
                    line++;
                }
            }
            //ak sa nenasla ani jedna zhoda hlasenie not found
            if(counter == 0) fprintf(stdout, "Not found\n");
            return 0;
        }
        //is_numerical == false tak vypise hlasenie
        else {
            fprintf(stderr, "False input -> please input sequence of digits from 0 to 9.\n");
            return 1;
        }
    }

    //ak bez argumentu tak vypise cely zoznam
    else if((argc == 1)){
        while(fgets(buffer, 101, stdin) != NULL){
            if(counter % 2 == 1){
                clean_string(buffer);
                clean_string(last_buffer);
                fprintf(stdout, "%s, %s\n", last_buffer, buffer);
            }
            strcpy(last_buffer, buffer); 
            counter++;       
        }
        return 0;
    }

    //ak vyse argumentov ako ma byt tak hlasenie false input
    else {
        fprintf(stderr, "False input -> please use syntax './tnine CISLO <seznam.txt'.\n");
        return 1;
    }
}
