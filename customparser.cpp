// Complete me
#include "customparser.h"
#include <iostream>
#include <sstream>

using namespace std;

const char* error_msg_1 = "Cannot read integer n";
const char* error_msg_2 = "Error in content parsing";
const char* error_msg_3 = "Error in user parsing";


void CustomFormatParser::parse(std::istream& is, std::vector<Content*>& content, std::vector<User*>& users)
{
    // Erase any old contents
    content.clear();
    users.clear();


    string whitespace = " \n\r\t\f\v";  //this is to eliminate any white spaces

    int numContent = 0;
    is >> numContent;
    if (is.fail()) {
        throw ParserError(error_msg_1);
    }

    //Initializes all variables except for episodes for series
    int id = 0;
    int type = -1;
    string name = "";
    int numReviews = 0;
    int totalStars = 0;
    int rating = 0;
    int numEpisodes = 0;

    //Initializes all contents
    for (int i = 0; i < numContent; i++) {
        //id handling
        is >> id;
        if (is.fail()) {
            throw ParserError(error_msg_2);
        }

        //type handling
        is >> type;
        if (is.fail()) {
            throw ParserError(error_msg_2);
        }

        is.ignore(1000, '\n');

        //name handling
        getline(is, name);
        if (is.fail()) {
            throw ParserError(error_msg_2);
        }
        size_t startingIndex = name.find_first_not_of(whitespace);
        name = name.substr(startingIndex); //to remove tab at the front

        //handles number of reviews
        is >> numReviews;
        if (is.fail()) {
            throw ParserError(error_msg_2);
        }

        //handles number of stars
        is >> totalStars;
        if (is.fail()) {
            throw ParserError(error_msg_2);
        }
        
        //handles rating
        is >> rating;
        if (is.fail()) {
            throw ParserError(error_msg_2);
        }

        //gets numEpisodes if the type is a series
        if (type == 1) {
          is >> numEpisodes;
          if (is.fail()) {
              throw ParserError(error_msg_2);
          }
        }

        is.ignore(1000, '\n');  //to skip to the next line

        //gets the string of users
        string user = "";
        getline(is, user);
        if (is.fail()) {
          throw ParserError(error_msg_2);
        }
        startingIndex = user.find_first_not_of(whitespace); //to get rid of the tab at the front
        if (startingIndex != string::npos) {  //checks if there are any users
          user = user.substr(startingIndex);
        }

        if (type == 0) {  //for a Movie
            //allocates memory for movie
            Content* temp = new Movie(id, name, numReviews, totalStars, rating);

            while (user.length() > 0) { //adds each user separately
              int indexOfSpace = user.find(' ');  //index of space
              if (indexOfSpace == -1) {  //case of the last viewer
                temp->addViewer(user);
                user = "";
              }
              else {  //case where there are more than one viewer
                temp->addViewer(user.substr(0, indexOfSpace));
                user = user.substr(indexOfSpace + 1);
              }
            }

            content.push_back(temp);
        }
        else {
            //for a series, you need numEpisodes
            //allocates memory for series
            Content* temp = new Series(id, name, numReviews, totalStars, rating, numEpisodes);


            while (user.length() > 0) { //adds each user separately
              int indexOfSpace = user.find(' ');  //index of space
              if (indexOfSpace == -1) {  //case of the last viewer
                temp->addViewer(user);
                user = "";
              }
              else {  //case where there are more than one viewer
                temp->addViewer(user.substr(0, indexOfSpace));
                user = user.substr(indexOfSpace + 1);
              }
            }

            content.push_back(temp);
        }
    }


    //adds all the users now
    string user = "";
    int ratingLimit = 0;
    while (is >> user) {
      //handles the rating limit
      is >> ratingLimit;
      if (is.fail()) {
        throw ParserError(error_msg_3);
      }
      is.ignore(1000, '\n');

      //allocates a new user
      User* u = new User(user, ratingLimit);


      getline(is, user);   //user is now all of the contentIDs
      int contentID = 0;
      stringstream allIDs(user);
      while (allIDs >> contentID) {
        u->addToHistory(contentID);
      }
      //resets the stringstream
      allIDs.clear();
      allIDs.str("");
      users.push_back(u);
    }

}