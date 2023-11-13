#include "strmsrv.h"
#include <iostream>
#include <fstream>
using namespace std;


StreamService::StreamService()

{
    // Update as needed
    cUser_ = NULL;  //initializes current user
}


StreamService::~StreamService()
{
    for (std::vector<User*>::iterator u = users_.begin(); u != users_.end(); u++) {
      delete (*u);  //deallocates user
    }
    for (std::vector<Content*>::iterator c = content_.begin(); c != content_.end(); c++) {
      delete (*c);  //deallocates content
    }
}


void StreamService::readAndParseData(std::istream& is, Parser& p)
{
    p.parse(is, content_, users_);
    cout << "Read " << content_.size() << " content items." << endl;
    cout << "Read " << users_.size() << " users." << endl;
}

void StreamService::userLogin(const std::string& uname)
{
    if (cUser_ != NULL) {  //checks if a user is currently logged in
        throw std::runtime_error("Please Logout First");
    }
    int index = getUserIndexByName(uname);
    if (index == -1) {  //if the username does not exist
        throw std::invalid_argument("Not a Valid User");
    }
    else {
        cUser_ = users_[index];
    }
}


void StreamService::userLogout()
{
    cUser_ = nullptr;
}


std::vector<CID_T> StreamService::searchContent(const std::string& partial) const
{
    std::vector<CID_T> results;
    for(size_t i = 0; i < content_.size(); i++){
        // TO DO - modify this to also push back when the string 'partial'
        //  is contained in the name of the current content. Lookup the string
        //  class documentation to find an appropriate function that can be used
        //  to do this simply.
        if(partial.compare("*") == 0){
          results.push_back(content_[i]->id());
        }
        else {
          size_t index = content_[i]->name().find(partial);
          if (index != string::npos) {   //if the partial string was found
            results.push_back(content_[i]->id());
          }
        }
    }
    return results;
}

std::vector<CID_T> StreamService::getUserHistory() const
{
    throwIfNoCurrentUser();
    return cUser_->history;
}


void StreamService::watch(CID_T contentID)
{
  throwIfNoCurrentUser();
  if (!isValidContentID(contentID)){  //checks for valid content id
    throw std::range_error("Invalid Content ID");
  }
  if (content_[contentID]->rating() > cUser_->ratingLimit) {  //checks if rating is above ratinglimit
    throw RatingLimitError("Rating is Not Appropriate");
  }
  else {
    content_[contentID]->addViewer(cUser_->uname);  //or else adds content to viewer's history
    if (!cUser_->haveWatched(contentID)) {
      cUser_->addToHistory(contentID);
    }
  }
}

void StreamService::reviewShow(CID_T contentID, int numStars)
{
  throwIfNoCurrentUser();
  if (!isValidContentID(contentID)) {  //checks for valid content id
    throw std::invalid_argument("Invalid Content ID");
  } 
  if (numStars > 5 || numStars < 0) {  //checks for review range 0-5
    throw ReviewRangeError("Review Out of Range");
  }
  content_[contentID]->review(numStars);
}


CID_T StreamService::suggestBestSimilarContent(CID_T contentID) const
{
    // Change this when you are ready, but for now it is a 
    // dummy implementation
    throwIfNoCurrentUser();
    if (!isValidContentID(contentID)) {
      throw std::invalid_argument("Content ID is not valid");
    }
    vector<User*> u;  //this will have all the users that watched the same show
    for (size_t i = 0; i < users_.size(); i++) {
      //checks if the user has watched and the user is not the current user
      if (users_[i]->haveWatched(contentID) && users_[i] != cUser_) {
        u.push_back(users_[i]);
      }
    }

    //this array stores the amount of times that each of the other contents 
    // have been watched by the users in u 
    int size = content_.size();
    int watched[size];
    //initializes all the values to 0
    for (int i = 0; i <  size; i++) {
      watched[i] = 0;
    }
    //goes through users in u
    for (size_t i = 0; i < u.size(); i++) {
      //checks if the user has watched any of the shows
      for (int c = 0; c < size; c++) {
        if (u[i]->haveWatched(c)) {
          //increments the amount of times the other users
          // have viewed the content
          watched[c] += 1;
        }
      }
    }

    CID_T ans = -1;

    //goes through the contents to find the largest
    for (int i = 0; i < size; i++) {
      //checks if the user has not watched too
      if (ans == -1) {
        if (!cUser_->haveWatched(i) && i != contentID) {
          ans = i;
        }
      }
      else {
        if (watched[i] > watched[ans] && !cUser_->haveWatched(i) && i != contentID) {
          ans = i;
        }
      }
    }
    return ans;
}


void StreamService::displayContentInfo(CID_T contentID) const
{
    // Do not alter this
    if(! isValidContentID(contentID)){
        throw std::invalid_argument("Watch: invalid contentID");
    }

    // Call the display abitlity of the appropriate content object
    for (size_t i = 0; i < content_.size(); i++) {
      if (content_[i]->id() == contentID) {
        content_[i]->display(cout);  //calls all the display methods
      }
    }
}


bool StreamService::isValidContentID(CID_T contentID) const
{
    return (contentID >= 0) && (contentID < (int) content_.size());
}


void StreamService::throwIfNoCurrentUser() const
{
    if(cUser_ == NULL){
        throw UserNotLoggedInError("No user is logged in");
    }
}

int StreamService::getUserIndexByName(const std::string& uname) const
{
    for(size_t i = 0; i < users_.size(); i++){
        if(uname == users_[i]->uname) {
            return (int)i;
        }
    }
    return -1;
}
