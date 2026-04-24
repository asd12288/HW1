
#include "prog2_ex1.h"


const char *error_strings[] = {
    "Success",
    "Invalid Arguments",
    "Cannot open file",
    "Out of memory",
    "Invalid line type",
    "Invalid line number",
    "Invalid price",
    "Line already exists",
    "Line Does not exist",
    "Station overflow",
    "Transport database is empty"
};

const char *type_strings[] = {"Bus", "Train", "Metro"};

const char *error_to_string(transport_result error)
{
    if (error <= TRANSPORT_EMPTY && error >= 0)
    {
        return error_strings[error];
    }

    return "Unknown error"; // Should not happen
}

const char *type_to_string(transport_type type)
{
    if (type <= METRO && type >= 0)
    {
        return type_strings[type];
    }
    return "Unknown transport type";
}

void prog2_report_error_message(transport_result error)
{
    fprintf(stderr, "%s\n", error_to_string(error));
}

void prog2_report_line(FILE *out_stream, line_id number, transport_type type, int num_stations, double price)
{
    fprintf(out_stream, "%5s Line %8d [%4d stations] %.2f$\n", type_to_string(type), number, num_stations, price);
}

void prog2_report_station(FILE *out_stream, const char *name)
{
    fprintf(out_stream, "\tStation: %s\n", name);
}

