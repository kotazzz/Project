#include <thread> //threads (WOW!)
#include <string> //int to string
#include <iostream>
#include <io.h> //_setmode (u16)
#include <fcntl.h>  //_setmode (u16) - ..._O_U16TEXT);
#include <Windows.h> //winapi
#define ptw pseudo_tab_write
using namespace std;
enum po_code {
	top_left, top_center, top_right,
	vertical, horizontal,
	vertical_to_left, center, vertical_to_right,
	bottom_left, bottom_center, bottom_right
	//┌─┬┐
	//│ ││
	//├─┼┤
	//└─┴┘
};
enum po_type {
	single, twice,
	twice_vertical, twice_horizontal
};



void pseudo_tab_write(po_code code, po_type type) {
	//┌─┬┐
	//│ ││
	//├─┼┤
	//└─┴┘
	wchar_t chars_single[] = L"┌┬┐│─├┼┤└┴┘";
	wchar_t chars_twice[] = L"╔╦╗║═╠╬╣╚╩╝";
	wchar_t chars_twice_vertical[] = L"╓╥╖║─╟╫╢╙╨╜";
	wchar_t chars_twice_horizontal[] = L"╒╤╕│═╞╪╡╘╧╛";

	switch (type)
	{
	case single:
		wcout << chars_single[(int)code];
		break;
	case twice:
		wcout << chars_twice[(int)code];
		break;
	case twice_vertical:
		wcout << chars_twice_vertical[(int)code];
		break;
	case twice_horizontal:;
		wcout << chars_twice_horizontal[(int)code];
		break;
	}
}
void set_console_size(int cols = 80, int lines = 30) {
	if (cols < 10)cols = 10;
	if (lines < 1)cols = 1;
	string set_system = "mode con cols=" + to_string(cols) + "lines=" + to_string(lines);
	system(set_system.c_str());
}
void set_cursor_position(int x, int y) {
	COORD cursor = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
}

void initialize() {
	setlocale(LC_ALL, "ru");
	_setmode(_fileno(stdout), _O_U16TEXT);
}

enum State {
	LEFT, RIGHT, UP, DOWN, ESCAPE, NONE
};
State state;
void threadFunction()
{

	HANDLE s_in = GetStdHandle(STD_INPUT_HANDLE);
	DWORD dr;
	INPUT_RECORD  rec;
	for (;;) {
		ReadConsoleInput(s_in, &rec, sizeof(INPUT_RECORD), &dr);
		FlushConsoleInputBuffer(s_in);

		switch (rec.Event.KeyEvent.wVirtualKeyCode) {
		case VK_LEFT:
			state = LEFT;
			break;
		case VK_RIGHT:
			state = RIGHT;
			break;
		case VK_UP:
			state = UP;
			break;
		case VK_DOWN:
			state = DOWN;
			break;
		case VK_ESCAPE:
			state = ESCAPE;
			break;
		}
	}


}

void threadFunction2() {
	set_console_size(10, 5);
	set_cursor_position(0, 0);
	ptw(po_code::top_left, po_type::twice);
	for (size_t i = 0; i < 8; i++)
		ptw(po_code::horizontal, po_type::twice);
	ptw(po_code::top_right, po_type::twice);
	for (size_t i = 0; i < 3; i++)
	{
		ptw(po_code::vertical, po_type::twice);
		for (size_t i = 0; i < 8; i++)
			wcout << L" ";
		ptw(po_code::vertical, po_type::twice);
	}
	ptw(po_code::bottom_left, po_type::twice);
	for (size_t i = 0; i < 8; i++)
		ptw(po_code::horizontal, po_type::twice);
	ptw(po_code::bottom_right, po_type::twice);
	set_cursor_position(1, 1);
	wcout << "";
	/********************************************************************************/
	int state_now;
	while (1) {
		if (state == ESCAPE) exit(0);
		state = NONE;
		Sleep(1);
	}
}

int main()
{
	initialize();
	std::thread thr(threadFunction);
	std::thread thr2(threadFunction2);
	thr.join();
	thr2.join();
	return 0;
}
/*


*/