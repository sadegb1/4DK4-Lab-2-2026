
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
#include "trace.h"
#include "main.h"
#include "output.h"
#include "packet_transmission.h"

/******************************************************************************/

/*
 * This function will schedule the end of a packet transmission at a time given
 * by event_time. At that time the function "end_packet_transmission" (defined
 * in packet_transmissionl.c) is executed. A packet object is attached to the
 * event and is recovered in end_packet_transmission.c.
 */

long
schedule_end_packet_transmission_event(Simulation_Run_Ptr simulation_run,
				       double event_time,
				       Server_Ptr link)
{
  Event event;

  event.description = "Packet Xmt End";
  event.function = end_packet_transmission_event;
  event.attachment = (void *) link;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

/******************************************************************************/

/*
 * This is the event function which is executed when the end of a packet
 * transmission event occurs. It updates its collected data then checks to see
 * if there are other packets waiting in the fifo queue. If that is the case it
 * starts the transmission of the next packet.
 */

void
end_packet_transmission_event(Simulation_Run_Ptr simulation_run, void * link)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr this_packet, next_packet;
  Server_Ptr completed_link;
  Server_Ptr downstream_link;
  Fifoqueue_Ptr downstream_buffer;
  Fifoqueue_Ptr waiting_buffer;
  double current_packet_delay;
  int source_index;

  TRACE(printf("End Of Packet.\n"););

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);
  completed_link = (Server_Ptr) link;

  /* 
   * Packet transmission is finished. Take the packet off the data link.
   */

  this_packet = (Packet_Ptr) server_get(completed_link);

  if(completed_link == data->link1) {
    if(uniform_generator() < data->p12) {
      this_packet->destination_id = 2;
      downstream_link = data->link2;
      downstream_buffer = data->buffer2;
    } else {
      this_packet->destination_id = 3;
      downstream_link = data->link3;
      downstream_buffer = data->buffer3;
    }

    if(server_state(downstream_link) == BUSY) {
      fifoqueue_put(downstream_buffer, (void *) this_packet);
    } else {
      start_transmission_on_link(simulation_run, this_packet, downstream_link);
    }

    if(fifoqueue_size(data->buffer1) > 0) {
      next_packet = (Packet_Ptr) fifoqueue_get(data->buffer1);
      start_transmission_on_link(simulation_run, next_packet, data->link1);
    }

    return;
  }

  waiting_buffer = completed_link == data->link2
      ? data->buffer2
      : data->buffer3;
  source_index = this_packet->source_id - 1;
  current_packet_delay = simulation_run_get_time(simulation_run) -
      this_packet->arrive_time;

  data->number_of_packets_processed++;
  data->packets_processed_by_source[source_index]++;
  data->accumulated_delay += current_packet_delay;
  data->accumulated_delay_by_source[source_index] += current_packet_delay;

  if(current_packet_delay > 0.020) {
    data->delay_over_20_counter++;
  }

  xfree((void *) this_packet);

  if(fifoqueue_size(waiting_buffer) > 0) {
    next_packet = (Packet_Ptr) fifoqueue_get(waiting_buffer);
    start_transmission_on_link(simulation_run, next_packet, completed_link);
  }

}

/*
 * This function ititiates the transmission of the packet passed to the
 * function. This is done by placing the packet in the server. The packet
 * transmission end event for this packet is then scheduled.
 */

void
start_transmission_on_link(Simulation_Run_Ptr simulation_run, 
			   Packet_Ptr this_packet,
			   Server_Ptr link)
{
  TRACE(printf("Start Of Packet.\n");)

  server_put(link, (void*) this_packet);
  this_packet->status = XMTTING;

  Simulation_Run_Data_Ptr data =
    (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);
  this_packet->service_time = get_packet_transmission_time(data, link);

  /* Schedule the end of packet transmission event. */
  schedule_end_packet_transmission_event(simulation_run,
	 simulation_run_get_time(simulation_run) + this_packet->service_time,
	 (void *) link);
}

/*
 * Get a packet transmission time. For now it is a fixed value defined in
 * simparameters.h
 */

double
get_packet_transmission_time(Simulation_Run_Data_Ptr data, Server_Ptr link)
{

  return link == data->link1 ? PACKET_XMT_TIME1 : PACKET_XMT_TIME23;
}


