
/*
 * 
 * Simulation_Run of A Single Server Queueing System
 * 
 * Copyright (C) 2014 Terence D. Todd Hamilton, Ontario, CANADA,
 * todd@mcmaster.ca
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "output.h"
#include "simparameters.h"
#include "packet_arrival.h"
#include "cleanup_memory.h"
#include "trace.h"
#include "main.h"

/******************************************************************************/

/* Run the simulation for each data rate and seed. */
int
main(void)
{
  Simulation_Run_Ptr simulation_run;
  Simulation_Run_Data data;

  unsigned RANDOM_SEEDS[] = {RANDOM_SEED_LIST, 0};
  unsigned random_seed;
  int j;
  int MEAN_ARRIVAL_RATE;

  printf("seed,MEAN_ARRIVAL_RATE_packets_per_second,voice_mean_delay_ms,"
	 "data_mean_delay_ms,voice_packets,data_packets\n");

  /* 
   * Loop for each random number generator seed, doing a separate
   * simulation_run run for each.
   */

    for(MEAN_ARRIVAL_RATE = MEAN_ARRIVAL_RATE_MIN;
      MEAN_ARRIVAL_RATE <= MEAN_ARRIVAL_RATE_MAX;
      MEAN_ARRIVAL_RATE += MEAN_ARRIVAL_RATE_STEP) {
    for(j = 0; (random_seed = RANDOM_SEEDS[j]) != 0; j++) {

      simulation_run = simulation_run_new();

      simulation_run_attach_data(simulation_run, (void *) & data);

      memset(&data, 0, sizeof(data));
      data.MEAN_ARRIVAL_RATE = MEAN_ARRIVAL_RATE;
      data.random_seed = random_seed;

      data.buffer = fifoqueue_new();
      data.link = server_new();

      random_generator_initialize(random_seed);

      schedule_voice_packet_arrival_event(simulation_run,
    					  simulation_run_get_time(simulation_run));
      if(MEAN_ARRIVAL_RATE > 0)
    	schedule_packet_arrival_event(simulation_run,
    				      simulation_run_get_time(simulation_run) +
              exponential_generator(1.0 / MEAN_ARRIVAL_RATE));

      while(data.number_of_packets_processed < RUNLENGTH)
    	simulation_run_execute_event(simulation_run);

      output_results(simulation_run);
      cleanup_memory(simulation_run);
    }
  }

  return 0;
}












