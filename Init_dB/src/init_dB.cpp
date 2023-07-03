#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>

void error_handler(MYSQL *dB)
{
    fprintf(stderr, "%s\n", mysql_error(dB));
    mysql_close(dB);
    exit(1);
}

int main(int argc, char **argv)
{
    MYSQL *dB = mysql_init(NULL);

    if (dB == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(dB));
        exit(1);
    }
    if (mysql_real_connect(dB, "localhost", "root", "mohsh2002",
            NULL, 0, NULL, 0) == NULL)
    {
        error_handler(dB);
    }
    if (mysql_query(dB, "DROP DATABASE IF EXISTS PROJECT"))
    {
        error_handler(dB);
    }
    if (mysql_query(dB, "CREATE DATABASE PROJECT"))
    {
        error_handler(dB);
    }
    if (mysql_query(dB, "USE PROJECT")) {
        error_handler(dB);
    }
    if (mysql_query(dB, "DROP TABLE IF EXISTS FACES")) {
        error_handler(dB);
    }
    if (mysql_query(dB, "CREATE TABLE FACES(id INT PRIMARY KEY AUTO_INCREMENT, Face_num INT, Time VARCHAR(255))")) {
        error_handler(dB);
    }
  mysql_close(dB);
  exit(0);
}
