#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"
#include "../include/json.hpp"

using json = nlohmann::json;

class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    Session(const Session& other);
    ~Session();

    void start();

    const std::vector<Watchable *> &getContent() const;

    const std::vector<BaseAction *> &getActionsLog() const;

    const std::unordered_map<std::string, User *> &getUserMap() const;

    User *getActiveUser() const;

private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    void insertMovies (json &jsonFile);
    void insertSeries (json &jsonFile);
};
#endif
