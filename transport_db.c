#include <string.h>
#include "prog2_ex1.h"
#include "prog2_ex1.c"
#include <stdio.h>
#include <stdlib.h>



void handle_file(int argc, char *argv[]);
void proccess_files(FILE *input, FILE *output);
void add_line(char type[], line_id id, int number_of_station, double price);
void remove_line(line_id id);
void add_station_to_line(line_id id, char *station_name);
void report_lines(char *type);
void report_stations(char *type);
void report_direction(char *from_station_name, char *to_station_name);
void proccess_line(char buffer[], FILE *output);



typedef struct line_t {
    int exists;
    line_id id_of_line;
    transport_type type_of_transport;
    int current_num_of_stations;
    int max_num_of_stations; // to check the overflow of line
    double price;
    char **stations;
} Line;


Line lines[MAX_LINES];

int main(int argc, char* argv[]) {

    handle_file(argc, argv);


    return 0;
}



void handle_file(int argc, char *argv[]) {

    if(!(argc == 1 || argc == 3 || argc == 5)) {
         prog2_report_error_message(TRANSPORT_CANNOT_OPEN_FILE);
    }
   
    FILE *input = stdin;
    FILE *output = stdout;


     if(argc == 3) {
         if(strcmp(argv[1], "-i") == 0) {    
           input = fopen(argv[2], "r");

           if(!input) {
                prog2_report_error_message(TRANSPORT_INVALID_ARGUMENTS);
                return;
           }
           

         } else if (strcmp(argv[1], "-o") == 0) {
             output = fopen(argv[2], "w");
             if(!output) {
                prog2_report_error_message(TRANSPORT_INVALID_ARGUMENTS);
                return;
           }

         }
         else {
            prog2_report_error_message(TRANSPORT_CANNOT_OPEN_FILE);
            return;
         }
     }



       if(argc == 5) {
         if(strcmp(argv[1], "-i" ) == 0 && strcmp(argv[3], "-o") == 0) {    
            input = fopen(argv[2], "r");
            output = fopen(argv[4], "w");
           

           if((!input) || (!output)) {
                prog2_report_error_message(1);
           }

         } else if(strcmp(argv[3], "-i" ) == 0 && strcmp(argv[1], "-o") == 0) {
            input = fopen(argv[4], "r");

            if(!input) {
                prog2_report_error_message(TRANSPORT_INVALID_ARGUMENTS);
                return;
            }

            output = fopen(argv[2], "w");
             // TODO: refactor
            if(!output) {
                prog2_report_error_message(TRANSPORT_INVALID_ARGUMENTS);
                fclose(input);
                return;
           }
         } else {
            prog2_report_error_message(0);
         }
     } 

     proccess_files(input, output);


     if(input != stdin) fclose(input);
     if(output != stdout) fclose(output);

}


void proccess_files(FILE *input, FILE *output) {

        char buffer[MAX_LEN];
        while(fgets(buffer, sizeof(buffer), input) != NULL) {

        if(buffer[0] == '\n') continue;
        if(buffer[0] == '#') continue;

        proccess_line(buffer, output);
    
        // get input from file
    }
}

void proccess_line(char buffer[], FILE *output) {

    char *cmd = strtok(buffer, " \t\n");
    if(!cmd) return;

    // Add 
    if(strcmp(cmd, "Add") == 0) {
        char *subcmd = strtok(NULL, " \t\n");
        if(!subcmd) return;

        if(strcmp(subcmd, "Line") == 0) {
            

            char *type = strtok(NULL, " \t\n");
            line_id id = atoi(strtok(NULL, " \t\n"));
            int number_of_station = atoi(strtok(NULL, " \t\n"));
            double price = atof(strtok(NULL, " \t\n"));
            add_line(type, id, number_of_station, price);


        } else if(strcmp(subcmd, "Station") == 0) {

                line_id id = atoi(strtok(NULL, " \t\n"));
                char *name = strtok(NULL, " \t\n");
                add_station_to_line(id, name);
        }
    } 

    // Remove Functions

     if(strcmp(cmd, "Remove") == 0) {
        strtok(NULL, " \t\n");
        line_id id = atoi(strtok(NULL, " \t\n"));
        remove_line(id);
    }
    
    // Report Functions

    if(strcmp(cmd, "Report") == 0) {
        char *subcmd = strtok(NULL, " \t\n");
        if(!subcmd) return;


        if(strcmp(subcmd, "Lines") == 0) {
            printf("Report Lines function\n");
        } else if(strcmp(subcmd, "Stations") == 0) {
            printf("Report Stations function\n");
        } else if(strcmp(subcmd, "Directions") == 0) {
            printf("Report Directions function\n");
        }

    } 


   
}

void add_line(char type[], line_id id, int number_of_station, double price) {

    
    if(price <= 0) {
        prog2_report_error_message(TRANSPORT_INVALID_PRICE);
    }

    // all the logic add here ...

}

void remove_line(line_id id) {
    printf("This is the id to Remove %d\n", id);

    // all the remove logic here
}

void add_station_to_line(line_id id, char *station_name) {
    printf("This is the station to add %s to this line %d \n", station_name, id);

    // all the add station to a line here
}

// void report_lines()


// function to check 
// fgets, fputs
// strtok
// atoi