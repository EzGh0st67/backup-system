#pragma once
#include <string>
#include <vector>
#include <chrono>

class RestorePoint {
public:
    RestorePoint(const std::string& name, const std::vector<std::string>& files)
        : name_(name)
        , backedUpFiles_(files)
        , creationTime_(std::chrono::system_clock::now())
    {}

    // Геттеры
    const std::string& getName() const { return name_; }
    const std::vector<std::string>& getBackedUpFiles() const { return backedUpFiles_; }
    std::chrono::system_clock::time_point getCreationTime() const { return creationTime_; }

private:
    std::string name_;
    std::vector<std::string> backedUpFiles_;
    std::chrono::system_clock::time_point creationTime_;
}; 