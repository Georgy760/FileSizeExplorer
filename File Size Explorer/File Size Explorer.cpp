#include <iostream>
#include <filesystem>
#include <thread>
std::atomic<uintmax_t> total_size(0);

static void calculate_directory_size(const std::filesystem::path& dir_path) {
    
    for (const auto& entry : std::filesystem::recursive_directory_iterator(dir_path)) {
        try {
            if (std::filesystem::is_regular_file(entry)) {
                total_size += std::filesystem::file_size(entry);
            }
        } catch (const std::filesystem::filesystem_error& ex){
            std::cerr << "Error accessing file " << entry.path() << ": " << ex.what() << '\n';
        }
    }
}


static void display_size_progress(){
    while(true){
        std::cout << "\rTotal size: " << total_size << " bytes" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }    
}
int main(){
    std::string path = "C:\\";

    try{
        if(std::filesystem::exists(path)){
            std::thread size_display_thread(display_size_progress);
            calculate_directory_size(path);
            size_display_thread.join();
            std::cout << "\nScanning complete. \n";
        } else {
        std::cerr << "Path " << path << " does not exist.\n";
        }
    } catch (const std::filesystem::filesystem_error& ex) {
        std::cerr << "Error accessing path " << path << ": " << ex.what() << '\n';
    }
    
    return 0;
}