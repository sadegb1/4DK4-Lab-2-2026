
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

/******************************************************************************/

#include <stdio.h>
#include "simparameters.h"
#include "main.h"
#include "output.h"

/******************************************************************************/

/*
 * This function outputs a progress message to the screen to indicate this are
 * working.
 */

void
output_progress_msg_to_screen(Simulation_Run_Ptr simulation_run)
{
  double percentage_done;
  Simulation_Run_Data_Ptr data;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  data->blip_counter++;

  if((data->blip_counter >= BLIPRATE)
     ||
     (data->number_of_packets_processed >= RUNLENGTH)) {

    data->blip_counter = 0;

    percentage_done =
      100 * (double) data->number_of_packets_processed/RUNLENGTH;

    printf("%3.0f%% ", percentage_done);

    printf("Successfully Xmtted Pkts  = %ld (Arrived Pkts = %ld) \r", 
	   data->number_of_packets_processed, data->arrival_count);

    fflush(stdout);
  }

}

/*
 * When a simulation_run run is completed, this function outputs various
 * collected statistics on the screen.
 */

void
output_results(Simulation_Run_Ptr simulation_run)
{
  double voice_mean_delay;
  double data_mean_delay;
  Simulation_Run_Data_Ptr data;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  voice_mean_delay = 1e3 * data->accumulated_delay_by_class[VOICE_TRAFFIC] /
    data->processed_by_class[VOICE_TRAFFIC];
  printf("%u,%d,%.3f,", data->random_seed, data->MEAN_ARRIVAL_RATE,
	 voice_mean_delay);

  if(data->processed_by_class[DATA_TRAFFIC] > 0) {
    data_mean_delay = 1e3 * data->accumulated_delay_by_class[DATA_TRAFFIC] /
      data->processed_by_class[DATA_TRAFFIC];
    printf("%.3f", data_mean_delay);
  }

  printf(",%ld,%ld\n", data->processed_by_class[VOICE_TRAFFIC],
	 data->processed_by_class[DATA_TRAFFIC]);
}



