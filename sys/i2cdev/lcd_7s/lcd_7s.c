/*
  
  lcd_7s.c

  This tools has been designed to let the user display several
  messages on the LCD screen of Seven Solutions products.
  The arguments parsing provide a flexible way to display various
  infos.

  Author: Benoit Rat - 2016

  Linux U8glib Example

  Universal 8bit Graphics Library
  
  Copyright (c) 2013, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  

*/

#include "../lcd_7s/logo_7s.h"

#include <unistd.h> //getopt()
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "u8g_linux.h"
u8g_t u8g;


#define SCREEN_W 128
#define SCREEN_H 64

#ifndef __GIT_VER__
#define __GIT_VER__ "x.x"
#endif

#ifndef __GIT_USR__
#define __GIT_USR__ "?"
#endif


void draw_img(const u8g_uint_t *img_bitmap, u8g_uint_t w, u8g_uint_t h)
{
	u8g_uint_t x,y;
	u8g_uint_t val;
	int tmp;
	for(y=0;y<h;y++) {
		for(x=0;x<w;x++)
		{
			tmp=(y*w+x);
			val=img_bitmap[tmp/8];
			if(val>0 && (val & (1 << (tmp%8))))
			{
				u8g_DrawPixel(&u8g,x,y);
			}
		}
	}
}

void draw_url(void)
{

}

void help(const char *progname)
{
	fprintf(stderr, "Usage: %s [OPTIONS] <i2cbus-id> [gpio]\n", progname);
	fprintf(stderr, "\nWhere OPTIONS is\n"
			"\t-t <txt> : Title of the menu (hide Logo)\n"
			"\t-l       : Display logo (default)\n"
			"\t-L       : Hide logo\n"
			"\t-1 <txt> : Display text in item 1\n"
			"\t-2 <txt> : Display text in item 2\n"
			"\t-3 <txt> : Display text in item 3\n"
			"\t-4 <txt> : Display text in item 4\n"
			"\t-5 <txt> : Display text in item 5\n"
			"\t-b <txt> : Display bottom text\n"
			"\t-B: Display border\n"
			"\t-v: Return the version and copyright message\n"
			"\t-h: Show this little help\n"
			"\n");
	exit(1);
}


int main(int argc, char **argv)
{
	uint8_t ok = 255;
	uint8_t i2c_bus;
	uint8_t rst_gpio=U8G_PIN_NONE;
	char opt=0; int optind=1;
	const char *bottom=0,*title=0;
	const char *items[5]={0};
	uint8_t flogo=1,fborder=0, nitems=0,yitem0=16;
	int i,x,y;
	const char *progname=argv[0];

	//Parse the specific arguments
	while ((opt = getopt(argc, argv, "1:2:3:4:5:t:b:BlLh?v")) != 0xFF)
	{
		optind++;
		switch (opt)
		{
		case 'l': flogo=1; break;
		case 'L': flogo=0; break;
		case '1': items[0]=argv[optind++]; nitems++; break;
		case '2': items[1]=argv[optind++]; nitems++; break;
		case '3': items[2]=argv[optind++]; nitems++; break;
		case '4': items[3]=argv[optind++]; nitems++; break;
		case '5': items[4]=argv[optind++]; nitems++; break;
		case 't': title=argv[optind++]; flogo=0; break;
		case 'b': bottom=argv[optind++]; break;
		case 'B': fborder=1; break;
		case 'h':
		case '?':
			help(progname);
			break;
		case 'v':
			printf("%s is based on u8glib\n"
					"Source code can be found at https://github.com/neub/u8glib\n"
					,argv[0]);
			printf("version: %s (%s)\nCompiled at %s %s\n",
				__GIT_VER__, __GIT_USR__, __DATE__, __TIME__);
			printf("\n"
					"Universal 8bit Graphics Library\n"
					"Copyright (c) 2013, olikraus@gmail.com\n"
					"All rights reserved.\n");

			exit(0);
		default:
			fprintf(stderr,"find: illegal option %c (%x)\n", opt,opt);
			help(progname);
			break;
		}
	}
	optind--;
	argc-=optind;
	argv+=optind;

	if (argc <= 1)
	{
		fprintf(stderr,"Missing arguments\n");
		help(progname);
	}

	i2c_bus = atoi(argv[1]);
	if(argc>2) rst_gpio = atoi(argv[2]);

	ok = u8g_InitI2CRst(&u8g, &u8g_dev_ssd1309_128x64_i2c, i2c_bus,rst_gpio);
	if (ok == 255) {
		fprintf(stderr, "No display driver selected (see src/linux/common/init.c)\n");
		exit(2);
	}
	if (!ok) exit(3);


	u8g_FirstPage(&u8g);
	do
	{
		//First Draw Logo
		if(flogo)
		{
			draw_img(logo_7s_bits,logo_7s_width,logo_7s_height);
			yitem0=40;
		}
		if(nitems==0 && flogo)
		{
			u8g_SetFont(&u8g, u8g_font_7x13);
			u8g_DrawStr(&u8g, 6,42,"Solutions");
		}
		else
		{
			if(title)
			{
				u8g_SetFont(&u8g, u8g_font_5x7);
				x=64-(((5*strlen(title))/2)+1);
				y=10;
				u8g_DrawStr(&u8g, x,y,title);
				y+=2;
				u8g_DrawLine(&u8g,x-2,y,x+5*strlen(title),y);
				yitem0=y+12;
			}


			u8g_SetFont(&u8g, u8g_font_5x7);
			y=yitem0;
			for(i=0;i<5;i++)
			{
				if(items[i])
				{
					if(strcmp(items[i],"\\n")==0) y-=6;
					else u8g_DrawStr(&u8g, 6,y,items[i]);
				}
				//printf("#%d > %d (%s)\n",i,y,items[i]);
				y+=9;
			}
		}

		if(bottom)
		{
			u8g_SetFont(&u8g, u8g_font_4x6);
			x=64-(((4*strlen(bottom))/2)+1);
			u8g_DrawStr(&u8g, x,fborder?62:64,bottom);
		}

		if(fborder)
		{
			u8g_DrawFrame(&u8g,0,0,SCREEN_W,SCREEN_H);
		}

	} while ( u8g_NextPage(&u8g) );

	return 0;
}

