
/*
 * 
 * Simulation of A Single Server Queueing System
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

#ifndef _SIMPARAMETERS_H_
#define _SIMPARAMETERS_H_

/******************************************************************************/

#define MEAN_ARRIVAL_RATE_MIN 0 /* packets per second */
#define MEAN_ARRIVAL_RATE_MAX 20 /* packets per second */
#define MEAN_ARRIVAL_RATE_STEP 1 /* packets per second */
#define DATA_MEAN_SERVICE_TIME 40e-3 /* seconds */
#define VOICE_CODEC_BIT_RATE 64e3 /* G.711 bits per second */
#define VOICE_PACKET_INTERVAL 20e-3 /* seconds */
#define PACKET_HEADER_BYTES 62
#define LINK_BIT_RATE 1e6 /* bits per second */
#define RUNLENGTH 100000 /* completed packets per run */

/* Comma separated list of random seeds to run. */
#define RANDOM_SEED_LIST 333333, 444444, 555555, 666666, 777777, 888888, 999999

#define VOICE_PACKET_PAYLOAD_BITS (VOICE_CODEC_BIT_RATE * VOICE_PACKET_INTERVAL)
#define VOICE_PACKET_BITS (VOICE_PACKET_PAYLOAD_BITS + PACKET_HEADER_BYTES * 8)
#define VOICE_PACKET_XMT_TIME (VOICE_PACKET_BITS / LINK_BIT_RATE)
#define BLIPRATE (RUNLENGTH/1000)

/******************************************************************************/

#endif /* simparameters.h */



