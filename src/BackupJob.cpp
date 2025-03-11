#include "../include/BackupJob.hpp"
#include <stdexcept>
#include <filesystem>
#include <algorithm>

BackupJob::BackupJob(const std::string& name, std::unique_ptr<IBackupAlgorithm> algorithm,
                     const std::string& storageLocation)
    : name(name)
    , backupAlgorithm(std::move(algorithm))
    , storageLocation(storageLocation)
{
    if (!std::filesystem::exists(storageLocation)) {
        throw std::runtime_error("Storage location does not exist: " + storageLocation);
    }
}

void BackupJob::addBackupObject(const std::string& path) {
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("File does not exist: " + path);
    }
    backupObjects.push_back(path);
}

void BackupJob::removeBackupObject(const std::string& path) {
    auto it = std::find(backupObjects.begin(), backupObjects.end(), path);
    if (it != backupObjects.end()) {
        backupObjects.erase(it);
    }
}

RestorePoint BackupJob::createRestorePoint(const std::string& pointName) {
    if (backupObjects.empty()) {
        throw std::runtime_error("No backup objects added to the job");
    }

    // Создаем точку восстановления
    RestorePoint point(pointName, backupObjects);
    
    // Выполняем резервное копирование
    std::string pointStoragePath = storageLocation + "/" + pointName;
    std::filesystem::create_directory(pointStoragePath);
    backupAlgorithm->backup(backupObjects, pointStoragePath);
    
    // Сохраняем точку восстановления
    restorePoints.push_back(point);
    return point;
}

const std::string& BackupJob::getName() const {
    return name;
}

const std::vector<std::string>& BackupJob::getBackupObjects() const {
    return backupObjects;
}

const std::vector<RestorePoint>& BackupJob::getRestorePoints() const {
    return restorePoints;
}

const std::string& BackupJob::getStorageLocation() const {
    return storageLocation;
}

const IBackupAlgorithm* BackupJob::getAlgorithm() const {
    return backupAlgorithm.get();
} 