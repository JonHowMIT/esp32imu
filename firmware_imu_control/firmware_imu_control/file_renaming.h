void logname(fs::FS &fs, char const *date, char *buff) { 
  int month, day, year;
  static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
  sscanf(date, "%s %d %d", buff, &day, &year);
  month = (strstr(month_names, buff)-month_names)/3+1;
  sprintf(buff, "/Test_%02d_%02d_%d_000.bin", day, month, year);
  bool file_success = false;
  int loop_counter = 0;
  while (loop_counter < 999){
      String temp = String(loop_counter);
      if (loop_counter < 100){
        temp = '0'+temp;
        }
      if (loop_counter < 10){
        temp = '0'+temp;
        }
      buff[17] = temp[0];
      buff[18] = temp[1];
      buff[19] = temp[2];
      if (!fs.exists(buff)){
        file_success = true;
        break;
      }
      loop_counter++;
  }
  if (!file_success){
    Serial.print("Cannot open SD file: ");    
  }
  else{
    Serial.print("Opened SD file: ");    
  }
  Serial.println(buff);    
}

//renameFile(SD, DATABIN, data_file_r);  // store last data read

void renameFile(fs::FS &fs, const char * path1, const char * path2){
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.printf("  Dir:  %24s",file.name());
            Serial.printf("                 ");
            time_t t= file.getLastWrite();
            struct tm * tmstruct = localtime(&t);
            Serial.printf("  Last write: %d-%02d-%02d %02d:%02d:%02d\n",(tmstruct->tm_year)+1900,( tmstruct->tm_mon)+1, tmstruct->tm_mday,tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.printf("  File: %24s",file.name());
            Serial.printf("  Size: %9d",file.size());
            time_t t= file.getLastWrite();
            struct tm * tmstruct = localtime(&t);
            Serial.printf("  Last write: %d-%02d-%02d %02d:%02d:%02d\n",(tmstruct->tm_year)+1900,( tmstruct->tm_mon)+1, tmstruct->tm_mday,tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);
        }
        file = root.openNextFile();
    }
}
