#include <FS.h>

class FileSystemHelper {
  public:
    FileSystemHelper () {
        SPIFFS.begin();
    }
    String readFile(String filePath, int maxSize = 2096) {         // Прочитать файл по заданному пути
      File targetFile = SPIFFS.open("/" + filePath, "r");
      if (!targetFile) {
        return "Failed to open file";
      }
      size_t size = targetFile.size();
      if (size > maxSize) {
        targetFile.close();
        return "Large";
      }
      String readedString = targetFile.readString();
      targetFile.close();
      return readedString;
    }
};
