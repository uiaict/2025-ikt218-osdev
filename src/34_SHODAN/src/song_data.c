#include "song/song.h"
#include "song/song_data.h"
#include "song/frequencies.h"


Note epic_melody[] = {
    {E4, 500}, {G4, 500}, {B4, 300}, {E5, 200}, 
    {D5, 200}, {B4, 300}, {G4, 500}, {B4, 300}, 
    {E5, 200}, {D5, 200}, {B4, 300}, {G4, 500}, 
    {B4, 300}, {E5, 200}, {G5, 200}, {E5, 300}, 
    {D5, 200}, {B4, 300}, {G4, 500}, {E4, 500}, 
    {G4, 500}, {B4, 300}, {E5, 200}, {D5, 200}, 
    {B4, 300}, {G4, 500}, {B4, 300}, {E5, 200}, 
    {D5, 200}, {B4, 300}, {G4, 500}, {B4, 300}, 
    {E5, 200}, {G5, 200}, {E5, 300}, {D5, 200}, 
    {B4, 300}, {G4, 500}, 
    {R, 500}
};


const size_t epic_melody_length = sizeof(epic_melody) / sizeof(Note);