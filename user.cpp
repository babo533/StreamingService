#include "user.h"
#include <algorithm>
#include <iostream>
using namespace std;


void User::addToHistory(CID_T contentID)
{
    history.push_back(contentID);
}


bool User::haveWatched(CID_T contentID)
{
    auto my_iterator = std::find(history.begin(), history.end(), contentID);
    return my_iterator != history.end();
}

