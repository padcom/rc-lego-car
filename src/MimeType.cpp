#include <Arduino.h>

String getMimeType(String filename) {
  int index = filename.lastIndexOf('.');
  String ext = index >= 0 ? filename.substring(index, filename.length()) : "";
  if (ext == ".html") return "text/html";
  else if (ext == ".js") return "text/javascript";
  else if (ext == ".css") return "text/css";
  else if (ext == ".png") return "image/png";
  else if (ext == ".jpg") return "image/jpeg";
  else return "application/octet-stream";
}
