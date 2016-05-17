/*
 * This file is part of the OpenKinect Project. http://www.openkinect.org
 *
 * Copyright (c) 2010 individual OpenKinect contributors. See the CONTRIB file
 * for details.
 *
 * This code is licensed to you under the terms of the Apache License, version
 * 2.0, or, at your option, the terms of the GNU General Public License,
 * version 2.0. See the APACHE20 and GPL2 files for the text of the licenses,
 * or the following URLs:
 * http://www.apache.org/licenses/LICENSE-2.0
 * http://www.gnu.org/licenses/gpl-2.0.txt
 *
 * If you redistribute this file in source form, modified or unmodified, you
 * may:
 *   1) Leave this header intact and distribute it under the same terms,
 *      accompanying it with the APACHE20 and GPL20 files, or
 *   2) Delete the Apache 2.0 clause and accompany it with the GPL2 file, or
 *   3) Delete the GPL v2 clause and accompany it with the APACHE20 file
 * In all cases you must keep the copyright notice intact and include a copy
 * of the CONTRIB file.
 *
 * Binary distributions must follow the binary distribution requirements of
 * either License.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "libfreenect.h"
#include <time.h>
#include <pthread.h>


volatile int die = 0;

int depth_window;

pthread_mutex_t depth_mutex = PTHREAD_MUTEX_INITIALIZER;


freenect_context *f_ctx;
freenect_device *f_dev;
int freenect_led;


int got_rgb = 0;
int got_depth = 0;
int depth_on = 1;

uint16_t depthArr[640];


uint16_t* getDepthArray()
{
	uint16_t depthArrFront[640];
	unsigned int i;
	pthread_mutex_lock(&depth_mutex);
	if (got_depth) 
	{
		for(i=0;i<640;i++)
			depthArrFront[i] = depthArr[i];
		got_depth = 0;
	}
	pthread_mutex_unlock(&depth_mutex);
	return depthArr;
	
}

void depth_cb(freenect_device *dev, void *v_depth, uint32_t timestamp)
{
	int i,j;
	uint16_t *depth = (uint16_t*)v_depth;

	pthread_mutex_lock(&depth_mutex);
	
	for (i=640*240,j; i<640*241; i++,j++) 
		depthArr[j] = depth[i];
	
	got_depth++;
	pthread_mutex_unlock(&depth_mutex);
}

int main(int argc, char **argv)
{
	int res;
	int user_device_number = 0;

	if (freenect_init(&f_ctx, NULL) < 0) 
	{
		printf("freenect_init() failed\n");
		return 1;
	}
	
	freenect_set_log_level(f_ctx, FREENECT_LOG_DEBUG);
	freenect_select_subdevices(f_ctx, (freenect_device_flags)(FREENECT_DEVICE_MOTOR | FREENECT_DEVICE_CAMERA));

	int nr_devices = freenect_num_devices (f_ctx);
	printf ("Number of devices found: %d\n", nr_devices);

	if (nr_devices < 1) 
	{
		freenect_shutdown(f_ctx);
		return 1;
	}

	if (freenect_open_device(f_ctx, &f_dev, user_device_number) < 0) {
		printf("Could not open device\n");
		freenect_shutdown(f_ctx);
		return 1;
	}

	freenect_set_led(f_dev,LED_GREEN);
	freenect_set_depth_callback(f_dev, depth_cb);
	freenect_set_depth_mode(f_dev, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_11BIT));
	
	freenect_start_depth(f_dev);
	
	int status = 0;
	while (!die && status >= 0) 
	{
		status = freenect_process_events(f_ctx);
	}

	if (status < 0) 
	{
		printf("Something went terribly wrong.  Aborting.\n");
		return 1;
	}

	printf("\nshutting down streams...\n");

	freenect_stop_depth(f_dev);

	freenect_close_device(f_dev);
	freenect_shutdown(f_ctx);

	printf("-- done!\n");
	return 0;
}
