#ifndef WORDGAME_H
#define WORDGAME_H

void shuffle_word(const char* word, char* shuffled);
int play_round(const char* original_word);
void collect_words();
void sort_highscores();
void show_highscores();
void start_word_game();


#endif
