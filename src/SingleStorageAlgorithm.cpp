#include "../include/SingleStorageAlgorithm.hpp"
#include <filesystem>
#include <fstream>
#include <stdexcept>

void SingleStorageAlgorithm::backup(const std::vector<std::string>& files, const std::string& storageLocation) {
    std::filesystem::path archivePath = std::filesystem::path(storageLocation) / "backup.dat";
    std::ofstream archive(archivePath, std::ios::binary);
    
    if (!archive) {
        throw std::runtime_error("Failed to create archive: " + archivePath.string());
    }

    for (const auto& file : files) {
        if (!std::filesystem::exists(file)) {
            throw std::runtime_error("Source file does not exist: " + file);
        }

        std::filesystem::path sourcePath(file);
        std::ifstream source(file, std::ios::binary);
        
        if (!source) {
            throw std::runtime_error("Failed to open source file: " + file);
        }

        // Записываем имя файла и его размер
        std::string filename = sourcePath.filename().string();
        size_t filenameSize = filename.size();
        size_t fileSize = std::filesystem::file_size(file);
        
        archive.write(reinterpret_cast<const char*>(&filenameSize), sizeof(filenameSize));
        archive.write(filename.c_str(), filenameSize);
        archive.write(reinterpret_cast<const char*>(&fileSize), sizeof(fileSize));

        // Копируем содержимое файла
        std::vector<char> buffer(8192);
        while (source) {
            source.read(buffer.data(), buffer.size());
            std::streamsize bytesRead = source.gcount();
            if (bytesRead > 0) {
                archive.write(buffer.data(), bytesRead);
            }
        }
    }
}

std::string SingleStorageAlgorithm::getName() const {
    return "Single Storage";
} 