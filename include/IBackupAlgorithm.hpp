#pragma once
#include <string>
#include <vector>
#include <fstream>

class IBackupAlgorithm {
public:
    virtual ~IBackupAlgorithm() = default;
    
    // Создает резервную копию файлов
    virtual void backup(const std::vector<std::string>& files, 
                       const std::string& storageLocation) = 0;
    
    // Возвращает название алгоритма
    virtual std::string getName() const = 0;
    
    // Проверяет существование файла
    virtual bool fileExists(const std::string& filePath) const {
        std::ifstream file(filePath);
        return file.good();
    }
}; 