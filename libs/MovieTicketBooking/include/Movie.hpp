#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <string>
#include <vector>

enum class Genre {
    ACTION,
    COMEDY,
    DRAMA,
    HORROR,
    ROMANCE,
    THRILLER,
    SCI_FI,
    DOCUMENTARY,
    ANIMATION,
    FAMILY
};

enum class Rating {
    G,
    PG,
    PG13,
    R,
    NC17
};

class Movie {
private:
    std::string id;
    std::string title;
    std::string description;
    Genre genre;
    int duration; // in minutes
    Rating rating;
    std::string director;
    std::vector<std::string> cast;
    std::string language;
    bool isActive;

public:
    Movie(const std::string& id, const std::string& title, const std::string& description,
          Genre genre, int duration, Rating rating, const std::string& director,
          const std::vector<std::string>& cast, const std::string& language);
    
    // Getters
    std::string getId() const;
    std::string getTitle() const;
    std::string getDescription() const;
    Genre getGenre() const;
    int getDuration() const;
    Rating getRating() const;
    std::string getDirector() const;
    std::vector<std::string> getCast() const;
    std::string getLanguage() const;
    bool getIsActive() const;
    
    // Setters
    void setTitle(const std::string& title);
    void setDescription(const std::string& description);
    void setGenre(Genre genre);
    void setDuration(int duration);
    void setRating(Rating rating);
    void setDirector(const std::string& director);
    void setCast(const std::vector<std::string>& cast);
    void setLanguage(const std::string& language);
    void setIsActive(bool isActive);
    
    // Utility methods
    std::string getGenreString() const;
    std::string getRatingString() const;
    std::string getDurationString() const;
    void addCastMember(const std::string& actor);
    void removeCastMember(const std::string& actor);
};

#endif // MOVIE_HPP 