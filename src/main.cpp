#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include "../include/BackupJob.hpp"
#include "../include/SplitStorageAlgorithm.hpp"
#include "../include/SingleStorageAlgorithm.hpp"

namespace fs = std::filesystem;

// Функция для создания тестовых файлов
void createTestFiles() {
    std::cout << "\nСоздание тестовых файлов...\n";
    
    try {
        // Создаем тестовые файлы с разным содержимым
        std::ofstream file1("test1.txt");
        file1 << "Это содержимое файла test1.txt\n"
              << "Первый тестовый файл для демонстрации работы системы резервного копирования.";
        file1.close();

        std::ofstream file2("test2.txt");
        file2 << "Это содержимое файла test2.txt\n"
              << "Второй тестовый файл для демонстрации работы системы резервного копирования.";
        file2.close();

        std::ofstream file3("test3.txt");
        file3 << "Это содержимое файла test3.txt\n"
              << "Третий тестовый файл для демонстрации работы системы резервного копирования.";
        file3.close();

        std::cout << "Тестовые файлы созданы успешно.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при создании тестовых файлов: " << e.what() << "\n";
        throw;
    }
}

// Функция для демонстрации работы с определенным алгоритмом резервного копирования
void demonstrateBackup(const std::string& jobName, std::unique_ptr<IBackupAlgorithm> algorithm) {
    try {
        std::cout << "\nДемонстрация работы с алгоритмом: " << algorithm->getName() << "\n";
        std::cout << "----------------------------------------\n";

        // Создаем задание резервного копирования
        BackupJob backupJob(jobName, std::move(algorithm), "backups/" + jobName);
        std::cout << "Создано задание резервного копирования: " << jobName << "\n";

        // Добавляем объекты для резервного копирования
        backupJob.addBackupObject("test1.txt");
        backupJob.addBackupObject("test2.txt");
        backupJob.addBackupObject("test3.txt");
        std::cout << "Добавлены файлы для резервного копирования\n";

        // Создаем первую точку восстановления
        auto restorePoint1 = backupJob.createRestorePoint("backup_point_1");
        std::cout << "Создана точка восстановления: " << restorePoint1.getName() << "\n";
        std::cout << "Количество файлов в точке восстановления: " << restorePoint1.getBackedUpFiles().size() << "\n";

        // Удаляем один файл из списка объектов
        backupJob.removeBackupObject("test2.txt");
        std::cout << "Удален файл test2.txt из списка объектов для резервного копирования\n";

        // Создаем вторую точку восстановления
        auto restorePoint2 = backupJob.createRestorePoint("backup_point_2");
        std::cout << "Создана точка восстановления: " << restorePoint2.getName() << "\n";
        std::cout << "Количество файлов в точке восстановления: " << restorePoint2.getBackedUpFiles().size() << "\n";

        // Выводим информацию о всех точках восстановления
        std::cout << "\nСписок всех точек восстановления:\n";
        for (const auto& point : backupJob.getRestorePoints()) {
            std::cout << "- " << point.getName() << " (создана: " 
                     << std::chrono::system_clock::to_time_t(point.getCreationTime()) 
                     << ")\n";
            std::cout << "  Файлы в точке восстановления:\n";
            for (const auto& file : point.getBackedUpFiles()) {
                std::cout << "  * " << file << "\n";
            }
        }

        std::cout << "----------------------------------------\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении резервного копирования: " << e.what() << "\n";
        throw;
    }
}

int main() {
    try {
        setlocale(LC_ALL, "Russian");
        std::cout << "Демонстрация работы системы резервного копирования\n";
        std::cout << "================================================\n";

        // Создаем тестовые файлы
        createTestFiles();

        // Создаем директорию для резервных копий, если её нет
        if (!fs::exists("backups")) {
            fs::create_directory("backups");
        }

        std::cout << "\nТестирование алгоритма раздельного хранения:\n";
        demonstrateBackup("SplitStorageJob", std::make_unique<SplitStorageAlgorithm>());

        std::cout << "\nТестирование алгоритма общего хранилища:\n";
        demonstrateBackup("SingleStorageJob", std::make_unique<SingleStorageAlgorithm>());

        // Проверяем наличие созданных резервных копий
        std::cout << "\nПроверка созданных резервных копий:\n";
        std::cout << "----------------------------------------\n";
        for (const auto& entry : fs::recursive_directory_iterator("backups")) {
            if (fs::is_regular_file(entry)) {
                std::cout << "Найден файл резервной копии: " << entry.path().string() << "\n";
                std::cout << "Размер файла: " << fs::file_size(entry) << " байт\n";
            }
        }

        std::cout << "\nДемонстрация завершена успешно!\n";
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << "\n";
        return 1;
    }
} 