#include "../include/Watchable.h"
#include <vector>
#include <utility>
#include <climits>

using namespace std;

/*
 * #####################################User###################################
 */
// constructor
User::User(const string &name) : history(), name(name) {}

// copy constructor
User::User(const User &other) : history(), name(other.name) {
    copyHistory(other);
}

// destructor
User::~User() {
    // clear all history pointers
    history.clear();

}

// getters
string User::getName() const {
    return name;
}

vector<Watchable *> User::get_history() const {
    return history;
}

// Helper Functions
void User::addToHistory(Watchable *currWatchable) {
    history.push_back(currWatchable);
}

void User::copyHistory(const User &other) {
    for (auto i : other.history)
        history.push_back(i);
}

void User::setName(const string &name) {
    User::name = name;
}

/*
 * ##########################LengthRecommenderUser###########################
 */
LengthRecommenderUser::LengthRecommenderUser(const string &name) : User(name) {} // constructor
User *LengthRecommenderUser::clone(const Session &s) {
    LengthRecommenderUser *newUser = new LengthRecommenderUser(this->getName());
    for (int i = 0; i < (signed) this->history.size(); i++) {
        int currWatchableID = history[i]->getId();
        newUser->history.push_back(s.getContent()[currWatchableID - 1]);
    }
    return newUser;
}

Watchable *LengthRecommenderUser::getRecommendation(Session &s) {
    Watchable *nextRecommendation = nullptr;
    double distance = INT_MAX;
    double avrLenOfHistory = 0;
    int sumOfAllContent = 0;
    // calculate the average length in history
    for (Watchable *x : history) {
        sumOfAllContent = sumOfAllContent + x->getLength();
    }
    avrLenOfHistory = (double) (sumOfAllContent) / history.size();
    // find if watchContent is in history
    for (auto &watchContent : s.getContent()) {
        if (std::find(history.begin(), history.end(), watchContent) == history.end()) {
            /* content does not contains watchContent */
            double tmpDistance = abs((double) (watchContent->getLength() - avrLenOfHistory));
            if (tmpDistance < distance) {
                nextRecommendation = watchContent;
                distance = tmpDistance;
            }
        }
    }
    return nextRecommendation;
}

/*
 * #####################RerunRecommenderUser########################
 */

RerunRecommenderUser::RerunRecommenderUser(const string &name) : User(name), lastRecommandedIndex(0) {} // constructor

User *RerunRecommenderUser::clone(const Session &s) {
    auto *newUser = new RerunRecommenderUser(this->getName());
    for (int i = 0; i < (signed) this->history.size(); i++) {
        int currWatchableID = history[i]->getId();
        newUser->history.push_back(s.getContent()[currWatchableID - 1]);
    }
    newUser->lastRecommandedIndex = this->lastRecommandedIndex;
    return newUser;
}

Watchable *RerunRecommenderUser::getRecommendation(Session &s) {
    int tempIndex = lastRecommandedIndex;
    lastRecommandedIndex = (lastRecommandedIndex + 1) % history.size();
    return history[tempIndex];
}

/*
 * ########################GenreRecommenderUser#####################
 */

GenreRecommenderUser::GenreRecommenderUser(const string &name) : User(name) {}

User *GenreRecommenderUser::clone(const Session &s) {
    auto *newUser = new GenreRecommenderUser(this->getName());
    for (int i = 0; i < (signed) this->history.size(); i++) {
        int currWatchableID = history[i]->getId();
        newUser->history.push_back(s.getContent()[currWatchableID - 1]);
    }
    return newUser;
}

Watchable *GenreRecommenderUser::getRecommendation(Session &s) {
    // create tagsVector
    vector<pair<string, int>> tagsVector = createVectorTags(s);
    // sort tagsVector first by value, then by key.
    vector<pair<string, int>> SortedTagsVector;
    SortedTagsVector = sortVectorTags(tagsVector);
    // return recommendation by most common tag
    // going over the sorted tags vector by decreasing order.
    Watchable *nextRecommendation = nullptr;
    for (int i = 0; i < (signed) SortedTagsVector.size(); i++) {
        // going over each watchable* in content
        for (auto j : s.getContent()) {
            // going over all tags of curr watchable*
            for (int k = 0; k < (signed) j->getTags().size(); k++) {
                string currTag = j->getTags()[k];
                if (tagsVector[i].first == currTag) {
                    auto itr = std::find(history.begin(), history.end(), j);
                    if (itr == history.cend()) { // if watchable* is not found in History -> return it
                        nextRecommendation = j;
                    }
                }
            }
        }
    }
    return nextRecommendation;
}

vector<pair<string, int>> GenreRecommenderUser::createVectorTags(Session &s) {
    vector<pair<string, int>> tagsVector;
    for (auto &i : history) {
        for (auto currTag : i->getTags()) {
            auto itr = std::find_if(tagsVector.begin(), tagsVector.end(),
                                    [&currTag](const pair<string, int> &element) { return element.first == currTag; });
            if (itr != tagsVector.cend()) { //currTag is found
                int index = distance(tagsVector.begin(), itr);
                tagsVector[index].second++;
            } else { // currTag is not found
                tagsVector.emplace_back(currTag, 1);
            }
        }
    }
    return tagsVector;
}

vector<pair<string, int>> GenreRecommenderUser::sortVectorTags(vector<pair<string, int>> tagsVector) {
    std::sort(tagsVector.begin(), tagsVector.end(), [](const pair<string, int> &x, const pair<string, int> &y) {
        // compare second value by decreasing order
        if (x.second != y.second)
            return x.second > y.second;
            // compare first by lexicographic order, only if second value is equal
        else
            return x.first < y.first;
    });
    return tagsVector;
}




