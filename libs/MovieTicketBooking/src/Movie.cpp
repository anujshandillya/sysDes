#include "../include/Movie.hpp"
#include <sstream>
#include <algorithm>

Movie::Movie(const std::string& id, const std::string& title, const std::string& description,
             Genre genre, int duration, Rating rating, const std::string& director,
             const std::vector<std::string>& cast, const std::string& language)
    : id(id), title(title), description(description), genre(genre), duration(duration),
      rating(rating), director(director), cast(cast), language(language), isActive(true) {}

std::string Movie::getId() const { return id; }
std::string Movie::getTitle() const { return title; }
std::string Movie::getDescription() const { return description; }
Genre Movie::getGenre() const { return genre; }
int Movie::getDuration() const { return duration; }
Rating Movie::getRating() const { return rating; }
std::string Movie::getDirector() const { return director; }
std::vector<std::string> Movie::getCast() const { return cast; }
std::string Movie::getLanguage() const { return language; }
bool Movie::getIsActive() const { return isActive; }

void Movie::setTitle(const std::string& title) { this->title = title; }
void Movie::setDescription(const std::string& description) { this->description = description; }
void Movie::setGenre(Genre genre) { this->genre = genre; }
void Movie::setDuration(int duration) { this->duration = duration; }
void Movie::setRating(Rating rating) { this->rating = rating; }
void Movie::setDirector(const std::string& director) { this->director = director; }
void Movie::setCast(const std::vector<std::string>& cast) { this->cast = cast; }
void Movie::setLanguage(const std::string& language) { this->language = language; }
void Movie::setIsActive(bool isActive) { this->isActive = isActive; }

std::string Movie::getGenreString() const {
    switch (genre) {
        case Genre::ACTION: return "Action";
        case Genre::COMEDY: return "Comedy";
        case Genre::DRAMA: return "Drama";
        case Genre::HORROR: return "Horror";
        case Genre::ROMANCE: return "Romance";
        case Genre::THRILLER: return "Thriller";
        case Genre::SCI_FI: return "Sci-Fi";
        case Genre::DOCUMENTARY: return "Documentary";
        case Genre::ANIMATION: return "Animation";
        case Genre::FAMILY: return "Family";
        default: return "Unknown";
    }
}

std::string Movie::getRatingString() const {
    switch (rating) {
        case Rating::G: return "G";
        case Rating::PG: return "PG";
        case Rating::PG13: return "PG-13";
        case Rating::R: return "R";
        case Rating::NC17: return "NC-17";
        default: return "Unknown";
    }
}

std::string Movie::getDurationString() const {
    int hours = duration / 60;
    int minutes = duration % 60;
    std::ostringstream oss;
    if (hours > 0) {
        oss << hours << "h ";
    }
    if (minutes > 0 || hours == 0) {
        oss << minutes << "m";
    }
    return oss.str();
}

void Movie::addCastMember(const std::string& actor) {
    if (std::find(cast.begin(), cast.end(), actor) == cast.end()) {
        cast.push_back(actor);
    }
}

void Movie::removeCastMember(const std::string& actor) {
    cast.erase(std::remove(cast.begin(), cast.end(), actor), cast.end());
} 