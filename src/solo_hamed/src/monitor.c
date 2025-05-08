#include "monitor.h"

static u16int *video_memory = (u16int *)0xB8000;
static u8int cursor_x = 0;
static u8int cursor_y = 0;

static void move_cursor()
{
   u16int cursorLocation = cursor_y * 80 + cursor_x;
   outb(0x3D4, 14);                  
   outb(0x3D5, cursorLocation >> 8); 
   outb(0x3D4, 15);                  
   outb(0x3D5, cursorLocation);      
}

static void scroll()
{
   u8int attributeByte = (0 << 4) | (15 & 0x0F);
   u16int blank = 0x20 | (attributeByte << 8);

   if(cursor_y >= 25)
   {
       int i;
       for (i = 0*80; i < 24*80; i++)
       {
           video_memory[i] = video_memory[i+80];
       }

       for (i = 24*80; i < 25*80; i++)
       {
           video_memory[i] = blank;
       }
       cursor_y = 24;
   }
}

void monitor_put(char c)
{
   u8int attributeByte = (0 << 4) | (15 & 0x0F);
   u16int attribute = attributeByte << 8;

   if (c == 0x08 && cursor_x)
       cursor_x--;
   else if (c == 0x09)
       cursor_x = (cursor_x+8) & ~(8-1);
   else if (c == '\r')
       cursor_x = 0;
   else if (c == '\n')
   {
       cursor_x = 0;
       cursor_y++;
   }
   else if(c >= ' ')
   {
       video_memory[cursor_y*80 + cursor_x] = c | attribute;
       cursor_x++;
   }

   if (cursor_x >= 80)
   {
       cursor_x = 0;
       cursor_y++;
   }

   scroll();
   move_cursor();
}

void monitor_clear()
{
   u8int attributeByte = (0 << 4) | (15 & 0x0F);
   u16int blank = 0x20 | (attributeByte << 8);

   int i;
   for (i = 0; i < 80*25; i++)
   {
       video_memory[i] = blank;
   }

   cursor_x = 0;
   cursor_y = 0;
   move_cursor();
}

void monitor_write(char *c)
{
   int i = 0;
   while (c[i])
   {
       monitor_put(c[i++]);
   }
}

void monitor_write_dec(u32int n)
{
   if (n == 0)
   {
       monitor_put('0');
       return;
   }

   u32int acc = n;
   char c[32];
   int i = 0;

   while (acc > 0 && i < 31)
   {
       c[i] = '0' + acc % 10;
       acc /= 10;
       i++;
   }
   c[i] = 0;

   char temp;
   int start = 0;
   int end = i - 1;
   while (start < end)
   {
       temp = c[start];
       c[start] = c[end];
       c[end] = temp;
       start++;
       end--;
   }
   monitor_write(c);
}

void monitor_write_hex(u32int n)
{
   if (n == 0)
   {
       monitor_write("0x0");
       return;
   }

   int i = 0;
   char hex_digits[16] = "0123456789ABCDEF";
   char output[11];

   output[0] = '0';
   output[1] = 'x';
   i = 2;

   int start = 28;

   while (start > 0 && ((n >> start) & 0xF) == 0)
   {
       start -= 4;
   }

   while (start >= 0 && i < 10)
   {
       output[i++] = hex_digits[(n >> start) & 0xF];
       start -= 4;
   }

   output[i] = 0;
   monitor_write(output);
}
