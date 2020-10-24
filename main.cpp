#include <cstdio>
#include <windows.h>

void remove_one() {
    keybd_event('\b', 0, 0, 0);
    Sleep(1);
    keybd_event('\b', 0, 2, 0);
}

void alt(const char *v) {
    keybd_event(VK_MENU, MapVirtualKey(VK_MENU, 0), 0, 0);
    Sleep(1);
    for (int i=0; v[i]; ++i) {
        int key = 96 + (v[i] & 15);
        keybd_event(key, MapVirtualKey(key, 0), 0, 0);
        Sleep(1);
        keybd_event(key, MapVirtualKey(key, 0), 2, 0);
        Sleep(1);
    }
    keybd_event(VK_MENU, MapVirtualKey(VK_MENU, 0), 2, 0);
}

int process(int state, int cnt) {
    if (state == 'A') {
        remove_one();
        if (cnt & 1) {
            alt("226");
        } else {
            alt("224");
        }
    } else if (state == 'C') {
        remove_one();
        alt("231");
    } else if (state == 'E') {
        const char *es[4] = {"233", "232", "235", "234"};
        remove_one();
        alt(es[cnt & 3]);
    } else if (state == 'I') {
        remove_one();
        if (cnt & 1) {
            alt("239");
        } else {
            alt("238");
        }
    } else if (state == 'O') {
        remove_one();
        alt("244");
    } else if (state == 512) {
        if (cnt & 1) {
            remove_one();
            keybd_event('O', 0, 0, 0);
            Sleep(1);
            keybd_event('O', 0, 2, 0);
            alt("235");
            return 1;
        } else {
            remove_one();
            remove_one();
            alt("156");
        }
    } else if (state == 'U') {
        const char *us[3] = {"249", "251", "252"};
        remove_one();
        alt(us[cnt % 3]);
    }
    return 0;
}

int precheck(int last) {
	return last == 'A' || last == 'C' || last == 'E' || last == 'I' || last == 'O' || last == 512 || last == 'U';
}

inline void substitute(int &v, const int value, int &no) {
    if (no) {
        --no;
    } else {
        v = value;
    }
}

int main() {
    int last, cnt = 0, no = 0;
    while (1) {
        for (int i=0; i<255; ++i) {
            if (GetAsyncKeyState(i) & 1) {
                if (i != 16 && i != 164 && i != 165 && i != 18 && !('a' <= i && i <= 'i') && i != 186) {
                    if (!no) cnt = 0;
                    if ((GetAsyncKeyState(16) & 0x8000) && 'A' <= i && i <= 'Z') {
                        substitute(last, i | 256, no);
                    } else if (last == 'O' && i == 'E') {
                        substitute(last, 512, no);
                    } else {
                        substitute(last, i, no);
                    }
                } else if (i == 186 && precheck(last)) {
                    ++no;
                    remove_one();
                    if (++cnt >= 2) {
                        Sleep(1);
                        no += process(last, cnt - 2);
                    }
                }
            }
        }
    }
    return 0;
}
