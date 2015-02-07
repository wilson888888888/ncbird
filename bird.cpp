#include <ncurses.h>
#include <unistd.h>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <iostream>

struct wall{
	int x;
	int hole;
};

int main(){
	initscr();
	noecho();
	curs_set(FALSE);
 	cbreak();
	nodelay(stdscr, TRUE);
	timeout(100);
	srand(time(NULL));
	int y = 0;
	int v = 0;
	int ch;
	std::vector<wall> walls;
	int count = 0;
	int max_x = 0, max_y = 0;
	getmaxyx(stdscr, max_y, max_x);
	bool lost = false;
	int score = 0;
	bool color = has_colors();
	int hsize = 6;
	if (color){
		start_color();
		init_pair(1, COLOR_YELLOW, COLOR_BLACK);
		init_pair(2, COLOR_GREEN, COLOR_BLACK);
	}
	while (!lost){
		if (score == 15)
			hsize += -2;
		if (score == 30)
			hsize += -2;
		count ++;
		if (count % 50 == 0){
			walls.push_back(wall());
			walls[walls.size() - 1].x = max_x + 1;
			walls[walls.size() - 1].hole = rand() % max_y;
		}
		for (int i = 0; i < walls.size(); i++){
			walls[i].x += -1;
			for (int j = 0; j < max_y; j++){
				if (abs(j - walls[i].hole) > hsize/2){
					if (color)
						attron(COLOR_PAIR(2));
					mvprintw(j, walls[i].x, "#");
					if (color)
						attroff(COLOR_PAIR(2));
				}
			}
			if (walls[i].x < 0){
				walls.erase(walls.begin() + i);
				score++;
			}
			if (walls[i].x == 10 && abs(walls[i].hole - y) > hsize/2)
				lost = true;
		}
		y += v;
		v += 1;
		ch = getch();
		if (ch == ' ')
			v = -2;
		clear();
		if (color)
			attron(COLOR_PAIR(1));
		mvprintw(y, 10, ">");
		if (color)
			attroff(COLOR_PAIR(1));
		std::stringstream strs;
		strs << "Score:" << score;
		std::string temp_str = strs.str();
		mvprintw(1, max_x - 12,(char*) temp_str.c_str() );
		refresh;
	}
	timeout(-1);
	erase();
	mvprintw(max_y / 2, max_x / 2, "GAME OVER");
	mvprintw(max_y / 2 + 2, max_x / 2, "Score: %i", score);
	refresh;
	getch();
	endwin();
}
