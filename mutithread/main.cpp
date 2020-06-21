#include "threadcase.h"
#include <stdlib.h>

using namespace std;
int main()
{
    int x = 10;
    string str("thread test");
    thread threadobj(p_thead, ref(str), ref(x));
    //this_thread::sleep_for(10);
    return 0;
}