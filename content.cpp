#include "content.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;


Content::Content(int id, std::string name, int nr, int ts, int rating)
{
  //initializes all the private variables;
    id_ = id;
    name_ = name;
    numReviews_ = nr;
    totalStars_ = ts;
    rating_ = rating;
}

Content::~Content()
{
  usersWhoViewed_.clear();
}

void Content::display(std::ostream& ostr) const
{
    ostr << "========" << endl;
    ostr << "ID: " << id_ << endl;
    ostr << "Name: " << name_ << endl;
    ostr << "Views: " << usersWhoViewed_.size() << endl;
    ostr << "Average review: " << setprecision(1) << fixed << getStarAverage() << endl;
    
    if(0 >= rating_ && rating_ < 5) {
        const char** ratingMapping = this->ratingStrings();
        ostr << "Rating: " << ratingMapping[rating_] << endl;
    }
}

int Content::id() const
{
    return id_;
}

std::string Content::name() const
{
    return name_;
}

int Content::rating() const
{
    return rating_;
}

void Content::review(int numStars) 
{
    numReviews_++;
    totalStars_ += numStars;
}

void Content::addViewer(const std::string& username)
{
    usersWhoViewed_.push_back(username);
}

const std::vector<std::string>& Content::getViewers() const
{
    return usersWhoViewed_;
}

bool Content::hasViewed(const std::string& uname) const
{
    for(size_t m = 0; m < usersWhoViewed_.size(); m++){
        if(usersWhoViewed_[m] == uname){
            return true;
        }
    }
    return false;
}

const char** Movie::ratingStrings() const
{
    // Global variable of movie rating mappings that you can use for 
    //  displaying Movie objects. We use "G" for rating 0 and 1.
    static const char* movierating[] = {"G", "G", "PG", "PG-13", "R"};
    return movierating;
}

const char** Series::ratingStrings() const
{
    // DEFAULT TO THE SERIES / TV Rating Mappings
    
    // Global variable of series rating mapping (0 = "TVY", 1 = "TVG", etc.)
    static const char* tvrating[] = {"TVY", "TVG", "TVPG", "TV14", "TVMA"};
    return tvrating;
}

double Content::getStarAverage() const 
{
  if (numReviews_ == 0) {
    return 0.0;
  }
  return (double)totalStars_/numReviews_;
}

int Series::numEpisodes() const
{
    // Should return the number of episodes in the series
    return numEpisodes_;
}

Movie::Movie(int id, std::string name, int nr, int ts, int rating)
 : Content(id, name, nr, ts, rating) {
}

Series::Series(int id, std::string name, int nr, int ts, int rating, int numEpisodes)
 : Content(id, name, nr, ts, rating) {
    numEpisodes_ = numEpisodes;
}

void Series::display(std::ostream& ostr) const {
    Content::display(ostr);
    ostr << "Episodes: " << numEpisodes_ << endl;
}

Series::~Series() {
}

Movie::~Movie() {
}