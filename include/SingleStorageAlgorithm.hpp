#pragma once
#include <filesystem>
#include <fstream>
#include <sstream>
#include "IBackupAlgorithm.hpp"

namespace fs = std::filesystem;

class SingleStorageAlgorithm : public IBackupAlgorithm {
public:
    void backup(const std::vector<std::string>& files, 
               const std::string& storageLocation) override {
        // Создаем директорию для хранения, если её нет
        fs::create_directories(storageLocation);

        // Создаем один архивный файл
        std::string archivePath = (fs::path(storageLocation) / "archive.bak").string();
        std::ofstream archive(archivePath, std::ios::binary);

        for (const auto& file : files) {
            if (!fileExists(file)) {
                throw std::runtime_error("Source file does not exist: " + file);
            }

            // Записываем имя файла и его размер
            std::string fileName = fs::path(file).filename().string();
            std::ifstream src(file, std::ios::binary);
            std::stringstream buffer;
            buffer << src.rdbuf();
            std::string content = buffer.str();

            archive << "FILE: " << fileName << "\n";
            archive << "SIZE: " << content.size() << "\n";
            archive << "CONTENT:\n" << content << "\n";
            archive << "END_FILE\n";
        }
    }

    std::string getName() const override {
        return "Single Storage Algorithm";
    }
}; 