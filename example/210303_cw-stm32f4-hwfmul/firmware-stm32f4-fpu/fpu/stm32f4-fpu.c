/*
    This file is part of the ChipWhisperer Example Targets
    Copyright (C) 2012-2017 NewAE Technology Inc.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "hal.h"
#include "simpleserial.h"

uint8_t get_pt(uint8_t* pt)
{
    float a[4],b[4];
    float c[4];
	uint8_t *pc = (uint8_t*)c;
    int i;

    for(i = 0 ; i < 4 ;i++) {
        //a[i] = (float)rand()/(float)(RAND_MAX/2.0);
        //b[i] = (float)rand()/(float)(RAND_MAX/2.0);
        a[i] = pt[i] * 2.5 + 1;
        b[i] = pt[i] * (-2.5) - 1;
    }

    trigger_high();
    
    for(i = 0 ; i < 4 ;i++) {
        c[i] = a[i] * b[i];
    }
    
    trigger_low();

    for(i = 0 ; i < 16 ;i++) {
        pt[i] = pc[i];
	}
    
	simpleserial_put('r', 16, pt);
	return 0x00;
}

uint8_t reset(uint8_t* x)
{
	return 0x00;
}

int main(void)
{
    platform_init();
	init_uart();	
	trigger_setup();
    init_fpu();

 	/* Uncomment this to get a HELLO message for debug */
	/*
	putch('h');
	putch('e');
	putch('l');
	putch('l');
	putch('o');
	putch('\n');
	*/

	simpleserial_init();
    #if SS_VER == SS_VER_2_0
	;
    #else
    simpleserial_addcmd('p', 16,  get_pt);
    simpleserial_addcmd('x',  0,   reset);
    #endif
    while(1)
        simpleserial_get();
}
