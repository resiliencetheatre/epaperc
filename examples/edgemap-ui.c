/*
#
# edgemap-ui adopted from [1] 
#
# [1] https://github.com/waveshareteam/e-Paper/tree/master/RaspberryPi_JetsonNano/c
#
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
*/

#include "EPD_Test.h"
#include "EPD_2in13_V3.h"
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>

//int EPD_2in13_V3_test(void)
int edgemap_bootmsg(void)
{
    int n;
    struct ifreq ifr;
    char array[] = "wlan0";
    n = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name , array , IFNAMSIZ - 1);
    ioctl(n, SIOCGIFADDR, &ifr);
    close(n);
    
    if(DEV_Module_Init()!=0){
        return -1;
    }
    EPD_2in13_V3_Init();
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_2in13_V3_WIDTH % 8 == 0)? (EPD_2in13_V3_WIDTH / 8 ): (EPD_2in13_V3_WIDTH / 8 + 1)) * EPD_2in13_V3_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        Debug("Failed to apply for black memory...\r\n");
        return -1;
    }
    
    // Show boot info and qrcode.bmp
	Paint_NewImage(BlackImage, EPD_2in13_V3_WIDTH, EPD_2in13_V3_HEIGHT, 90, WHITE);
    Paint_SelectImage(BlackImage);
    GUI_ReadBmp("/mnt/pic/qrcode.bmp", 0, 0);
    Paint_DrawString_EN(5, 5, "Edgemap", &Font20,WHITE, BLACK );
    Paint_DrawString_EN(5, 25, inet_ntoa(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr), &Font12,WHITE, BLACK );
    Paint_DrawString_EN(5, 45, "Full world OSM", &Font12,WHITE, BLACK );
    Paint_DrawString_EN(5, 60, "Full world terrain", &Font12,WHITE, BLACK );
    Paint_DrawString_EN(5, 75, "Imagery:", &Font12,WHITE, BLACK );
    Paint_DrawString_EN(5, 90, " Kiev,Sevast,KWI City", &Font12,WHITE, BLACK );
    Paint_DrawString_EN(20, 110, "This page erases after 30 s.", &Font12,WHITE, BLACK );
    EPD_2in13_V3_Display(BlackImage);
    DEV_Delay_ms(30000);
    free(BlackImage);

    // Turn display off
	EPD_2in13_V3_Init();
    EPD_2in13_V3_Clear();
    EPD_2in13_V3_Sleep();
    BlackImage = NULL;
    DEV_Delay_ms(2000); //important, at least 2s
    DEV_Module_Exit();
    return 0;
}

int edgemap_message(char *message_text,int delay)
{
    if(DEV_Module_Init()!=0){
        return -1;
    }
    EPD_2in13_V3_Init();
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_2in13_V3_WIDTH % 8 == 0)? (EPD_2in13_V3_WIDTH / 8 ): (EPD_2in13_V3_WIDTH / 8 + 1)) * EPD_2in13_V3_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        Debug("Failed to apply for black memory...\r\n");
        return -1;
    }
    
    EPD_2in13_V3_Clear();
    // Show boot info and qrcode.bmp
	Paint_NewImage(BlackImage, EPD_2in13_V3_WIDTH, EPD_2in13_V3_HEIGHT, 90, WHITE);
    Paint_SelectImage(BlackImage);
    GUI_ReadBmp("/opt/eink/pic/white.bmp", 0, 0);
    Paint_DrawString_EN(5, 20, message_text, &Font12,WHITE,BLACK);
    EPD_2in13_V3_Display(BlackImage);
    if ( delay > 0 )
        DEV_Delay_ms(delay); // TODO: Together with clear option?
        
    free(BlackImage);

    // Turn display off
    if ( delay > 0 ) 
    {
        EPD_2in13_V3_Init();
        EPD_2in13_V3_Clear();
    }
    EPD_2in13_V3_Sleep();
    BlackImage = NULL;
    DEV_Delay_ms(2000); //important, at least 2s
    DEV_Module_Exit();
    return 0;
}
