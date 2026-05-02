#include <string.h>
#include "prog2_ex1.h"
#include <stdio.h>
#include <stdlib.h>

//------------------------------------------- Functions ------------------------------------------- //
void run_program(int argc, char *argv[]);
void proccess_files(FILE *input, FILE *output);
void add_line(char type[], line_id id, int number_of_station, double price);
void remove_line(line_id id);
void add_station_to_line(line_id id, char *station_name);
void report_db_lines(char *type, FILE *output);
void report_stations(int id, FILE *output);
void report_direction(char *from_station_name, char *to_station_name, FILE *output);
void proccess_line(char buffer[], FILE *output);
transport_type parse_type(const char *type);
void free_memory_allocation(int id);
int check_invalid_line(int id);
int check_if_transport_exist(int id);

//------------------------------------------- Structs ------------------------------------------- //

typedef struct line_t
{
    line_id id_of_line;
    transport_type type_of_transport;
    int current_num_of_stations;
    int max_num_of_stations;
    double price;
    char **stations;
} Line;

Line *db_lines[MAX_LINES];

//------------------------------------------- Main ------------------------------------------- //

int main(int argc, char *argv[])
{
    for (int i = 0; i < MAX_LINES; i++)
    {
        db_lines[i] = NULL;
    }

    run_program(argc, argv);
    return 0;
}

void run_program(int argc, char *argv[])
{
    if (!(argc == 1 || argc == 3 || argc == 5))
    {
        prog2_report_error_message(TRANSPORT_INVALID_ARGUMENTS);
        return;
    }

    FILE *input = stdin;
    FILE *output = stdout;

    if (argc == 3)
    {
        if (strcmp(argv[1], "-i") == 0)
        {
            input = fopen(argv[2], "r");

            if (!input)
            {
                prog2_report_error_message(TRANSPORT_CANNOT_OPEN_FILE);
                return;
            }
        }
        else if (strcmp(argv[1], "-o") == 0)
        {
            output = fopen(argv[2], "w");

            if (!output)
            {
                prog2_report_error_message(TRANSPORT_CANNOT_OPEN_FILE);
                return;
            }
        }
        else
        {
            prog2_report_error_message(TRANSPORT_INVALID_ARGUMENTS);
            return;
        }
    }

    if (argc == 5)
    {
        if (strcmp(argv[1], "-i") == 0 && strcmp(argv[3], "-o") == 0)
        {
            input = fopen(argv[2], "r");
            output = fopen(argv[4], "w");

            if ((!input) || (!output))
            {
                prog2_report_error_message(TRANSPORT_CANNOT_OPEN_FILE);

                if (input)
                    fclose(input);

                if (output)
                    fclose(output);

                return;
            }
        }
        else if (strcmp(argv[3], "-i") == 0 && strcmp(argv[1], "-o") == 0)
        {
            input = fopen(argv[4], "r");

            if (!input)
            {
                prog2_report_error_message(TRANSPORT_CANNOT_OPEN_FILE);
                return;
            }

            output = fopen(argv[2], "w");

            if (!output)
            {
                prog2_report_error_message(TRANSPORT_CANNOT_OPEN_FILE);
                fclose(input);
                return;
            }
        }
        else
        {
            prog2_report_error_message(TRANSPORT_INVALID_ARGUMENTS);
            return;
        }
    }

    proccess_files(input, output);

    if (input != stdin)
        fclose(input);

    if (output != stdout)
        fclose(output);
}

void proccess_files(FILE *input, FILE *output)
{
    char buffer[MAX_LEN];

    while (fgets(buffer, sizeof(buffer), input) != NULL)
    {
        if (buffer[0] == '\n' || buffer[0] == '\r' || buffer[0] == '\0')
            continue;

        if (buffer[0] == '#')
            continue;

        proccess_line(buffer, output);
    }

    for (int i = 0; i < MAX_LINES; i++)
    {
        free_memory_allocation(i);
    }
}

