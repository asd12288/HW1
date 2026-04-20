#include <string.h>
#include "prog2_ex1.h"
#include "prog2_ex1.c"
#include <stdio.h>
#include <stdlib.h>



void handle_file(int argc, char *argv[]);


int main(int argc, char* argv[]) {
   
    handle_file(argc, argv);


    return 0;
}



void handle_file(int argc, char *argv[]) {

  
    // Checking args 
    if(argc > 5 || argc == 2 || argc == 4) {
        prog2_report_error_message(0);
    }

    FILE *fp_input = stdin;
    FILE *fp_output = stdout;


     if(argc == 3) {
         if(strcmp(argv[1], "-i") == 0) {    
           fp_input = fopen(argv[2], "r");

           if(fp_input == NULL) {
                prog2_report_error_message(1);
           }

         } else if (strcmp(argv[1], "-o") == 0) {
             fp_output = fopen(argv[2], "w");

             // TODO: refactor
             if(fp_output == NULL) {
                prog2_report_error_message(1);
           }
         }
     }

       if(argc == 5) {
         if(strcmp(argv[1], "-i" ) == 0 && strcmp(argv[3], "-o") == 0) {    
            fp_input = fopen(argv[2], "r");
            fp_output = fopen(argv[4], "w");

           // TODO: refactor
           if(fp_input == NULL || fp_output == NULL) {
                prog2_report_error_message(1);
           }

         } else if(strcmp(argv[3], "-i" ) == 0 && strcmp(argv[1], "-o") == 0) {
            fp_input = fopen(argv[4], "r");
            fp_output = fopen(argv[2], "w");
             // TODO: refactor
            if(fp_input == NULL || fp_output == NULL) {
                prog2_report_error_message(1);
           }
         }
     }
     
}






