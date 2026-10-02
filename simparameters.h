
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

#define PACKET_ARRIVAL_RATE1 750 /* packets per second */
#define PACKET_ARRIVAL_RATE23 500 /* packets per second */
#define PACKET_LENGTH 1e3 /* bits */
#define LINK_BIT_RATE1 1e6 /* bits per second */
#define LINK_BIT_RATE23 1e6 /* bits per second */
#define RUNLENGTH 10e6 /* packets */

/* Probability of routing a Link 1 packet to Link 2 */
#define P12_LIST 0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1

/* Comma separated list of random seeds to run. */
#define RANDOM_SEED_LIST 400315188, 400381481, 400385757

#define PACKET_XMT_TIME1 ((double) PACKET_LENGTH/LINK_BIT_RATE1)
#define PACKET_XMT_TIME23 ((double) PACKET_LENGTH/LINK_BIT_RATE23)
#define BLIPRATE (RUNLENGTH/1000)

/******************************************************************************/

#endif /* simparameters.h */