void proccess_line(char buffer[], FILE *output)
{
    char *cmd = strtok(buffer, " \t\n\r");

    if (!cmd)
        return;

    if (strcmp(cmd, "Remove") == 0)
    {
        strtok(NULL, " \t\n\r");
        line_id id = atoi(strtok(NULL, " \t\n\r"));
        remove_line(id);
    }

    //------------------------------------------- Add ------------------------------------------- //

    if (strcmp(cmd, "Add") == 0)
    {
        char *subcmd = strtok(NULL, " \t\n\r");

        if (!subcmd)
            return;

        if (strcmp(subcmd, "Line") == 0)
        {
            char *type = strtok(NULL, " \t\n\r");
            line_id id = atoi(strtok(NULL, " \t\n\r"));
            int number_of_station = atoi(strtok(NULL, " \t\n\r"));
            double price = atof(strtok(NULL, " \t\n\r"));

            add_line(type, id, number_of_station, price);
        }
        else if (strcmp(subcmd, "Station") == 0)
        {
            strtok(NULL, " \t\n\r");
            strtok(NULL, " \t\n\r");

            line_id id = atoi(strtok(NULL, " \t\n\r"));
            char *name = strtok(NULL, " \t\n\r");

            add_station_to_line(id, name);
        }
    }

    //------------------------------------------- Report ------------------------------------------- //

    if (strcmp(cmd, "Report") == 0)
    {
        char *subcmd = strtok(NULL, " \t\n\r");

        if (!subcmd)
            return;

        if (strcmp(subcmd, "Lines") == 0)
        {
            char *type = strtok(NULL, " \t\n\r");
            report_db_lines(type, output);
        }
        else if (strcmp(subcmd, "Stations") == 0)
        {
            int id = atoi(strtok(NULL, " \t\n\r"));
            report_stations(id, output);
        }
        else if (strcmp(subcmd, "Directions") == 0)
        {
            char *from = strtok(NULL, " \t\n\r");
            char *to = strtok(NULL, " \t\n\r");

            report_direction(from, to, output);
        }
    }
}

// ------------------------------------------- Add ------------------------------------------- //

void add_line(char *type, line_id id, int number_of_station, double price)
{

    transport_type type_line = parse_type(type);
    if (!type_line) {
        prog2_report_error_message(TRANSPORT_INVALID_LINE_TYPE);
        return;
    }

    if (id < 0 || id >= MAX_LINES) {
        prog2_report_error_message(TRANSPORT_INVALID_LINE_NUMBER);
        return;
    }

    if (price <= 0) {
        prog2_report_error_message(TRANSPORT_INVALID_PRICE);
        return;
    }

    if (db_lines[id] != NULL) {
        prog2_report_error_message(TRANSPORT_ALREADY_EXISTS);
        return;
    }

    db_lines[id] = malloc(sizeof(Line));

    if (db_lines[id] == NULL)
    {
        prog2_report_error_message(TRANSPORT_OUT_OF_MEMORY);
        for (int i = 0; i < MAX_LINES; i++)
            free_memory_allocation(i);
        exit(0);
    }

    if (number_of_station <= 0)
    {
        prog2_report_error_message(TRANSPORT_INVALID_ARGUMENTS);
        free(db_lines[id]);
        db_lines[id] = NULL;
        return;
    }

    db_lines[id]->id_of_line = id;
    db_lines[id]->type_of_transport = type_line;
    db_lines[id]->max_num_of_stations = number_of_station;
    db_lines[id]->current_num_of_stations = 0;
    db_lines[id]->price = price;
    db_lines[id]->stations = NULL;
}

void add_station_to_line(line_id id, char *station_name)
{
    if (!check_if_transport_exist(id))
    {
        return;
    }

    if (db_lines[id]->current_num_of_stations + 1 > db_lines[id]->max_num_of_stations)
    {
        prog2_report_error_message(TRANSPORT_STATION_OVERFLOW);
        return;
    }

    char **temp = realloc(
        db_lines[id]->stations,
        (db_lines[id]->current_num_of_stations + 1) * sizeof(char *)
    );

    if (temp == NULL)
    {
        prog2_report_error_message(TRANSPORT_OUT_OF_MEMORY);
        for (int i = 0; i < MAX_LINES; i++)
            free_memory_allocation(i);
        exit(0);
    }

    db_lines[id]->stations = temp;

    db_lines[id]->stations[db_lines[id]->current_num_of_stations] =
        malloc((strlen(station_name) + 1) * sizeof(char));

    if (db_lines[id]->stations[db_lines[id]->current_num_of_stations] == NULL)
    {
        prog2_report_error_message(TRANSPORT_OUT_OF_MEMORY);
        for (int i = 0; i < MAX_LINES; i++)
            free_memory_allocation(i);
        exit(0);
    }

    strcpy(db_lines[id]->stations[db_lines[id]->current_num_of_stations], station_name);

    db_lines[id]->current_num_of_stations++;
}

void remove_line(line_id id)
{
    if (!check_if_transport_exist(id))
    {
        return;
    }

    for (int i = 1; i <= db_lines[id]->current_num_of_stations; i++)
    {
        free(db_lines[id]->stations[(db_lines[id]->current_num_of_stations) - i]);
    }

    free(db_lines[id]->stations);
    free(db_lines[id]);

    db_lines[id] = NULL;
}

