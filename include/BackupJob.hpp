#pragma once
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include "IBackupAlgorithm.hpp"
#include "RestorePoint.hpp"

class BackupJob {
public:
    BackupJob(const std::string& name, std::unique_ptr<IBackupAlgorithm> algorithm, 
             const std::string& storageLocation = "backups")
        : name_(name)
        , algorithm_(std::move(algorithm))
        , storageLocation_(storageLocation)
    {
        if (!algorithm_) {
            throw std::invalid_argument("Algorithm cannot be null");
        }
    }

    // Добавить объект для резервного копирования
    void addBackupObject(const std::string& path) {
        if (!algorithm_->fileExists(path)) {
            throw std::runtime_error("File does not exist: " + path);
        }
        backupObjects_.push_back(path);
    }

    // Удалить объект из списка для резервного копирования
    void removeBackupObject(const std::string& path) {
        auto it = std::find(backupObjects_.begin(), backupObjects_.end(), path);
        if (it != backupObjects_.end()) {
            backupObjects_.erase(it);
        }
    }

    // Создать точку восстановления
    RestorePoint createRestorePoint(const std::string& pointName) {
        if (backupObjects_.empty()) {
            throw std::runtime_error("No backup objects added");
        }

        // Создаем директорию для точки восстановления
        std::string pointPath = storageLocation_ + "/" + pointName;
        algorithm_->backup(backupObjects_, pointPath);

        // Создаем и сохраняем точку восстановления
        RestorePoint point(pointName, backupObjects_);
        restorePoints_.push_back(point);
        return point;
    }

    // Геттеры
    const std::string& getName() const { return name_; }
    const std::vector<RestorePoint>& getRestorePoints() const { return restorePoints_; }
    const std::vector<std::string>& getBackupObjects() const { return backupObjects_; }

private:
    std::string name_;
    std::unique_ptr<IBackupAlgorithm> algorithm_;
    std::string storageLocation_;
    std::vector<std::string> backupObjects_;
    std::vector<RestorePoint> restorePoints_;
}; 