/* This provides a crude manner of testing the performance of a broker in messages/s. */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mosquitto.h>
#include <string.h>
#include <unistd.h>


static bool run = true;
static int message_count = 0;
static struct timeval start, stop;

void my_connect_callback(struct mosquitto *mosq, void *obj, int rc)
{
	printf("rc: %d\n", rc);
	gettimeofday(&start, NULL);
}

void my_disconnect_callback(struct mosquitto *mosq, void *obj, int result)
{
	run = false;
}

void my_publish_callback(struct mosquitto *mosq, void *obj, int mid)
{
	;
}

int main(int argc, char *argv[])
{
	struct mosquitto *mosq;
	int i;
	double dstart, dstop, diff;


	start.tv_sec = 0;
	start.tv_usec = 0;
	stop.tv_sec = 0;
	stop.tv_usec = 0;

	mosquitto_lib_init();

	mosq = mosquitto_new("perftest", true, NULL);
	mosquitto_connect_callback_set(mosq, my_connect_callback);
	mosquitto_disconnect_callback_set(mosq, my_disconnect_callback);
	mosquitto_publish_callback_set(mosq, my_publish_callback);

	mosquitto_connect(mosq, "127.0.0.1", atoi(argv[1]), 600);

	mosquitto_loop_start(mosq);

		mosquitto_publish(mosq, NULL, "perf/test", strlen(argv[1]), argv[1], 0, false);

	while(true)
	{

		mosquitto_publish(mosq, NULL, "perf/test", strlen(argv[1]), argv[1], 0, false);
		sleep(1);
	}
	mosquitto_loop_stop(mosq, false);

	dstart = (double)start.tv_sec*1.0e6 + (double)start.tv_usec;
	dstop = (double)stop.tv_sec*1.0e6 + (double)stop.tv_usec;
	diff = (dstop-dstart)/1.0e6;


	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return 0;
}
