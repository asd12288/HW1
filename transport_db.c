#include <string.h>
#include "prog2_ex1.h"
#include "prog2_ex1.c"
#include <stdio.h>
#include <stdlib.h>



//------------------------------------------- Functions ------------------------------------------- //
void run_program(int argc, char *argv[]);
void proccess_files(FILE *input, FILE *output);
void add_line(char type[], line_id id, int number_of_station, double price);
void remove_line(line_id id);
void add_station_to_line(line_id id, char *station_name);
void report_db_lines(char *type);
void report_stations(char *type);
void report_direction(char *from_station_name, char *to_station_name);
void proccess_line(char buffer[], FILE *output);
transport_type parse_type(const char *type);

//------------------------------------------- Structs ------------------------------------------- //

typedef struct line_t {
    int exists;
    line_id id_of_line;
    transport_type type_of_transport;
    int current_num_of_stations;
    int max_num_of_stations; // to check the overflow of line
    double price;
    char **stations;
} Line;


Line *db_lines[MAX_LINES];

//------------------------------------------- Main ------------------------------------------- //

int main(int argc, char* argv[]) {

  for(int i = 0; i < MAX_LINES; i++) {
    db_lines[i] = NULL;
  }

    run_program(argc, argv);


    return 0;
}



void run_program(int argc, char *argv[]) {

    if(!(argc == 1 || argc == 3 || argc == 5)) {
         prog2_report_error_message(TRANSPORT_INVALID_ARGUMENTS);
    }
   
    FILE *input = stdin;
    FILE *output = stdout;


     if(argc == 3) {
         if(strcmp(argv[1], "-i") == 0) {    
           input = fopen(argv[2], "r");

           if(!input) {
                prog2_report_error_message(TRANSPORT_CANNOT_OPEN_FILE);
                return;
           }
           

         } else if (strcmp(argv[1], "-o") == 0) {
             output = fopen(argv[2], "w");
             if(!output) {
                prog2_report_error_message(TRANSPORT_CANNOT_OPEN_FILE);
                return;
           }

         }
         else {
            prog2_report_error_message(TRANSPORT_INVALID_ARGUMENTS);
            return;
         }
     }



       if(argc == 5) {
         if(strcmp(argv[1], "-i" ) == 0 && strcmp(argv[3], "-o") == 0) {    
            input = fopen(argv[2], "r");
            output = fopen(argv[4], "w");
           

           if((!input) || (!output)) {
                prog2_report_error_message(TRANSPORT_CANNOT_OPEN_FILE);
           }

         } else if(strcmp(argv[3], "-i" ) == 0 && strcmp(argv[1], "-o") == 0) {
            input = fopen(argv[4], "r");

            if(!input) {
                prog2_report_error_message(TRANSPORT_CANNOT_OPEN_FILE);
                return;
            }

            output = fopen(argv[2], "w");
             // TODO: refactor
            if(!output) {
                prog2_report_error_message(TRANSPORT_CANNOT_OPEN_FILE);
                fclose(input);
                return;
           }
         } else {
            prog2_report_error_message(TRANSPORT_INVALID_ARGUMENTS);
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


                // add cut of text 
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


        if(strcmp(subcmd, "db_lines") == 0) {

            printf("Report db_lines function\n");

        } else if(strcmp(subcmd, "Stations") == 0) {
            printf("Report Stations function\n");
        } else if(strcmp(subcmd, "Directions") == 0) {
            printf("Report Directions function\n");
        }

    } 


   
}

void add_line(char type[], line_id id, int number_of_station, double price) {


    if(id >= MAX_LINES || id < 0) {
      prog2_report_error_message(TRANSPORT_INVALID_LINE_NUMBER);
      return;
    }


    if(db_lines[id] != NULL) {
        prog2_report_error_message(TRANSPORT_ALREADY_EXISTS);
        return;
    }

    transport_type type_line = parse_type(type);

    if(!(type_line)) {
      prog2_report_error_message(TRANSPORT_INVALID_LINE_TYPE);
      return;
    }
 

    if(price <= 0) {
      prog2_report_error_message(TRANSPORT_INVALID_PRICE);
      return;
    }


    db_lines[id] = malloc(sizeof(Line));


    if(!db_lines[id]) {
        prog2_report_error_message(TRANSPORT_OUT_OF_MEMORY);
        return;
    }

    if(number_of_station <= 0) {
        prog2_report_error_message(TRANSPORT_INVALID_ARGUMENTS);
        return;
    }

   
    db_lines[id]->exists = 1;
    db_lines[id]->id_of_line = id;
    db_lines[id]->type_of_transport = type_line;
    db_lines[id]->max_num_of_stations = number_of_station;
    db_lines[id]->current_num_of_stations = 0;
    db_lines[id]->price = price;

    db_lines[id]->stations = malloc(number_of_station * sizeof(char *));

    if(!db_lines[id]->stations) {
      free(db_lines[id]);
      db_lines[id] = NULL;
      prog2_report_error_message(TRANSPORT_OUT_OF_MEMORY);
      return;
    }

}

void remove_line(line_id id) {


    if(!(db_lines[id]->exists)) {
        prog2_report_error_message(TRANSPORT_DOESNT_EXIST);
        return;
    }


    // remmeber the free the stations connected to the line as well!

    printf("Ready to remove line!\n");

    // all the remove logic here
}

void add_station_to_line(line_id id, char *station_name) {


    if(db_lines[id] == NULL) {
        prog2_report_error_message(TRANSPORT_DOESNT_EXIST);
        return;
    }

     if(id >= MAX_LINES || id < 0) {
      prog2_report_error_message(TRANSPORT_INVALID_LINE_NUMBER);
      return;
    }

    if(db_lines[id]->current_num_of_stations + 1  > db_lines[id]->max_num_of_stations) {
        prog2_report_error_message(TRANSPORT_STATION_OVERFLOW);
        return;
    }


    db_lines[id]->stations[db_lines[id]->current_num_of_stations] = (char *)malloc(sizeof(char ) * strlen(station_name) + 1);

    if(db_lines[id]->stations[db_lines[id]->current_num_of_stations] == NULL) {
        prog2_report_error_message(TRANSPORT_OUT_OF_MEMORY);
        return;
    }
    db_lines[id]->current_num_of_stations++;

    printf("This is the num of stations %d %s\n", db_lines[id]->current_num_of_stations, db_lines[id]->stations[0]);

}



transport_type parse_type(const char *type) {

if(!type) return 0;

  if(strcmp(type, "BUS") == 0) return BUS;
  if(strcmp(type, "METRO") == 0) return METRO;
  if(strcmp(type, "TRAIN") == 0) return TRAIN;

  return 0;
}