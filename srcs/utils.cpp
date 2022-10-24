// #include <cstring>
// #include <iostream>

// using namespace std;
#include "../incs/utils.hpp"

int show_error(string s)
{
    std::cerr << s;
    return 1;
}

void change_events(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter,
                    uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
    struct kevent temp_event;

    EV_SET(&temp_event, ident, filter, flags, fflags, data, udata);
    change_list.push_back(temp_event);
}
