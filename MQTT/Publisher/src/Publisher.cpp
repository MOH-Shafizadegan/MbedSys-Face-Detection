#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include <stdio.h>
#include "MQTTClient.h"
#include <mysql.h>
#include <string>
#include <vector>
#include <iostream>

#define ADDRESS     "localhost"
#define CLIENTID    "pub"

using namespace std;

MYSQL* dB;
int face_num = 0;
char* date_Time = nullptr;

void publish(MQTTClient client, char* topic, char* payload) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = payload;
    pubmsg.payloadlen = strlen((char *)pubmsg.payload);
    pubmsg.qos = 2; //send message once and recieve it once
    pubmsg.retained = 0;
    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, 1000L);
    printf("Message '%s' with delivery token %d delivered\n", payload, token);
}

int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    char* payload = (char*)message->payload;
    printf("Received operation %s\n", payload);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

vector<string> read_data(MYSQL* dB)
{
    // Declare a vector to store the values
    vector<string> data;

    // Declare a string to store the query
    string query = "SELECT * FROM FACES ORDER BY id DESC LIMIT 1";

    // Execute the query
    if (mysql_query(dB, query.c_str()))
    {
        // Print an error message if the query fails
        cerr << "Query error: " << mysql_error(dB) << endl;
        return data;
    }

    // Store the result
    MYSQL_RES* result = mysql_store_result(dB);

    // Check if the result is valid
    if (result == NULL)
    {
        // Print an error message if the result is null
        cerr << "Result error: " << mysql_error(dB) << endl;
        return data;
    }

    // Fetch the last row
    MYSQL_ROW last_row = mysql_fetch_row(result);

    // Check if the last row is null
    if (last_row == NULL)
    {
        // Print an error message if the last row is null
        cerr << "Row fetch error: " << mysql_error(dB) << endl;
        return data;
    }
    
    // Get the value of the second field as a string
    string value2 = last_row[1] ? last_row[1] : "NULL";
    data.push_back(value2);

    // Get the value of the third field as a string
    string value3 = last_row[2] ? last_row[2] : "NULL";
    data.push_back(value3);

    // Free the result
    mysql_free_result(result);

    // Return the vector of data
    return data;
}


int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.username = "face_pub";
    conn_opts.password = "pubpass";

    MQTTClient_setCallbacks(client, NULL, NULL, on_message, NULL);

    //MySQL
    dB = mysql_init(NULL);
    if (dB == nullptr)
    {
        fprintf(stderr, "%s\n", mysql_error(dB));
        exit(1);
    }
    if (mysql_real_connect(dB, "localhost", "root", "mohsh2002",
            NULL, 0, NULL, 0) == nullptr)
    {
    	fprintf(stderr, "%s\n", mysql_error(dB));
        mysql_close(dB);
        exit(1);
    }
    if (mysql_query(dB, "USE PROJECT"))
    {
       fprintf(stderr, "%s\n", mysql_error(dB));
       mysql_close(dB);
       exit(1);
    }
    //MySQL

    int rc;
    
    while (true) {
        if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
            printf("Failed to connect, return code %d\n", rc);
            exit(-1);
        }
        //getting number of faces
        sleep(1);
        
        vector<string> data = read_data(dB);
        
        string send = "n=" + data[0] + ", " + data[1];
        int n = send.length();
        char char_send[n + 1];
        strcpy(char_send, send.c_str());
        publish(client, "Dev0/face", char_send);
    }

    MQTTClient_disconnect(client, 1000);
    MQTTClient_destroy(&client);
    return rc;
}

