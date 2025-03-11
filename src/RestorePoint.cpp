#include "../include/RestorePoint.hpp"

RestorePoint::RestorePoint(const std::string& name, const std::vector<std::string>& files)
    : name(name)
    , backedUpFiles(files)
    , creationTime(std::chrono::system_clock::now())
{}

const std::string& RestorePoint::getName() const {
    return name;
}

const std::vector<std::string>& RestorePoint::getFiles() const {
    return backedUpFiles;
}

const std::chrono::system_clock::time_point& RestorePoint::getCreationTime() const {
    return creationTime;
} 