#ifndef __PROG2_EX1__
#define __PROG2_EX1__

/*
 * prog2_ex1.h
 * This file contains the definitions of the functions that you need to implement in prog2_ex1.c.
 * !! You should not modify this file !!
 * Author: David Avdat
 */

#include <stdio.h>

#define MAX_LINES 1000
#define MAX_LEN 256
#define NOT_AVAILABLE "N/A"

typedef int line_id;

/*
 * Transport type enumeration
 */
typedef enum {
    BUS = 1,
    TRAIN = 2,
    METRO = 4,
    ALL = 7,
} transport_type;

/*
 * Transport error enumeration
*/
typedef enum 
{
    TRANSPORT_SUCCESS,
    TRANSPORT_INVALID_ARGUMENTS,
    TRANSPORT_CANNOT_OPEN_FILE,
    TRANSPORT_OUT_OF_MEMORY,
    TRANSPORT_INVALID_LINE_TYPE,
    TRANSPORT_INVALID_LINE_NUMBER,
    TRANSPORT_INVALID_PRICE,
    TRANSPORT_ALREADY_EXISTS,
    TRANSPORT_DOESNT_EXIST,
    TRANSPORT_STATION_OVERFLOW,
    TRANSPORT_EMPTY

} transport_result;

/*
 * Function prototypes
 */

/*
function: prog2_report_error_message
 * Description: Reports an error message based on the provided transport error code
 * Parameters: error - the transport error code
 */
void prog2_report_error_message(transport_result error);

/*
function: prog2_report_line
 * Description: Reports information about a transport line
 * Parameters: out_stream - the output stream to write to
 *             number - the line number
 *             type - the transport type (BUS, TRAIN, METRO)
 *             num_stations - the actual number of stations that were added to the line
 *             price - the price of the line
 */
void prog2_report_line(FILE *out_stream, line_id number, transport_type type, int num_stations, double price);

/*
function: prog2_report_station
 * Description: Reports information about a transport station
 * Parameters: out_stream - the output stream to write to
 *             name - the name of the station
 */
void prog2_report_station(FILE *out_stream, const char *name);

#endif
