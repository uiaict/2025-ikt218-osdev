/**
 * port_io.c
 * I/O port operations for x86.
 */

 #include "port_io.h"

 uint8_t inb(uint16_t port) {
     uint8_t result;
     __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
     return result;
 }
 
 void outb(uint16_t port, uint8_t value) {
     __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
 }
 
 uint16_t inw(uint16_t port) {
     uint16_t result;
     __asm__ volatile("inw %1, %0" : "=a"(result) : "Nd"(port));
     return result;
 }
 
 void outw(uint16_t port, uint16_t value) {
     __asm__ volatile("outw %0, %1" : : "a"(value), "Nd"(port));
 }
 
 uint32_t inl(uint16_t port) {
     uint32_t result;
     __asm__ volatile("inl %1, %0" : "=a"(result) : "Nd"(port));
     return result;
 }
 
 void outl(uint16_t port, uint32_t value) {
     __asm__ volatile("outl %0, %1" : : "a"(value), "Nd"(port));
 }
 