
#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>


class Session;

class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>& tags);
    virtual ~Watchable();
    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    virtual std::string shortToString() const = 0;

    const long getId() const;
    int getLength() const;

    const std::vector<std::string> &getTags() const;

    bool operator<(const Watchable &rhs) const;

    bool operator>(const Watchable &rhs) const;

    bool operator<=(const Watchable &rhs) const;

    bool operator>=(const Watchable &rhs) const;

    bool operator==(const Watchable &rhs) const;

    bool operator!=(const Watchable &rhs) const;


private:
    const long id;
    int length;
    std::vector<std::string> tags;

};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    Movie(const Movie& other);
    virtual std::string toString() const;

    virtual Watchable* getNextWatchable(Session&) const;

    const std::string &getName() const;

    virtual std::string shortToString () const;

private:
    std::string name;


};


class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual std::string shortToString () const;

    const std::string &getSeriesName() const;

    int getSeason() const;

    int getEpisode() const;

    long getNextEpisodeId() const;

private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;

};
#endif

