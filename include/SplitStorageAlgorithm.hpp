#pragma once
#include <filesystem>
#include <fstream>
#include "IBackupAlgorithm.hpp"

namespace fs = std::filesystem;

class SplitStorageAlgorithm : public IBackupAlgorithm {
public:
    void backup(const std::vector<std::string>& files, 
               const std::string& storageLocation) override {
        // Создаем директорию для хранения, если её нет
        fs::create_directories(storageLocation);

        // Копируем каждый файл отдельно
        for (const auto& file : files) {
            if (!fileExists(file)) {
                throw std::runtime_error("Source file does not exist: " + file);
            }

            // Получаем имя файла из пути
            std::string fileName = fs::path(file).filename().string();
            std::string destPath = (fs::path(storageLocation) / fileName).string();

            // Копируем файл
            std::ifstream src(file, std::ios::binary);
            std::ofstream dst(destPath, std::ios::binary);
            dst << src.rdbuf();
        }
    }

    std::string getName() const override {
        return "Split Storage Algorithm";
    }
}; 