void report_db_lines(char *type, FILE *output)
{
    transport_type type_line = parse_type(type);

    if (!type_line)
    {
        prog2_report_error_message(TRANSPORT_INVALID_LINE_TYPE);
        return;
    }

    int transport_empty = 1;

    for (int i = 0; i < MAX_LINES; i++)
    {
        if (db_lines[i] != NULL && (db_lines[i]->type_of_transport == type_line || type_line == 7))
        {
            transport_empty = 0;

            prog2_report_line(
                output,
                db_lines[i]->id_of_line,
                db_lines[i]->type_of_transport,
                db_lines[i]->current_num_of_stations,
                db_lines[i]->price
            );
        }
    }

    if (transport_empty)
    {
        prog2_report_error_message(TRANSPORT_EMPTY);
    }
}

void report_stations(int id, FILE *output)
{
    if (!check_if_transport_exist(id))
    {
        return;
    }

    prog2_report_line(
        output,
        db_lines[id]->id_of_line,
        db_lines[id]->type_of_transport,
        db_lines[id]->current_num_of_stations,
        db_lines[id]->price
    );

    for (int i = 0; i < db_lines[id]->current_num_of_stations; i++)
    {
        prog2_report_station(output, db_lines[id]->stations[i]);
    }
}

void report_direction(char *from, char *to, FILE *output)
{
    int notFoundTo = 1;

    for (int i = 0; i < MAX_LINES; i++)
    {
        if (db_lines[i] == NULL)
            continue;

        // same station 
        if (strcmp(from, to) == 0)
        {
            for (int j = 0; j < db_lines[i]->current_num_of_stations; j++)
            {
                if (strcmp(db_lines[i]->stations[j], from) == 0)
                {
                    int n = db_lines[i]->current_num_of_stations;

                    prog2_report_line(
                        output,
                        db_lines[i]->id_of_line,
                        db_lines[i]->type_of_transport,
                        db_lines[i]->current_num_of_stations,
                        db_lines[i]->price
                    );

                    prog2_report_station(output, db_lines[i]->stations[0]);
                    prog2_report_station(output, db_lines[i]->stations[n - 1]);

                    notFoundTo = 0;
                    break;
                }
            }

            continue;
        }

        int notFoundFrom = 1;

        for (int j = 0; j < db_lines[i]->current_num_of_stations; j++)
        {
            if (notFoundFrom)
            {
                if (strcmp(db_lines[i]->stations[j], from) == 0)
                {
                    notFoundFrom = 0;
                }
            }
            else if (strcmp(db_lines[i]->stations[j], to) == 0)
            {
                int n = db_lines[i]->current_num_of_stations;

                prog2_report_line(
                    output,
                    db_lines[i]->id_of_line,
                    db_lines[i]->type_of_transport,
                    db_lines[i]->current_num_of_stations,
                    db_lines[i]->price
                );

                prog2_report_station(output, db_lines[i]->stations[0]);
                prog2_report_station(output, db_lines[i]->stations[n - 1]);

                notFoundTo = 0;
                break;
            }
        }
    }

    if (notFoundTo)
    {
        prog2_report_error_message(TRANSPORT_DOESNT_EXIST);
    }
}

transport_type parse_type(const char *type)
{
    if (!type)
        return ALL;

    if (strcmp(type, "BUS") == 0)
        return BUS;

    if (strcmp(type, "METRO") == 0)
        return METRO;

    if (strcmp(type, "TRAIN") == 0)
        return TRAIN;

    return 0;
}

void free_memory_allocation(int id)
{
    if (db_lines[id] != NULL)
    {
        for (int i = 1; i <= db_lines[id]->current_num_of_stations; i++)
        {
            free(db_lines[id]->stations[(db_lines[id]->current_num_of_stations) - i]);
        }

        free(db_lines[id]->stations);
        free(db_lines[id]);

        db_lines[id] = NULL;
    }
}

int check_invalid_line(int id)
{
    if (id >= MAX_LINES || id < 0)
    {
        prog2_report_error_message(TRANSPORT_INVALID_LINE_NUMBER);
        return 1;
    }

    return 0;
}

int check_if_transport_exist(int id)
{
    if (check_invalid_line(id))
    {
        return 0;
    }

    if (db_lines[id] == NULL)
    {
        prog2_report_error_message(TRANSPORT_DOESNT_EXIST);
        return 0;
    }

    return 1;
}

