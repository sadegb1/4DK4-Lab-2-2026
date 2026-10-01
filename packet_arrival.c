
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

#include <math.h>
#include <stdio.h>
#include "main.h"
#include "packet_transmission.h"
#include "packet_arrival.h"

/******************************************************************************/

/*
 * This function will schedule a packet arrival at a time given by
 * event_time. At that time the function "packet_arrival" (located in
 * packet_arrival.c) is executed. An object can be attached to the event and
 * can be recovered in packet_arrival.c.
 */

long int
schedule_packet_arrival_event(Simulation_Run_Ptr simulation_run,
            double event_time,
            Arrival_Source_Ptr source)
{
  Event event;

  event.description = "Packet Arrival";
  event.function = packet_arrival_event;
  event.attachment = (void *) source;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

/******************************************************************************/

/*
 * This is the event function which is executed when a packet arrival event
 * occurs. It creates a new packet object and places it in either the fifo
 * queue if the server is busy. Otherwise it starts the transmission of the
 * packet. It then schedules the next packet arrival event.
 */

void
packet_arrival_event(Simulation_Run_Ptr simulation_run, void * ptr)
{
  Simulation_Run_Data_Ptr data;
  Arrival_Source_Ptr source;
  Packet_Ptr new_packet;
  Server_Ptr links[3];
  Fifoqueue_Ptr buffers[3];
  int source_index;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);
  source = (Arrival_Source_Ptr) ptr;
  source_index = source->switch_id - 1;

  if(source_index < 0 || source_index >= 3) {
    return;
  }

  links[0] = data->link1;
  links[1] = data->link2;
  links[2] = data->link3;
  buffers[0] = data->buffer1;
  buffers[1] = data->buffer2;
  buffers[2] = data->buffer3;

  data->arrival_count++;

  new_packet = (Packet_Ptr) xmalloc(sizeof(Packet));
  new_packet->arrive_time = simulation_run_get_time(simulation_run);
  new_packet->source_id = source->switch_id;
  new_packet->destination_id = source->switch_id;
  new_packet->service_time = 0.0;
  new_packet->status = WAITING;

  if(server_state(links[source_index]) == BUSY) {
    fifoqueue_put(buffers[source_index], (void *) new_packet);
  } else {
    start_transmission_on_link(simulation_run, new_packet, links[source_index]);
  }

  schedule_packet_arrival_event(simulation_run,
			simulation_run_get_time(simulation_run) +
				exponential_generator(1.0 / source->arrival_rate),
			source);
}



