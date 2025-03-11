#include "../include/SplitStorageAlgorithm.hpp"
#include <filesystem>
#include <fstream>
#include <stdexcept>

void SplitStorageAlgorithm::backup(const std::vector<std::string>& files, const std::string& storageLocation) {
    for (const auto& file : files) {
        if (!std::filesystem::exists(file)) {
            throw std::runtime_error("Source file does not exist: " + file);
        }

        std::filesystem::path sourcePath(file);
        std::filesystem::path destPath = std::filesystem::path(storageLocation) / sourcePath.filename();
        
        try {
            std::filesystem::copy_file(
                sourcePath,
                destPath,
                std::filesystem::copy_options::overwrite_existing
            );
        } catch (const std::filesystem::filesystem_error& e) {
            throw std::runtime_error("Failed to backup file " + file + ": " + e.what());
        }
    }
}

std::string SplitStorageAlgorithm::getName() const {
    return "Split Storage";
} 