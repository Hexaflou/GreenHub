#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mysql/mysql.h>
#include <pthread.h>

#define MYSQL_SADDR 	"134.214.223.14"
#define MYSQL_SLOGIN	"greenhub"
#define MYSQL_SPWD  	"greenhub-pass"
#define MYSQL_SBASE 	"greenhub"
#define MYSQL_SPORT	0 /* auto */

#define DATETIME_LENGHT 20

static MYSQL mysql;

/* Put in the given char the time in Datetime format */
static void getDateTime(char * return_date)
{
	time_t t;
	time(& t);
	/************************TODO**************************/
}

int gLogsInit()
{
	mysql_init(&mysql);
	mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"option");
	return 0;
}

int gLogsClose()
{
	return 0;
}

int gLogsLog(char mac[40], double value)
{
	
	char instruction[500];
	if(mysql_real_connect(&mysql,MYSQL_SADDR,MYSQL_SLOGIN,
								MYSQL_SPWD,MYSQL_SBASE,MYSQL_SPORT,NULL,0))
    {
		sprintf(instruction,"INSERT INTO greenhub_state"
		" (captured_at, value, sensor_id) SELECT NOW(), '%lf',"
		" s.id FROM greenhub_sensor s WHERE s.mac_address = '%s';",
		value,mac);
		
		mysql_query(&mysql,instruction);
		
        mysql_close(&mysql);
        return 0;
    }
    else
    {
        printf("Une erreur s'est produite lors de la connexion à la BDD!");
    }
    return -1;
}

int main()
{
    gLogsInit();
    gLogsLog("28",42);
    gLogsClose();
    return 0;
}

