
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
#include "output.h"
#include "simparameters.h"
#include "packet_arrival.h"
#include "cleanup_memory.h"
#include "trace.h"
#include "main.h"

/******************************************************************************/

/*
 * main.c declares and creates a new simulation_run with parameters defined in
 * simparameters.h. The code creates a fifo queue and server for the single
 * server queueuing system. It then loops through the list of random number
 * generator seeds defined in simparameters.h, doing a separate simulation_run
 * run for each. To start a run, it schedules the first packet arrival
 * event. When each run is finished, output is printed on the terminal.
 */

int
main(void)
{
  Simulation_Run_Ptr simulation_run;
  Simulation_Run_Data data;

  /*
   * Declare and initialize our random number generator seeds defined in
   * simparameters.h
   */

  unsigned RANDOM_SEEDS[] = {RANDOM_SEED_LIST, 0};
  unsigned random_seed;
  int j;
  int source_index;

  printf("p_12, Random Seed, mean_delay_switch1_ms, mean_delay_switch2_ms, mean_delay_switch3_ms\n");

  data.arrival_sources[0].switch_id = 1;
  data.arrival_sources[0].arrival_rate = PACKET_ARRIVAL_RATE1;
  data.arrival_sources[1].switch_id = 2;
  data.arrival_sources[1].arrival_rate = PACKET_ARRIVAL_RATE23;
  data.arrival_sources[2].switch_id = 3;
  data.arrival_sources[2].arrival_rate = PACKET_ARRIVAL_RATE23;

  double P12[] = {P12_LIST};

  /*
  * Loop for each p_12 (probability of routing a Link 1 packet to Link 2)
  */
  for (int i=0; i<sizeof(P12)/sizeof(P12[0]); i++) {
    /* 
    * Loop for each random number generator seed, doing a separate
    * simulation_run run for each.
    */
    j = 0;
    while ((random_seed = RANDOM_SEEDS[j++]) != 0) {

      simulation_run = simulation_run_new(); /* Create a new simulation run. */

      /*
      * Set the simulation_run data pointer to our data object.
      */

      simulation_run_attach_data(simulation_run, (void *) & data);

      /* 
      * Initialize the simulation_run data variables, declared in main.h.
      */
      
      data.blip_counter = 0;
      data.arrival_count = 0;
      data.number_of_packets_processed = 0;
      data.accumulated_delay = 0.0;
      data.random_seed = random_seed;
      data.p12 = P12[i];
      data.delay_over_20_counter = 0;
      for(source_index = 0; source_index < 3; source_index++) {
        data.packets_processed_by_source[source_index] = 0;
        data.accumulated_delay_by_source[source_index] = 0.0;
      }

      /* 
      * Create the packet buffer and transmission link, declared in main.h.
      */

      data.buffer1 = fifoqueue_new();
      data.buffer2 = fifoqueue_new();
      data.buffer3 = fifoqueue_new();
      data.link1   = server_new();
      data.link2   = server_new();
      data.link3   = server_new();

      /* 
      * Set the random number generator seed for this run.
      */
      random_generator_initialize(random_seed);

      /* 
      * Schedule the initial packet arrival for the current clock time (= 0).
      */
      for(source_index = 0; source_index < 3; source_index++) {
        schedule_packet_arrival_event(simulation_run,
            simulation_run_get_time(simulation_run),
            &data.arrival_sources[source_index]);
      }

      /* 
      * Execute events until we are finished. 
      */
      while(data.number_of_packets_processed < RUNLENGTH) {
        simulation_run_execute_event(simulation_run);
      }

      /*
      * Output results and clean up after ourselves.
      */

      output_results(simulation_run);
      cleanup_memory(simulation_run);
    }
  }

  // getchar();   /* Pause before finishing. */
  return 0;
}












